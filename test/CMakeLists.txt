add_executable (test_placement test_placement.cpp) #../src/placement.h
set_property(TARGET test_placement PROPERTY CXX_STANDARD 20)
# include_directories (${SRMModule_SOURCE_DIR}/src) 
include_directories (${SRMModule_SOURCE_DIR}/include) 
target_link_libraries (test_placement
    Placement
    )