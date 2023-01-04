#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include "placement.h"

/* ~~~~~NOTES~~~~~
 * - Probably want either class or struct for nodes + len
 * - Also would be nice to know if an order of the available node blocks from smallest to greatest
 * - Would be nice to have total number of blocks in the system
 */

/**
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

/**
  * Takes in the number blocks to allocate and the nodes and returns the number of 
  * blocks that need to be allocated from each node
  */
std::vector<int> node_alloc(std::vector<PlacementNode> nodes, int num_blocks)
{
    // The number of blocks to remove from each blocks
    std::vector<int> schedule(nodes.size());
    int base_blocks = num_blocks / nodes.size();
    int rem_blocks = num_blocks % nodes.size();
    int accum_blocks = 0;
    std::sort(nodes.begin(), nodes.end(), PlacementNode::cmp); 
    print_nodes(nodes);

    int avail_blocks = 0;
    for(auto n : nodes) avail_blocks += n.get_blocks();
    if(avail_blocks < num_blocks)
    {
        std::cout << "Not enough blocks\n  requested " << num_blocks << " but only " << avail_blocks << " are available" << std::endl;
        exit(-1);
    }

    for(int i=0; i<nodes.size(); i++)
    {
        PlacementNode n = nodes[i];
        if(n.get_blocks() < base_blocks)
        {
            std::cout << "  block " << n.get_index() << ":" << n.get_blocks()
                      << " less than " << base_blocks << std::endl;
            accum_blocks += base_blocks - n.get_blocks();
            schedule[n.get_index()] = n.get_blocks();
            continue;
        }
        if(n.get_blocks() == base_blocks)
        {
            schedule[n.get_index()] = base_blocks;
            continue;
        }
        int rem_nodes = nodes.size() - i;
        int sub_blocks = base_blocks + ceil(rem_blocks / rem_nodes) + ceil(accum_blocks / rem_nodes);
        rem_blocks -= ceil(rem_blocks / rem_nodes);
        accum_blocks -= ceil(accum_blocks / rem_nodes);
        schedule[n.get_index()] = sub_blocks;
    }

    return schedule;
}

void exec_schedule(std::vector<PlacementNode> &nodes, std::vector<int> sched)
{
    for(int i=0; i<std::min(sched.size(), nodes.size()); i++)
    {
        nodes[i].allocate_blocks(sched[i]);
    }
    print_nodes(nodes);

}

int node_test()
{
    int blocks = 10;
    std::vector<PlacementNode> nodes = {
        {0, blocks},
        {1, blocks},
        {2, blocks},
        {3, blocks},
        {4, blocks},
        };
    std::vector<float> mem_factor = {1, 1, 1, 1, 1};

    auto sched = node_alloc(nodes, 10);
    exec_schedule(nodes, sched);
    print_nodes(nodes);

    sched = node_alloc(nodes, 7);
    exec_schedule(nodes, sched);
    print_nodes(nodes);

    sched = node_alloc(nodes, 4);
    exec_schedule(nodes, sched);
    print_nodes(nodes);

    sched = node_alloc(nodes, 60);
    exec_schedule(nodes, sched);
    print_nodes(nodes);

    return 0;
}

int flat_placement_test()
{
    int blocks = 10;
    int num_blocks = 5;
    PlacementStatus s;
    Placement placement(num_blocks, blocks);

    s = placement.flat_allocate(10);
    assert(s == PlacementStatus::GOOD);
    s = placement.flat_allocate(7);
    assert(s == PlacementStatus::GOOD);
    s = placement.flat_allocate(4);
    assert(s == PlacementStatus::GOOD);
    s = placement.flat_allocate(27);
    assert(s == PlacementStatus::GOOD);
    s = placement.flat_allocate(60);
    assert(s == PlacementStatus::OUT_OF_SPACE);

    return 0;
}

int mem_placement_test()
{
    int blocks = 10;
    int num_blocks = 5;
    PlacementStatus s;
    Placement placement(num_blocks, blocks);

    for(int i=0; i<num_blocks; i++)
    {
        placement.node_hearbeat({i, static_cast<float>(.5+(i*.1))});
    }

    s = placement.mem_allocate(10);
    assert(s == PlacementStatus::GOOD);
    s = placement.mem_allocate(7);
    assert(s == PlacementStatus::GOOD);
    s = placement.mem_allocate(4);
    assert(s == PlacementStatus::GOOD);
    s = placement.mem_allocate(27);
    assert(s == PlacementStatus::GOOD);
    s = placement.mem_allocate(60);
    assert(s == PlacementStatus::OUT_OF_SPACE);

    return 0;
}

int main(int argc, char const *argv[])
{
    flat_placement_test();
    mem_placement_test();
    node_test();

    return 0;
}
