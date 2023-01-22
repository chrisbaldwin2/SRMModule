#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include "placement.h"

/* ~~~~~NOTES~~~~~
 * DONE Probably want either class or struct for nodes + len
 * DONE Also would be nice to know if an order of the available node blocks from smallest to greatest
 * DONE Would be nice to have total number of blocks in the system
 */


int flat_placement_test()
{
    int blocks = 10;
    int num_blocks = 5;
    PlacementStatus s;
    Placement placement(num_blocks, blocks);

    s = placement.flat_allocate(10);
    assert(s == PlacementStatus::GOOD);
    auto avail_blocks = placement.avail_blocks();
    assert(avail_blocks == blocks * num_blocks - 10);
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

    auto mem_cap = placement.mem_cap();
    assert(mem_cap == .5+.6+.7+.8+.9);

    s = placement.mem_allocate(10);
    assert(s == PlacementStatus::GOOD);
    auto avail_blocks = placement.avail_blocks();
    assert(avail_blocks == blocks * num_blocks - 10);
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

int node_test()
{
    int max_blocks = 10;
    PlacementNode a = {0, max_blocks};
    PlacementNode b = {1, max_blocks};

    auto blocks = a.get_max_blocks();
    assert(blocks == max_blocks);
    blocks = a.allocate_blocks(4);
    assert(blocks == 4);
    // Test get_blocks
    blocks = a.get_blocks();
    assert(blocks == max_blocks - 4);
    blocks = a.allocate_blocks(8);
    assert(blocks == max_blocks-4);
    blocks = a.free_blocks(5);
    assert(blocks == 5);
    blocks = a.free_all_blocks();
    assert(blocks == max_blocks - 5);

    auto index = a.get_index();
    assert(index == 0);
    index = b.get_index();
    assert(index == 1);

    auto mem_f = a.get_mem_factor();
    assert(mem_f = 1.0);
    a.set_mem_factor(.7);
    mem_f = a.get_mem_factor();
    assert(mem_f = .7);
    auto cmp = PlacementNode::mem_cmp(a, b);
    assert(cmp == false);
    cmp = PlacementNode::cmp(a, b);
    assert(cmp == false);
    a.allocate_blocks(2);
    cmp = PlacementNode::cmp(a, b);
    assert(cmp == true);



    return 0;
}

int test_srm()
{
    float mem_prct = .8;
    int needed_blocks = 10;

    auto result = srm_blocks(mem_prct, needed_blocks);
    assert(result == 8);
    mem_prct = .5;
    result = srm_blocks(mem_prct, needed_blocks);
    assert(result == 5);
    needed_blocks = 5;
    result = srm_blocks(mem_prct, needed_blocks);
    assert(result == 3);
    mem_prct = -3;
    try {
        result = srm_blocks(mem_prct, needed_blocks);
    }
    catch( const std::invalid_argument& e ) {
        std::cout << e.what() << std::endl; 
        return 0;
    }
    return -1;
}

int main(int argc, char const *argv[])
{
    flat_placement_test();
    mem_placement_test();
    node_test();
    test_srm();
    return 0;
}
