#include "mesh.h"
#include "multiblock.h"
#include <fstream>
#include <string>
#include <iostream>

// Function to save data in legacy VTK format
void saveToVTK(const mesh_base* obj, const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    auto N = obj->get_size_nodes();
    int nx = N.x; int ny = N.y; int nz = N.z;

    // Write VTK header
    outfile << "# vtk DataFile Version 2.0" << std::endl;
    outfile << "My VTK Data" << std::endl;
    outfile << "ASCII" << std::endl;
    outfile << "DATASET UNSTRUCTURED_GRID" << std::endl;
    outfile << "\n";
    // outfile << "DIMENSIONS " << nx << " " << ny << " " << nz << std::endl;
    outfile << "POINTS " << nx * ny * nz << " float" << std::endl;

    for(node_index i = 0; i < obj->get_N_nodes(); i++)
    {
        outfile << obj->get_node(i).position << "\n";
    }
    outfile << "\n";

    const uint N_cells = obj->get_N_cells();
    outfile << "CELLS " << N_cells << " " << 9 * N_cells << std::endl;
    for(cell_index i = 0; i < obj->get_N_cells(); i++)
    {
        outfile << "8 " << obj->get_cell(i).cell_vertices << "\n";
    }
    outfile << "\n";

    // Write cell types
    outfile << "CELL_TYPES " << N_cells << std::endl;
    for (uint i = 0; i < N_cells; ++i) {
        outfile << "12 \n"; // VTK_HEXAHEDRON cell type
    }
    outfile << std::endl;

    outfile.close();
}

void saveToVTK(const multi_block& mesh, const std::string& filename)
{
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    // Write VTK header
    outfile << "# vtk DataFile Version 2.0" << std::endl;
    outfile << "My VTK Data" << std::endl;
    outfile << "ASCII" << std::endl;
    outfile << "DATASET UNSTRUCTURED_GRID" << std::endl;
    outfile << "\n";

    outfile << "POINTS " << mesh.get_N_nodes() << " float" << std::endl;
    for(auto const obj : mesh.blocks)
    {
        for(uint i = 0; i < obj->get_N_nodes(); i++)
        {
            outfile << obj->get_node(i).position << "\n";
        }
    }
    outfile << "\n";

    const uint N_cells = mesh.get_N_cells();
    outfile << "CELLS " << N_cells << " " << 9 * N_cells << std::endl;
    int j = 0;
    for(auto const obj : mesh.blocks)
    {
        for(uint i = 0; i < obj->get_N_cells(); i++)
        {
            outfile << "8 " << obj->get_cell(i).cell_vertices + mesh.node_idx_offsets[j] << "\n";
        }
        j++;
    }
    outfile << "\n";

    // Write cell types
    outfile << "CELL_TYPES " << N_cells << std::endl;
    for (uint i = 0; i < N_cells; ++i) {
        outfile << "12 \n";
    }
    outfile << std::endl;

    outfile.close();
}
