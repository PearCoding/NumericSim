SET(SRC_MAIN
 CG.h
 CG.inl
 Config.h
 ComplexNumber.h
 ComplexNumber.inl
 CountableSet.h
 CountableSet.inl
 Exceptions.h
 Iterative.h
 Iterative.inl
 LU.h
 LU.inl
 OutputStream.h
 Simplex.h
 Simplex.inl
 Types.h
 Utils.h
 Vector.h
 Vector.inl)

SET(SRC_MATRIX
 matrix/BaseMatrix.h
 matrix/BaseMatrix.inl
 matrix/DenseMatrix.h
 matrix/DenseMatrix.inl
 matrix/FixedMatrix.h
 matrix/FixedMatrix.inl
 matrix/MatrixCheck.h
 matrix/MatrixCheck.inl
 matrix/MatrixConstructor.h
 matrix/MatrixConstructor.inl
 matrix/MatrixConverter.h
 matrix/MatrixConverter.inl
 matrix/MatrixOperations.h
 matrix/MatrixOperations.inl
 matrix/SparseMatrix.h
 matrix/SparseMatrix.inl)
SOURCE_GROUP("Header Files\\Matrix" FILES ${SRC_MATRIX})

SET(SRC_MESH
 mesh/HyperCube.h
 mesh/HyperCube.inl
 mesh/Mesh.h
 mesh/Mesh.inl)
SOURCE_GROUP("Header Files\\Mesh" FILES ${SRC_MESH})

SET(SRC_SF
 sf/PolyShapeFunction.h
 sf/PolyShapeFunction.inl
 sf/ShapeFunction.h
 sf/ShapeFunction.inl)
SOURCE_GROUP("Header Files\\ShapeFunction" FILES ${SRC_SF})

SET(SRC_EXPORT
 export/VTKExporter.h
 export/VTKExporter.inl)
SOURCE_GROUP("Header Files\\Export" FILES ${SRC_EXPORT})

SET(SRC ${SRC_MAIN} ${SRC_MATRIX} ${SRC_MESH} ${SRC_SF} ${SRC_EXPORT})
 
add_library(ns_lib STATIC ${SRC})
set_target_properties(ns_lib PROPERTIES LINKER_LANGUAGE CXX)
