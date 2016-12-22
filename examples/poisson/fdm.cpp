#include "Types.h"
#include "Iterative.h"

#include <iostream>
#include <fstream>
#include <chrono>

NS_USE_NAMESPACE;

/*
* Poisson equation solver for boundary problems.
*/

//#define SPARSE_PATTER_OUTPUT // Enable this to create a huge file showing the patterns of the sparse matrix

/* Boundary modes:
* 0 - Dirichlet boundaries
* 1 - Neumann boundaries (TODO)
* 2 - Mix boundaries (TODO)
*/
#define BOUNDARY_MODE (0)

#define LIN(x,y) ((y)*XC + (x))
int main(int argc, char** argv)
{
	constexpr double pi = 3.141592;

	// Some parameters to change... Feel free to play around :)
	constexpr double RELAX_PAR = 1.2;// SOR weight parameter (0,2]
	constexpr double grid_width = 1;
	constexpr double grid_height = 1;
	constexpr double H = 0.005;
	constexpr double K = 0.005;

	//auto f = [=](double x, double y) { return std::exp(-100 * (y + x));};
	auto f = [=](double x, double y) { return -std::sin(pi * x)*std::sin(pi * y);};
	//auto b = [=](double x, double y) { return std::sin(2 * pi * x / grid_width) + std::cos(2 * pi * y / grid_height);};
	auto b = [=](double x, double y) { return 0;};

	// Everything after this comment does not need to be changed.
	constexpr Dimension XC = grid_width / H + 1;
	constexpr Dimension YC = grid_height / K + 1;
	constexpr Dimension D = XC*YC;

	// Some precalculated constants
	constexpr double IH = 1 / H;
	constexpr double IK = 1 / K;
	constexpr double H2 = H*H;
	constexpr double K2 = K*K;
	constexpr double R2 = K2 / H2;
	constexpr double C = -2 * (1 + R2);

	std::cout << "XC " << XC << " YC " << YC << " D " << D << std::endl;

	SparseMatrix<double> A(D, D);
	DynamicVector<double> B;
	B.resize(D);

	auto p1_start = std::chrono::high_resolution_clock::now();
	for (Index x = 0; x < XC; ++x)
	{
#if BOUNDARY_MODE == 1//Neumann
		A.set(LIN(x, 0), LIN(x, 0), -IK);
		A.set(LIN(x, 1), LIN(x, 1), IK);

		A.set(LIN(x, YC - 1), LIN(x, YC - 1), IK);
		A.set(LIN(x, YC - 2), LIN(x, YC - 2), -IK);
#else// Dirichlet
		A.set(LIN(x, 0), LIN(x, 0), 1);
		A.set(LIN(x, YC - 1), LIN(x, YC - 1), 1);
#endif
		B.set(LIN(x, 0), b(x*H, 0));
		B.set(LIN(x, YC - 1), b(x*H, (YC - 1)*K));
	}

	for (Index y = 0; y < YC; ++y)
	{
#if BOUNDARY_MODE == 1// Neumann
		A.set(LIN(0, y), LIN(0, y), -IH);
		A.set(LIN(1, y), LIN(1, y), IH);

		A.set(LIN(XC - 1, y), LIN(XC - 1, y), IH);
		A.set(LIN(XC - 2, y), LIN(XC - 2, y), -IH);
#else// Dirichlet
		A.set(LIN(0, y), LIN(0, y), 1);
		A.set(LIN(XC - 1, y), LIN(XC - 1, y), 1);
#endif
		B.set(LIN(0, y), b(0, y*K));
		B.set(LIN(XC - 1, y), b((XC - 1)*H, y*K));
	}

#if BOUNDARY_MODE == 1
# define ST (2)
#else
# define ST (1)
#endif
	for (Index y = ST; y < YC - ST; ++y)
	{
		for (Index x = ST; x < XC - ST; ++x)
		{
			A.set(LIN(x, y), LIN(x - 1, y), R2);
			A.set(LIN(x, y), LIN(x, y), C);
			A.set(LIN(x, y), LIN(x + 1, y), R2);
			A.set(LIN(x, y), LIN(x, y - 1), 1);
			A.set(LIN(x, y), LIN(x, y + 1), 1);
			B.set(LIN(x, y), K2*f(x*H, y*K));
		}
	}

	auto p1_diff = std::chrono::high_resolution_clock::now() - p1_start;

	std::cout << "Sparse Matrix: Entries " << A.filledCount()
		<< " [" << 100 * (A.filledCount() / (double)A.size()) << "%] ["
		<< std::chrono::duration_cast<std::chrono::milliseconds>(p1_diff).count()
		<< " ms]" << std::endl;

	// Output matrix
#ifdef SPARSE_PATTER_OUTPUT
	std::ofstream sparse_pattern("poisson_pattern.txt");
	for (Index y = 0; y < D; ++y)
	{
		for (Index x = 0; x < D; ++x)
		{
			if (A.has(y, x))
				sparse_pattern << "x ";
			else
				sparse_pattern << "  ";
		}
		sparse_pattern << std::endl;
	}
	sparse_pattern.close();
#endif

	std::cout << "Calculating..." << std::endl;

	// Iterations
	uint32 iterations = 0;
	DynamicVector<double> X;
	X.resize(D);

	auto p2_start = std::chrono::high_resolution_clock::now();
	X = Iterative::serial::sor<SparseMatrix, double>(A, B, X, RELAX_PAR, 1024, 1e-4, &iterations);
	auto p2_diff = std::chrono::high_resolution_clock::now() - p2_start;

	std::cout << iterations << " Iterations ["
		<< std::chrono::duration_cast<std::chrono::seconds>(p2_diff).count()
		<< " s]" << std::endl;

	// Writing dat
	std::ofstream data("poisson_fdm.dat");
	for (Index y = 0; y < YC; ++y)
	{
		for (Index x = 0; x < XC; ++x)
		{
			data << X.at(LIN(x, y)) << " ";
		}
		data << std::endl;
	}
	data.close();

	return 0;
}