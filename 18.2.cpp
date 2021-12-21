#include <iostream>
#include <sstream>
#include <vector>
#include <memory>

class Number {
private:
	int value = -1, depth = -1;
	bool simple = true;
	Number *parent = nullptr;
	std::unique_ptr<Number> left{}, right{};
public:	
	Number() = default;
	Number(int depth, Number *parent) : depth(depth), parent(parent) {}
	Number(int val, int depth, Number *parent) : Number(depth, parent) { value = val; }
	Number(int lval, int rval, int depth, Number *parent) : Number(depth, parent) {
		left = std::make_unique<Number>(lval, depth + 1, this);
		right = std::make_unique<Number>(rval, depth + 1, this);
	}
	
	inline int get() const { return value; }
	inline const int *vaddr() const { return &value; }
	inline bool is_simple() const { return simple; }
	
	inline void recalc() { recalc_rec(0); }
	inline void set_left(std::unique_ptr<Number> &&ptr) { left = std::move(ptr); simple = false; }
	inline void set_right(std::unique_ptr<Number> &&ptr) { right = std::move(ptr); simple = false; }
	inline void set_parent(Number *p) { parent = p; }
	
	void reduce() {
		while (find_and_explode() || find_and_split());
	}
	
	int magnitude() const {
		if (simple)
			return value;
		return 3 * left->magnitude() + 2 * right->magnitude();
	}
	
	std::vector<int *> flatten() {
		std::vector<int *> ret{};
		flatten_rec(ret);
		return ret;
	}
	
	Number operator+(Number &other) {
		Number ret{};
		
		ret.set_right(std::make_unique<Number>(std::move(other)));
		ret.set_left(std::make_unique<Number>(std::move(*this)));
		ret.recalc();
		
		return ret;
	}
	
	friend std::ostream& operator<<(std::ostream &ost, const Number &num) {
		if (num.simple)
			ost << num.value;
		else
			ost << '[' << *num.left << ',' << *num.right << ']';
		return ost;
	}

	friend std::istream& operator>>(std::istream &ist, Number &num) {
		char c;
		ist >> c;
		if (num.depth == -1)
			num.depth = 0;
		if (c != '[')
			num.value = c & 15;
		else {
			num.left = std::make_unique<Number>(num.depth + 1, &num);
			num.right = std::make_unique<Number>(num.depth + 1, &num);
			num.simple = false;
			
			ist >> *num.left >> c >> *num.right >> c;
		}
		return ist;
	}
private:
	inline bool is_left_in_parent() const {
		return parent->left.get() == this;
	}

	void recalc_rec(int dep) {
		depth = dep;
		if (!simple) {
			left->recalc_rec(dep + 1);
			right->recalc_rec(dep + 1);
			left->set_parent(this);
			right->set_parent(this);
		}
	}
	
	void flatten_rec(std::vector<int *> &vec) {
		if (simple)
			vec.push_back(&value);
		else {
			left->flatten_rec(vec);
			right->flatten_rec(vec);
		}
	}	
	
	void explode(Number *top_num) {		
		const auto flat = top_num->flatten();
		int *lval = nullptr, *rval = nullptr;
		
		for (std::size_t i = 0; i < flat.size() && (!lval || !rval); ++i) {
			if (i != 0 && flat[i] == left->vaddr())
				lval = flat[i - 1];
			if (i != (flat.size() - 1) && flat[i] == right->vaddr())
				rval = flat[i + 1];
		}
		
		if (lval) *lval += left->get();		
		if (rval) *rval += right->get();
			
		if (is_left_in_parent())
			parent->left = std::make_unique<Number>(0, depth, parent);
		else
			parent->right = std::make_unique<Number>(0, depth, parent);
	}
	
	bool find_and_explode() {
		bool done = false;
		find_and_explode_rec(this, done);
		return done;
	}
	
	void find_and_explode_rec(Number *top_num, bool &done) {
		if (done || simple)
			return;
		if (depth >= 4) {
			done = true;
			explode(top_num);
		}
		else {
			left->find_and_explode_rec(top_num, done);
			right->find_and_explode_rec(top_num, done);
		}
	}
	
	void split() {
		const int lval = value / 2, rval = lval + (value & 1);
		
		value = -1;
		simple = false;
		left = std::make_unique<Number>(lval, depth + 1, this);
		right = std::make_unique<Number>(rval, depth + 1, this);
	}
	
	bool find_and_split() {
		bool done = false;
		find_and_split_rec(done);
		return done;
	}
	
	void find_and_split_rec(bool &done) {
		if (done)
			return;
		if (simple) {
			if (value > 9) {
				split();
				done = true;
			}
		}
		else {
			left->find_and_split_rec(done);
			right->find_and_split_rec(done);
		}
	}
};

Number copyadd(const Number &a, const Number &b) {
	Number op1, op2;
	std::stringstream ss{};
	
	ss << a;
	ss >> op1;
	ss << b;
	ss >> op2;
	
	Number ret = op1 + op2;
	ret.reduce();
	
	return ret;
}

int main() {
	int mag_max = 0;
	Number n;
	std::vector<Number> nums{};
	
	while (std::cin >> n)
		nums.push_back(std::move(n));
		
	for (std::size_t i = 0; i < nums.size(); ++i)
		for (std::size_t j = 0; j < nums.size(); ++j)
			if (i != j)
				mag_max = std::max(mag_max, copyadd(nums[i], nums[j]).magnitude());

	
	std::cout << mag_max << '\n';
}


























