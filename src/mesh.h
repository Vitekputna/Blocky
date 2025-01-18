#pragma once
#include <vector>
#include <iterator>
#include "entities.h"
// #include "transformations.h"

typedef unsigned int uint;

// Base mesh class
class mesh_base
{
    protected:
    uint nx, ny, nz;                // Number of nodes in each axis
    double dx, dy, dz;              // Nodal distance in each axis
    double x_min, y_min, z_min;     // Min positions
    double x_max, y_max, z_max;     // Max positions

    public:
    mesh_base();        //Constructor
    ~mesh_base();       //Destructor

    virtual uint get_N_nodes() const;
    virtual uint get_N_cells() const;
    virtual uint get_N_faces() const;

    virtual point3ui get_size_nodes() const;
    virtual point3ui get_size_cells() const;

    virtual double get_volume(uint i, uint j, uint k) const = 0;
    virtual point3d get_node_position(uint i, uint j, uint k) const;
    virtual point3d get_cell_position(uint i, uint j, uint k) const;
    virtual point3d get_xface_position(uint i, uint j, uint k) const;
    virtual point3d get_yface_position(uint i, uint j, uint k) const;
    virtual point3d get_zface_position(uint i, uint j, uint k) const;

    // entity access
    virtual node get_node(node_index idx) const;
    virtual face get_face(face_index idx) const;
    virtual cell get_cell(cell_index idx) const;

    // adjacency 
    virtual list<uint,6> get_node_neighbours(node_index index) const;
    virtual face_ownership get_face_neighbours(face_index index) const;
    virtual list<uint,6> get_cell_neighbours(cell_index index) const;

    // index conversion
    node_index local_node_index(uint i, uint j, uint k) const;
    cell_index local_cell_index(uint i, uint j, uint k) const;

    point3ui node_coord(node_index idx) const;
    point3ui cell_coord(cell_index idx) const;
    
};

// Simple block class

struct division
{
    double min, max, n_nodes;
    division(double min, double max, double n) : min{min}, max{max}, n_nodes{n} {};
};

/* Creates a cartesian block mesh
 positions, areas and volumes are computed */
class simple_block : public mesh_base
{
    public:
    simple_block();
    ~simple_block();

    //Scalar operators
    double get_volume(uint i, uint j, uint k) const;
    
    point3d get_node_position(uint i, uint j, uint k) const;
    point3d get_cell_position(uint i, uint j, uint k) const;

    point3d get_xface_position(uint i, uint j, uint k) const;
    point3d get_yface_position(uint i, uint j, uint k) const;
    point3d get_zface_position(uint i, uint j, uint k) const;

    node get_node(node_index idx) const;
    face get_face(face_index idx) const;
    cell get_cell(cell_index idx) const;

    list<uint,6> get_node_neighbours(node_index index) const;
    face_ownership get_face_neighbours(face_index index) const;
    list<uint,6> get_cell_neighbours(cell_index index) const {};

    /*Create a cartesian block mesh with given bounds and number of nodes in each axis*/
    void create_block(division x, division y = division(0,1,2), division z = division(0,1,2));
};

// // Transformed block class
// class transformed_block : public simple_block
// {
//     public:
//     // Transformation
//     std::vector<transformation*> trans;

//     transformed_block();
//     ~transformed_block();

//     void add_transformation(transformation* T);

//     // uint get_N_nodes() const;
//     double get_volume(uint i, uint j, uint k) const;
//     // point3ui get_size_nodes() const;
//     // point3ui get_size_cells() const;
//     point3d get_node_position(uint i, uint j, uint k) const;
//     point3d get_cell_position(uint i, uint j, uint k) const;

//     list<uint,6> get_node_neighbours(node_index index) const {};
//     face_ownership get_face_neighbours(face_index index) const {};
//     list<uint,6> get_cell_neighbours(cell_index index) const {};
// };

