#include <iostream>
#include <vector>
#include <cmath>

void print_nodes(std::vector<int> nodes);

/* ~~~~~NOTES~~~~~
 * - Probably want either class or struct for nodes + len
 * - Also would be nice to know if an order of the available node blocks from smallest to greatest
 * - Would be nice to have total number of blocks in the system
 */

/*
 * 10 / 5 = 2
 * 
 * 2   | 2   | 2   | 2   | 2
 * 
 * 2   | 2   | 2   | 2   | 2
 * 
 * 
 * 7 / 5 = 1.4
 * 
 * 1.4 | 1.4 | 1.4 | 1.4 | 1.4
 * 
 * 2   | 2   | 1   | 1   | 1 
 */
int greedy_alloc(std::vector<int> &nodes, int num_blocks){
    // First make sure that all nodes have enough blocks to take off the top
    float sub_blocks = (float)num_blocks / (float)nodes.size();
    std::cout << "GREEDY: Num Blocks = " << num_blocks << " Sub Blocks = " << sub_blocks << std::endl;
    // Check that each node has enough blocks for quick greedy
    for(int i=0; i<nodes.size(); i++)
    {
        // Blocks must have ceil(blocks / len) if < (blocks % len) and floor(blocks / len) otherwise 
        int local_sub_blocks;
        if((num_blocks % nodes.size()) && (i < (num_blocks % nodes.size()))) 
            local_sub_blocks = std::ceil(sub_blocks);
        else
            local_sub_blocks = std::floor(sub_blocks);
        if(nodes[i] < local_sub_blocks)
        {
            std::cout << "GREEDY: Exit early on block " << i << " node:" << nodes[i] << " sub:" << local_sub_blocks << std::endl;
            return -1; //BAD 
        }
    }
    // Good blocks
    for(int i=0; i<nodes.size(); i++)
    {
        // Blocks must have ceil(blocks / len) if < (blocks % len) and floor(blocks / len) otherwise 
        int local_sub_blocks;
        if((num_blocks % nodes.size()) && (i < (num_blocks % nodes.size()))) 
            local_sub_blocks = std::ceil(sub_blocks);
        else
            local_sub_blocks = std::floor(sub_blocks);
        // std::cout << "\t" << i << " node: " << nodes[i] << " sub: " << local_sub_blocks << " result: " << nodes[i] - local_sub_blocks << std::endl;
        nodes[i] = nodes[i] - local_sub_blocks;
    }

    // print_nodes(nodes);

    return 0; //GOOD
}

int smart_alloc(std::vector<int> nodes, std::vector<float> mem_factor, int num_blocks){
    // First make sure that all nodes have enough blocks to take off the top
    float mem_cap = 0;
    for(auto m : mem_factor) mem_cap += m;
    float sub_blocks = num_blocks / mem_cap;

    std::vector<int> block_schedule;
    int block_accum = 0;

    // Check that each node has enough blocks for quick smart
    for(int i=0; i<nodes.size(); i++)
    {
        // Must round blocks to int value
        int local_sub_blocks = std::round(sub_blocks * mem_factor[i]);
        if(nodes[i] < local_sub_blocks) // Want to check for accumulated rounding error
            return -1; //BAD 
        block_schedule[i] = local_sub_blocks;

    }
    // Good blocks
    for(int i=0; i<nodes.size(); i++)
    {
        // Blocks must have ceil(blocks / len) if < (blocks % len) and floor(blocks / len) otherwise 
        int local_sub_blocks = std::round(sub_blocks * mem_factor[i]);
        nodes[i] -= local_sub_blocks;
    }

    return 0; //GOOD
}

void print_nodes(std::vector<int> nodes){
    
    for(int i=0; i<nodes.size(); i++)
    {
        std::cout << nodes[i] << "\t";
    }
    std::cout << std::endl;
}

int main(int argc, char const *argv[])
{
    int blocks = 10;
    std::vector<int> nodes = {blocks, blocks, blocks, blocks, blocks};
    std::vector<float> mem_factor = {1, 1, 1, 1, 1};

    print_nodes(nodes);

    greedy_alloc(nodes, 10);
    print_nodes(nodes);

    greedy_alloc(nodes, 7);
    print_nodes(nodes);

    greedy_alloc(nodes, 50);
    print_nodes(nodes);

    return 0;
}
