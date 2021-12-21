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
		return (
			x >= r.x1 &&
			y >= r.y1 &&
			x <= r.x2 &&
			y <= r.y2
		);
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

std::vector<Point> trajectory(Point origin, int vx, int vy, int steps) {
	std::vector<Point> traj{origin};
	traj.reserve(steps + 1);
	
	while (steps--) {
		origin += {vx, vy--};
		vx -= sgn(vx);
		traj.push_back(origin);
	}
	
	return traj;
}

void print_traj(int vx, int vy, int steps) {
	for (const auto &p : trajectory({0,0}, vx, vy, steps))
		std::cout << p << '\n';	
	std::cout << '\n';	
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

// x=155..215, y=-132..-72

int main() {
	Rect target{155, -72, 215, -132};
	
	int vy = -target.y2 - 1;
	int vx = rtriang(target.x2);
	
	print_traj(vx, vy, 270);
}

















