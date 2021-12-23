#include <iostream>
#include <array>

constexpr int ten_turn_score(int player, int starting_pos) {
	const int scores[2][10] {
		{30, 40, 50, 60, 50, 60, 70, 80, 50, 60},
		{55, 55, 45, 55, 65, 55, 55, 45, 55, 65},
	};
	
	return scores[player - 1][starting_pos - 1];
}

struct Die {
	int result = -3, rolls = 0;
	
	int roll() {
		rolls += 3;
		switch (result += 9) {
			case 300:
				result = 0;
				return 200;
			case 303:
				result = 3;
				return 103;
			case 306:
				return result = 6;
			default:
				return result;
		}
	}
	
	void advance(int turns = 1) {
		result = (result + 9 * turns) % 300;
		rolls += 3 * turns;
	}
	
	void reset() {
		rolls = 0;
		result = -3;
	}
};


struct Pos {
	static const int max_pos = 10;
	int value;
	
	inline Pos operator+=(Pos p) {
		return {value = ((value + p.value - 1) % max_pos + 1)};
	}
	
	inline int operator+=(int n) {
		return value = ((value + n - 1) % max_pos + 1);
	}
};

std::array<int, 2> simulate(int p1_start_pos, int p2_start_pos, int turns, Die &die) {
	Pos p1{p1_start_pos}, p2{p2_start_pos};
	int score1 = (turns / 10) * ten_turn_score(1, p1_start_pos);
	int score2 = (turns / 10) * ten_turn_score(2, p2_start_pos);
	
	die.advance((turns / 10) * 20);	
	for (int i = 0; i < turns % 10; ++i) {
		score1 += (p1 += die.roll());
		if (score1 >= 1000) break;
		score2 += (p2 += die.roll());
		if (score2 >= 1000) break;
	}
	
	return {score1, score2};
}

void simulate(int p1_start_pos, int p2_start_pos, Die &die) {
	const int turns = std::min(
		10000 / ten_turn_score(1, p1_start_pos),
		10000 / ten_turn_score(2, p2_start_pos)
	);
	
	const auto result = simulate(p1_start_pos, p2_start_pos, turns + 1, die);
	std::cout << "Player 1 score: " << result[0] << '\n'
			  << "Player 2 score: " << result[1] << '\n'
			  << "Rolls: " << die.rolls << '\n';
}

int main() {
	Die die{};
	
	simulate(1, 2, die);
}






















