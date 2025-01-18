#include "mesh.h"
#include <iostream>
#include <cmath>
#include <limits.h>

using namespace std;

// Base class
mesh_base::mesh_base() {}
mesh_base::~mesh_base() {}
uint mesh_base::get_N_nodes() const { return nx * ny * nz; }
uint mesh_base::get_N_cells() const { return (nx - 1) * (ny - 1) * (nz - 1); }
uint mesh_base::get_N_faces() const { return (nx * (ny - 1) * (nz - 1) + ny * (nz - 1) * (nx - 1) + nz * (nx - 1) * (ny - 1)); }

double mesh_base::get_volume(uint i, uint j, uint k) const { return 0; }
point3ui mesh_base::get_size_nodes() const { return point3ui(nx, ny, nz); }
point3ui mesh_base::get_size_cells() const { return point3ui(nx - 1, ny - 1, nz - 1); }
point3d mesh_base::get_node_position(uint i, uint j, uint k) const { return point3d(0, 0, 0); }
point3d mesh_base::get_cell_position(uint i, uint j, uint k) const { return point3d(0, 0, 0); }
point3d mesh_base::get_xface_position(uint i, uint j, uint k) const { return point3d(0, 0, 0); }
point3d mesh_base::get_yface_position(uint i, uint j, uint k) const { return point3d(0, 0, 0); }
point3d mesh_base::get_zface_position(uint i, uint j, uint k) const { return point3d(0, 0, 0); }

node mesh_base::get_node(node_index index) const { return node(point3d(0, 0, 0)); }
face mesh_base::get_face(face_index index) const { return face(); }
cell mesh_base::get_cell(cell_index index) const { return cell(); }

list<uint, 6> mesh_base::get_node_neighbours(node_index idx) const { return list<uint, 6>(); }
face_ownership mesh_base::get_face_neighbours(face_index index) const { return face_ownership(); }
list<uint, 6> mesh_base::get_cell_neighbours(cell_index index) const { return list<uint, 6>(); }

node_index mesh_base::local_node_index(uint i, uint j, uint k) const { return node_index(i + j * nx + k * nx * ny); }
cell_index mesh_base::local_cell_index(uint i, uint j, uint k) const { return cell_index(i + j * (nx - 1) + k * (nx - 1) * (ny - 1)); }

point3ui mesh_base::node_coord(node_index idx) const
{
    uint index = idx.index;

    uint k = std::floor(index / (nx * ny));
    index = index - k * nx * ny;
    uint j = std::floor(index / nx);
    index = index - j * nx;
    uint i = index;

    return point3ui(i, j, k);
}
point3ui mesh_base::cell_coord(cell_index idx) const
{
    uint index = idx.index;

    uint k = std::floor(index / ((nx - 1) * (ny - 1)));
    index = index - k * (nx - 1) * (ny - 1);
    uint j = std::floor(index / (nx - 1));
    index = index - j * (nx - 1);
    uint i = index;

    return point3ui(i, j, k);
}

// Block class
simple_block::simple_block() {}
simple_block::~simple_block() {}

double simple_block::get_volume(uint i, uint j, uint k) const { return dx * dy * dz; }
point3d simple_block::get_node_position(uint i, uint j, uint k) const
{
    if (i >= nx || j >= ny || k >= nz)
    {
        cout << "Node out of bounds\n";
        return point3d(0, 0, 0);
    }

    return point3d(i * dx + x_min, j * dy + y_min, k * dz + z_min);
}
point3d simple_block::get_cell_position(uint i, uint j, uint k) const
{
    if (i >= nx - 1 || j >= ny - 1 || k >= nz - 1)
    {
        cout << "Cell out of bounds\n";
        return point3d(0, 0, 0);
    }

    return point3d(i * dx + dx / 2 + x_min, j * dy + dy / 2 + y_min, k * dz + dz / 2 + z_min);
}

point3d simple_block::get_xface_position(uint i, uint j, uint k) const
{
    return (get_node_position(i, j, k) + get_node_position(i, j + 1, k) + get_node_position(i, j, k + 1) + get_node_position(i, j + 1, k + 1)) * 0.25;
}

