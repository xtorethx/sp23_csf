#ifndef CACHESIMFUNCS_H
#define CACHESIMFUNCS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include "cachesimfuncs.h"

//TO DO: Write Functions
struct Slot{
    uint32_t tag;
    // default valid to false, adding slot to set set to true
    bool valid;
    uint32_t load_ts, access_ts;
    bool dirty;
};

struct Set{
    std::map <uint32_t, Slot *> index; //used to map offsets to valid blocks, which valid blocks have been stored in set, key is tag value is pointer to corresponding slot
};

struct Cache{
    std::map <uint32_t, Set *> sets; // map indices to sets
};

// memory address is tag index offset
// given: num sets, num blocks, num bytes/ block (= block size)
// tag is
//read memory access trace from standard input
void read(char ls, char memaddress[], char tmp) {
    scanf("%c", ls);
    scanf("%10s", memaddress);
    scanf("%c", tmp);
}

//write to standard output
void write() {
}

//char to int helper function
int hexchar_to_int(char hex) {
    char hexchar[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    for (int i = 0; i < 16; i++) {
        if (hexchar[i] == hex) {
            return i;
        }
    }
}

//hex to dec function 
unsigned hex_to_dec(char hex[]) {
    unsigned sum;
    for (int i = 0; i < 8; i++) {
        unsigned prod = 1;
        for (int j = 7-i; j >0; j--) {
            prod *= 16;
        }
        prod *= hexchar_to_int(hex[i]);
        sum += prod;
    }
    return sum;
}

unsigned get_tag(unsigned address, unsigned blocksize, unsigned numsetsize) {
    return address >> (blocksize + numsets);
}

unsigned get_index(unsigned address, unsigned blocksize, unsigned numsetsize) {
    unsigned index = address << (32 - (blocksize + numsetsize));
    index = index >> blocksize;
    return index;
}

unsigned get_offset(unsigned address, unsigned blocksize) {
    return address >> (32 - blocksize);
}

struct Cache buildCache(unsigned numsets, unsigned blocksperset, unsigned bytesperblock) {
    struct Cache cache;
    cache.sets = std::map <uint32_t, Set *> sets;
    for (int i = 0; i < numsets; i++) {
        struct Set set;
        for (int j = 0; j < blocksperset; j++) {
            set.index
        }
        sets[i] = 
    }
}

//int to slot 
struct Slot mem_to_slot(unsigned address) {
    struct Slot slot;
    unsigned tag = get_tag(address);
}

//add Slot to Set
void slot_to_set() {
}

//add Set to Cache
void set_to_cache() {
}

//load
void load(unsigned address, struct Cache cache) {
    unsigned tag = get_tag(address);
    unsigned index = get_index(address);
    unsigned offset = get_offset(address);
    //if hit --> do nothing
    
    std::map <uint32_t, Set *> sets = cache.sets;
    
    for (auto it = sets.begin(); it != sets.end(); it++) { // go through the sets in cache
        if (it -> first == index) { //at correct index
            std::map <uint32_t, Slot *> slots = (*it -> second).index;
            for (auto it2 = slots.begin(); it2 != slots.end(); it2++) { // found the set, go through the offset in the set
                if (it2 -> first == offset) { // found the slot, now to load
                    //if tag is null --> fill
                    if ((*it2 -> second).tag == NULL) {
                        (*it2 -> second).tag = tag;
                    }
                    //if tag not null --> LRU (not applicable for direct mapping)
                    else if ((*it2 -> second).tag != tag) {
                        (*it2 -> second).tag = tag;
                    }
                }
            }
        }
    }
}

//store
void store(unsigned address, struct Cache cache, bool wt, bool wb, bool wa, bool nwa) {
    //if hit
    unsigned tag = get_tag(address);
    unsigned index = get_index(address);
    unsigned offset = get_offset(address);
    //if hit --> do nothing
    
    std::map <uint32_t, Set *> sets = cache.sets;

    bool hit = false; //check index exists
    bool val_match = false; //check if value matches (in filled slot)
    
    for (auto it = sets.begin(); it != sets.end(); it++) { // go through the sets in cache
        if (it -> first == index) { //at correct index
            std::map <uint32_t, Slot *> slots = (*it -> second).index;
            for (auto it2 = slots.begin(); it2 != slots.end(); it2++) { // found the set, go through the offset in the set
                if (it2 -> first == offset) { // found the slot, now to load
                    //if tag is null --> fill
                    if ((*it2 -> second).tag == tag) {//cache hit
                        hit = true;
                        if (wt) {
                            //increment write-through counter
                        }
                        else {
                            //increment write-back counter
                            (*it2 -> second).dirty = true;
                        }
                    }
                    else {//cache miss
                        if (wa) {
                            (*it2 -> second).tag = tag
                            //increment write-allocate counter

                        }
                        else {
                            //increment no-write-allocate counter
                        }
                    }
                }
            }
        }
    }
}

#endif