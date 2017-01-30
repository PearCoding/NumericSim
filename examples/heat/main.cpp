#include "matrix/Matrix.h"
#include "Iterative.h"

#include <iostream>
#include <fstream>
#include <chrono>

NS_USE_NAMESPACE;

/*
* Heat equation solver for boundary and initial value problems.
*/

//#define SPARSE_PATTERN_OUTPUT // Enable this to create a huge file showing the patterns of the sparse matrix

/* Boundary modes:
* 0 - Dirichlet boundaries
* 1 - Neumann boundaries (TODO)
* 2 - Mix boundaries (TODO)
*/
#define BOUNDARY_MODE (0)

#define LIN(t,x,y) ((t)*YC*XC + (y)*XC + (x))
int main(int argc, char** argv)
{
	constexpr double pi = 3.141592;

	// Some parameters to change... Feel free to play around :)
	constexpr double RELAX_PAR = 1.2;// SOR weight parameter (0,2]
	constexpr double grid_width = 1;
	constexpr double grid_height = 1;
	constexpr double time_end = 1;
	constexpr double HX = 0.01;
	constexpr double HY = 0.01;
	constexpr double HT = 0.01;

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

#if BOUNDARY_MODE == 1
# define ST (2)
#else
# define ST (1)
#endif

	constexpr size_t expected = TC*(XC + YC) * 2 + (XC - 2 * ST)*(YC - 2 * ST) * 2 + (TC - 1)*(XC - 2 * ST)*(YC - 2 * ST) * 6;

	std::cout << "TC " << TC << " XC " << XC << " YC " << YC << " D " << D << " Expected Entries: " << expected << std::endl;

	SparseMatrix<double> A(D, D, expected);
	DynamicVector<double> B;
	B.resize(D);

	auto p1_start = std::chrono::high_resolution_clock::now();

	// Boundary conditions
	for (Index t = 0; t < TC; ++t)
	{
		for (Index y = 0; y < YC; ++y)
		{
			for (Index x = 0; x < XC; ++x)
			{
				const double ft = t * HT;
				const double fx = x * HX;
				const double fy = y * HY;

				const Index mid = LIN(t, x, y);

				if (t == 0 || y == 0 || x == 0 || y == YC - 1 || x == XC - 1) // Initial conditions
				{
#if BOUNDARY_MODE == 1//Neumann
#else// Dirichlet
					A.set(mid, mid, 1);

					if(t != 0)
						B.set(mid, b(ft, fx, fy)); // Boundary
					else
						B.set(mid, t0(fx, fy)); // Initial
#endif
				}
				else
				{
					const auto kf = k(ft, fx, fy);

					// This is in topological order and important for efficiency
					A.set(mid, LIN(t - 1, x, y), -IHT);
					A.set(mid, LIN(t, x, y - 1), -IHY2*kf);
					A.set(mid, LIN(t, x - 1, y), -IHX2*kf);

					A.set(mid, mid, C*kf + IHT);

					A.set(mid, LIN(t, x + 1, y), -IHX2*kf);
					A.set(mid, LIN(t, x, y + 1), -IHY2*kf);

					B.set(mid, f(ft, fx, fy));
				}
			}
		}

		std::cout << "t=" << t << std::endl;
	}

	auto p1_diff = std::chrono::high_resolution_clock::now() - p1_start;

	std::cout << "Sparse Matrix: Entries " << A.filled_count()
		<< " [" << 100 * (A.filled_count() / (double)A.size()) << "%] ["
		<< std::chrono::duration_cast<std::chrono::milliseconds>(p1_diff).count()
		<< " ms]" << std::endl;

	// Output matrix
#ifdef SPARSE_PATTERN_OUTPUT
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
	size_t iterations = 0;
	DynamicVector<double> X;
	X.resize(D);

	auto p2_start = std::chrono::high_resolution_clock::now();
	X = Iterative::serial::sor(A, B, X, RELAX_PAR, 1024, 1e-4, &iterations);
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