#include <iostream>
#include <string>
#include <vector>

class HeightMap {
private:
    std::size_t width = 0;
    std::vector<std::string> val = {};
public:
    int low_point_risks() const {
        int ret = 0;
        
        for (int i = 0; i < (int)val.size(); ++i)
            for (int j = 0; j < (int)width; ++j)
                ret += low_point(i, j) ? (val[i][j] & 0xF) + 1 : 0;
        
        return ret;             
    }
    
    bool low_point(int i, int j) const {
        const char v = val[i][j];
        
        return !(
            (j > 0 && val[i][j - 1] <= v)
            || (j < (int)width - 1 && val[i][j + 1] <= v)
            || (i > 0 && val[i - 1][j] <= v)
            || (i < (int)val.size() - 1 && val[i + 1][j] <= v)
        );
    }

    const std::string& operator[](int row) const {
        return val[row];
    }
    
    std::string& operator[](int row) {
        return val[row];        
    }
    
    friend std::istream& operator>>(std::istream &ist, HeightMap &hm) {
        std::string in;
        
        ist >> in;
        hm.width = in.length();
        do
            hm.val.push_back(std::move(in));
        while (ist >> in);
        
        return ist;
    }
    
    friend std::ostream& operator<<(std::ostream &ost, const HeightMap &hm) {
        for (const auto &str : hm.val)
            ost << str << '\n';     
        return ost;
    }
};

int main() {
    HeightMap hm{}; 
    std::cin >> hm; 
    std::cout << hm.low_point_risks() << '\n';  
}











