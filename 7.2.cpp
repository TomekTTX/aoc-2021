#include <iostream>
#include <numeric>
#include <vector>
#include <cstdint>

inline constexpr float sma(float avg, float elem, int64_t n) {
	return (avg * n + elem) / (n + 1);
}

inline constexpr int64_t triang(int64_t x) {
	return x * (x + 1) / 2;
}

int64_t distsum(const std::vector<int64_t> &positions, int64_t pos) {
	return std::accumulate(positions.begin(), positions.end(), 0,
		[=](int64_t a, int64_t b){ return a + triang(std::abs(b - pos)); });
}

int main() {
	float avg = 0;
	int64_t in, count = 0;
	std::vector<int64_t> crabs{};
	
	while (std::cin >> in) {
		std::cin.get();
		avg = sma(avg, in, count++);
		crabs.push_back(in);
	}

	const int64_t d1 = distsum(crabs, int64_t(avg));
	const int64_t d2 = distsum(crabs, int64_t(avg) + 1);
	const int64_t dir = (d1 > d2) ? 1 : -1;
	
	int64_t last = ~(1 << 31);
	for (int64_t pos = int64_t(avg) + dir, dist = d1; dist < last; pos += dir) {
		last = dist;
		dist = distsum(crabs, pos);
	}
	
	std::cout << "fuel="  << last << '\n';
}































