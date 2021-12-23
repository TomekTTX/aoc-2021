#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <unordered_set>

struct Point {
	int x, y;
	
	std::array<Point, 4> neighbours() const {
		return {{{x - 1,y}, {x + 1,y}, {x,y - 1}, {x,y + 1}}};
	}
	
    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
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

class HeightMap {
private:
	std::size_t width = 0;
	std::vector<std::string> val{};
public:	
	bool low_point(int i, int j) const {
		const char v = val[i][j];
		
		return !(
			(j > 0 && val[i][j - 1] <= v)
			|| (j < (int)width - 1 && val[i][j + 1] <= v)
			|| (i > 0 && val[i - 1][j] <= v)
			|| (i < (int)val.size() - 1 && val[i + 1][j] <= v)
		);
	}
	
	std::vector<Point> low_points() const {
		std::vector<Point> ret{};
		
		for (int i = 0; i < (int)val.size(); ++i)
			for (int j = 0; j < (int)width; ++j)
				if (low_point(i, j))
					ret.push_back({j, i});
		
		return ret;
	}
	
	int basin_size(int x, int y) const {
		std::queue<Point> points{};
		std::unordered_set<Point> visited{};
		
		points.emplace(x, y);
		while (!points.empty()) {
			const Point &cur = points.front();
			
			points.pop();			
			visited.insert(cur);
			for (const auto &p : cur.neighbours())
				if (in_boundary(p) && val[p.y][p.x] != '9' && !visited.contains(p))
					points.push(p);
		}
		
		return visited.size();
	}
	
	friend std::istream& operator>>(std::istream &ist, HeightMap &hm) {
		std::string in;
		
		ist >> in;
		hm.width = in.length();
		do
			hm.val.push_back(std::move(in));
		while (ist >> in);
		
		return ist;
	}
	
	friend std::ostream& operator<<(std::ostream &ost, const HeightMap &hm) {
		for (const auto &str : hm.val)
			ost << str << '\n';		
		return ost;
	}
private:
	inline bool in_boundary(int x, int y) const {
		return (
			(x >= 0)
			&& (x < (int)width)
			&& (y >= 0)
			&& (y < (int)val.size())
		);
	}
	
	inline bool in_boundary(const Point &p) const {
		return in_boundary(p.x, p.y);
	}
};

int main() {
	std::vector<int> sizes{};
	HeightMap hm{};	
	std::cin >> hm;

	for (const auto &p : hm.low_points()) {
		sizes.push_back(hm.basin_size(p.x, p.y));
	}
	std::sort(sizes.begin(), sizes.end(), [](auto a, auto b){ return a > b; });
	
	std::cout << "product=" << sizes[0] * sizes[1] * sizes[2] << '\n'; 
}





















