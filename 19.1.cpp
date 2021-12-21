#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <unordered_map>
#include <unordered_set>

struct Point {
    int x, y, z;
    
    std::array<Point, 24> interp() const {
        return {{
            {x,y,z}, {x,-y,-z}, {x,-z,y}, {x,z,-y},
            {-x,z,y}, {-x,-z,-y}, {-x,-y,z}, {-x,y,-z},
            
            {y,z,x}, {y,-z,-x}, {y,-x,z}, {y,x,-z},
            {-y,x,z}, {-y,-x,-z}, {-y,-z,x}, {-y,z,-x},
            
            {z,x,y}, {z,-x,-y}, {z,-y,x}, {z,y,-x},
            {-z,y,x}, {-z,-y,-x}, {-z,-x,y}, {-z,x,-y},
        }};
    }
    
    std::array<Point, 24> relation_to(const Point &other) const {
        std::array<Point, 24> ret{};
        
        int ctr = 0;
        for (const auto &p : other.interp())
            ret[ctr++] = (*this - p).abs();
                
        return ret;
    }
    
    void rotate(int rotate_no) {
        const Point new_pt = interp()[rotate_no];
        x = new_pt.x;
        y = new_pt.y;
        z = new_pt.z;
    }
    
    const Point& rotated(int rotate_no) const {
        return interp()[rotate_no];
    }
    
    inline Point abs() const {
        return { std::abs(x), std::abs(y), std::abs(z) };       
    }
    
    inline Point operator+(const Point &other) const {
        return {
            x + other.x,
            y + other.y,
            z + other.z,
        };
    }   
    
    inline Point operator-(const Point &other) const {
        return {
            x - other.x,
            y - other.y,
            z - other.z,
        };
    }
    
    inline Point operator-() const {
        return {-x,-y,-z};
    }
    
    inline Point operator+=(const Point &other) {
        return {
            x += other.x,
            y += other.y,
            z += other.z,
        };
    }
    
    inline Point operator-=(const Point &other) {
        return {
            x -= other.x,
            y -= other.y,
            z -= other.z,
        };
    }
    
    inline bool operator==(const Point &other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    friend std::istream& operator>>(std::istream &ist, Point &p) {
        char d;
        ist >> p.x >> d >> p.y >> d >> p.z;
        return ist;
    }
    
    friend std::ostream& operator<<(std::ostream &ost, const Point &p) {
        ost << p.x << ',' << p.y << ',' << p.z;
        return ost;
    }
};

struct Transformation {
    int rotation = 0;
    Point translation = {0,0,0};
};

struct Scanner {
    int id = -1;
    std::vector<Point> points{};
        
    inline static bool valid_init(int ch) {
        return ch == '-' || (ch >= '0' && ch <= '9');
    }
    
    void transform(const Transformation &t) {
        for (auto &p : points) {
            p.rotate(t.rotation);
            p += t.translation;
        }
    }
    
    inline bool operator==(const Scanner &other) const {
        return id == other.id;
    }
        
    friend std::istream& operator>>(std::istream &ist, Scanner &s) {
        Point p;
        
        ist.ignore(~0U, 'r');
        ist >> s.id;
        ist.ignore(5);
        
        s.points.clear();
        do {
            ist >> p;
            s.points.push_back(p);
            if (ist.get() == '\r')
                ist.get();
        } while (valid_init(ist.peek()));
            
        return ist;
    }
    
    friend std::ostream& operator<<(std::ostream &ost, const Scanner &s) {
        ost << "=== scanner " << s.id << " ===\n";
        for (const auto &p : s.points)
            ost << p << '\n';
        return ost;
    }
};

namespace std {
    template <>
    struct hash<Point>  {
        std::size_t operator()(const Point &p) const {
            return (
                 std::hash<int>()(p.x)       ^
                (std::hash<int>()(p.y) << 1) ^
                (std::hash<int>()(p.z) << 2)
            );
        }
    };
    
    template <>
    struct hash<Scanner>  {
        std::size_t operator()(const Scanner &s) const {
            return std::hash<int>()(s.id);
        }
    };
}

Transformation get_transf(Point max, const std::vector<Point> &ref, const std::vector<Point> &target) {
    for (const auto &a : ref) {
        for (const auto &b : target) {
            const auto comp = a.relation_to(b);
            for (std::size_t i = 0; i < comp.size(); ++i) {
                if (comp[i] == max) {
                    return {int(i), a - b.rotated(i)};
                }
            }
        }
    }
    return {};
}

bool normalize(const Scanner &ref, Scanner &target) {   
    std::unordered_map<Point, int> map{};
    
    for (const auto &a : ref.points)
        for (auto &b : target.points)
            for (const auto &p : a.relation_to(b))
                map[p]++;
                
    const auto max_elem = std::max_element(
        map.begin(), map.end(), [](auto a, auto b){ return a.second < b.second; }
    );
    if (max_elem->second < 12)
        return false;
        
    Point max_point = max_elem->first;
    Transformation transf = get_transf(max_point, ref.points, target.points);
    target.transform(transf);
    
    return true;
}

int main() {
    std::vector<Scanner> normalized{};
    std::list<Scanner> unnormalized{};
    std::unordered_set<Point> beacons{};
    
    Scanner s;  
    std::cin >> s;
    normalized.push_back(s);
    
    while (std::cin >> s)
        unnormalized.push_back(s);
            
    while (!unnormalized.empty()) { 
        std::unordered_set<Scanner> norms{};
        
        for (const auto &norm : normalized) {
            for (auto &unnorm : unnormalized) {
                if (!norms.contains(unnorm) && normalize(norm, unnorm)) {
                    std::cout << "normalized " << unnorm.id << " using " << norm.id << '\n';
                    norms.insert(unnorm);
                }
            }
        }
        
        for (const auto &s : norms) {
            unnormalized.remove(s);
            normalized.push_back(s);
        }
        norms.clear();
    }
    
    for (const auto &s : normalized)
        for (const auto &p : s.points)
            beacons.insert(p);
            
    std::cout << "\nbeacon count: " << beacons.size() << '\n';
}




