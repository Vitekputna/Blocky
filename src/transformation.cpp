#include "transformations.h"
#include <cmath>

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
nonlinear_transformation::nonlinear_transformation(point3d(*point_transfomation)(const point3d& point),double(*jacobian)(const point3d& point))
{
    trans = point_transfomation;
    jacob = jacobian;
}
point3d nonlinear_transformation::get_point(const point3d& point) const {return trans(point);}
double nonlinear_transformation::jacobian(const point3d& point) const {return jacob(point);}


point3d nonlinear_transformations::exponential_scaling::transformed_point(const point3d& point,
                                                                          const point3d& end_point,
                                                                          const point3d& direction)
{
    return point3d(point.x * std::exp(direction.x * point.x) / std::exp(direction.x * end_point.x),
                   point.y * std::exp(direction.y * point.y) / std::exp(direction.y * end_point.y),
                   point.z * std::exp(direction.z * point.z) / std::exp(direction.z * end_point.z));
}

double nonlinear_transformations::exponential_scaling::jacobian(const point3d& point)
{
    return 1;
}


point3d nonlinear_transformations::cylindrical_transformation::transformed_point(const point3d& point, double alpha, double r)
{
    return point3d((point.x + r) * cos(alpha * point.y),
                   (point.x + r) * sin(alpha * point.y),
                    point.z);
}

double nonlinear_transformations::cylindrical_transformation::jacobian(const point3d& point)
{
    return 1;
}