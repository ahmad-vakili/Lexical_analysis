#include "scanner.h"
#include <fstream>

int main() {
    std::ifstream input("input.abc");
    if(!input) { 
        std::cerr<<"Error: Cannot open input.abc\n";
        return 1;
    }

    std::string line;
    int row=1;
    while(std::getline(input, line)) {
        scanLine(line, row);
        row++;
    }
    return 0;
}
