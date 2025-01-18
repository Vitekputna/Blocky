#include <iostream>
// #include "src/entities.h"
#include "src/mesh.h"
#include "src/vtk_exporter.h"
// #include "src/transformations.h"

int main()
{
//     transformed_block mesh;

    // auto R = linear_transformation(transformation_matrix(std::vector<double>{1,0,1,0,1,0,-1,0,1}));

    // auto E = nonlinear_transformation(
    //     [](const point3d& a){return nonlinear_transformations::exponential_scaling::transformed_point(a,point3d(2,1,1),point3d(1,0,0));},nonlinear_transformations::exponential_scaling::jacobian);

    // auto T = nonlinear_transformation(
    //     [](const point3d& a){return nonlinear_transformations::cylindrical_transformation::transformed_point(a,2*3.14159,1);},nonlinear_transformations::cylindrical_transformation::jacobian);

    // mesh.add_transformation(&R);
    // mesh.add_transformation(&E);
    // mesh.add_transformation(&T);

    // simple_block mesh1;
    // mesh1.create_block(division(0,1,6),division(0,1,6),division(0,1,6));

    // transformed_block mesh2;
    // mesh2.create_block(division(1,2,10),division(0,2,10),division(0,1,10));
    // // mesh2.add_transformation(&E);

    // multi_block mesh;
    // mesh.add_block(&mesh1);
    // mesh.add_block(&mesh2);

    // saveToVTK(&mesh,"test.vtk");

    simple_block mesh;
    mesh.create_block(division(0, 1, 11), division(0, 1, 2), division(0, 1, 2));

    for (uint i = 0; i < mesh.get_N_cells(); i++)
    {
        std::cout << mesh.get_cell(i) << "\n\n";
    }

    for (uint i = 0; i < mesh.get_N_faces(); i++)
    {
        std::cout << mesh.get_face(i) << "\n\n";
    }

    saveToVTK(&mesh, "test.vtk");

    return 0;
}