#ifndef CACHESIMFUNCS_H
#define CACHESIMFUNCS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include "cachesimfuncs.h"

//TO DO: Write Functions
struct Slot{
    uint32_t tag;
    bool valid; // true if filled, false by default (unfilled)
    uint32_t load_ts, access_ts;
    bool dirty;
};

struct Set{
    uint32_t offset;
    Slot * slot;
};

struct Sets {
    std::vector <struct Set> sets; //in direct mapping/fully associative, size of 1 
    uint32_t index; //non-applicable in fully associative
};

struct Cache{
    std::vector <struct Sets> sets_list; // list of sets, each set corresponds to one index
    unsigned numsets;
    unsigned blocksperset;
    unsigned bytesperblock;
};

struct Cache buildCache(unsigned numsets, unsigned blocksperset, unsigned bytesperblock) {
    struct Slot slot;
    struct Set create_set;
    struct Sets create_sets;
    struct Cache cache;
    
    std::vector <struct Set> sets;
    std::vector <struct Sets> sets_list;

    for (int i = 0; i < numsets; i++) { //initialize each set
        uint32_t offset = 0;

        for (int j = 0; j < blocksperset; j++) { //initialize blocks in each set
            //initializing a slot
            slot.valid = false;
            slot.dirty = false;
            slot.tag = NULL;
            slot.load_ts = NULL;
            slot.access_ts = NULL;

            create_set.offset = offset;
            create_set.slot = &slot;
            offset++;//unique offset value in each set
            sets.push_back(create_set); //add block (set) to sets
        }
        create_sets.sets = sets;
        create_sets.index = NULL; //??? is index initialized to null? 
        sets_list.push_back(create_sets);
        sets.clear(); //empty vector to use for next block
    }
    cache.sets_list = sets_list;
    cache.numsets = numsets;
    cache.blocksperset = blocksperset;
    cache.bytesperblock = bytesperblock;
}

unsigned get_tag(unsigned address, unsigned blocksperset, unsigned numsets) {
    unsigned blockbits = log2(blocksperset) + 1;
    unsigned setbits = log2(numsets) + 1;
    return address >> (blockbits + setbits);
}

unsigned get_index(unsigned address, unsigned blocksperset, unsigned numsets) {
    unsigned blockbits = log2(blocksperset) + 1;
    unsigned setbits = log2(numsets) + 1;
    unsigned index = address << (32 - (blockbits + setbits));
    index = index >> blockbits;
    return index;
}

unsigned get_offset(unsigned address, unsigned blocksperset) {
    unsigned blockbits = log2(blocksperset) + 1;
    return address >> (32 - blocksperset);
}

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

// //char to int helper function
// int hexchar_to_int(char hex) {
//     char hexchar[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
//     for (int i = 0; i < 16; i++) {
//         if (hexchar[i] == hex) {
//             return i;
//         }
//     }
// }

// //hex to dec function 
// unsigned hex_to_dec(char hex[]) {
//     unsigned sum;
//     for (int i = 0; i < 8; i++) {
//         unsigned prod = 1;
//         for (int j = 7-i; j >0; j--) {
//             prod *= 16;
//         }
//         prod *= hexchar_to_int(hex[i]);
//         sum += prod;
//     }
//     return sum;
// }

//memory address turns slot to valid
struct Slot mem_to_slot(struct Cache cache, unsigned address, unsigned blocksize, unsigned numsetsize) {
    struct Slot slot;
    unsigned tag = get_tag(address, blocksize, numsetsize);
    unsigned index = get_index(address, blocksize, numsetsize);
    unsigned offset = get_offset(address, blocksize);
    for (int i = 0; i < numsets; i++) {
        std::map <uint32_t, Slot *> slots;
        for (int j = 0; j < blocksperset; j++) {
            struct Slot slot;
            slot.valid = false;
            slot.dirty = false;
            slots[i] = slot;
        }
        sets[i] = slots;
    }

}

//add Slot to Set
void slot_to_set() {
}

//add Set to Cache
void set_to_cache() {
}

