add_library (Placement placement.cpp)
include_directories (${SRMModule_SOURCE_DIR}/include) 
set_property(TARGET Placement PROPERTY CXX_STANDARD 20)
