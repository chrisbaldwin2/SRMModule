#include <vector>

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

class Placement
{
    int num_nodes;
    int blocks_per_node;
    std::vector<int> node_blocks; // # nodes, value of # blocks
    

    Placement(int num_nodes, int blocks_per_node);
    PlacementStatus allocate(int num_blocks);
    void node_hearbeat(PlacementHeartbeat heartbeat);

};

Placement::Placement(int num_nodes, int blocks_per_node)
{
    this->num_nodes = num_nodes;
    this->blocks_per_node = blocks_per_node;
}

PlacementStatus Placement::allocate(int num_blocks)
{
    
}

void Placement::node_hearbeat(PlacementHeartbeat heartbeat)
{

}