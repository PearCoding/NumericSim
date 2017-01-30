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
#include "loader/MeshObjLoader.h"
#include "loader/MeshTriangleLoader.h"

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
//#define USE_CG

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

// Definition

const auto source_function = [](const FixedVector<Number,2>& v) -> Number {
	return std::sin(NS_PI * v[0])*std::sin(NS_PI * v[1]);
};

const auto boundary_function = [](const FixedVector<Number,2>& v) -> Number {
	return 0;
};

template<int Order>
void handleMesh(Mesh<Number, 2>& mesh, int M)
{
	typedef PolyShapeFunction<Number,2,Order> SF;
	SF::prepareMesh(mesh);

	const size_t VertexSize = mesh.vertices().size();
	std::cout << "  Order = " << Order << std::endl;
	std::cout << "  DOFs = " << VertexSize << std::endl;
	std::cout << "  Elements = " << mesh.elements().size() << std::endl;

	const int32 MatrixSize = VertexSize*VertexSize;
	std::cout << "  Matrix size = " << VertexSize << "x" << VertexSize << "  (" << MatrixSize << ")" << std::endl;

	// --------------------------------
	// Assembly matrix
	SparseMatrix<Number> A(VertexSize, VertexSize);
	DynamicVector<Number> B(VertexSize);

	std::cout << "Assembling matrix and righthand side..." << std::endl;

	const auto p1_start = std::chrono::high_resolution_clock::now();

	//Here each element has the same shape function (linear or second order)
	SF sf;
	GaussLegendreQuadrature<Number,2,Order+1> quadrature;

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

		for(Index i = 0; i < SF::DOF; ++i)
		{
			const Index globalI = element->DOFVertices[i]->GlobalIndex;

			for(Index j = 0; j < SF::DOF; ++j)
			{
				const Index globalJ = element->DOFVertices[j]->GlobalIndex;
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

	std::cout << "  Entries: " << A.filled_count() 
				<< " (Efficiency: " << 100*(1-A.filled_count()/(float)A.size()) << "%)" << std::endl;

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
		//const Number det = std::abs(elem->Element.determinant());
		const Number H = elem->Element.diameter();

		const Number cellError = 0;/*det * quadrature.eval(
			[&](const FixedVector<Number,2>& local) -> Number
			{
				return source_function(element->Element.toGlobal(local));
			});*/
		
		Number faceError = 0;
		for(Index i = 0; i < 3; ++i)
		{
			// Directional finite difference
			FixedVector<Number, 2> u1;
			u1[0] = (X.at(elem->Neighbors[i]->Vertices[0]->GlobalIndex) - X.at(elem->Vertices[i]->GlobalIndex)) /
				(elem->Neighbors[i]->Vertices[0]->Vertex-elem->Vertices[i]->Vertex).mag();
			u1[1] = (X.at(elem->Neighbors[i]->Vertices[1]->GlobalIndex) - X.at(elem->Vertices[i]->GlobalIndex)) /
				(elem->Neighbors[i]->Vertices[1]->Vertex-elem->Vertices[i]->Vertex).mag();
			faceError += (elem->Neighbors[i]->Vertices[0]->Vertex-elem->Neighbors[i]->Vertices[1]->Vertex).mag() *
				//std::pow(elem->Neighbors[i]->Elements[0]->Element.faceNormal(i).dot(u1), 2);
				u1.magSqr();

		}
		const Number nk = H*H*cellError + 0.5*faceError;

		PostError.set(k, PostErrorFactor * nk);
		k++;
	}

	if(M == 0)
		std::cout << "  Exact Error |u-u_h| = " << ExactError.max() << " (Maximum Norm)" << std::endl;

	std::cout << "  Post Error |grad(e)| <= " << PostError.max() << " (Maximum Norm)" << std::endl;

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

	std::map<std::string, DynamicVector<Number>*> pointData;
	pointData["Results"] = &X;
	if(M == 0)
		pointData["ExactError"] = &ExactError;

	std::map<std::string, DynamicVector<Number>*> cellData;
	cellData["PostError"] = &PostError;

	VTKExporter<Number,2>::write<DynamicVector<Number> >("poisson_fem.vtu", mesh, pointData, cellData,
		VOO_ElementDeterminant |
		VOO_ElementMatrix |
		VOO_VertexBoundaryLabel |
		VOO_VertexImplicitLabel |
		(Order == 2 ? VOO_IsQuadratic : 0));
}

/**
 * Main entry
 */
int main(int argc, char** argv)
{
	if(argc < 3)
	{
		std::cout << "Not enough arguments given! Use 'example_poisson_fem O M <DEPENDS ON M>'" << std::endl;
		return -1;
	}

	int Order = 1;// Order (1 or 2)
	int32 M = 0;// M == 0 -> Generic mesh; Everything else are precomputed meshes
	try
	{
		Order = std::stol(argv[1]);
		M = std::stol(argv[2]);
	}
	catch(...)
	{
		std::cout << "Invalid arguments given. " << std::endl;
		return -2;
	}

	if(M < 0 || M > 2)
	{
		std::cout << "Invalid M given. Should be zero for generic grid mesh, one for obj mesh and two for triangle .node and .ele mesh" << std::endl;
		return -4;
	}

	// --------------------------------
	// Calculating basic constants
	Mesh<Number,2> mesh;
	
	const auto p0_start = std::chrono::high_resolution_clock::now();
	if(M == 0)
	{
		if(argc != 4)
		{
			std::cout << "Need additional N!" << std::endl;
			return -3;
		}

		int32 N = std::stol(argv[3]);
		if(N < 1)
		{
			std::cout << "Invalid N given. Should be greater than 1" << std::endl;
			return -3;
		}

		std::cout << "Generating mesh with " << N << "x" << N << " rectangles..." << std::endl;
		mesh = HyperCube<Number,2>::generate(
			Vector2D<Dimension>{(Dimension)N,(Dimension)N},
			Vector2D<Number>{DomainDistance[0], DomainDistance[1]},
			Vector2D<Number>{DomainStart[0],DomainStart[1]}
		);
	}
	else if(M == 1)
	{
		if(argc != 4)
		{
			std::cout << "Need additional filename!" << std::endl;
			return -3;
		}
		std::cout << "Loading mesh " << argv[3] << "..." << std::endl;
		mesh = MeshObjLoader<Number,2>::loadFile(argv[3]);
		mesh.setupBoundaries();
	}
	else if(M == 2)
	{
		if(argc != 5)
		{
			std::cout << "Need additional paths to .node and .ele files! (First .node, than .ele)" << std::endl;
			return -3;
		}
		std::cout << "Loading mesh " << argv[3] << " and " << argv[4] << " ..." << std::endl;
		mesh = MeshTriangleLoader<Number>::loadFile(argv[3], argv[4]);
		mesh.setupBoundaries();
	}

	const auto p0_diff = std::chrono::high_resolution_clock::now() - p0_start;
	std::cout << "Mesh setup took " 
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

	if(Order == 2)
		handleMesh<2>(mesh, M);
	else
		handleMesh<1>(mesh, M);

	std::cout << "Finished!" << std::endl;
	return 0;
}
