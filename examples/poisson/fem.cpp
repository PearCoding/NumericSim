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

#include "mesh/HyperCube.h"
#include "mesh/Mesh.h"
#include "mesh/MeshObjLoader.h"

#include "sf/PolyShapeFunction.h"

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

constexpr Number C = 1;// Constant number to add diagonally to sparse matrix A to achieve better conditions

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

	int32 M = 0;
	int32 N = 0;
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
	Mesh::Mesh<Number,2> mesh;
	
	if(M == 0)
	{
		std::cout << "Generating mesh with " << N << "x" << N << " rectangles..." << std::endl;
		mesh = Mesh::HyperCube<Number,2>::generate(
			Vector2D<Dimension>{(Dimension)N,(Dimension)N},
			Vector2D<Number>{DomainDistance[0], DomainDistance[1]},
			Vector2D<Number>{DomainStart[0],DomainStart[1]}
		);
	}
	else if(M == 1)
	{
		std::cout << "Loading mesh 1..." << std::endl;
		mesh = Mesh::MeshObjLoader<Number,2>::loadString(MESH_1);
	}
	else if(M == 2)
	{
		std::cout << "Loading mesh 2..." << std::endl;
		mesh = Mesh::MeshObjLoader<Number,2>::loadString(MESH_2);
	}
	else if(M == 3)
	{
		std::cout << "Loading mesh 3..." << std::endl;
		mesh = Mesh::MeshObjLoader<Number,2>::loadString(MESH_3);
	}

	std::cout << "  Preparing elements..." << std::endl;
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

	std::cout << "Assembling matrix...";
	if(M == 0)
		std::cout << " (" << ExpectedEntries << " entries expected)";
	
	std::cout << std::endl;

	const auto p1_start = std::chrono::high_resolution_clock::now();

	//Here each element has the same shape function (linear or second order)
	SF::PolyShapeFunction<Number,2,1> sf;
	const auto d0 = sf.gradient(0,FixedVector<Number,2>{0,0});
	const auto d1 = sf.gradient(1,FixedVector<Number,2>{0,0});
	const auto d2 = sf.gradient(2,FixedVector<Number,2>{0,0});

#ifdef VERBOSE_LOG
	std::cout << d0 << std::endl;
	std::cout << d1 << std::endl;
	std::cout << d2 << std::endl;
#endif
	
	// Cell based assembling (Not optimized due to better understanding)
	for(Mesh::MeshElement<Number,2>* element : mesh.elements())
	{
		const Number integral = Simplex<Number,2>::unitVolume() * std::abs(element->Element.determinant());
		const auto mat = element->Element.gradient();
		const FixedVector<Number,2> delta[3] = {mat.mul(d0), mat.mul(d1), mat.mul(d2)};

		FixedMatrix<Number,3,3> elemMat;

#ifdef VERBOSE_LOG
		std::cout << mat << std::endl;
#endif

		for(Index i = 0; i < 3; ++i)
		{
			for(Index j = 0; j < 3; ++j)
			{
				const auto val = integral * delta[i].dot(delta[j]);
				if(std::abs(val) > EPS)
					elemMat.set(i,j, val);
			}
		}

#ifdef VERBOSE_LOG
		std::cout << elemMat << std::endl;
#endif
		for(Index i = 0; i < 3; ++i)
		{
			const Index globalI = element->Vertices[i]->GlobalIndex;

			for(Index j = 0; j < 3; ++j)
			{
				const Index globalJ = element->Vertices[j]->GlobalIndex;
				const Number prev = A.at(globalI, globalJ);
				A.set(globalI, globalJ,
					prev + elemMat.at(i,j));
			}
		}
	}

	std::cout << "  Entries: " << A.filledCount() 
				<< " (Efficiency: " << 100*(1-A.filledCount()/(float)A.size()) << "%)" << std::endl;

	// --------------------------------
	std::cout << "Assembling right-hand side..." << std::endl;
	DynamicVector<Number> B(VertexSize);
	
	const auto boundary_function = [](const FixedVector<Number,2>& v) {
		return std::sin(NS_PI_F * v[0])*std::sin(NS_PI_F * v[1]); };

	//constexpr Number unit_int = 140737488355328/(Number)2778046668940015;//2/(NS_PI_F*NS_PI_F);
	// Vertex based simple quadrature for linear shape function
	for(Mesh::MeshVertex<Number,2>* v : mesh.vertices())
	{
		Number val = boundary_function(v->Vertex)/3;
		if(std::abs(val) <= EPS)
			continue;
		Number f = 0;

		for(Mesh::MeshElement<Number,2>* e : v->Elements)
			f += e->Element.volume();//std::abs(e->Element.determinant());

		B.set(v->GlobalIndex, val*f);
	}

	const auto p1_diff = std::chrono::high_resolution_clock::now() - p1_start;
	std::cout << "Assembling took " 
		<< std::chrono::duration_cast<std::chrono::seconds>(p1_diff).count()
		<< " s" << std::endl;

	// --------------------------------
	std::cout << "Calculating..." << std::endl;

	if(C > 0)
	{
		for(Index i = 0; i < A.rows(); ++i)
			A.set(i,i,A.at(i,i)+C);
	}

	size_t iterations = 0;
	DynamicVector<Number> X(VertexSize);

	const auto p2_start = std::chrono::high_resolution_clock::now();
#ifdef USE_CG
	X = CG::serial::cg(A, B, X, 1024, 1e-4, &iterations);
#else
	X = Iterative::serial::sor(A, B, X, RELAX_PAR, 1024, 1e-4, &iterations);
#endif
	const auto p2_diff = std::chrono::high_resolution_clock::now() - p2_start;

	std::cout << "  " << iterations << " Iterations ["
		<< std::chrono::duration_cast<std::chrono::seconds>(p2_diff).count()
		<< " s]" << std::endl;

	// --------------------------------
	// Writing output
	{
		std::ofstream data("poisson_fem_data.dat");
		for (Mesh::MeshVertex<Number,2>* vertex : mesh.vertices())
		{
			data << vertex->Vertex[0] << " " 
				<< vertex->Vertex[1] << " " 
				<< X.at(vertex->GlobalIndex) << std::endl;
		}
		data.close();
	}

	{
		std::ofstream data("poisson_fem_mesh.dat");
		for (Mesh::MeshElement<Number,2>* elem : mesh.elements())
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
		{
			data << v << std::endl;
		}
		data.close();
	}

	Export::VTKExporter<Number,2>::write("poisson_fem.vtu", mesh, X);

	return 0;
}
