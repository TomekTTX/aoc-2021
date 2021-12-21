#include <iostream>
#include <array>
#include <vector>
#include <bitset>
#include <unordered_set>

struct Rect {
    int x1, y1, x2, y2;
    
    Rect shrunk(int n) const {
        return {x1 + n, y1 + n, x2 - n, y2 - n};
    }
};

struct Point {
    int x, y;
        
    inline bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
    
    inline bool in(const Rect &r) const {
        return (
            x >= r.x1 &&
            x <= r.x2 &&
            y >= r.y1 &&
            y <= r.y2
        );
    }
    
    std::array<Point, 9> get_3x3() const {
        return {{
            {x-1, y-1}, {x  , y-1}, {x+1, y-1}, 
            {x-1, y  }, {x  , y  }, {x+1, y  }, 
            {x-1, y+1}, {x  , y+1}, {x+1, y+1}, 
        }};
    }
    
    friend std::ostream& operator<<(std::ostream &ost, const Point &p) {
        ost << '(' << p.x << ',' << p.y << ')';
        return ost;
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

class PointCloud {
private:
    std::unordered_set<Point> points{};
    std::vector<Point> buffer{};
    Point top_left{0,0}, bottom_right{0,0};
public:
    inline bool contains(Point p) const { return points.contains(p); }  
    inline std::size_t size() const { return points.size(); }
    
    inline void insertq(Point p) { points.insert(p); }
    inline void stage(Point p) { buffer.push_back(p); } 
    
    Rect boundary() const { 
        return {
            top_left.x,
            top_left.y,
            bottom_right.x,
            bottom_right.y,
        };
    };
    
    inline const std::unordered_set<Point>& get_points() const {
        return points;
    }
    
    void insert(Point p) {
        points.insert(p);
        
        top_left.x = std::min(top_left.x, p.x);
        top_left.y = std::min(top_left.y, p.y);
        bottom_right.x = std::max(bottom_right.x, p.x);
        bottom_right.y = std::max(bottom_right.y, p.y);
    }   
    
    void commit() {
        points.clear();
        top_left = {0,0};
        bottom_right = {0,0};
        
        for (const auto &p : buffer)
            insert(p);
        buffer.clear();
        buffer.reserve(64);
    }   
    
    uint16_t assemble_3x3(Point p) const {
        uint16_t ret = 0;
        
        for (const auto &pt : p.get_3x3()) {
            ret <<= 1;
            ret |= contains(pt);
        }
        
        return ret;
    }
    
    friend std::ostream& operator<<(std::ostream &ost, const PointCloud &pc) {
        const Rect r = pc.boundary();
        
        for (int y = r.y1; y <= r.y2; ++y) {
            for (int x = r.x1; x <= r.x2; ++x) 
                std::cout << (pc.contains({x,y}) ? '#' : '.');
            std::cout << '\n';
        }
        
        return ost;
    }
};

class Image {
private:
    PointCloud pixels{};
    std::bitset<512> alg_bits{};
    int times_enhanced = 0;
public:
    inline std::size_t size() const { return pixels.size(); }

    void printalg() const {
        for (int i = 0; i < 512; ++i)
            std::cout << (alg_bits[i] ? '#' : '.');
        std::cout << '\n';
    }

    void enhance() {
        const Rect r = pixels.boundary();
        
        for (int y = r.y1 - 5; y <= r.y2 + 5; ++y) {
            for (int x = r.x1 - 5; x <= r.x2 + 5; ++x) {
                const Point p{x,y};
                
                if (alg_bits[pixels.assemble_3x3(p)])
                    pixels.stage(p);                
            }           
        }
        
        pixels.commit();
        times_enhanced++;
        infinity_cropout();
    }

    friend std::ostream& operator<<(std::ostream &ost, const Image &im) {
        ost << im.pixels << '\n';       
        return ost;
    }
    
    friend std::istream& operator>>(std::istream &ist, Image &im) {
        std::size_t y = 0;
        std::string line;
        
        for (std::size_t i = 0; i < im.alg_bits.size(); ++i)
            im.alg_bits[i] = (ist.get() == '#');
        ist.ignore();
        
        while (ist >> line) {
            for (std::size_t x = 0; x < line.length(); ++x)
                if (line[x] == '#')
                    im.pixels.insert({int(x),int(y)});
            y++;
        }
            
        return ist;
    }
private:
    void infinity_cropout() {
        if ((times_enhanced & 1) || !alg_bits[0])
            return;
        const Rect r = pixels.boundary().shrunk(8);
        
        for (const auto &p : pixels.get_points()) {
            if (p.in(r))
                pixels.stage(p);
        }
        pixels.commit();
    }
};

int main() {
    int enhances = 50; // 2 for part 1; 50 for part 2
    Image img{};
    
    std::cin >> img;

    while (enhances--)
        img.enhance();
        
    std::cout << img << "\nlit pixels = " << img.size() << '\n';
}


