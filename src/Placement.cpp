#include <iostream>
#include <cmath>
#include <numeric>
#include <vector>
#include "placement.h"

bool PlacementNode::cmp(PlacementNode a, PlacementNode b)
{
    return a.get_blocks() < b.get_blocks();
}

int PlacementNode::get_index() 
{
    return this->index;
}

int PlacementNode::get_blocks() 
{
    return this->blocks;
}

void PlacementNode::allocate_all_blocks()
{
    // Send required messages to node
    this->blocks = 0;
} 

void PlacementNode::allocate_blocks(int num_blocks) 
{
    // Send required messages to node
    this->blocks -= num_blocks;
}

PlacementNode::PlacementNode(int index, int blocks) 
{
    this->index = index;
    this->blocks = blocks;
    this->mem_factor = 1.0;
}

Placement::Placement(int num_nodes, int blocks_per_node)
{
    for(int i=0; i<num_nodes; i++)
    {
        nodes.push_back({i, blocks_per_node});
    }
}

/**
  * Takes in the number blocks to allocate and the nodes and returns the number of 
  * blocks that need to be allocated from each node
  */
PlacementStatus Placement::flat_allocate(int num_blocks)
{
    // The number of blocks to remove from each blocks
    int base_blocks = num_blocks / nodes.size();
    int rem_blocks = num_blocks % nodes.size();
    int accum_blocks = 0;
    std::sort(nodes.begin(), nodes.end(), PlacementNode::cmp); 
    print_nodes(nodes);

    if(avail_blocks() < num_blocks)
    {
        std::cout << "Not enough blocks\n  requested " << num_blocks << " but only " << avail_blocks() << " are available" << std::endl;
        return PlacementStatus::OUT_OF_SPACE;
    }

    for(int i=0; i<nodes.size(); i++)
    {
        if(nodes[i].get_blocks() < base_blocks)
        {
            std::cout << "  block " << nodes[i].get_index() << ":" << nodes[i].get_blocks() 
                      << " less than " << base_blocks << std::endl;
            accum_blocks += base_blocks - nodes[i].get_blocks();
            nodes[i].allocate_all_blocks();
            continue;
        }
        if(nodes[i].get_blocks() == base_blocks)
        {
            nodes[i].allocate_all_blocks();
            continue;
        }
        int rem_nodes = nodes.size() - i;
        int sub_blocks = base_blocks + ceil(rem_blocks / rem_nodes) + ceil(accum_blocks / rem_nodes);
        rem_blocks -= ceil(rem_blocks / rem_nodes);
        accum_blocks -= ceil(accum_blocks / rem_nodes);
        nodes[i].allocate_blocks(sub_blocks);
    }

    return PlacementStatus::GOOD;
}

PlacementStatus Placement::mem_allocate(int num_blocks)
{

    return PlacementStatus::GOOD;
}

int Placement::avail_blocks()
{
    int avail_blocks = 0;
    for(auto n : nodes) avail_blocks += n.get_blocks();
    return avail_blocks;
}

void Placement::node_hearbeat(PlacementHeartbeat heartbeat)
{

}

void print_nodes(std::vector<int> nodes){
    
    for(int i=0; i<nodes.size(); i++)
    {
        std::cout << nodes[i] << "\t";
    }
    std::cout << std::endl;
}

void print_nodes(std::vector<PlacementNode> nodes){
    
    for(int i=0; i<nodes.size(); i++)
    {
        std::cout << nodes[i].get_index() << ":" << nodes[i].get_blocks() << "\t";
    }
    std::cout << std::endl;
}