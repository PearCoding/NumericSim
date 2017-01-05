#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include "matrix/SparseMatrix.h"
#include "matrix/MatrixOperations.h"
#include "Iterative.h"
#include "CG.h"
#include "Vector.h"
#include "Simplex.h"
#include "OutputStream.h"

#include "mesh/HyperCube.h"
#include "mesh/Mesh.h"
#include "mesh/MeshObjLoader.h"

#include "sf/PolyShapeFunction.h"

#include "quadrature/Quadrature.h"

#include "export/VTKExporter.h"

NS_USE_NAMESPACE;

/**
 * Source Code is optimized for 2d calculation,
 * but has already support n-dimensional calculation, when possible to implement easily.
 */
 
/*
 * Undef if you want to use the iterative SOR algorithm
 */
#define USE_CG

/*
 * Uncomment to produce many verbose debug information.
 * Attention: Very slow!
 */
//#define VERBOSE_LOG

/*
 * Uncomment to enable experimental matrix condition improvement.
 * Still merges to the right solution.
 */
#define AVG_MID_STRONG_BOUNDARY

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

typedef double Number;

constexpr float DomainStart[2]    = {0,0};
constexpr float DomainEnd[2]      = {1,1};
constexpr float DomainDistance[2] = {DomainEnd[0]-DomainStart[0],
									 DomainEnd[1]-DomainStart[1]};

constexpr double RELAX_PAR = 1.2;// SOR weight parameter (0,2]

constexpr Number EPS = 1e-8;

constexpr Number C = 0;// Constant number to add diagonally to sparse matrix A to achieve better conditions
constexpr Number PostErrorFactor = 1;// Propotional factor, often written as C in equations

constexpr Dimension ShapeFunctionOrder = 1;// Currently only first order implemented
constexpr Dimension QuadratureOrder = ShapeFunctionOrder + 1;

//const char* MESH_0 = -> Grid
const char* MESH_1 = 
	#include "circle.obj.inl"
	;

const char* MESH_2 = 
	#include "triangles.obj.inl"
	;

const char* MESH_3 = 
	#include "half_circle.obj.inl"
	;

// Definition

const auto source_function = [](const FixedVector<Number,2>& v) -> Number {
	return std::sin(NS_PI * v[0])*std::sin(NS_PI * v[1]);
};

const auto boundary_function = [](const FixedVector<Number,2>& v) -> Number {
	return 0;
};

/**
 * Main entry
 */
