#define NS_ALLOW_CHECKS

#include "Test.h"
#include "mesh/Mesh.h"
#include "mesh/HyperCube.h"
#include "OutputStream.h"

NS_USE_NAMESPACE;

template<typename T>
NS_BEGIN_TESTCASE_T1(Mesh2D)
NS_TEST("generator")
{
	constexpr Dimension S = 10;
	try
	{
		Mesh<T,2> mesh = HyperCube<T,2>::generate(
			Vector2D<Dimension>{S,S},
			Vector2D<T>{1,1},
			Vector2D<T>{0,0});
		
		mesh.prepare();
		mesh.validate();

		NS_CHECK_EQ(mesh.vertices().size(), (S+1)*(S+1));
		NS_CHECK_EQ(mesh.elements().size(), S*S*2);
	}
	catch (const NSException& exception)
	{
		NS_GOT_EXCEPTION(exception);
	}
}
NS_TEST("boundary")
{
	constexpr Dimension S = 10;
	try
	{
		Mesh<T,2> mesh1 = HyperCube<T,2>::generate(
			Vector2D<Dimension>{S,S},
			Vector2D<T>{1,1},
			Vector2D<T>{0,0});
		mesh1.prepare();

		Mesh<T,2> mesh2 = HyperCube<T,2>::generate(
			Vector2D<Dimension>{S,S},
			Vector2D<T>{1,1},
			Vector2D<T>{0,0});
		for(auto& v : mesh2.vertices())
			v->Flags = 0;
		mesh2.setupBoundaries();
		mesh2.prepare();

		NS_CHECK_EQ(mesh1.vertices().size(), mesh2.vertices().size());
		NS_CHECK_EQ(mesh1.elements().size(), mesh2.elements().size());

		size_t boundaries1 = 0;
		for(const auto& v : mesh1.vertices())
		{
			if(v->Flags & MVF_StrongBoundary)
				boundaries1++;
		}

		size_t boundaries2 = 0;
		for(const auto& v : mesh2.vertices())
		{
			if(v->Flags & MVF_StrongBoundary)
				boundaries2++;
		}
		
		NS_CHECK_EQ(boundaries2, boundaries1);
	}
	catch (const NSException& exception)
	{
		NS_GOT_EXCEPTION(exception);
	}
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(Mesh2D, float);
NST_TESTCASE_T1(Mesh2D, double);
NST_TESTCASE_T1(Mesh2D, std::complex<double>);
NST_END_MAIN