#define NS_ALLOW_CHECKS

#include "Test.h"
#include "mesh/Mesh.h"
#include "OutputStream.h"

#include "mesh/MeshObjLoader.h"

NS_USE_NAMESPACE;

static const char* OBJ =
	#include "sphere.obj.inl"
	;

template<typename T>
NS_BEGIN_TESTCASE_T1(ObjLoader)
NS_TEST("load")
{
	try
	{
		auto mesh = Mesh::MeshObjLoader<T,3>::loadString(OBJ);
		mesh.validate();
	}
	catch (const NSException& exception)
	{
		NS_GOT_EXCEPTION(exception);
	}
}
NS_END_TESTCASE()

NST_BEGIN_MAIN
NST_TESTCASE_T1(ObjLoader, float);
NST_TESTCASE_T1(ObjLoader, double);
NST_TESTCASE_T1(ObjLoader, ComplexNumber<double>);
NST_END_MAIN