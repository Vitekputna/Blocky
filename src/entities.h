#pragma once
#include <iostream>
#include <ostream>
#include <vector>

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec) {
    out << "[";
    if (!vec.empty()) {
        out << vec[0];
        for (size_t i = 1; i < vec.size(); ++i) {
            out << ", " << vec[i];
        }
    }
    out << "]";
    return out;
}

template<typename T>
struct point3 {
    T x, y, z;
    point3(){}
    point3(T x, T y, T z) : x{x}, y{y}, z{z} {}
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const point3<T>& point)
{
    out << point.x << " " << point.y << " " << point.z;
    return out;
}

typedef point3<double> point3d;
typedef point3<uint> point3ui;

template<typename T,int N>
struct list{
    T nodes[N];
    list(){}
};

template<typename T, int N>
std::ostream& operator<<(std::ostream& out, const list<T,N>& list)
{
    for (int i = 0; i < N; ++i) {
        out << list.nodes[i] << " ";
    }
    return out;
}

struct node{
    point3d position;
    node(point3d point) : position(point) {}
};

struct face{
    double face_size;
    point3d position;
    point3d normal;
    list<uint,4> face_vertices;
    face(){}
};

struct cell{
    point3d position;
    double volume;
    list<uint,8> cell_vertices;
    cell(){}
};