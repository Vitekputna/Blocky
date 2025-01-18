#pragma once
#include "mesh.h"
#include "entities.h"
#include "transformations.h"
#include <vector>

// Mesh of multiple blocks
class multi_block
{
    public:
    // mesh block storage
    std::vector<mesh_base*> blocks;
    std::vector<uint> node_idx_offsets = {0};

    multi_block();
    ~multi_block();

    void add_block(mesh_base* block);

    uint get_N_cells() const;
    uint get_N_blocks() const;
    uint get_N_nodes() const;
};