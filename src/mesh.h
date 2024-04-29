#pragma once
#include <vector>
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

    virtual double get_volume(uint i, uint j, uint k) const = 0;
    virtual point3ui get_size() const;
    virtual point3d get_node_position(uint i, uint j, uint k) const;
    virtual point3d get_cell_position(uint i, uint j, uint k) const;

    // //Vector operators
    // virtual std::vector<uint> get_node_neighbours(uint i, uint j, uint k);
    // virtual std::vector<uint> get_cell_neighbours(uint i, uint j, uint k);

    // //Dimension and size
    // virtual bool set_dimension();
    // virtual uint get_dimension();
    // virtual uint get_size();
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
    point3ui get_size() const;
    point3d get_node_position(uint i, uint j, uint k) const;
    point3d get_cell_position(uint i, uint j, uint k) const;

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
    transformation* trans;

    transformed_block();
    ~transformed_block();

    void set_transformation(transformation* T);
    double get_volume(uint i, uint j, uint k) const;
    point3ui get_size() const;
    point3d get_node_position(uint i, uint j, uint k) const;
    point3d get_cell_position(uint i, uint j, uint k) const;
};
