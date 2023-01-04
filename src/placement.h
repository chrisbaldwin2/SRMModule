#ifndef __placement_h__
#define __placement_h__


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
    int blocks;
    float mem_factor;
  public:
    PlacementNode(int index, int blocks);
    int get_index();
    int get_blocks();
    void set_mem_factor(float mem_factor);
    float get_mem();
    void allocate_blocks(int num_blocks);
    void allocate_all_blocks();
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

#endif