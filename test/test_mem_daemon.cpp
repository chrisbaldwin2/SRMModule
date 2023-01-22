#include <iostream>
#include "placement.h"

int test_mem_time(const uint num_blocks = default_blocks)
{
    float mem_time;

    MemTimer timer = MemTimer();
    mem_time = timer.test_mem_time(num_blocks);
    
    std::cout << "Time to read " << num_blocks << " blocks: " << mem_time << " microseconds" << std::endl;
    float mem_bandwidth = timer.time_to_bandwidth(num_blocks, mem_time);
    std::cout << "Memory Bandwidth " << mem_bandwidth << " MBps" << std::endl;

    return 0;
}

int main(int argc, char const *argv[])
{
    test_mem_time();
    test_mem_time(2*default_blocks);
    return 0;
}
