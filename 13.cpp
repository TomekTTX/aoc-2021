#include <iostream>
#include <unordered_set>
#include <vector>

void msg(const char *m) {
	std::cout << m << '\n';
}

struct Point {
    int x, y;
	
	Point reflected(int axis, bool vertical) const {
		if (vertical)
			return {2 * axis - x, y};
		return {x, 2 * axis - y};
	}

    inline bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
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
};

namespace std {
    template <>
    struct hash<Point>  {
        std::size_t operator()(const Point &p) const {
            return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1);
        }
    };
}

class FoldablePointSet {
private:
	std::unordered_set<Point> points{};
	std::vector<Point> add_buffer{}, del_buffer{};
	Point begin{0,0}, end{0,0};
public:	
	inline std::size_t count() const { return points.size(); }
	
	void add_point(Point p) {
		begin.x = std::min(p.x, begin.x);
		begin.y = std::min(p.y, begin.y);		
		end.x = std::max(p.x, end.x);
		end.y = std::max(p.y, end.y);
		
		points.insert(p);
	}
	
	void fold(int axis, bool vertical) {
		if (vertical)
			if (axis >= (begin.x + end.x) / 2)
				fold_left(axis);
			else
				fold_right(axis);
		else
			if (axis >= (begin.y + end.y) / 2)
				fold_up(axis);
			else
				fold_down(axis);
				
		//dump_buffers();
		flush();
		size_to_fit();
	}	
	
    friend std::ostream& operator<<(std::ostream &ost, const FoldablePointSet &set) {
        for (int y = set.begin.y; y <= set.end.y; ++y) {
			for (int x = set.begin.x; x <= set.end.x; ++x)
				ost << (set.points.contains({x,y}) ? '#' : '.');
			ost << '\n';
		}
        return ost;
    }   
private:
	void fold_left(int axis) {
		msg("FOLD LEFT");
		for (const auto &p : points) {
			if (p.x > axis) {
				add_buffer.push_back(p.reflected(axis, true));
				del_buffer.push_back(p);
			}
		}
		
	}
	
	void fold_right(int axis) {
		msg("FOLD RIGHT");
		for (const auto &p : points) {
			if (p.x < axis) {
				add_buffer.push_back(p.reflected(axis, true));
				del_buffer.push_back(p);
			}
		}		
	}
	
	void fold_up(int axis) {
		msg("FOLD UP");
		for (const auto &p : points) {
			if (p.y > axis) {
				add_buffer.push_back(p.reflected(axis, false));
				del_buffer.push_back(p);
			}
		}		
	}
	
	void fold_down(int axis) {	
		msg("FOLD DOWN");	
		for (const auto &p : points) {
			if (p.y < axis) {
				add_buffer.push_back(p.reflected(axis, false));
				del_buffer.push_back(p);
			}
		}
	}
	
	void flush() {
		for (const auto &p : del_buffer)
			points.erase(p);
		for (const auto &p : add_buffer)
			add_point(p);
		add_buffer.clear();
		del_buffer.clear();
	}
	
	void dump_buffers() const {
		for (std::size_t i = 0; i < add_buffer.size(); ++i)
			std::cout << del_buffer[i] << " -> " << add_buffer[i] << '\n';
	}
	
	void size_to_fit() {
		begin.x = begin.y = 0;
		end.x = end.y = 0;
		for (const auto &p : points) {		
			begin.x = std::min(p.x, begin.x);
			begin.y = std::min(p.y, begin.y);		
			end.x = std::max(p.x, end.x);
			end.y = std::max(p.y, end.y);		
		}
	}
};

int main() {
	FoldablePointSet set{};
	Point p;
	
	while (std::cin >> p)
		set.add_point(p);
			
	set.fold(655, true);
	set.fold(447, false);
	set.fold(327, true);
	set.fold(223, false);
	set.fold(163, true);
	set.fold(111, false);
	set.fold(81, true);
	set.fold(55, false);
	set.fold(40, true);
	set.fold(27, false);
	set.fold(13, false);
	set.fold(6, false);
	
	std::cout << set << '\n';
}



























