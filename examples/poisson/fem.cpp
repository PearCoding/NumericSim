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

#include "sf/PolyShapeFunction.h"

NS_USE_NAMESPACE;

/*
 * Undef if you want to use the iterative SOR algorithm
 */
//#define USE_CG

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
	Mesh::Mesh<Number,2> mesh = Mesh::HyperCube<Number,2>::generate(
		Vector2D<Dimension>{(Dimension)N,(Dimension)N},
		Vector2D<Number>{DomainDistance[0], DomainDistance[1]},
		Vector2D<Number>{DomainStart[0],DomainStart[1]}
	);

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
	SparseMatrix<Number> A(VertexSize, VertexSize, ExpectedEntries);

	std::cout << "Assembling matrix... (" << ExpectedEntries << " entries expected)" << std::endl;

	const auto p1_start = std::chrono::high_resolution_clock::now();

	SF::PolyShapeFunction<Number,2,1> sf;
	const auto d0 = sf.gradient(0,FixedVector<Number,2>{0,0});
	const auto d1 = sf.gradient(1,FixedVector<Number,2>{0,0});
	const auto d2 = sf.gradient(2,FixedVector<Number,2>{0,0});

	std::cout << d0 << std::endl;
	std::cout << d1 << std::endl;
	std::cout << d2 << std::endl;
	
	// Cell based assembling
	for(Mesh::MeshElement<Number,2>* element : mesh.elements())
	{
		const Number integral = Simplex<Number,2>::unitVolume() * std::abs(element->Element.determinant());
		const auto mat = element->Element.gradient();
		const FixedVector<Number,2> delta[3] = {mat.mul(d0), mat.mul(d1), mat.mul(d2)};

		for(Index i = 0; i < 3; ++i)
		{
			const Index globalI = element->Vertices[i]->GlobalIndex;

			for(Index j = 0; j < 3; ++j)
			{
				const Index globalJ = element->Vertices[j]->GlobalIndex;
				const Number prev = A.at(globalI, globalJ);

				A.set(globalI, globalJ,
					prev + integral * delta[i].dot(delta[j]));
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

	// Vertex based simple quadrature for linear shape function
	for(Mesh::MeshVertex<Number,2>* v : mesh.vertices())
	{
		Number val = boundary_function(v->Vertex)/3;
		Number f = 0;

		for(Mesh::MeshElement<Number,2>* e : v->Elements)
			f += e->Element.volume();

		B.set(v->GlobalIndex, val*f);
	}

	const auto p1_diff = std::chrono::high_resolution_clock::now() - p1_start;
	std::cout << "Assembling took " 
		<< std::chrono::duration_cast<std::chrono::seconds>(p1_diff).count()
		<< " s" << std::endl;

	// --------------------------------
	std::cout << "Calculating..." << std::endl;

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

	return 0;
}
