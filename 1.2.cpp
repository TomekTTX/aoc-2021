#include <iostream>
#include <vector>

int main() {
	unsigned int x, count = 0;
	auto data = std::vector<unsigned int>();
	
	while (std::cin >> x)
		data.push_back(x);
		
	for (unsigned int i = 0, last_sum = ~0U, sum; i < data.size() - 2; ++i) {
		sum = data[i] + data[i + 1] + data[i + 2];
		count += (sum > last_sum);
		last_sum = sum;
	}
	
	std::cout << count << '\n';
}
