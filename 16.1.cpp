#include <iostream>
#include <vector>
#include <bitset>
#include <string>
#include <numeric>

class Bitstream {
    using bit_iter = std::vector<bool>::iterator;
private:
    std::vector<bool> bitstream{};
    bit_iter it;
    uint32_t vsum = 0;
public:
    Bitstream(const std::string &hex) {
        for (char c : hex) {
            const auto bits = std::bitset<4>(val(c));
            for (std::size_t i = 0; i < 4; ++i)
                bitstream.push_back(bits[3 - i]);
        }
        bitstream.shrink_to_fit();
        it = bitstream.begin();
    }
    
    inline bool next() {return *(it++); }

    uint32_t num(uint8_t bit_count) {
        uint32_t ret = 0;
        while (bit_count--) {
            ret <<= 1;
            ret |= next();
        }
        return ret;
    }
    
    uint64_t packet() {
        uint8_t version = num(3);
        uint8_t type = num(3);
        
        vsum += version;        
        //std::cout << "Packet of version " << short(version) << " and type " << short(type) << '\n';
        
        return type == 4 ? literal() : op_packet(type);
    }
    
    inline uint32_t get_vsum() const { return vsum; }
private:
    constexpr inline static unsigned char val(char c) {
        return ((c | ((c >> 3) & 8)) + (c >> 6)) & 15;
    }
    
    uint64_t literal() {
        uint64_t ret = 0;
        bool keep_going;
        
        do {
            keep_going = next();
            ret <<= 4;
            ret |= num(4);
        } while (keep_going);
        
        //std::cout << "  Literal " << ret << '\n';
        return ret;
    }
    
    uint64_t aggregate(const std::vector<uint64_t> &buf, uint8_t type) {
        uint64_t ret;
        
        //std::cout << "  # Aggregate [ ";
        //for (auto a : buf)
            //std::cout << a << ' ';
        //std::cout << "] (";
        switch (type) {
            case 0:
                //std::cout << "SUM";
                ret = std::accumulate(buf.begin(), buf.end(), 0ULL);
                break;
            case 1:
                //std::cout << "PRODUCT";
                ret = std::accumulate(buf.begin(), buf.end(), 1ULL,
                    std::multiplies<uint64_t>() );          
                break;
            case 2:
                //std::cout << "MIN";
                ret = *std::min_element(buf.begin(), buf.end());    
                break;
            case 3:
                //std::cout << "MAX";
                ret = *std::max_element(buf.begin(), buf.end());    
                break;      
            case 5:
                //std::cout << "GREATER THAN";
                ret = buf[0] > buf[1];      
                break;  
            case 6:
                //std::cout << "LESS THAN";
                ret = buf[0] < buf[1];  
                break;      
            case 7:
                //std::cout << "EQUAL";
                ret = buf[0] == buf[1];
                break;
            default:            
                throw "Invalid packet type";
        }
        //std::cout << ") -> " << ret << '\n';
        return ret;
    }
    
    uint64_t op_packet(uint8_t type) {
        if (next())
            return quantified_op_packet(type, num(11));
        else 
            return measured_op_packet(type, num(15));
    }
    
    uint64_t measured_op_packet(uint8_t type, uint32_t len) {
        std::vector<uint64_t> packet_results{};
        const bit_iter packet_start = it;
        
        //std::cout << "  Measured of length " << len << '\n';
        while (it - packet_start < len)
            packet_results.push_back(packet());
            
        return aggregate(packet_results, type);     
    }
    
    uint64_t quantified_op_packet(uint8_t type, uint32_t cnt) {
        std::vector<uint64_t> packet_results{};
        
        //std::cout << "  Quantified of count " << cnt << '\n';
        while (cnt--)
            packet_results.push_back(packet());
        
        return aggregate(packet_results, type);         
    }
};

int main() {
    std::string hex;
    std::cin >> hex;
    
    Bitstream bitstream{hex};       
    const uint64_t result = bitstream.packet();
    
    std::cout << "version sum = " << bitstream.get_vsum() << '\n'
              << "packet value = " << result << '\n';
     
}

















