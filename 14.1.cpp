#include <iostream>
#include <list>
#include <unordered_map>
#include <stack>

constexpr const int steps = 4;

class PolymerTemplate {
private:
	std::unordered_map<short, char> map{};
public:
	void add(char before, char after, char product) {
		map.insert({combine(before, after), product});
	}

	char get(char before, char after) const {
		const short key = combine(before, after);
		return map.contains(key) ? map.at(key) : '-';
	}
private:
	inline constexpr static short combine(char before, char after) {
		return (short(before) << 8) | after;
	}
};

std::list<char> base_list() {
	std::list<char> ret{};
	std::string line;
	
	std::cin >> line;
	for (char c : line)
		ret.push_back(c);
		
	return ret;
}

std::unordered_map<char, int> get_stats(const std::list<char> &polymer) {
	std::unordered_map<char, int> stat{};
	
	for (char c : polymer) {
		if (stat.contains(c))
			stat[c]++;
		else
			stat[c] = 1;
	}
	
	return stat;
}

int main() {
	PolymerTemplate pol_temp{};
	std::list<char> polymer = base_list();
	
	for (std::string line; std::getline(std::cin, line); )
		pol_temp.add(line[0], line[1], line[6]);
		
	for (int i = 0; i < steps; ++i) {
		std::stack<char> buffer{};
		char after = '\0';
		
		for (auto it = polymer.rbegin(); it != polymer.rend(); ++it) {
			if (after != '\0')
				buffer.push(pol_temp.get(*it, after));
			after = *it;
		}
		for (auto it = ++polymer.begin(); it != polymer.end(); ++it) {
			polymer.insert(it, buffer.top());
			buffer.pop();
		}
	}
	
	//for (char c : polymer)
	//	std::cout << c;
	//std::cout << '\n';
	
	int max = 0, min = ~(1 << 31);
	for (const auto &[chr, cnt] : get_stats(polymer)) {
		std::cout << chr << ": " << cnt << '\n';
		min = std::min(min, cnt);
		max = std::max(max, cnt);
	}
	//std::cout << "difference = " << max - min << '\n';
}


















