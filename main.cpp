#include <iostream>
#include "src/mesh.h"
#include "src/vtk_exporter.h"
#include "src/transformation.h"

int main()
{
    transformed_block mesh;
    // auto T = linear_transformation(transformation_matrix(std::vector<double>{1,0,1,0,1,0,-1,0,1}));
    
    auto T = nonlinear_transformation();

    mesh.set_transformation(&T);
    mesh.create_block(division(0,1,51),division(0,1,51),division(0,1,51));

    saveToVTK(&mesh,"test.vtk");

    return 0;
}
