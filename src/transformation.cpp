#include "transformations.h"

point3d operator*(const transformation_matrix& T, const point3d& point)
{
    point3d result;

    result.x = T.A[0]*point.x + T.A[1]*point.y + T.A[2]*point.z;
    result.y = T.A[3]*point.x + T.A[4]*point.y + T.A[5]*point.z;
    result.z = T.A[6]*point.x + T.A[7]*point.y + T.A[8]*point.z;

    return result;
}

// Base class transformation
transformation::transformation(){}
point3d transformation::get_point(const point3d& point) const {return point3d(0,0,0);}
double transformation::jacobian(const point3d& point) const {return 0;}

// Linear transformation
linear_transformation::linear_transformation(transformation_matrix T) : matrix{T} {}
point3d linear_transformation::get_point(const point3d& point) const {return matrix*point;}
double linear_transformation::jacobian(const point3d& point) const {return matrix.J;}

// Nonlinear transformation
nonlinear_transformation::nonlinear_transformation(){}
nonlinear_transformation::get_point(const point3d& point) const {return trans(point);}
nonlinear_transformation::jacobian(const point3d& point) const {return jacob(point);}
