#include "entities.h"

bool operator<(node_index a, node_index b){return a.index < b.index;}
bool operator>(node_index a, node_index b){return a.index > b.index;}
bool operator==(node_index a, node_index b){return a.index == b.index;}

bool operator<(face_index a, face_index b){return a.index < b.index;}
bool operator>(face_index a, face_index b){return a.index > b.index;}
bool operator==(face_index a, face_index b){return a.index == b.index;}


bool operator<(cell_index a, cell_index b){return a.index < b.index;}
bool operator>(cell_index a, cell_index b){return a.index > b.index;}
bool operator==(cell_index a, cell_index b){return a.index == b.index;}

std::ostream& operator<<(std::ostream& out, const face& f)
{
    out << "face:\n";
    out << "Area\t" << f.face_area << "\n";
    out << "Position\t" << f.position << "\n";
    out << "Normal\t" << f.normal << "\n";
    out << "owner\t" << f.ownership.owner << "\n";
    out << "neighbour\t" << f.ownership.neighbour << "\n";
    out << "boundary idx\t" << f.boundary_idx << "\n";
    out << "end face";
    return out;
}

std::ostream& operator<<(std::ostream& out, const cell& c) {
    out << "cell:\n";
    
    out << "Volume\t" << c.volume << "\n";
    out << "Position\t" << c.position << "\n";

    out << "end cell";
    return out;
}

double norm(const point3<double> &point)
{
    return std::sqrt(point.x * point.x + point.y * point.y + point.z * point.z);
}