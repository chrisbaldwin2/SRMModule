add_executable (test_placement test_placement.cpp) #../src/placement.h
add_executable (test_mem_daemon test_mem_daemon.cpp) #../src/placement.h
set_property(TARGET test_placement PROPERTY CXX_STANDARD 20)
set_property(TARGET test_mem_daemon PROPERTY CXX_STANDARD 20)
# include_directories (${SRMModule_SOURCE_DIR}/src) 
include_directories (${SRMModule_SOURCE_DIR}/include) 
target_link_libraries (test_placement
    Placement
    )
target_link_libraries (test_mem_daemon
    Placement
    )