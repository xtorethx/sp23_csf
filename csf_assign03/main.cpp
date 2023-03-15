#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "cachesimfuncs.h"
#include "cachesimfuncs.h"

using namespace std;

void show_commands() {
	std::cout << "List of arguments Iin order):" << std::endl;































	
	std::cout << "\tNumber of sets in the cache (positve power-of-2)" << std::endl;
	std::cout << "\tnumber of blocks in each set (a positive power-of-2)" << std::endl;
	std::cout << "\tnumber of bytes in each block (a positive power-of-2, at least 4)" << std::endl;
	std::cout << "\twrite-allocate or no-write-allocate" <<std::endl;
	std::cout << "\t\tfor write-allocate we bring the relevant memory block into the cache before the store proceeds" << std::endl;
    std::cout << "\t\tfor no-write-allocate a cache miss during a store does not modify the cache" << std::endl;
	std::cout << "\twrite-through or write-back" << std::endl;
    std::cout << "\t\tfor write-through a store writes to the cache as well as to memory" << std::endl;
    std::cout << "\t\tfor write-back a store writes to the cache only and marks the block dirty;\n\t\t if the block is evicted later, it has to be written back to memory before being replaced" << std::endl;
	std::cout << "\tlru (least-recently-used) or fifo evictions" <<std::endl;
    std::cout << "\t\tfor lru (least-recently-used) we evict the block that has not been accessed the longest" << std::endl;
    std::cout << "\t\tfor fifo (first-in-first-out) we evict the block that has been in the cache the longest" << std::endl;
}

int main(int argc, char** argv) {
    //TO DO: Write main
}
