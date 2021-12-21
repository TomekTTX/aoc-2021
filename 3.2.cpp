#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

constexpr const int bit_count = 12;

const std::string& filter_input(const std::vector<std::string> &data, bool invert) {
	bool common_bit;
	int bit = -1, counter;
	std::size_t valid_count = data.size();
	auto valid = std::vector<bool>(valid_count, true);
	
	while (++bit < bit_count && valid_count > 1) {
		counter = 0;
		for (std::size_t i = 0; i < data.size(); ++i)
			if (valid[i])
				counter += (data[i][bit] & 1) ? 1 : -1;
		
		common_bit = (counter >= 0);
		
		for (std::size_t i = 0; i < data.size(); ++i) {
			if (valid[i] && (data[i][bit] & 1) != (common_bit ^ invert)) {
				valid[i] = false;
				valid_count--;
			}
		}
	}
	
	for (std::size_t i = 0; i < data.size(); ++i)
		if (valid[i])
			return data[i];
	return data[0];
}

int main() {
	std::string input;
	auto data = std::vector<std::string>();
	
	while (std::cin >> input)
		data.push_back(std::move(input));
	
	std::string o2 = filter_input(data, false);
	std::string co2 = filter_input(data, true);
	
	std::cout << "O2=" << o2 << '\n'
			  << "CO2=" << co2 << '\n'
			  << "product="
			  << std::stoi(o2, nullptr, 2) * std::stoi(co2, nullptr, 2)
			  << '\n';
}  
