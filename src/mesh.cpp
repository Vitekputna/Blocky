#include "mesh.h"
#include <iostream>
using namespace std;

// Base class
mesh_base::mesh_base(){cout << "Mesh base constructor\n";}
mesh_base::~mesh_base(){cout << "Mesh base destructor\n";}
double mesh_base::get_volume(uint i, uint j, uint k) const {return 0;}
point3ui mesh_base::get_size() const
{
    return point3ui(nx,ny,nz);
}
point3d mesh_base::get_node_position(uint i, uint j, uint k) const {return point3d(0,0,0);}
point3d mesh_base::get_cell_position(uint i, uint j, uint k) const {return point3d(0,0,0);}


// Block class
simple_block::simple_block(){cout << "Mesh block constructor\n";}
simple_block::~simple_block(){cout << "Mesh block destructor\n";}

double simple_block::get_volume(uint i, uint j, uint k) const {return dx*dy*dz;}
point3ui simple_block::get_size() const {return point3ui(nx,ny,nz);}
point3d simple_block::get_node_position(uint i, uint j, uint k) const
{
    if(i >= nx || j >= ny || k >= nz)
    {
        cout << "Node out of bounds\n";
        return point3d(0,0,0);
    }

    return point3d( i*dx + x_min, j*dy + y_min, k*dz + z_min );
}
point3d simple_block::get_cell_position(uint i, uint j, uint k) const
{
    if(i >= nx-1 || j >= ny-1 || k >= nz-1)
    {
        cout << "Cell out of bounds\n";
        return point3d(0,0,0);
    }

    return point3d( i*dx + dx/2 + x_min, j*dy + dy/2 + y_min, k*dz + dz/2 + z_min);
}
void simple_block::create_block(division x, division y, division z)
{
    // X-axis
    nx = x.n_nodes;
    dx = ( x.max - x.min ) / (nx - 1);
    x_min = x.min; x_max = x.max;

    // Y-axis
    ny = y.n_nodes;
    dy = ( y.max - y.min ) / (ny - 1);
    y_min = y.min; y_max = y.max;

    // Z-axis
    nz = z.n_nodes;
    dz = ( z.max - z.min ) / (nz - 1);
    z_min = z.min; z_max = z.max;
}


// Transformed block
transformed_block::transformed_block(){}
transformed_block::~transformed_block(){}

void transformed_block::add_transformation(transformation* T) {trans.push_back(T);}

double transformed_block::get_volume(uint i, uint j, uint k) const 
{
    double J = 1;
    point3d point( i*dx + dx/2 + x_min, j*dy + dy/2 + y_min, k*dz + dz/2 + z_min);

    for(auto const& T : trans) {
        J *= T->jacobian(point);
        point = T->get_point(point);
    }

    return J*dx*dy*dz;
}
point3ui transformed_block::get_size() const {return point3ui(nx,ny,nz);}

point3d transformed_block::get_node_position(uint i, uint j, uint k) const 
{
    if(i >= nx || j >= ny || k >= nz)
    {
        cout << "Node out of bounds\n";
        return point3d(0,0,0);
    }

    point3d point( i*dx + x_min, j*dy + y_min, k*dz + z_min );

    for(auto const& T : trans) {point = T->get_point(point);}

    return point;
}
point3d transformed_block::get_cell_position(uint i, uint j, uint k) const 
{
    if(i >= nx-1 || j >= ny-1 || k >= nz-1)
    {
        cout << "Cell out of bounds\n";
        return point3d(0,0,0);
    }

    point3d point( i*dx + dx/2 + x_min, j*dy + dy/2 + y_min, k*dz + dz/2 + z_min);

    for(auto const& T : trans) {point = T->get_point(point);}

    return point;
}

