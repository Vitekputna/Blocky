#pragma once
#include <iostream>
#include <ostream>
#include <vector>
#include <cmath>

template <typename T>
std::ostream &operator<<(std::ostream &out, const std::vector<T> &vec)
{
    out << "[";
    if (!vec.empty())
    {
        out << vec[0];
        for (size_t i = 1; i < vec.size(); ++i)
        {
            out << ", " << vec[i];
        }
    }
    out << "]";
    return out;
}

// Point

template <typename T>
struct point3
{
    T x, y, z;
    point3() {}
    point3(T x, T y, T z) : x{x}, y{y}, z{z} {}
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const point3<T> &point)
{
    out << point.x << " " << point.y << " " << point.z;
    return out;
}

template <typename T>
point3<T> operator+(const point3<T> &point_1, const point3<T> &point_2)
{
    point3<T> result;

    result.x = point_1.x + point_2.x;
    result.y = point_1.y + point_2.y;
    result.z = point_1.z + point_2.z;

    return result;
}

template <typename T>
point3<T> operator-(const point3<T> &point_1, const point3<T> &point_2)
{
    point3<T> result;

    result.x = point_1.x - point_2.x;
    result.y = point_1.y - point_2.y;
    result.z = point_1.z - point_2.z;

    return result;
}

template <typename T>
point3<T> operator*(const point3<T> &point_1, const T val)
{
    point3<T> result;

    result.x = point_1.x * val;
    result.y = point_1.y * val;
    result.z = point_1.z * val;

    return result;
}

double norm(const point3<double> &point);

typedef point3<double> point3d;
typedef point3<uint> point3ui;

// List

template <typename T, int N>
struct list
{
    T nodes[N];
    list() {}
};

template <typename T, int N>
std::ostream &operator<<(std::ostream &out, const list<T, N> &list)
{
    for (int i = 0; i < N; ++i)
    {
        out << list.nodes[i] << " ";
    }
    return out;
}

template <typename T, int N>
list<T, N> operator+(const list<T, N> &A, T b)
{
    list<T, N> out;

    for (int i = 0; i < N; ++i)
    {
        out.nodes[i] = A.nodes[i] + b;
    }

    return out;
}

// Face ownership struct
struct face_ownership
{
    uint owner;
    uint neighbour;

    face_ownership() {};
    face_ownership(uint owner, uint neighbour) : owner(owner), neighbour(neighbour) {};
};

// Mesh entities

struct node
{
    point3d position;
    node(point3d point) : position(point) {}
};

struct face
{
    double face_area;
    point3d position;
    point3d normal;
    face_ownership ownership;
    list<uint, 4> face_vertices;
    int boundary_idx = -1;

    /*
    z towards you
    x: 0 left 1 right
    y: 2 bottom 3 top
    z: 4 back 5 front
    -1 Not a boundary wall
    */

    face() {}
    face(double area, point3d pos,
         point3d norm, list<uint, 4> vert)
        : face_area(area), position(pos),
          normal(norm), face_vertices(vert) {}
};

struct cell
{
    double volume;
    point3d position;
    list<uint, 8> cell_vertices;
    cell() {}
    cell(double vol,
         point3d pos,
         list<uint, 8> vert)
        : volume(vol), position(pos),
          cell_vertices(vert) {}
};

std::ostream &operator<<(std::ostream &out, const face &f);
std::ostream &operator<<(std::ostream &out, const cell &c);

// Entitie indices

struct node_index
{
    uint index;

    node_index(uint val) : index(val) {}

    void operator++() { index++; }
    void operator++(int) { index++; }

    friend bool operator<(node_index a, node_index b);
    friend bool operator>(node_index a, node_index b);
    friend bool operator==(node_index a, node_index b);
};

struct face_index
{
    uint index;

    face_index(uint val) : index(val) {}

    void operator++() { index++; }
    void operator++(int) { index++; }

    friend bool operator<(face_index a, face_index b);
    friend bool operator>(face_index a, face_index b);
    friend bool operator==(face_index a, face_index b);
};

struct cell_index
{
    uint index;

    cell_index(uint val) : index(val) {}

    void operator++() { index++; }
    void operator++(int) { index++; }

    friend bool operator<(cell_index a, cell_index b);
    friend bool operator>(cell_index a, cell_index b);
    friend bool operator==(cell_index a, cell_index b);
};