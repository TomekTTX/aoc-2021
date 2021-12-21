#include <iostream>
#include <array>
#include <vector>

constexpr const unsigned int inf = ~0U;

struct Point {
	int64_t x, y;
	
	std::array<Point, 4> neighbours() const {
		return {{{x - 1,y}, {x + 1,y}, {x,y - 1}, {x,y + 1}}};
	}
	
    bool operator==(const Point &other) const {
        return x == other.x && y == other.y;
    }
	
	friend std::ostream& operator<<(std::ostream &ost, const Point &p) {
		ost << '(' << p.x << ',' << p.y << ')';
		return ost;
	}
};

struct Node {
	unsigned short value;
	bool visited = false;	
	unsigned int dist = inf;
};

class Matrix {
private:
	std::size_t w = 0, h = 0;
	std::vector<Node> arr{};
public:
	void add_row(const std::string &str) {
		if (w == 0)
			w = str.length();
		else if (str.length() != w)
			return;
		arr.reserve(arr.size() + w);
		for (char c : str)
			arr.emplace_back(c & 0xF);
		h++;
	}
	
	Point min_dist_point() const {
		std::size_t ind = 0;
		unsigned int mindist = inf;
		
		for (std::size_t i = 0; i < arr.size(); ++i) {
			if (!arr[i].visited && arr[i].dist < mindist) {
				mindist = arr[i].dist;
				ind = i;
			}
		}
		
		return {int64_t(ind % w), int64_t(ind / w)};
	}
	
	void upscale(unsigned short magnitude) {
		std::vector<Node> new_arr{};
		new_arr.reserve(w * h * magnitude * magnitude);
		
		for (unsigned short i = 0; i < magnitude; ++i) {
			for (std::size_t j = 0; j < h; ++j) {
				for (unsigned short k = 0; k < magnitude; ++k) {
					for (std::size_t l = 0; l < w; ++l) {
						unsigned short val = ((*this)(j, l).value + i + k);
						while (val > 9)
							val -= 9;
						new_arr.emplace_back(val);
					}
				}
			}	
		}
		
		w *= magnitude;
		h *= magnitude;
		arr = new_arr;
	}
	
	inline bool in_boundary(Point p) const {
		return (
			p.x >= 0 &&
			p.y >= 0 &&
			std::size_t(p.x) < w &&
			std::size_t(p.y) < h
		);
	}
	
	inline std::size_t width() const { return w; }
	inline std::size_t height() const { return h; }
	inline const Node& operator[](Point p) const { return arr[p.y * w + p.x];	}	
	inline Node& operator[](Point p) { return arr[p.y * w + p.x];	}
		
	inline const Node& operator()(std::size_t row, std::size_t col) const {
		return arr[row * w + col];
	}
	
	inline Node& operator()(std::size_t row, std::size_t col) {
		return arr[row * w + col];
	}
	
	friend std::ostream& operator<<(std::ostream &ost, const Matrix &m) {
		for (std::size_t i = 0; i < m.h; ++i) {
			for (std::size_t j = 0; j < m.w; ++j)
				ost << m.arr[i * m.w + j].value;
			ost << '\n';
		}
		return ost;
	}
private:
};

unsigned int shortest_path(Matrix &m) {
	const Point end{
		int64_t(m.width()) - 1,
		int64_t(m.height()) - 1
	};
	Point cur{0,0};
	
	m[cur].dist = 0;
	
	while (!m[end].visited) {
		m[cur].visited = true;
		for (const auto &p : cur.neighbours()) {
			if (!m.in_boundary(p) || m[p].visited)
				continue;
			m[p].dist = std::min(m[cur].dist + m[p].value, m[p].dist);
			//std::cout << p << " -> " << m[p].dist << '\n';
		}		
		cur = m.min_dist_point();
	}
	
	return m[end].dist;
}

int main() {
	Matrix mat{};
	std::string line;
	
	while (std::cin >> line)
		mat.add_row(line);
	mat.upscale(5);
		
	//std::cout << mat << '\n';
	std::cout << shortest_path(mat) << '\n';
}





















