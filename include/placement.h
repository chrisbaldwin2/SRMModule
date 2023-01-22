#ifndef __placement_h__
#define __placement_h__

#define BAD -1
#include <vector>
#define default_blocks 1024

typedef enum PlacementStatus
{
    GOOD,
    OUT_OF_SPACE,
    RETRY
} PlacementStatus;

typedef struct
{
    int node_id;
    float mem_factor;
} PlacementHeartbeat;

class PlacementNode
{
  private:
    int index;
    int max_blocks;
    int blocks;
    float mem_factor;
  public:
    PlacementNode(int index, int blocks);
    int get_index();
    int get_blocks();
    int get_max_blocks();
    void set_mem_factor(float mem_factor);
    float get_mem_factor();
    int free_blocks(int num_blocks);
    int free_all_blocks();
    int allocate_blocks(int num_blocks);
    int allocate_all_blocks();
    static bool cmp(PlacementNode a, PlacementNode b);
    static bool mem_cmp(PlacementNode a, PlacementNode b);
};

class Placement
{
  private:
    std::vector<PlacementNode> nodes; // # nodes, value of # blocks
    
  public:
    Placement(int num_nodes, int blocks_per_node);
    // The total mem capacity of all nodes
    float mem_cap();
    // The number of blocks which are in node_blocks
    int avail_blocks();
    // Greedily allocates blocks, does not use memory bandwidth
    PlacementStatus flat_allocate(int num_blocks);
    // Greedily allocates blocks, does not use memory bandwidth
    PlacementStatus mem_allocate(int num_blocks);
    // Updates the node memory bandwidth
    void node_hearbeat(PlacementHeartbeat heartbeat);

};

int srm_blocks(float mem_prct, int needed_blocks);

void print_nodes(std::vector<int> nodes);
void print_nodes(std::vector<PlacementNode> nodes);
void print_mem(std::vector<PlacementNode> nodes);


/** Subtract stop and start timepoints and cast it to required unit. 
  * Predefined units are nanoseconds, microseconds, milliseconds,
  * seconds, minutes, hours. Count converts the chrono type to float type.
  */

class MemTimer
{
  private:
    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point stop;
    float duration;
    float diff_clocks(std::chrono::high_resolution_clock::time_point start, std::chrono::high_resolution_clock::time_point stop);
  public:
    MemTimer();
    void start_timer();
    void stop_timer();
    // Returns the duration in microseconds
    float get_duration();
    /** Get the time it takes to read num_blocks bytes. 
      * The time is returned in microseconds.
      */
    float test_mem_time(const uint num_blocks);
    static float time_to_bandwidth(const uint num_blocks, const float time);
};

#endif