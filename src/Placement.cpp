#include <iostream>
#include <cmath>
#include <numeric>
#include <vector>
#include <stdexcept>
#include "placement.h"

bool PlacementNode::cmp(PlacementNode a, PlacementNode b)
{
    return a.get_blocks() < b.get_blocks();
}

bool PlacementNode::mem_cmp(PlacementNode a, PlacementNode b)
{
    return a.get_mem_factor() > b.get_mem_factor();
}

int PlacementNode::get_index() 
{
    return this->index;
}

int PlacementNode::get_blocks() 
{
    return this->blocks;
}

int PlacementNode::get_used_blocks() 
{
    return this->max_blocks - this->blocks;
}

int PlacementNode::get_max_blocks() 
{
    return this->max_blocks;
}

void PlacementNode::set_mem_factor(float mem_factor) 
{
    this->mem_factor = mem_factor;
}

float PlacementNode::get_mem_factor() 
{
    return this->mem_factor;
}

bool PlacementNode::node_full()
{
    return this->blocks == 0;
}

// Returns the number of blocks allocated
int PlacementNode::allocate_all_blocks()
{
    int r_blocks = blocks;
    // Send required messages to node
    this->blocks = 0;
    return r_blocks;
} 

// Returns the number of blocks allocated
int PlacementNode::allocate_blocks(int num_blocks) 
{
    int r_blocks = std::min(num_blocks, blocks);
    // Send required messages to node
    this->blocks -= r_blocks;
    return r_blocks;
}

// Returns the number of blocks freed
int PlacementNode::free_blocks(int num_blocks) 
{
    int r_blocks = std::min(num_blocks, max_blocks - blocks);
    // Send required messages to node
    this->blocks += r_blocks;
    return r_blocks;
}

// Returns the number of blocks freed
int PlacementNode::free_all_blocks() 
{
    int r_blocks = blocks;
    // Send required messages to node
    this->blocks = max_blocks;
    return r_blocks;
}