//load direct mapping (set_length = number of blocks per each set/index)
void load_dm(unsigned address, struct Cache cache) {
    unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
    unsigned index = get_index(address, cache.blocksperset, cache.numsets);
    unsigned offset = get_offset(address, cache.blocksperset);
    
    std::vector <struct Sets> sets_list = cache.sets_list;

    bool existing_ind = false; //check for if index exists, if does not then needs to be added
    
    for (auto& it : sets_list) {
        if (it.index == index) { //found index
            existing_ind = true;
            for (auto& it2 : it.sets) {
                if ((*it2.slot).tag != tag) { //load miss
                    (*it2.slot).tag = tag;
                }
            }
        }
    }

    //do we need this? I think so right??? do we need to do LRU on this too? 
    bool exit = false; //check to break out of loop

    if (!existing_ind) {//index does not exist --> load in 
        for (auto& it : sets_list) {
            if(exit) {
                break;
            }
            for (auto& it2 : it.sets) {
                if(exit) {
                    break;
                }
                if (!(*it2.slot).valid) { //empty 
                    (*it2.slot).tag = tag;
                    (*it2.slot).valid = true;//filled
                    it.index = index;
                    exit = true;
                }
            }
        }
    }

    // //iterate through cache and set corresponding tag
    // for (auto it = sets.begin(); it != sets.end(); it++) { // go through the sets in cache
    //     if (it -> first == index) { //at correct index
    //         std::map <uint32_t, Slot *> slots = (*it -> second).offset;
    //         for (auto it2 = slots.begin(); it2 != slots.end(); it2++) { // found the set, go through the offset in the set
    //             //increment all the valid load access timestamps of slots at correct index
    //             if (it2 -> first == offset) { // found the slot, now to load
    //                 //if tag is null --> fill
    //                 if ((*it2 -> second).tag == NULL || (*it2 -> second).tag != tag ) { //if not filled or tag doesn't match
    //                     (*it2 -> second).tag = tag;
    //                 }
    //             }
    //         }
    //     }
    // }
}

//loading fully associative
void load_fa(unsigned address, struct Cache cache) {
    unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
    unsigned offset = get_offset(address, cache.blocksperset);

    std::vector <struct Sets> sets_list = cache.sets_list;

    bool hit = false; //cache hit checker
    uint32_t hit_access_ts = 0;//access_ts holder
    
    for (auto& it : sets_list) {//check if hit
        for (auto& it2 : it.sets) {
            if ((*it2.slot).valid && (*it2.slot).tag == tag) { //load hit
                hit = true;
                hit_access_ts = (*it2.slot).access_ts; //get access timestamp of hit
            }
        }
    }

    if (hit) {//cache hit, update access timestamps
        for (auto& it : sets_list) {
            for (auto& it2 : it.sets) {
                if ((*it2.slot).valid && (*it2.slot).access_ts <= hit_access_ts) { 
                    if ((*it2.slot).access_ts == hit_access_ts) {
                        (*it2.slot).access_ts = 0; //reset most recently accessed to 0
                    }
                    (*it2.slot).access_ts++; //increment non-accessed timestamps
                }
            }
        }
    }

    if (!hit) {//load miss --> load in to empty slot
        for (auto& it : sets_list) {
            for (auto& it2 : it.sets) {
                if ((*it2.slot).valid) { //increment load timestamp for valid blocks
                    (*it2.slot).load_ts++;
                    (*it2.slot).access_ts++;
                }
                if (!(*it2.slot).valid && !hit) { //empty
                    (*it2.slot).tag = tag;
                    (*it2.slot).valid = true;
                    it2.offset = offset;
                    (*it2.slot).load_ts = 0;
                    (*it2.slot).access_ts = 0;
                    hit = true;
                }
            }
        }
    }

    if (!hit) { //load miss and no empty slots --> LRU
        //evict block --> LRU
        unsigned LRU = cache.blocksperset - 1;
        for (auto& it : sets_list) {
            for (auto& it2 : it.sets) {
                if ((*it2.slot).valid && (*it2.slot).access_ts != LRU) { //increment non-LRU filled blocks
                    (*it2.slot).load_ts++;
                    (*it2.slot).access_ts++;
                }
                if ((*it2.slot).valid && (*it2.slot).access_ts == LRU && !hit) { //locate LRU
                    (*it2.slot).tag = tag;
                    it2.offset = offset;
                    (*it2.slot).load_ts = 0;
                    (*it2.slot).access_ts = 0;
                    hit = true;
                }
            }
        }
    }
}

//loading set associative
void load_sa(unsigned address, struct Cache cache) {
    unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
    unsigned index = get_index(address, cache.blocksperset, cache.numsets);
    unsigned offset = get_offset(address, cache.blocksperset);
    
    std::vector <struct Sets> sets_list = cache.sets_list;

    bool existing_ind = false; //check for if index exists, if does not then needs to be added
    bool hit = false;
    
    for (auto& it : sets_list) {
        if (it.index == index) { //found index
            existing_ind = true;
            for (auto& it2 : it.sets) {
                if ((*it2.slot).tag == tag) { //load hit
                    hit = true;
                }
            }
            if (!hit) {//load miss
                for (auto& it2 : it.sets) {
                    if (hit/*placeholder; if timestamp matches criteria*/) {
                        (*it2.slot).tag = tag;
                    }
                }

            }
        }
    }

    //do we need this? I think so right??? do we need to do LRU on this too? 
    bool exit = false; //check to break out of loop

    if (!hit) {//index does not exist --> load in 
        for (auto& it : sets_list) {
            if(exit) {
                break;
            }
            for (auto& it2 : it.sets) {
                if(exit) {
                    break;
                }
                if (!(*it2.slot).valid) { //empty 
                    (*it2.slot).tag = tag;
                    (*it2.slot).valid = true;//filled
                    it.index = index;
                    exit = true;
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
            std::map <uint32_t, Slot *> slots = (*it -> second).offset;
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