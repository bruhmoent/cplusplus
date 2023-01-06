#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

void encrypt_hex(const std::string& input_file, const std::string& output_file)
{
    std::ifstream in(input_file, std::ios::binary);
    std::ofstream out(output_file, std::ios::binary);
    if (!in) {
        std::cerr << "Error opening input file: " << input_file << std::endl;
        return;
    }
    if (!out) {
        std::cerr << "Error opening output file: " << output_file << std::endl;
        return;
    }
    char c;
    while (in.get(c)) {
        out << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    in.close();
    out.close();
}
