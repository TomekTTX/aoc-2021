#include <iostream>
#include <unordered_map>
#include <vector>
#include <cmath>

inline constexpr int sgn(int x) {
    return (0 < x) - (x < 0);
}

struct Point {
    int x, y;
    
    std::vector<Point> line_to(const Point &other) const {
        std::vector<Point> ret{};   
        const int xdiff = other.x - x, ydiff = other.y - y;
        const int dx = sgn(xdiff), dy = sgn(ydiff);
        int iter = std::max(abs(xdiff), abs(ydiff)) + 1;
        
        //if (xdiff != 0 && ydiff != 0)
        //  return ret;
        
        ret.reserve(iter);
        for (int xp = x, yp = y; iter-- && *this != other; xp += dx, yp += dy)
            ret.push_back(Point{xp, yp});
        
        return ret;
    }
    
    friend std::istream& operator>>(std::istream &ist, Point &p) {
        char discard;
        ist >> p.x >> discard >> p.y;
        return ist;
    }   
    
    friend std::ostream& operator<<(std::ostream &ost, const Point &p) {
        ost << '(' << p.x << ',' << p.y << ')';
        return ost;
    }   

    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Point &other) const {
        return !(*this == other);
    }
};

namespace std {
    template <>
    struct hash<Point>  {
        std::size_t operator()(const Point &p) const {
            return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
        }
    };
}

struct VentDiagram {
    Point end{0,0};
    std::unordered_map<Point, int> diagram{};
    
    void add_point(const Point &p) {        
        if (diagram.count(p) == 0) {
            diagram[p] = 1;
            end.x = std::max(p.x, end.x);
            end.y = std::max(p.y, end.y);
        }
        else
            diagram[p]++;
    }
    
    void add_line(const std::vector<Point> &line) {
        for (const auto &p : line)
            add_point(p);
    }
    
    int count_overlaps() const {
        int ret = 0;
        
        for (const auto &a : diagram)
            ret += (a.second > 1);
            
        return ret;
    }
    
    friend std::ostream& operator<<(std::ostream &ost, const VentDiagram &diag) {
        for (int y = 0; y <= diag.end.y; ++y) {
            ost << '\n';
            for (int x = 0; x <= diag.end.x; ++x) {
                const Point p{x, y};
                if (diag.diagram.count(p) != 0)
                    ost << diag.diagram.at(p);
                else
                    ost << '.';
            }
        }
        return ost;
    }   
};


int main() {
    char discard[3];
    Point p1, p2;
    VentDiagram diagram;
    
    while (std::cin >> p1 >> discard >> p2)
        diagram.add_line(p1.line_to(p2));
    
    //std::cout << diagram << "\n\n";
    std::cout << diagram.count_overlaps() << " overlaps\n";
}















