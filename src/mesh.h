#pragma once
#include <vector>
#include <iterator>
#include "entities.h"
#include "transformations.h"

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
    virtual double get_volume(uint i, uint j, uint k) const = 0;
    virtual point3ui get_size_nodes() const;
    virtual point3ui get_size_cells() const;
    virtual point3d get_node_position(uint i, uint j, uint k) const;
    virtual point3d get_cell_position(uint i, uint j, uint k) const;

    // entity access
    virtual node get_node(uint index) const;
    virtual face get_face(uint index) const;
    virtual cell get_cell(uint index) const;


    // //Vector operators
    // virtual std::vector<uint> get_node_neighbours(uint i, uint j, uint k);
    // virtual std::vector<uint> get_cell_neighbours(uint i, uint j, uint k);

    // //Dimension and size
    // virtual bool set_dimension();
    // virtual uint get_dimension();
    // virtual uint get_size();

    template<typename entity>
    struct iterator{
        using interator_category = std::random_access_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type = entity;
        using pointer = entity*;
        using reference = entity&;

        iterator(uint idx) : index(idx) {}

        // reference operator*() const {}

        private:
        uint index;
    };
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
    // uint get_N_nodes() const;
    double get_volume(uint i, uint j, uint k) const;
    // point3ui get_size_nodes() const;
    // point3ui get_size_cells() const;
    point3d get_node_position(uint i, uint j, uint k) const;
    point3d get_cell_position(uint i, uint j, uint k) const;

    node get_node(uint index) const;
    face get_face(uint index) const;
    cell get_cell(uint index) const;

    // //Vector operators
    // virtual std::vector<uint> get_node_neighbours(uint i, uint j, uint k);
    // virtual std::vector<uint> get_cell_neighbours(uint i, uint j, uint k);

    // //Dimension and size
    // virtual bool set_dimension();
    // virtual uint get_dimension();
    // virtual uint get_size();

    /*Create a cartesian block mesh with given bounds and number of nodes in each axis*/
    void create_block(division x, division y = division(0,1,2), division z = division(0,1,2));
};

// Transformed block class
class transformed_block : public simple_block
{
    public:
    // Transformation
    std::vector<transformation*> trans;

    transformed_block();
    ~transformed_block();

    void add_transformation(transformation* T);

    // uint get_N_nodes() const;
    double get_volume(uint i, uint j, uint k) const;
    // point3ui get_size_nodes() const;
    // point3ui get_size_cells() const;
    point3d get_node_position(uint i, uint j, uint k) const;
    point3d get_cell_position(uint i, uint j, uint k) const;
};


// Mesh of multiple blocks
class multi_block
{
    protected:

    public:
    // mesh block storage
    std::vector<mesh_base*> blocks;

    multi_block();
    ~multi_block();

    void add_block(mesh_base* block);

    uint get_N_cells() const;
    uint get_N_blocks() const;
    uint get_N_nodes() const;
};