add_executable (main main.cpp) #../src/placement.h
set_property(TARGET main PROPERTY CXX_STANDARD 20)
include_directories (${SRMModule_SOURCE_DIR}/src) 
include_directories (${SRMModule_SOURCE_DIR}/include) 
target_link_libraries (main
                       Placement
                       )