point3d simple_block::get_yface_position(uint i, uint j, uint k) const
{
    return (get_node_position(i, j, k) + get_node_position(i + 1, j, k) + get_node_position(i + 1, j, k + 1) + get_node_position(i, j, k + 1)) * 0.25;
}

point3d simple_block::get_zface_position(uint i, uint j, uint k) const
{
    return (get_node_position(i, j, k) + get_node_position(i + 1, j, k) + get_node_position(i + 1, j + 1, k) + get_node_position(i, j + 1, k)) * 0.25;
}

void simple_block::create_block(division x, division y, division z)
{
    // X-axis
    nx = x.n_nodes;
    dx = (x.max - x.min) / (nx - 1);
    x_min = x.min;
    x_max = x.max;

    // Y-axis
    ny = y.n_nodes;
    dy = (y.max - y.min) / (ny - 1);
    y_min = y.min;
    y_max = y.max;

    // Z-axis
    nz = z.n_nodes;
    dz = (z.max - z.min) / (nz - 1);
    z_min = z.min;
    z_max = z.max;
}

node simple_block::get_node(node_index idx) const
{
    uint index = idx.index;
    const point3ui size = this->get_size_nodes();
    const uint ny = size.y;
    const uint nx = size.x;

    uint k = std::floor(index / (nx * ny));
    index = index - k * nx * ny;
    uint j = std::floor(index / nx);
    index = index - j * nx;
    uint i = index;

    return node(get_node_position(i, j, k));
}

face simple_block::get_face(face_index idx) const
{
    face result;
    uint index = idx.index;

    const uint nfx = nx * (ny - 1) * (nz - 1);
    const uint nfy = ny * (nz - 1) * (nx - 1);

    // face position
    if (index >= 0 && index < nfx) // x face
    {
        // compute x face index
        const uint k = std::floor(index / (nx * (ny - 1)));
        const uint j = std::floor((index - k * nx * (ny - 1)) / nx);
        const uint i = index - k * nx * (ny - 1) - j * nx;

        result.position = get_xface_position(i, j, k);
        result.face_area = dy * dz;

        if(i == 0) result.boundary_idx = 0;
        else if (i == nx-1) result.boundary_idx = 1;

        result.face_vertices.nodes[0] = local_node_index(i,j,k).index;
        result.face_vertices.nodes[1] = local_node_index(i,j+1,k).index;
        result.face_vertices.nodes[2] = local_node_index(i,j,k+1).index;
        result.face_vertices.nodes[3] = local_node_index(i,j+1,k+1).index;

    }
    else if (index >= nfx && index < nfx + nfy) // y face
    {
        // compute y face
        index -= nfx;

        const uint k = std::floor(index / ((nx - 1) * ny));
        const uint j = std::floor((index - k * (nx - 1) * ny) / (nx - 1));
        const uint i = index - k * (nx - 1) * ny - j * (nx - 1);

        result.position = get_yface_position(i, j, k);
        result.face_area = dx * dz;

        if(j == 0) result.boundary_idx = 2;
        else if (j == ny-1) result.boundary_idx = 3;

        result.face_vertices.nodes[0] = local_node_index(i,j,k).index;
        result.face_vertices.nodes[1] = local_node_index(i+1,j,k).index;
        result.face_vertices.nodes[2] = local_node_index(i,j,k+1).index;
        result.face_vertices.nodes[3] = local_node_index(i+1,j,k+1).index;
    }
    else // z face
    {
        // compute z face
        index -= nfx + nfy;

        const uint k = std::floor(index / ((nx - 1) * (ny - 1)));
        const uint j = std::floor((index - k * (nx - 1) * (ny - 1)) / (nx - 1));
        const uint i = index - k * (nx - 1) * (ny - 1) - j * (nx - 1);

        result.position = get_zface_position(i, j, k);
        result.face_area = dx * dy;

        if(k == 0) result.boundary_idx = 4;
        else if (k == nz-1) result.boundary_idx = 5;

        result.face_vertices.nodes[0] = local_node_index(i,j,k).index;
        result.face_vertices.nodes[1] = local_node_index(i+1,j,k).index;
        result.face_vertices.nodes[2] = local_node_index(i,j+1,k).index;
        result.face_vertices.nodes[3] = local_node_index(i+1,j+1,k).index;
    }

    result.ownership = get_face_neighbours(idx);

    if (result.ownership.neighbour != std::numeric_limits<uint>::max())
    {
        auto O = cell_coord(result.ownership.owner);
        auto N = cell_coord(result.ownership.neighbour);
        result.normal = (get_cell_position(N.x, N.y, N.z) - get_cell_position(O.x, O.y, O.z)) * (1 / norm(get_cell_position(N.x, N.y, N.z) - get_cell_position(O.x, O.y, O.z)));
    }
    else result.normal = point3d(0,0,0);

    return result;
}

