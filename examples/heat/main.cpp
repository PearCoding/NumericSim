#include "Types.h"
#include "Iterative.h"

#include <iostream>
#include <fstream>
#include <chrono>

NS_USE_NAMESPACE;

/*
* Heat equation solver for boundary and initial value problems.
*/

//#define SPARSE_PATTER_OUTPUT // Enable this to create a huge file showing the patterns of the sparse matrix

/* Boundary modes:
* 0 - Dirichlet boundaries
* 1 - Neumann boundaries (TODO)
* 2 - Mix boundaries (TODO)
*/
#define BOUNDARY_MODE (0)

#define LIN(t,x,y) ((y)*XC*TC + (x)*TC + (t))
int main(int argc, char** argv)
{
	constexpr double pi = 3.141592;

	// Some parameters to change... Feel free to play around :)
	constexpr double RELAX_PAR = 1.2;// SOR weight parameter (0,2]
	constexpr double grid_width = 1;
	constexpr double grid_height = 1;
	constexpr double time_end = 1;
	constexpr double HX = 0.05;
	constexpr double HY = 0.05;
	constexpr double HT = 0.05;

	auto k = [=](double t, double x, double y) { return 0.6;};
	auto f = [=](double t, double x, double y) { return 0;};
	auto b = [=](double t, double x, double y) { return 0;};// Boundary condition
	auto t0 = [=](double x, double y) { return 2 * (std::exp(10 * x*(1 - x)*y*(1 - y)) - 1);};// Initial condition

												   // Everything after this comment does not need to be changed.
	constexpr Dimension XC = grid_width / HX + 1;
	constexpr Dimension YC = grid_height / HY + 1;
	constexpr Dimension TC = time_end / HT + 1;
	constexpr Dimension D = TC*XC*YC;

	// Some precalculated constants
	constexpr double HX2 = HX*HX;
	constexpr double HY2 = HY*HY;
	constexpr double IHX2 = 1 / HX2;
	constexpr double IHY2 = 1 / HY2;
	constexpr double IHT = 1 / HT;
	constexpr double C = 2 * (IHX2 + IHY2);

	std::cout << "TC " << TC << "XC " << XC << " YC " << YC << " D " << D << std::endl;

	SparseMatrix<double> A(D, D);
	Vector<double> B(D);

	auto p1_start = std::chrono::high_resolution_clock::now();

	// Boundary conditions
	for (Index t = 0; t < TC; ++t)
	{
		for (Index x = 0; x < XC; ++x)
		{
#if BOUNDARY_MODE == 1//Neumann
#else// Dirichlet
			A.set(LIN(t, x, 0), LIN(t, x, 0), 1);
			A.set(LIN(t, x, YC - 1), LIN(t, x, YC - 1), 1);
#endif
			B.set(LIN(t, x, 0), b(t*HT, x*HX, 0));
			B.set(LIN(t, x, YC - 1), b(t*HT, x*HX, (YC - 1)*HY));
		}

		for (Index y = 0; y < YC; ++y)
		{
#if BOUNDARY_MODE == 1// Neumann
#else// Dirichlet
			A.set(LIN(t, 0, y), LIN(t, 0, y), 1);
			A.set(LIN(t, XC - 1, y), LIN(t, XC - 1, y), 1);
#endif
			B.set(LIN(t, 0, y), b(t*HT, 0, y*HY));
			B.set(LIN(t, XC - 1, y), b(t*HT, (XC - 1)*HX, y*HY));
		}
	}

	// Matrix
#if BOUNDARY_MODE == 1
# define ST (2)
#else
# define ST (1)
#endif
	// Initial condition
	for (Index x = ST; x < XC - ST; ++x)
	{
		for (Index y = ST; y < YC - ST; ++y)
		{
			A.set(LIN(0, x, y), LIN(0, x, y), 1);
			A.set(LIN(0, x, y), LIN(0, x, y), 1);

			B.set(LIN(0, x, y), t0(x*HX, y*HY));
			B.set(LIN(0, x, y), t0(x*HX, y*HY));
		}
	}

	for (Index t = 1; t < TC; ++t)
	{
		for (Index y = ST; y < YC - ST; ++y)
		{
			for (Index x = ST; x < XC - ST; ++x)
			{
				const auto kf = k(t, x, y);

				A.set(LIN(t, x, y), LIN(t, x, y), C*kf + IHT);

				A.set(LIN(t, x, y), LIN(t - 1, x, y), -IHT);

				A.set(LIN(t, x, y), LIN(t, x - 1, y), -IHX2*kf);
				A.set(LIN(t, x, y), LIN(t, x + 1, y), -IHX2*kf);

				A.set(LIN(t, x, y), LIN(t, x, y - 1), -IHY2*kf);
				A.set(LIN(t, x, y), LIN(t, x, y + 1), -IHY2*kf);

				B.set(LIN(t, x, y), f(t*HT, x*HX, y*HY));
			}
		}
	}

	auto p1_diff = std::chrono::high_resolution_clock::now() - p1_start;

	std::cout << "Sparse Matrix: Entries " << A.filledCount()
		<< " [" << 100 * (A.filledCount() / (double)A.size()) << "%] ["
		<< std::chrono::duration_cast<std::chrono::milliseconds>(p1_diff).count()
		<< " ms]" << std::endl;

	// Output matrix
#ifdef SPARSE_PATTER_OUTPUT
	std::ofstream sparse_pattern("heat_pattern.txt");
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
	Vector<double> X(D);

	auto p2_start = std::chrono::high_resolution_clock::now();
	X = Iterative::serial::sor<SparseMatrix, double>(A, B, X, RELAX_PAR, 1024, 1e-4, &iterations);
	auto p2_diff = std::chrono::high_resolution_clock::now() - p2_start;

	std::cout << iterations << " Iterations ["
		<< std::chrono::duration_cast<std::chrono::seconds>(p2_diff).count()
		<< " s]" << std::endl;

	// Writing dat
	std::ofstream data("heat_data.dat");

	data << TC << " " << 0 << " " << time_end << std::endl;
	data << XC << " " << 0 << " " << grid_width << std::endl;
	data << YC << " " << 0 << " " << grid_height << std::endl;
	data << std::endl;

	for (Index t = 0; t < TC; ++t)
	{
		for (Index y = 0; y < YC; ++y)
		{
			for (Index x = 0; x < XC; ++x)
			{
				data << X.at(LIN(t, x, y)) << " ";
			}
			data << std::endl;
		}
		data << std::endl;
	}
	data.close();

	return 0;
}