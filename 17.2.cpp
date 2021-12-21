#include <iostream>
#include <array>
#include <vector>

inline constexpr int sgn(int x) {
    return (0 < x) - (x < 0);
}

struct Rect {
    int x1, y1, x2, y2;
};

struct Point {
    int64_t x, y;
    
    bool in(const Rect &r) const {
        bool ret = (
            x >= r.x1 &&
            y <= r.y1 &&
            x <= r.x2 &&
            y >= r.y2
        );
        return ret;
    }

    Point operator+(const Point &other) const {
        return {x + other.x && y + other.y};
    }
    
    Point operator+=(const Point &other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
    
    friend std::ostream& operator<<(std::ostream &ost, const Point &p) {
        ost << '(' << p.x << ',' << p.y << ')';
        return ost;
    }
};

int isqrt(int y) {
    int bottom = 0;
    int middle;
    int top = y + 1;

    while (top - bottom > 1) {
        middle = (bottom + top) / 2;

        if (middle * middle <= y)
            bottom = middle;
        else
            top = middle;
    }

    return bottom;
}

inline int rtriang(int y) {
    return (isqrt(8 * y + 1) - 1) / 2;
}


bool check_trajectory(Point point, int vx, int vy, const Rect &r) {
    while (vy > 0 || point.y >= r.y2) {
        if (point.in(r))
            return true;        
        point += {vx, vy--};
        vx -= sgn(vx);
    }
    return false;
}

int min_triang(int xbegin, int xend) {
    int last = rtriang(xbegin - 1);
    for (int x = xbegin, cur; x <= xend; ++x) {
        cur = rtriang(x);
        if (cur != last)
            return cur;
        last = cur;
    }
    return rtriang(xbegin);
}

int find_trajectories(const Rect &target) {
    const int vy_min = target.y2;
    const int vy_max = -target.y2 - 1;
    const int vx_min = min_triang(target.x1, target.x2);
    const int vx_max = target.x2;
    int ret = 0;
    
    std::cout << "Vx_min = " << vx_min << '\n'
              << "Vx_max = " << vx_max << '\n'
              << "Vy_min = " << vy_min << '\n'
              << "Vy_max = " << vy_max << "\n\n";
    
    for (int vy = vy_min; vy <= vy_max; ++vy) {
        for (int vx = vx_min; vx <= vx_max; ++vx) {
            const bool result = check_trajectory({0,0}, vx, vy, target);
            ret += result;
            std::cout << result;
        }
        std::cout << '\n';
    }
    std::cout << '\n';
    
    return ret;
}

int main() {
    Rect target{155, -72, 215, -132};
    std::cout << find_trajectories(target) << '\n';
}

/*


std::vector<Point> trajectory(Point origin, int vx, int vy, int steps) {
    std::vector<Point> traj{origin};
    traj.reserve(steps + 1);
    
    while (steps--) {
        origin += {vx, vy--};
        vx -= sgn(vx);
        traj.push_back(origin);
    }
    
    return traj;
    
    
void print_traj(int vx, int vy, int steps) {
    for (const auto &p : trajectory({0,0}, vx, vy, steps))
        std::cout << p << '\n'; 
    std::cout << '\n';  
}
}
*/















