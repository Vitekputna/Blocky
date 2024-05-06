#include <iostream>
#include "src/mesh.h"
#include "src/vtk_exporter.h"
#include "src/transformations.h"

int main()
{
    transformed_block mesh;

    auto R = linear_transformation(transformation_matrix(std::vector<double>{1,0,1,0,1,0,-1,0,1}));
    
    auto E = nonlinear_transformation(
        [](const point3d& a){return nonlinear_transformations::exponential_scaling::transformed_point(a,point3d(1,1,1),point3d(0,0,1));},nonlinear_transformations::exponential_scaling::jacobian);

    auto T = nonlinear_transformation(
        [](const point3d& a){return nonlinear_transformations::cylindrical_transformation::transformed_point(a,2*3.14159,1);},nonlinear_transformations::cylindrical_transformation::jacobian);

    mesh.add_transformation(&R);
    mesh.add_transformation(&E);
    // mesh.add_transformation(&T);

    mesh.create_block(division(0,1,51),division(0,1,201),division(0,1,51));

    saveToVTK(&mesh,"test.vtk");

    return 0;
}