#include <iostream>
#include <sstream>
#include <vector>
#include <array>

struct Board {
	const static int size = 5;
	bool bingo = false;
	std::array<int, size * size> board{};
	std::array<bool, size * size> checked{ false };
	
	bool is_bingo() {
		if (bingo)
			return true;
		for (int i = 0; i < size; ++i)
			if (horizontal_check(i))
				return bingo = true;
		for (int i = 0; i < size; ++i)
			if (vertical_check(i))
				return bingo = true;
		return false;
	}
	
	bool horizontal_check(int row) const {
		for (int col = 0; col < size; ++col)
			if (!is_checked(row, col))
				return false;
		return true;
	}	
	
	bool vertical_check(int col) const {
		for (int row = 0; row < size; ++row)
			if (!is_checked(row, col))
				return false;
		return true;
	}	
	
	int score(int final_number) {
		if (!is_bingo())
			return 0;
			
		int ret = 0;
		for (int i = 0; i < size ; ++i)
			for (int j = 0; j < size; ++j)
				if (!is_checked(i, j))
					ret += (*this)(i, j);
					
		return final_number * ret;
	}
	
	bool is_checked(int row, int col) const {
		return checked[row * size + col];
	}

	void check(int row, int col) {
		checked[row * size + col] = true;
	}
	
	void check_value(int value) {
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				if ((*this)(i, j) == value) {
					check(i, j);
					break;
				}
			}
		}			
	}
	
	const int &operator()(int row, int col) const {
		return board[row * size + col];
	}
	
	int &operator()(int row, int col) {
		return board[row * size + col];
	}	
	
	friend std::istream& operator>>(std::istream &ist, Board &b) {
		for (int i = 0; i < size; ++i)
			for (int j = 0; j < size; ++j)
				ist >> b(i, j);
		return ist;
	}	
	
	friend std::ostream& operator<<(std::ostream &ost, const Board &b) {
		for (int i = 0; i < size; ++i) {
			ost << '\n';
			for (int j = 0; j < size; ++j)
				ost << b(i, j) << ' ';
		}
		return ost;
	}
};

std::vector<int> get_numbers() {
	std::string line, part;
	std::vector<int> numbers{};
	
	std::getline(std::cin, line);
	std::stringstream stream{line};
	while (std::getline(stream, part, ','))
		numbers.push_back(std::stoi(part));
		
	return numbers;
}

std::vector<Board> get_boards() {
	Board b;
	std::vector<Board> boards{};
	
	while (std::cin >> b)
		boards.push_back(std::move(b));
		
	return boards;	
}

int first_win(std::vector<Board> &boards, const std::vector<int> &numbers, int *index) {
	int numbers_added = 0;
	for (int num : numbers) {
		for (*index = 0; *index < (int)boards.size(); ++*index) {
			boards[*index].check_value(num);
			if (++numbers_added >= 5 && boards[*index].is_bingo())
				return boards[*index].score(num);
		}
	}
	return -1;
}

int main() {
	const auto numbers = get_numbers();
	auto boards = get_boards();
	int index, score = first_win(boards, numbers, &index);
		
	std::cout << "Board " << index << ':'
			  << boards[index] << '\n'
			  << "Score: " << score << '\n';
}












