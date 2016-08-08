SET(SRC_MAIN
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
 Types.h
 Utils.h
 Vector.h
 Vector.inl)

SET(SRC_MATRIX
 matrix/Matrix.h
 matrix/Matrix.inl
 matrix/MatrixConstructor.h
 matrix/MatrixConstructor.inl
 matrix/MatrixConverter.h
 matrix/MatrixConverter.inl
 matrix/SparseMatrix.h
 matrix/SparseMatrix.inl)
SOURCE_GROUP("Header Files\\Matrix" FILES ${SRC_MATRIX})

SET(SRC ${SRC_MAIN} ${SRC_MATRIX})
 
add_library(ns_lib STATIC ${SRC})
set_target_properties(ns_lib PROPERTIES LINKER_LANGUAGE CXX)
