#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include "matrix/SparseMatrix.h"
#include "Iterative.h"
#include "CG.h"
#include "Vector.h"
#include "Simplex.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

/*
 * Undef if you want to use the iterative SOR algorithm
 */
#define USE_CG

/**
 * Program to solve the poisson equation in 2d with
 * f = sin(pi*x)sin(pi*y)
 * and dirichlet boundary u = 0
 *
 * Should be called with a commandline argument N.
 * N is the rectangle count for each dimension
 *
 * Internally a triangle representation is used.
 */

typedef float Number;

constexpr float DomainStart[2]    = {0,0};
constexpr float DomainEnd[2]      = {1,1};
constexpr float DomainDistance[2] = {DomainEnd[0]-DomainStart[0],
									 DomainEnd[1]-DomainStart[1]};

constexpr double RELAX_PAR = 1.2;// SOR weight parameter (0,2]


/**
 * Main entry
 */
int main(int argc, char** argv)
{
	if(argc != 2)
	{
		std::cout << "Not enough arguments given! Use 'example_poisson_fem N'" << std::endl;
		return -1;
	}

	int32 N = 0;
	try
	{
		std::string str = argv[1];
		N = std::stol(str);
	}
	catch(...)
	{
		std::cout << "Invalid N given. Should be an INTEGER greater 2" << std::endl;
		return -2;
	}

	if(N < 2)
	{
		std::cout << "Invalid N given. Should be greater than 2" << std::endl;
		return -3;
	}

	// --------------------------------
	// Calculating basic constants
	std::cout << "Generating mesh with " << N << "x" << N << " rectangles..." << std::endl;
	const float H[3] = {DomainDistance[0]/N,
						DomainDistance[1]/N,
						std::sqrt(DomainDistance[0]*DomainDistance[0] + DomainDistance[1]*DomainDistance[1])/N};
	const float InvH[3] = {1/H[0],1/H[1],1/H[2]};
	std::cout << "  H = [" << H[0] << ", " << H[1] << "] Triangle Hypotenuse " << H[2] << std::endl;

	const int32 TriangleCount = 2*N;

	const Triangle2D<Number> ReferenceTriangle({{0,0}, {H[0],0}, {0,H[1]}});
	const Number AreaTriangle = ReferenceTriangle.volume();
	std::cout << "  Triangle Area: " << AreaTriangle << std::endl;

	const int32 VertexCount = N+1;
	const int32 VertexSize = VertexCount*VertexCount;
	std::cout << "  Vertices = " << VertexCount << "x" << VertexCount << "  (" << VertexSize << ")" << std::endl;

	const int32 MatrixSize = VertexSize*VertexSize;
	std::cout << "  Matrix size = " << VertexSize << "x" << VertexSize << "  (" << MatrixSize << ")" << std::endl;

	// --------------------------------
	// Assembly matrix
	// Calculate for one row
	Number row[7] = {0,0,0, 0, 0,0,0};
	{
		/* Based on numbering:
		 * (0,  0) ----- (Hx,  0) ----- ...
		 *   |  \           |  \           |
		 *   |     \   T1   |     \        |
		 *   |   T2   \     |   T3   \     |
		 *   |           \  |           \  |
		 * (0, Hy) ----- (Hx, Hy) ----- (2Hx, Hy)
		 *   |  \           |  \           |
		 *   |     \   T4   |     \   T5   |
		 *   |        \     |   T6   \     |
		 *   |           \  |           \  |
		 *   ...   ----- (Hx,2Hy) ----- (2Hx,2Hy)
		 */
		Triangle2D<Number> t1({{   0,   0},{   H[0],  H[1]},{   H[0],     0}});
		Triangle2D<Number> t2({{   0,   0},{      0,  H[1]},{   H[0],  H[1]}});
		Triangle2D<Number> t3({{H[0],   0},{   H[0],  H[1]},{ 2*H[0],  H[1]}});
		Triangle2D<Number> t4({{   0,H[1]},{   H[0],2*H[1]},{   H[0],  H[1]}});
		Triangle2D<Number> t5({{H[0],H[1]},{ 2*H[0],2*H[1]},{ 2*H[0],  H[1]}});
		Triangle2D<Number> t6({{H[0],H[1]},{   H[0],2*H[1]},{ 2*H[0],2*H[1]}});

		Triangle2D<Number> dt1 = t1.gradient();
		Triangle2D<Number> dt2 = t2.gradient();
		Triangle2D<Number> dt3 = t3.gradient();
		Triangle2D<Number> dt4 = t4.gradient();
		Triangle2D<Number> dt5 = t5.gradient();
		Triangle2D<Number> dt6 = t6.gradient();

		// std::cout << t1.area() << " "  << t2.area() << " " 
		// << t3.area() << " " << t4.area() << " " 
		// << t5.area() << " " << t6.area() << std::endl;

		//T1
		row[0] += dt1.vertices[0][0]*dt1.vertices[0][0] + dt1.vertices[0][1]*dt1.vertices[0][1];
		row[3] += dt1.vertices[1][0]*dt1.vertices[1][0] + dt1.vertices[1][1]*dt1.vertices[1][1];//Sii
		row[1] += dt1.vertices[2][0]*dt1.vertices[2][0] + dt1.vertices[2][1]*dt1.vertices[2][1];
		//T2
		row[0] += dt2.vertices[0][0]*dt2.vertices[0][0] + dt2.vertices[0][1]*dt2.vertices[0][1];
		row[2] += dt2.vertices[1][0]*dt2.vertices[1][0] + dt2.vertices[1][1]*dt2.vertices[1][1];
		row[3] += dt2.vertices[2][0]*dt2.vertices[2][0] + dt2.vertices[2][1]*dt2.vertices[2][1];//Sii
		//T3
		row[1] += dt3.vertices[0][0]*dt3.vertices[0][0] + dt3.vertices[0][1]*dt3.vertices[0][1];
		row[3] += dt3.vertices[1][0]*dt3.vertices[1][0] + dt3.vertices[1][1]*dt3.vertices[1][1];//Sii
		row[4] += dt3.vertices[2][0]*dt3.vertices[2][0] + dt3.vertices[2][1]*dt3.vertices[2][1];
		//T4
		row[2] += dt4.vertices[0][0]*dt4.vertices[0][0] + dt4.vertices[0][1]*dt4.vertices[0][1];
		row[5] += dt4.vertices[1][0]*dt4.vertices[1][0] + dt4.vertices[1][1]*dt4.vertices[1][1];
		row[3] += dt4.vertices[2][0]*dt4.vertices[2][0] + dt4.vertices[2][1]*dt4.vertices[2][1];//Sii
		//T5
		row[3] += dt5.vertices[0][0]*dt5.vertices[0][0] + dt5.vertices[0][1]*dt5.vertices[0][1];//Sii
		row[6] += dt5.vertices[1][0]*dt5.vertices[1][0] + dt5.vertices[1][1]*dt5.vertices[1][1];
		row[4] += dt5.vertices[2][0]*dt5.vertices[2][0] + dt5.vertices[2][1]*dt5.vertices[2][1];
		//T6
		row[3] += dt6.vertices[0][0]*dt6.vertices[0][0] + dt6.vertices[0][1]*dt6.vertices[0][1];//Sii
		row[5] += dt6.vertices[1][0]*dt6.vertices[1][0] + dt6.vertices[1][1]*dt6.vertices[1][1];
		row[6] += dt6.vertices[2][0]*dt6.vertices[2][0] + dt6.vertices[2][1]*dt6.vertices[2][1];
	}
	for(uint32 i = 0; i < 7; ++i)
		row[i] /= AreaTriangle;

	const uint32 ExpectedEntries = 7*VertexSize - 2 - 2*(VertexCount) - 2*(VertexCount+1);
	SparseMatrix<Number> A(VertexSize, VertexSize, ExpectedEntries);
	std::cout << "Assembling matrix... (" << ExpectedEntries << " entries expected)" << std::endl;
	std::cout << "  Single Row: [... " << row[0] << " , "  << row[1]
			<< " ... " << row[2] << " , " << row[3] << " , " << row[4] << " ... " 
			<< row[5] << " , " << row[6] << " ...]" << std::endl;

	for(uint32 i = 0; i < VertexSize; ++i)
	{
		if(i >= VertexCount+1)
			A.set(i, i-VertexCount-1, row[0]);

		if(i >= VertexCount)
			A.set(i, i-VertexCount, row[1]);

		if(i >= 1)
			A.set(i, i-1, row[2]);

		A.set(i,i, row[3]);

		if(i+1 < VertexSize)
			A.set(i, i+1, row[4]);

		if(i+VertexCount < VertexSize)
			A.set(i, i+VertexCount, row[5]);
		
		if(i+VertexCount+1 < VertexSize)
			A.set(i, i+VertexCount+1, row[6]);
	}
	std::cout << "  Entries: " << A.filledCount() 
				<< " (Efficiency: " << 100*(1-A.filledCount()/(float)A.size()) << "%)" << std::endl;

	// --------------------------------
	std::cout << "Assembling right-hand side..." << std::endl;
	DynamicVector<Number> B(VertexSize);
	for(uint32 xi = 0; xi < VertexCount; ++xi)
	{
		const float bc = 0.33333f*AreaTriangle*std::sin(NS_PI_F * xi * H[0]);
		
		uint32 factor = 1;
		if(xi>0)
			factor++;
		if(xi<VertexCount-1)
			factor++;
		
		for(uint32 yi = 0; yi < VertexCount; ++yi)
		{
			uint32 factor2 = factor;
			if(yi>0)
			{
				factor2++;
				if(xi>0)
					factor2++;
			}

			if(yi<VertexCount-1)
			{
				factor2++;
				if(xi<VertexCount-1)
					factor2++;
			}

			B.set(yi*VertexCount+xi, factor2*bc*std::sin(NS_PI_F * yi * H[1]));
		}
	}

	// --------------------------------
	std::cout << "Calculating..." << std::endl;

	uint32 iterations = 0;
	DynamicVector<Number> X(VertexSize);

	auto p2_start = std::chrono::high_resolution_clock::now();
#ifdef USE_CG
	X = CG::serial::cg<SparseMatrix, Number>(A, B, X, 1024, 1e-4, &iterations);
#else
	X = Iterative::serial::sor<SparseMatrix, Number>(A, B, X, RELAX_PAR, 1024, 1e-4, &iterations);
#endif
	auto p2_diff = std::chrono::high_resolution_clock::now() - p2_start;

	std::cout << iterations << " Iterations ["
		<< std::chrono::duration_cast<std::chrono::seconds>(p2_diff).count()
		<< " s]" << std::endl;

	// --------------------------------
	// Writing output
	std::ofstream data("poisson_fem.dat");
	for (Index y = 0; y < VertexCount; ++y)
	{
		for (Index x = 0; x < VertexCount; ++x)
			data << X.at(y*VertexCount+x) << " ";
		data << std::endl;
	}
	data.close();

	// Cleanup
	//std::cout << A << std::endl;

	return 0;
}