cell simple_block::get_cell(cell_index idx) const
{
    uint index = idx.index;
    point3ui size = this->get_size_cells();
    uint ny = size.y;
    uint nx = size.x;

    uint k = std::floor(index / (nx * ny));
    index = index - k * nx * ny;
    uint j = std::floor(index / nx);
    index = index - j * nx;
    uint i = index;

    cell res;

    size = this->get_size_nodes();
    ny = size.y;
    nx = size.x;

    uint node1 = i + j * nx + k * nx * ny;
    uint node2 = node1 + 1;
    uint node3 = node1 + nx + 1;
    uint node4 = node1 + nx;
    uint node5 = node1 + nx * ny;
    uint node6 = node2 + nx * ny;
    uint node7 = node3 + nx * ny;
    uint node8 = node4 + nx * ny;

    res.cell_vertices.nodes[0] = node1;
    res.cell_vertices.nodes[1] = node2;
    res.cell_vertices.nodes[2] = node3;
    res.cell_vertices.nodes[3] = node4;
    res.cell_vertices.nodes[4] = node5;
    res.cell_vertices.nodes[5] = node6;
    res.cell_vertices.nodes[6] = node7;
    res.cell_vertices.nodes[7] = node8;

    res.volume = this->get_volume(i, j, k);
    res.position = this->get_cell_position(i, j, k);

    return res;
}

list<uint, 6> simple_block::get_node_neighbours(node_index idx) const
{
    list<uint, 6> neighbours;
    const uint index = idx.index;

    // check boundary nodes
    point3ui coord = node_coord(index);

    if (coord.x == 0 || coord.x == nx - 1 ||
        coord.y == 0 || coord.y == ny - 1 ||
        coord.z == 0 || coord.z == nz - 1)
    {
        return neighbours;
    }

    neighbours.nodes[0] = index + 1;       // front
    neighbours.nodes[1] = index - 1;       // back
    neighbours.nodes[2] = index + nx;      // left
    neighbours.nodes[3] = index - nx;      // right
    neighbours.nodes[4] = index + nx * ny; // top
    neighbours.nodes[6] = index - nx * ny; // bottom

    return neighbours;
}

