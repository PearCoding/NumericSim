#define NS_ALLOW_CHECKS

#include "Test.h"
#include "mesh/Mesh.h"
#include "sf/PolyShapeFunction.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

template<typename T>
NS_BEGIN_TESTCASE_T1(PolySF)
NS_TEST("Order1")
{
	try
	{
		FixedVector<T,2> x0 = {0,0};
		FixedVector<T,2> x1 = {1,1};
		FixedVector<T,2> x2 = {0,1};

		Mesh<T,2> mesh;
		mesh.addVertex(new MeshVertex<T,2>(x0));
		mesh.addVertex(new MeshVertex<T,2>(x1));
		mesh.addVertex(new MeshVertex<T,2>(x2));

		MeshElement<T,2>* elem = new MeshElement<T,2>();
		elem->Vertices[0] = mesh.vertex(0);
		elem->Vertices[1] = mesh.vertex(1);
		elem->Vertices[2] = mesh.vertex(2);
		mesh.addElement(elem);

		mesh.setupNeighbors();
		mesh.setupBoundaries();
		
		mesh.prepare();

		PolyShapeFunction<T,2,1>::prepareMesh(mesh);

		mesh.validate();

		NS_CHECK_EQ(mesh.vertices().size(), 3);
		NS_CHECK_EQ(mesh.elements().size(), 1);

		NS_CHECK_EQ(elem->DOFVertices[0]->Vertex,x0); 
		NS_CHECK_EQ(elem->DOFVertices[1]->Vertex,x1); 
		NS_CHECK_EQ(elem->DOFVertices[2]->Vertex,x2); 

		PolyShapeFunction<T,2,1> sf;
		NS_CHECK_EQ(sf.value(0, elem->Element.toLocal(x0)), 1);
		NS_CHECK_EQ(sf.value(1, elem->Element.toLocal(x1)), 1);
		NS_CHECK_EQ(sf.value(2, elem->Element.toLocal(x2)), 1);

		NS_CHECK_EQ(sf.value(0, elem->Element.toLocal(x1)), 0);
		NS_CHECK_EQ(sf.value(1, elem->Element.toLocal(x2)), 0);
		NS_CHECK_EQ(sf.value(2, elem->Element.toLocal(x0)), 0);
	}
	catch (const NSException& exception)
	{
		NS_GOT_EXCEPTION(exception);
	}
}
NS_TEST("Order2")
{
	try
	{
		FixedVector<T,2> x0 = {0,0};
		FixedVector<T,2> x1 = {1,1};
		FixedVector<T,2> x2 = {0,1};
		FixedVector<T,2> x3 = {0.5,0.5};
		FixedVector<T,2> x4 = {0.5,1};
		FixedVector<T,2> x5 = {0,0.5};

		Mesh<T,2> mesh;
		mesh.addVertex(new MeshVertex<T,2>(x0));
		mesh.addVertex(new MeshVertex<T,2>(x1));
		mesh.addVertex(new MeshVertex<T,2>(x2));

		MeshElement<T,2>* elem = new MeshElement<T,2>();
		elem->Vertices[0] = mesh.vertex(0);
		elem->Vertices[1] = mesh.vertex(1);
		elem->Vertices[2] = mesh.vertex(2);
		mesh.addElement(elem);

		mesh.setupNeighbors();
		mesh.setupBoundaries();
		mesh.prepare();

		PolyShapeFunction<T,2,2>::prepareMesh(mesh);
		
		mesh.validate();

		NS_CHECK_EQ(mesh.vertices().size(), 6);
		NS_CHECK_EQ(mesh.elements().size(), 1);

		NS_CHECK_EQ(elem->DOFVertices[0]->Vertex,x0); 
		NS_CHECK_EQ(elem->DOFVertices[1]->Vertex,x1); 
		NS_CHECK_EQ(elem->DOFVertices[2]->Vertex,x2); 
		NS_CHECK_EQ(elem->DOFVertices[3]->Vertex,x3); 
		NS_CHECK_EQ(elem->DOFVertices[4]->Vertex,x4); 
		NS_CHECK_EQ(elem->DOFVertices[5]->Vertex,x5);

		PolyShapeFunction<T,2,2> sf;
		NS_CHECK_EQ(sf.value(0, elem->Element.toLocal(x0)), 1);
		NS_CHECK_EQ(sf.value(1, elem->Element.toLocal(x1)), 1);
		NS_CHECK_EQ(sf.value(2, elem->Element.toLocal(x2)), 1);
		NS_CHECK_EQ(sf.value(3, elem->Element.toLocal(x3)), 1);
		NS_CHECK_EQ(sf.value(4, elem->Element.toLocal(x4)), 1);
		NS_CHECK_EQ(sf.value(5, elem->Element.toLocal(x5)), 1);

		NS_CHECK_EQ(sf.value(0, elem->Element.toLocal(x1)), 0);
		NS_CHECK_EQ(sf.value(1, elem->Element.toLocal(x2)), 0);
		NS_CHECK_EQ(sf.value(2, elem->Element.toLocal(x3)), 0);
		NS_CHECK_EQ(sf.value(3, elem->Element.toLocal(x4)), 0);
		NS_CHECK_EQ(sf.value(4, elem->Element.toLocal(x5)), 0);
		NS_CHECK_EQ(sf.value(5, elem->Element.toLocal(x0)), 0);
	}
	catch (const NSException& exception)
	{
		NS_GOT_EXCEPTION(exception);
	}
}
NS_TEST("GridOrder2")
{
	try
	{
		FixedVector<T,2> x0 = {0,0};
		FixedVector<T,2> x1 = {1,0};
		FixedVector<T,2> x2 = {1,1};
		FixedVector<T,2> x3 = {0,1};
		FixedVector<T,2> x4 = {0.5,0};
		FixedVector<T,2> x5 = {1,0.5};
		FixedVector<T,2> x6 = {0.5,1};
		FixedVector<T,2> x7 = {0,0.5};
		FixedVector<T,2> x8 = {0.5,0.5};

		Mesh<T,2> mesh;
		mesh.addVertex(new MeshVertex<T,2>(x0));
		mesh.addVertex(new MeshVertex<T,2>(x1));
		mesh.addVertex(new MeshVertex<T,2>(x2));
		mesh.addVertex(new MeshVertex<T,2>(x3));

		MeshElement<T,2>* elem1 = new MeshElement<T,2>();
		elem1->Vertices[0] = mesh.vertex(0);
		elem1->Vertices[1] = mesh.vertex(1);
		elem1->Vertices[2] = mesh.vertex(3);
		mesh.addElement(elem1);

		MeshElement<T,2>* elem2 = new MeshElement<T,2>();
		elem2->Vertices[0] = mesh.vertex(1);
		elem2->Vertices[1] = mesh.vertex(2);
		elem2->Vertices[2] = mesh.vertex(3);
		mesh.addElement(elem2);

		mesh.setupNeighbors();
		mesh.setupBoundaries();
		mesh.prepare();

		PolyShapeFunction<T,2,2>::prepareMesh(mesh);
		
		mesh.validate();

		NS_CHECK_EQ(mesh.vertices().size(), 9);
		NS_CHECK_EQ(mesh.elements().size(), 2);

		NS_CHECK_EQ(elem1->Neighbors[0]->Elements[1], elem2);
		NS_CHECK_EQ(elem2->Neighbors[1]->Elements[0], elem1);

		NS_CHECK_EQ(elem1->DOFVertices[0]->Vertex,x0); 
		NS_CHECK_EQ(elem1->DOFVertices[1]->Vertex,x1); 
		NS_CHECK_EQ(elem1->DOFVertices[2]->Vertex,x3); 
		NS_CHECK_EQ(elem1->DOFVertices[3]->Vertex,x4); 
		NS_CHECK_EQ(elem1->DOFVertices[4]->Vertex,x8); 
		NS_CHECK_EQ(elem1->DOFVertices[5]->Vertex,x7);

		NS_CHECK_EQ(elem2->DOFVertices[0]->Vertex,x1); 
		NS_CHECK_EQ(elem2->DOFVertices[1]->Vertex,x2); 
		NS_CHECK_EQ(elem2->DOFVertices[2]->Vertex,x3); 
		NS_CHECK_EQ(elem2->DOFVertices[3]->Vertex,x5); 
		NS_CHECK_EQ(elem2->DOFVertices[4]->Vertex,x6); 
		NS_CHECK_EQ(elem2->DOFVertices[5]->Vertex,x8);
	}
	catch (const NSException& exception)
	{
		NS_GOT_EXCEPTION(exception);
	}
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(PolySF, float);
NST_TESTCASE_T1(PolySF, double);
NST_END_MAIN