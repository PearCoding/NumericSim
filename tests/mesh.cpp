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
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(Mesh2D, float);
NST_TESTCASE_T1(Mesh2D, double);
NST_TESTCASE_T1(Mesh2D, ComplexNumber<double>);
NST_END_MAIN