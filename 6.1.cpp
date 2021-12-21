#include <iostream>
#include <vector>
#include <numeric>

template <typename T>
class CircularBuffer {
private:
	std::size_t size, spin = 0;
	std::vector<T> arr;
public:
	CircularBuffer() : size(0), arr(std::vector<T>()) {}
	CircularBuffer(std::size_t size) : size(size), arr(std::vector<T>(size)) {}
	CircularBuffer(std::size_t size, T init) : size(size), arr(std::vector<T>(size, init)) {}
	CircularBuffer(std::vector<T> &&vec) : size(vec.size()), arr(vec) {}
	
	inline std::size_t len() const { return size; }	
	inline const std::vector<T> &vec() const { return arr; }
	
	void rotate(std::size_t dist = 1) {
		spin = (spin + dist) % size;
	}

	T operator[](std::size_t index) const {
		return arr[norm(index)];
	}
	
	T& operator[](std::size_t index) {		
		return arr[norm(index)];
	}
	
	friend std::ostream &operator<<(std::ostream &ost, const CircularBuffer &cb) {
		ost << "[ ";
		for (std::size_t i = 0; i < cb.size; ++i)
			ost << cb[i] << " ";
		ost << ']';
		return ost;
	}
private:
	inline int norm(std::size_t index) const {
		return (index + spin) % size;
	}
};


int main() {
	constexpr const std::size_t max_age = 10, days = 80;
	
	CircularBuffer<int> fish{max_age, 0};
	
	int x;
	while (std::cin >> x) {
		fish[x]++;
		std::cin.get();
	}
			
	//std::cout << fish << '\n';
	for (std::size_t i = 0; i < days; ++i) {			
		fish[7] += fish[0];
		fish[9] += fish[0];
		fish[0] = 0;
		
		fish.rotate();	
		
		//std::cout << fish << '\n';		
	}
	
	std::cout << std::accumulate(fish.vec().cbegin(), fish.vec().cend(), 0) << '\n';
}




















