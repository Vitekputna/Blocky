#pragma once
#include "entities.h"

// Linear transformation
struct transformation_matrix
{
    double J;
    std::vector<double> A;
    transformation_matrix(){}
    transformation_matrix(std::vector<double> matrix)
    {
        if(matrix.size() > 9)
        {
            std::cout << "Matrix too big\n";
            exit(1);
        }
        A = matrix;
        
        J = A[0]*A[4]*A[8] + A[3]*A[7]*A[2] + A[1]*A[5]*A[6]
          - A[2]*A[4]*A[6] - A[0]*A[5]*A[7] - A[1]*A[3]*A[8];
    }
};

point3d operator*(const transformation_matrix& T, const point3d& point);


// Nonlinear transformations



struct transformation
{
    transformation();

    virtual point3d get_point(const point3d& point) const = 0;
    virtual double jacobian(const point3d& point) const;
};

struct linear_transformation : public transformation
{
    private:
    transformation_matrix matrix;   

    public:
    linear_transformation(transformation_matrix matrix);

    point3d get_point(const point3d& point) const;
    double jacobian(const point3d& point) const;
};

struct nonlinear_transformation : public transformation
{
    private:
    point3d(*trans)(const point3d& point);
    double(*jacob)(const point3d& point);

    public:
    nonlinear_transformation(point3d(*trans)(const point3d& point),double(*jacob)(const point3d& point));

    point3d get_point(const point3d& point) const;
    double jacobian(const point3d& point) const;
};
