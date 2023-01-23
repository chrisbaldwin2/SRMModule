#include <iostream>
#include <chrono>
#include "placement.h"

float MemTimer::diff_clocks(std::chrono::high_resolution_clock::time_point start, std::chrono::high_resolution_clock::time_point stop)
{
    // Subtract stop and start timepoints and
    // cast it to required unit. Predefined units
    // are nanoseconds, microseconds, milliseconds,
    // seconds, minutes, hours. Count converts the 
    // chrono type to float type.
    float duration = std::chrono::duration<float, std::chrono::microseconds::period>(stop - start).count();
    return duration;
}

float MemTimer::test_mem_time(const uint num_blocks = default_blocks)
{
    char* buf = new char[num_blocks];
    volatile char b = buf[0];
    start_timer();

    // Read some memory
    for(int i=0; i<num_blocks; i++)
    {
        b = buf[i];
    }

    stop_timer();

    float duration = diff_clocks(start, stop);
    
    delete [] buf;

    return duration;
}

void MemTimer::start_timer()
{
    start = std::chrono::high_resolution_clock::now();
    stop = start;
    duration = 0;
}

void MemTimer::stop_timer()
{
    stop = std::chrono::high_resolution_clock::now();
    duration = diff_clocks(start, stop);
}

float MemTimer::get_duration()
{
    return duration;
}

float MemTimer::time_to_bandwidth(const uint num_blocks, const float time)
{
    float bandwidth = (float)num_blocks / time;
    return bandwidth;
}

MemTimer::MemTimer()
{
    start = std::chrono::high_resolution_clock::now();
    stop = start;
    duration = 0;
}