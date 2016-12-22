SET(SRC
 mesh/MeshObjLoader.h
 mesh/MeshObjLoader.inl
 mesh/MeshObjLoader.cpp)
 
add_library(ns_objloader STATIC ${SRC})
set_target_properties(ns_objloader PROPERTIES LINKER_LANGUAGE CXX)
