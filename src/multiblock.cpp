#include "multiblock.h"

// Multiblock mesh
multi_block::multi_block(){}
multi_block::~multi_block(){}

void multi_block::add_block(mesh_base* block)
{
    blocks.push_back(block);
    node_idx_offsets.push_back(block->get_N_nodes());
}

uint multi_block::get_N_blocks() const {return blocks.size();}
uint multi_block::get_N_nodes() const
{
    uint N = 0;

    for(auto const& block : blocks)
    {
        N += block->get_N_nodes();
    }

    return N;
}
uint multi_block::get_N_cells() const
{
    uint N = 0;

    for(auto const& block : blocks)
    {
        N += block->get_N_cells();
    }

    return N;
}