PlacementNode::PlacementNode(int index, int blocks) 
{
    this->index = index;
    this->blocks = blocks;
    this->max_blocks = blocks;
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
    print_nodes();

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

/** Memory allocation Notes
  * 
  * Can either try to find the optimal allocation or just allocate the blocks incrementally
  * 
  * Incermental allocation:
  *  - Creates a vector of current node latency + incremental latency (incremental latency = 1 / mem_factor)
  *  - Sorts the vector by latency
  *  - Choose the node with lowest incremental latency
  * 
  * Optimal allocation:
  *  - Make a "schedule" of blocks which should be allocated to each node
  *  - Diff the schedule from the current allocation
  *  - Sort based on the diff
  *  - Allocate the blocks to the nodes with the highest diff
  */
 
PlacementStatus Placement::mem_allocate(int num_blocks)
{
    int accum_blocks = 0;
    int total_blocks = 0;
    float mem_cap = this->mem_cap();
    std::sort(nodes.begin(), nodes.end(), PlacementNode::cmp); 
    // print_nodes();
    
    if(avail_blocks() < num_blocks)
    {
        std::cout << "Not enough blocks\n  requested " << num_blocks << " but only " << avail_blocks() << " are available" << std::endl;
        return PlacementStatus::OUT_OF_SPACE;
    }

    for(int i=0; i<nodes.size(); i++)
    {
        float mem_prct = nodes[i].get_mem_factor() / mem_cap;
        int rem_nodes = nodes.size() - i;
        int i_srm_blocks = srm_blocks(mem_prct, num_blocks);
        // The number of blocks to be removed from the node
        int sub_blocks = std::min(i_srm_blocks + static_cast<int>(ceil(accum_blocks / rem_nodes)),  num_blocks - total_blocks);
        // The number of blocks which could be allocated to the node
        int rm_blocks = nodes[i].allocate_blocks(sub_blocks);
        total_blocks += rm_blocks;
        // If negative, accumulated blocks have been harvested otherwise accumulated blocks are added
        accum_blocks += i_srm_blocks - rm_blocks;
        std::cout << i << ": rem_nodes " << rem_nodes << " mem_prct " << mem_prct << " srm_blocks " << i_srm_blocks << " sub_blocks " << sub_blocks << " rm_blocks " << rm_blocks << std::endl;
    }

    int scrap_blocks = num_blocks-total_blocks+accum_blocks;
    std::cout << "Scrap " << scrap_blocks << std::endl;
    total_blocks += incremental_block_allocate(scrap_blocks);
    print_nodes();
    std::cout << "Total " << total_blocks << " Requested " << num_blocks << " mem_cap " << mem_cap << std::endl;
    if(total_blocks < num_blocks)
    {
        return PlacementStatus::RETRY;
    }

    return PlacementStatus::GOOD;
}

int Placement::avail_blocks()
{
    int avail_blocks = 0;
    for(auto n : nodes) avail_blocks += n.get_blocks();
    return avail_blocks;
}

float Placement::mem_cap()
{
    float mem_cap = 0;
    for(auto n : nodes) mem_cap += n.get_mem_factor();
    // print_mem();
    return mem_cap;
}

void Placement::node_hearbeat(PlacementHeartbeat heartbeat)
{
    nodes[heartbeat.node_id].set_mem_factor(heartbeat.mem_factor);
}

void Placement::print_nodes()
{
    for(int i=0; i<nodes.size(); i++)
    {
        std::cout << nodes[i].get_index() << ":" << nodes[i].get_blocks() << "\t";
    }
    std::cout << std::endl;
}

void Placement::print_mem()
{
    for(int i=0; i<nodes.size(); i++)
    {
        std::cout << nodes[i].get_index() << ":" << nodes[i].get_mem_factor() << "\t";
    }
    std::cout << "Cap: " << mem_cap() << std::endl;
}

int Placement::incremental_block_allocate(int num_blocks)
{
    std::vector<std::tuple<int, float>> nodes_latency;
    for(int i=0; i<nodes.size(); i++)
    {
        if(!nodes[i].node_full())
        {
            nodes_latency.push_back(std::tuple(i, (nodes[i].get_used_blocks() + 1) / nodes[i].get_mem_factor()));
            std::cout << i << ": " << std::get<float>(nodes_latency[i]) << std::endl;
        }
    }
    
    int blocks = 0;
    for(; blocks < num_blocks; blocks++)
    {
        std::sort(nodes_latency.begin(), nodes_latency.end(), 
            [](std::tuple<int, float> a, std::tuple<int, float> b){return std::get<float>(a) < std::get<float>(b);});
        
        for(int i=0; i<nodes.size(); i++)
        {
            std::cout << std::get<int>(nodes_latency[i]) << ": latency " << std::get<float>(nodes_latency[i]) << " full " << nodes[std::get<int>(nodes_latency[i])].node_full() << std::endl;
            // if the node is not full, allocate a block
            if (!nodes[std::get<int>(nodes_latency[i])].node_full()) 
            {
                if(nodes[std::get<int>(nodes_latency[i])].allocate_blocks(1) == 1)
                {
                    nodes_latency[i] = std::tuple(std::get<int>(nodes_latency[i]), (std::get<float>(nodes_latency[i]) + (1 / nodes[std::get<int>(nodes_latency[i])].get_mem_factor())));
                    goto next_loop;
                }
            }
        }
        // If we go through all nodes without allocating a block, throw an error
        throw new std::runtime_error("Could not allocate block in incremental_block_allocate");
        // Good allocation
      next_loop:
        continue;
    }
    return blocks;
}

int srm_blocks(float mem_prct, int num_blocks)
{
    if ( mem_prct < 0 ) {
        throw std::invalid_argument( "received negative mem_prct" );
    }
    int blocks = floor(mem_prct * num_blocks);
    std::cout << "mem_prct " << mem_prct << " raw_blocks " << mem_prct * num_blocks << " num_blocks " << num_blocks << " blocks " << blocks << std::endl;
    return blocks;
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

void print_mem(std::vector<PlacementNode> nodes){
    
    for(int i=0; i<nodes.size(); i++)
    {
        std::cout << nodes[i].get_index() << ":" << nodes[i].get_mem_factor() << "\t";
    }
    std::cout << std::endl;
}