#include <iostream>

constexpr const int win_score = 21;
constexpr const int univ[] = {0,0,0,1,3,6,7,6,3,1};
static int64_t wins1, wins2;

struct Pos {
    static const int max_pos = 10;
    int value;

    inline Pos operator+(int n) {
        return {(value + n - 1) % max_pos + 1};
    }

    friend int operator+(int n, Pos p) {
        return n + p.value;
    }
};

void simulate_rec(int score1, int score2, Pos pos1, Pos pos2, int turn, int64_t un) {
    if (score1 >= win_score) {
        wins1 += un;
        return;
    }
    if (score2 >= win_score) {
        wins2 += un;
        return;
    }
    Pos cur;
    if (turn & 1) {
        for (int i = 3; i <= 9; ++i) {
            cur = pos1 + i;
            simulate_rec(score1 + cur, score2, cur, pos2, turn + 1, un * univ[i]);
        }
    }
    else {
        for (int i = 3; i <= 9; ++i) {
            cur = pos2 + i;
            simulate_rec(score1, score2 + cur, pos1, cur, turn + 1, un * univ[i]);
        }
    }
}

void simulate(int p1_pos, int p2_pos) {
    Pos init1{p1_pos}, init2{p2_pos};
    wins1 = 0;
    wins2 = 0;

    simulate_rec(0, 0, init1, init2, 1, 1);
}

int main() {
    simulate(1,2);
    std::cout << wins1 << ' ' << wins2 << '\n';
}











