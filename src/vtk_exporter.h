#include "mesh.h"
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

    for(uint i = 0; i < obj->get_N_nodes(); i++)
    {
        outfile << obj->get_node(i).position << "\n";
    }

    // Write cell data
    const uint N_cells = (nx - 1) * (ny - 1) * (nz - 1);
    outfile << "CELLS " << N_cells << " " << 9 * N_cells << std::endl;
    for (int k = 0; k < nz - 1; ++k) {
        for (int j = 0; j < ny - 1; ++j) {
            for (int i = 0; i < nx - 1; ++i) {
                int node1 = i + j * nx + k * nx * ny;
                int node2 = node1 + 1;
                int node3 = node1 + nx;
                int node4 = node3 + 1;
                int node5 = node1 + nx * ny; 
                int node6 = node2 + nx * ny;
                int node7 = node3 + nx * ny;
                int node8 = node4 + nx * ny;
                outfile << "8 " << node1 << " " << node2 << " " << node4 << " " << node3 << " " << node5 << " " << node6 << " " << node8 << " " << node7 << std::endl;
            }
        }
    }
    outfile << "\n";

    // Write cell types
    outfile << "CELL_TYPES " << (nx - 1) * (ny - 1) * (nz - 1) << std::endl;
    for (int i = 0; i < (nx - 1) * (ny - 1) * (nz - 1); ++i) {
        outfile << "12 \n"; // VTK_HEXAHEDRON cell type
    }
    outfile << std::endl;

    // Write cell data
    // outfile << "CELL_DATA " << (nx - 1) * (ny - 1) * (nz - 1) << std::endl;
    // outfile << "SCALARS Volume float 1" << std::endl;
    // outfile << "LOOKUP_TABLE default" << std::endl;
    // for (int k = 0; k < nz - 1; ++k) {
    //     for (int j = 0; j < ny - 1; ++j) {
    //         for (int i = 0; i < nx - 1; ++i) {
    //             double volume = obj->get_volume(i, j, k);
    //             outfile << volume << std::endl;
    //         }
    //     }
    // }

    outfile.close();
}

void saveToVTK(const multi_block& blocks, const std::string& filename)
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
    outfile << "POINTS " << blocks.get_N_nodes() << " float" << std::endl;

    for(auto const& obj : blocks.blocks)
    {
        auto N = obj->get_size_nodes();
        int nx = N.x; int ny = N.y; int nz = N.z;

        // Write node positions
        for (int k = 0; k < nz; ++k) {
            for (int j = 0; j < ny; ++j) {
                for (int i = 0; i < nx; ++i) {
                    outfile << obj->get_node_position(i, j, k) << std::endl;
                }
            }
        }
        outfile << "\n";
    }

    // // Write cell data
    // const uint N_cells = blocks.get_N_cells();
    // outfile << "CELLS " << N_cells << " " << 9 * N_cells << std::endl;

    // for (int k = 0; k < nz - 1; ++k) {
    //     for (int j = 0; j < ny - 1; ++j) {
    //         for (int i = 0; i < nx - 1; ++i) {
    //             int node1 = i + j * nx + k * nx * ny;
    //             int node2 = node1 + 1;
    //             int node3 = node1 + nx;
    //             int node4 = node3 + 1;
    //             int node5 = node1 + nx * ny;
    //             int node6 = node2 + nx * ny;
    //             int node7 = node3 + nx * ny;
    //             int node8 = node4 + nx * ny;
    //             outfile << "8 " << node1 << " " << node2 << " " << node4 << " " << node3 << " " << node5 << " " << node6 << " " << node8 << " " << node7 << std::endl;
    //         }
    //     }
    // }
    // outfile << "\n";

    // // Write cell types
    // outfile << "CELL_TYPES " << (nx - 1) * (ny - 1) * (nz - 1) << std::endl;
    // for (int i = 0; i < (nx - 1) * (ny - 1) * (nz - 1); ++i) {
    //     outfile << "12 \n"; // VTK_HEXAHEDRON cell type
    // }
    // outfile << std::endl;

    // // Write cell data
    // // outfile << "CELL_DATA " << (nx - 1) * (ny - 1) * (nz - 1) << std::endl;
    // // outfile << "SCALARS Volume float 1" << std::endl;
    // // outfile << "LOOKUP_TABLE default" << std::endl;
    // // for (int k = 0; k < nz - 1; ++k) {
    // //     for (int j = 0; j < ny - 1; ++j) {
    // //         for (int i = 0; i < nx - 1; ++i) {
    // //             double volume = obj->get_volume(i, j, k);
    // //             outfile << volume << std::endl;
    // //         }
    // //     }
    // // }

    // outfile.close();
}