face_ownership simple_block::get_face_neighbours(face_index idx) const
{
    uint index = idx.index;

    const uint nfx = nx * (ny - 1) * (nz - 1);
    const uint nfy = ny * (nz - 1) * (nx - 1);

    if (index >= 0 && index < nfx) // x face
    {
        // compute x face index
        const uint k = std::floor(index / (nx * (ny - 1)));
        const uint j = std::floor((index - k * nx * (ny - 1)) / nx);
        const uint i = index - k * nx * (ny - 1) - j * nx;

        face_ownership res;

        auto face_pos = get_xface_position(i, j, k);

        if (face_pos.x == x_min) // left edge
        {
            res.owner = local_cell_index(i, j, k).index; // Owner
            res.neighbour = std::numeric_limits<uint>::max();                          // Neighbour
        }
        else if (face_pos.x == x_max) // right edge
        {
            res.owner = local_cell_index(i - 1, j, k).index; // Owner
            res.neighbour = std::numeric_limits<uint>::max();                              // Neigh
        }
        else
        {
            res.owner = local_cell_index(i - 1, j, k).index; // Owner
            res.neighbour = local_cell_index(i, j, k).index; // Neighbour
        }

        return res;
    }
    else if (index >= nfx && index < nfx + nfy) // y face
    {
        // compute y face
        index -= nfx;

        const uint k = std::floor(index / ((nx - 1) * ny));
        const uint j = std::floor((index - k * (nx - 1) * ny) / (nx - 1));
        const uint i = index - k * (nx - 1) * ny - j * (nx - 1);

        face_ownership res;

        auto face_pos = get_yface_position(i, j, k);

        if (face_pos.y == y_min) // left edge
        {
            res.owner = local_cell_index(i, j, k).index; // Owner
            res.neighbour = std::numeric_limits<uint>::max();                          // Neighbour
        }
        else if (face_pos.y == y_max) // right edge
        {
            res.owner = local_cell_index(i, j - 1, k).index; // Owner
            res.neighbour = std::numeric_limits<uint>::max();                              // Neigh
        }
        else
        {
            res.owner = local_cell_index(i, j - 1, k).index; // Owner
            res.neighbour = local_cell_index(i, j, k).index; // Neighbour
        }

        return res;
    }
    else // z face
    {
        // compute z face
        index -= nfx + nfy;

        const uint k = std::floor(index / ((nx - 1) * (ny - 1)));
        const uint j = std::floor((index - k * (nx - 1) * (ny - 1)) / (nx - 1));
        const uint i = index - k * (nx - 1) * (ny - 1) - j * (nx - 1);

        face_ownership res;

        auto face_pos = get_zface_position(i, j, k);

        if (face_pos.z == z_min) // left edge
        {
            res.owner = local_cell_index(i, j, k).index; // Owner
            res.neighbour = std::numeric_limits<uint>::max();                          // Neighbour
        }
        else if (face_pos.z == z_max) // right edge
        {
            res.owner = local_cell_index(i, j, k - 1).index; // Owner
            res.neighbour = std::numeric_limits<uint>::max();                              // Neigh
        }
        else
        {
            res.owner = local_cell_index(i, j, k - 1).index; // Owner
            res.neighbour = local_cell_index(i, j, k).index; // Neighbour
        }

        return res;
    }
}

// Transformed block
// transformed_block::transformed_block() {}
// transformed_block::~transformed_block() {}

// void transformed_block::add_transformation(transformation *T) { trans.push_back(T); }

// double transformed_block::get_volume(uint i, uint j, uint k) const
// {
//     double J = 1;
//     point3d point(i * dx + dx / 2 + x_min, j * dy + dy / 2 + y_min, k * dz + dz / 2 + z_min);

//     for (auto const &T : trans)
//     {
//         J *= T->jacobian(point);
//         point = T->get_point(point);
//     }

//     return J * dx * dy * dz;
// }

// point3d transformed_block::get_node_position(uint i, uint j, uint k) const
// {
//     if (i >= nx || j >= ny || k >= nz)
//     {
//         cout << "Node out of bounds\n";
//         return point3d(0, 0, 0);
//     }

//     point3d point(i * dx + x_min, j * dy + y_min, k * dz + z_min);

//     for (auto const &T : trans)
//     {
//         point = T->get_point(point);
//     }

//     return point;
// }
// point3d transformed_block::get_cell_position(uint i, uint j, uint k) const
// {
//     if (i >= nx - 1 || j >= ny - 1 || k >= nz - 1)
//     {
//         cout << "Cell out of bounds\n";
//         return point3d(0, 0, 0);
//     }

//     point3d point(i * dx + dx / 2 + x_min, j * dy + dy / 2 + y_min, k * dz + dz / 2 + z_min);

//     for (auto const &T : trans)
//     {
//         point = T->get_point(point);
//     }

//     return point;
// }
