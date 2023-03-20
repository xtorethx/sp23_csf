#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
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
    unsigned numsets;
    unsigned blocksperset;
    unsigned bytesperblock;
    std::string store_miss;
    std::string store_hit;
    std::string evict_alg;

    //arguments
    std::getline(std::cin, store_miss);
    std::getline(std::cin, store_hit);
    std::getline(std::cin, evict_alg);

    numsets = atoi(argv[1]);
    blocksperset = atoi(argv[2]);
    bytesperblock = atoi(argv[3]);
    store_miss = argv[4];
    store_hit = argv[5];
    evict_alg = argv[7];

    bool wb;
    bool wa;
    if (store_hit == "write-back") {
        wb = true;
    } else {
        wb = false;
    }

    if (store_miss == "write-allocate") {
        wa = true;
    } else {
        wa = false;
    }
    if (argc != 7) {
        std::cerr << "Wrong number of inputs\n";
        return 1;
    }
    //throw errors
    //block size is not a power of 2
    if (!((bytesperblock != 0) && ((bytesperblock & (bytesperblock - 1)) == 0))) {
        std::cerr << "block size is not a power of 2\n";
        return 1;
    }
    //number of sets is not a power of 2
    if (!((numsets != 0) && ((numsets & (numsets - 1)) == 0))) {
        std::cerr << "number of sets is not a power of 2\n";
        return 1;
    }
    //block size is less than 4
    if (bytesperblock < 4) {
        std::cerr << "block size is less than 4\n";
        return 1;
    }
    //write-back and no-write allocate were both specified
    if (store_hit == "write-back" && store_miss == "no-write-allocate") {
        std::cerr << "write-back and no-write allocate were both specified\n";
        return 1;
    }

    struct Cache cache = buildCache(numsets, blocksperset, bytesperblock);

    char ls;
    char memaddress[11];
    char tmp;
    int loads = 0;
    int i = 0;

    read(cache, ls, memaddress, tmp);

    while (i != EOF) {
        unsigned address =  hex_to_dec(memaddress);
        if (ls == 'l') {
            if (blocksperset == 1) {
                load_dm(address, cache);
            } else if (numsets == 1) {
                load_fa(address, cache);
            } else {
                load_sa(address, cache);
            }
        } else if (ls == 's') {
            if (blocksperset == 1) {
                store_dm(address, cache, wb, wa);
            } else if (numsets == 1) {
                store_fa(address, cache, wb, wa);
            } else {
                store_sa(address, cache, wb, wa);
            }
        }
        int i = read(cache, ls, memaddress, tmp);
    }

    write(cache);
}
