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