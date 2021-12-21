#include <iostream>
#include <numeric>
#include <vector>

inline constexpr float sma(float avg, float elem, int n) {
	return (avg * n + elem) / (n + 1);
}

int distsum(const std::vector<int> &positions, int pos) {
	return std::accumulate(positions.begin(), positions.end(), 0,
		[=](int a, int b){ return a + std::abs(b - pos); });
}

int main() {
	float avg = 0;
	int in, count = 0;
	std::vector<int> crabs{};
	
	while (std::cin >> in) {
		std::cin.get();
		avg = sma(avg, in, count++);
		crabs.push_back(in);
	}

	const int d1 = distsum(crabs, int(avg));
	const int d2 = distsum(crabs, int(avg) + 1);	
	const int dir = (d1 > d2) ? 1 : -1;
	
	int last = ~(1 << 31);
	for (int pos = int(avg) + dir, dist = d1; dist < last; pos += dir) {
		last = dist;
		dist = distsum(crabs, pos);
		std::cout << dist << '\n';
	}
	
	std::cout << "fuel="  << last << '\n';
}































