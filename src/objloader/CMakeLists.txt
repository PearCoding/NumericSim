SET(SRC
 loader/MeshObjLoader.h
 loader/MeshObjLoader.inl
 loader/MeshObjLoader.cpp)
 
add_library(ns_objloader STATIC ${SRC})
set_target_properties(ns_objloader PROPERTIES LINKER_LANGUAGE CXX)