int main(int argc, char** argv)
{
	if(argc != 2 && argc != 3)
	{
		std::cout << "Not enough arguments given! Use 'example_poisson_fem M N'" << std::endl;
		return -1;
	}

	int32 M = 0;// M == 0 -> Generic mesh; Everything else are precomputed meshes
	int32 N = 0;// Mesh size in M == 0
	try
	{
		M = std::stol(argv[1]);
		if(argc == 3)
			N = std::stol(argv[2]);
	}
	catch(...)
	{
		std::cout << "Invalid arguments given. " << std::endl;
		return -2;
	}

	if(M == 0 && N < 2)
	{
		std::cout << "Invalid N given. Should be greater than 2" << std::endl;
		return -3;
	}

	if(M < 0 || M > 3)
	{
		std::cout << "Invalid M given. Should be one for generic grid mesh and 1, 2 or 3 for precomputed mesh 1, 2 or 3" << std::endl;
		return -4;
	}

	// --------------------------------
	// Calculating basic constants
	Mesh<Number,2> mesh;
	
	const auto p0_start = std::chrono::high_resolution_clock::now();
	if(M == 0)
	{
		std::cout << "Generating mesh with " << N << "x" << N << " rectangles..." << std::endl;
		mesh = HyperCube<Number,2>::generate(
			Vector2D<Dimension>{(Dimension)N,(Dimension)N},
			Vector2D<Number>{DomainDistance[0], DomainDistance[1]},
			Vector2D<Number>{DomainStart[0],DomainStart[1]}
		);
	}
	else if(M == 1)
	{
		std::cout << "Loading mesh 1..." << std::endl;
		mesh = MeshObjLoader<Number,2>::loadString(MESH_1);
	}
	else if(M == 2)
	{
		std::cout << "Loading mesh 2..." << std::endl;
		mesh = MeshObjLoader<Number,2>::loadString(MESH_2);
	}
	else if(M == 3)
	{
		std::cout << "Loading mesh 3..." << std::endl;
		mesh = MeshObjLoader<Number,2>::loadString(MESH_3);
	}

	const auto p0_diff = std::chrono::high_resolution_clock::now() - p0_start;
	std::cout << "Full generation took " 
		<< std::chrono::duration_cast<std::chrono::seconds>(p0_diff).count()
		<< " s" << std::endl;

	// --------------------------------
	std::cout << "Preparing elements..." << std::endl;
	mesh.prepare();

	try
	{
		mesh.validate();
	}
	catch(const NSException& e)
	{
		std::cout << "  Couldn't generate mesh: " << e.what() << std::endl;
		return -1;
	}

	const int32 VertexCount = N+1;
	const size_t VertexSize = mesh.vertices().size();
	std::cout << "  Vertices = " << VertexSize << std::endl;
	std::cout << "  Elements = " << mesh.elements().size() << std::endl;

	const int32 MatrixSize = VertexSize*VertexSize;
	std::cout << "  Matrix size = " << VertexSize << "x" << VertexSize << "  (" << MatrixSize << ")" << std::endl;

	// --------------------------------
	// Assembly matrix
	const uint32 ExpectedEntries = 6*(N-1)*(N-1) + 4*2*(N-1) + 4*2*(N-1) + 2*2 + 3*2;
	SparseMatrix<Number> A(VertexSize, VertexSize, M == 0 ? ExpectedEntries : 0);
	DynamicVector<Number> B(VertexSize);

	std::cout << "Assembling matrix and righthand side...";
	if(M == 0)
		std::cout << " (" << ExpectedEntries << " entries expected)";
	
	std::cout << std::endl;

	const auto p1_start = std::chrono::high_resolution_clock::now();

	//Here each element has the same shape function (linear or second order)
	typedef PolyShapeFunction<Number,2,ShapeFunctionOrder> SF;
	SF sf;
	GaussLegendreQuadrature<Number,2,QuadratureOrder> quadrature;

	// Cell based assembling
	/*
	 * Potential optimization would be to calculate the shape function gradient value once
	 * for the reference simplex and multiply it only with the element dependent inverse jacobian.
	 */
	for(MeshElement<Number,2>* element : mesh.elements())
	{
		const Number det = std::abs(element->Element.determinant());
		
		FixedMatrix<Number,SF::DOF,SF::DOF> elemMat;
		FixedVector<Number,SF::DOF> elemVec;

		const auto invJacob = element->Element.inverseMatrix();

#ifdef VERBOSE_LOG
		std::cout << invJacob << std::endl;
#endif

		for(Index i = 0; i < SF::DOF; ++i)
		{
			for(Index j = 0; j < SF::DOF; ++j)
			{
				const Number val = det * quadrature.eval(
					[&](const FixedVector<Number,2>& local) -> Number
					{
						return (invJacob.mul(sf.gradient(i,local))).
							dot(invJacob.mul(sf.gradient(j,local)));
					});

				if(std::abs(val) > EPS)
					elemMat.set(i,j, val);
			}

			const Number f = det * quadrature.eval(
				[&](const FixedVector<Number,2>& local) -> Number
				{
#ifdef VERBOSE_LOG
					std::cout << local << " " << element->Element.toGlobal(local) << "; ";
#endif
					return sf.value(i, local) * source_function(element->Element.toGlobal(local));
				});
#ifdef VERBOSE_LOG
			std::cout << std::endl;
#endif
			if(std::abs(f) > EPS)
				elemVec.set(i, f);
		}

#ifdef VERBOSE_LOG
		std::cout << elemMat << std::endl;
#endif

		// TODO: DOF/shape function based mapping
		for(Index i = 0; i < SF::DOF; ++i)
		{
			const Index globalI = element->Vertices[i]->GlobalIndex;

			for(Index j = 0; j < SF::DOF; ++j)
			{
				const Index globalJ = element->Vertices[j]->GlobalIndex;
				A.set(globalI, globalJ, A.at(globalI, globalJ) + elemMat.at(i,j));
			}

			B.set(globalI, B.at(globalI) + elemVec.at(i));
		}
	}

	// Mid number for better matrix condition
#ifdef AVG_MID_STRONG_BOUNDARY
	Number avgMid = 0;
	for(MeshVertex<Number,2>* vertex : mesh.vertices())
		avgMid += std::abs(A.at(vertex->GlobalIndex, vertex->GlobalIndex));
	avgMid /= mesh.vertices().size();
#else
	constexpr Number avgMid = 1;
#endif

	// Setup boundary constraints
	for(MeshVertex<Number,2>* vertex : mesh.vertices())
	{
		if(!(vertex->Flags & MVF_StrongBoundary))
			continue;

		const Number g = boundary_function(vertex->Vertex);

		for(Index i = 0; i < VertexSize; ++i)
		{
			B.set(i, B.at(i) - A.at(i, vertex->GlobalIndex)*g);

			A.set(vertex->GlobalIndex, i, 0);
			A.set(i, vertex->GlobalIndex, 0);
		}

		A.set(vertex->GlobalIndex, vertex->GlobalIndex, avgMid);
		B.set(vertex->GlobalIndex, avgMid*g);
	}

	std::cout << "  Entries: " << A.filledCount() 
				<< " (Efficiency: " << 100*(1-A.filledCount()/(float)A.size()) << "%)" << std::endl;

	const auto p1_diff = std::chrono::high_resolution_clock::now() - p1_start;
	std::cout << "Full assembling took " 
		<< std::chrono::duration_cast<std::chrono::seconds>(p1_diff).count()
		<< " s" << std::endl;

	// --------------------------------
	std::cout << "Calculating..." << std::endl;

	if(std::abs(C) > 0)
	{
		for(Index i = 0; i < A.rows(); ++i)
			A.set(i,i,A.at(i,i)+C);
	}

	std::cout << "  cond(A)=" << Operations::cond(A) << std::endl;

	size_t iterations = 0;
	DynamicVector<Number> X(VertexSize);

	const auto p2_start = std::chrono::high_resolution_clock::now();
#ifdef USE_CG
	X = CG::serial::cg(A, B, X, 1024, 1e-4, &iterations);
#else
	X = Iterative::serial::sor(A, B, X, RELAX_PAR, 1024, 1e-4, &iterations);
#endif
	const auto p2_diff = std::chrono::high_resolution_clock::now() - p2_start;

	std::cout << "  " << iterations << " Iterations" << std::endl;
	std::cout << "Full calculation took " 
		<< std::chrono::duration_cast<std::chrono::seconds>(p2_diff).count()
		<< " s" << std::endl;

	// --------------------------------
	std::cout << "Estimating Error..." << std::endl;
	DynamicVector<Number> ExactError(VertexSize);

	if(M == 0)// We know the analytical solution!
	{
		const auto solution = [](const FixedVector<Number,2>& v) {
			return std::sin(NS_PI * v[0]) * std::sin(NS_PI * v[1]) / (2*NS_PI*NS_PI); };

		for(MeshVertex<Number,2>* v : mesh.vertices())
		{
			Number u = solution(v->Vertex);
			Number uh = X.at(v->GlobalIndex);
			ExactError.set(v->GlobalIndex, std::abs(u-uh));
		}
	}

	DynamicVector<Number> PostError(mesh.elements().size());
	Index k = 0;
	for(MeshElement<Number,2>* elem : mesh.elements())
	{
		const Number det = std::abs(elem->Element.determinant());
		const Number H = elem->Element.outerRadius();

		const Number cellError = 0;/*det * quadrature.eval(
			[&](const FixedVector<Number,2>& local) -> Number
			{
				return source_function(element->Element.toGlobal(local));
			});*/
		
		Number faceError = 0;
		for(Index i = 0; i < 3; ++i)
		{
			faceError += det* quadrature.eval(
			[&](const FixedVector<Number,2>& local) -> Number
			{
				return elem->Element.faceNormal(i).dot(elem->Element.gradient(i));
			});
		}
		const Number nk = H*cellError + 0.5*std::sqrt(H)*faceError;

		PostError.set(k, PostErrorFactor * nk);
		k++;
	}

	if(M == 0)
		std::cout << "  Exact Error |u-u_h| = " << ExactError.max() << " (Maximum Norm)" << std::endl;

	Number fullPostError = 0;
	for (auto v : PostError)
		fullPostError += v*v;
	std::cout << "  Post Error |grad(e)| <= " << std::sqrt(fullPostError) << " (L2 Norm)" << std::endl;

	// --------------------------------
	std::cout << "Generating Output..." << std::endl;
	{
		std::ofstream data("poisson_fem_data.dat");
		for (MeshVertex<Number,2>* vertex : mesh.vertices())
		{
			data << vertex->Vertex[0] << " " 
				<< vertex->Vertex[1] << " " 
				<< X.at(vertex->GlobalIndex) << std::endl;
		}
		data.close();
	}

	{
		std::ofstream data("poisson_fem_mesh.dat");
		for (MeshElement<Number,2>* elem : mesh.elements())
		{
			data << (elem->Vertices[0]->GlobalIndex+1) << " " 
				<< (elem->Vertices[1]->GlobalIndex+1) << " " 
				<< (elem->Vertices[2]->GlobalIndex+1) << std::endl;
		}
		data.close();
	}

	{
		std::ofstream data("poisson_fem_A.dat");
		for (SparseMatrixIterator<Number> it = A.begin(); it != A.end(); ++it)
		{
			data << (it.row()+1) << " " 
				<< (it.column()+1) << " " 
				<< *it << std::endl;
		}
		data.close();
	}

	{
		std::ofstream data("poisson_fem_B.dat");
		for (auto v : B)
			data << v << std::endl;
		data.close();
	}

	{
		std::ofstream data("poisson_fem_ExactError.dat");
		for (auto v : ExactError)
			data << v << std::endl;
		data.close();
	}

	{
		std::ofstream data("poisson_fem_PostError.dat");
		for (auto v : PostError)
			data << v << std::endl;
		data.close();
	}

	VTKExporter<Number,2>::write<DynamicVector<Number> >("poisson_fem.vtu", mesh, X, &PostError,
		VOO_ElementDeterminant |
		VOO_ElementMatrix);

	std::cout << "Finished!" << std::endl;
	return 0;
}
