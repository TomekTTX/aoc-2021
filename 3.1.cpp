#include <iostream>

constexpr const int bit_count = 12;

int main() {
	int counter[bit_count] = { 0 }, gamma = 0, epsilon = 0;
	char input[bit_count + 1];
	
	while (std::cin >> input)
		for (int i = 0; i < bit_count; ++i)
			counter[i] += (input[i] & 1) ? 1 : -1;
			
	for (int i = 0; i < bit_count; ++i) {
		gamma <<= 1;
		gamma |= (counter[i] > 0);
	}

	epsilon = ~gamma & ((1 << bit_count) - 1);
	
	std::cout << "gamma=" << gamma << '\n'
		<< "epsilon=" << epsilon << '\n'
		<< "product=" << epsilon * gamma << '\n';
}
