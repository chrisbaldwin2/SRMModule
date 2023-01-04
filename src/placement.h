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
    float mem_usage;
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
    void allocate_blocks(int num_blocks);
    void allocate_all_blocks();
    static bool cmp(PlacementNode a, PlacementNode b);
};

class Placement
{
  private:
    std::vector<PlacementNode> nodes; // # nodes, value of # blocks
    
  public:
    Placement(int num_nodes, int blocks_per_node);
    // The number of blocks which are in node_blocks
    int avail_blocks();
    // Greedily allocates blocks, does not use memory bandwidth
    PlacementStatus flat_allocate(int num_blocks);
    // Greedily allocates blocks, does not use memory bandwidth
    PlacementStatus mem_allocate(int num_blocks);
    // Updates the node memory bandwidth
    void node_hearbeat(PlacementHeartbeat heartbeat);

};

void print_nodes(std::vector<int> nodes);
void print_nodes(std::vector<PlacementNode> nodes);

#endif