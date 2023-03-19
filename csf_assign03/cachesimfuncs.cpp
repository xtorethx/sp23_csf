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

// //add Slot to Set
// void slot_to_set() {
// }

// //add Set to Cache
// void set_to_cache() {
// }

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
    if (!existing_ind) {//index does not exist --> load in 
        for (auto& it : sets_list) {
            for (auto& it2 : it.sets) {
                if (!(*it2.slot).valid && !existing_ind) { //empty 
                    (*it2.slot).tag = tag;
                    (*it2.slot).valid = true;//filled
                    it2.offset = offset;
                    it.index = index;
                    existing_ind = true;
                    (*it2.slot).load_ts = 0;
                    (*it2.slot).access_ts = 0;
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

    std::vector <struct Sets> sets_list = cache.sets_list; //1 set

    bool hit = false; //cache hit checker
    uint32_t hit_access_ts = 0;//access_ts holder
    
    for (auto& it : sets_list) {//check if hit; 1 sets
        for (auto& it2 : it.sets) { //each sets has n blocks
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
                    else {
                        (*it2.slot).access_ts++; //increment non-accessed timestamps
                    }
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
    uint32_t hit_access_ts = 0;//access_ts holder
    
    for (auto& it : sets_list) { //n sets
        if (it.index == index) { //found index
            existing_ind = true;
            for (auto& it2 : it.sets) { //1 block
                if ((*it2.slot).valid && (*it2.slot).tag == tag) { //load hit
                    hit = true;
                    hit_access_ts = (*it2.slot).access_ts; //get access timestamp of hit
                }
            }

            if (hit) {//cache hit, update access timestamps
                for (auto& it2 : it.sets) {
                    if ((*it2.slot).valid && (*it2.slot).access_ts <= hit_access_ts) { 
                        if ((*it2.slot).access_ts == hit_access_ts) {
                        (*it2.slot).access_ts = 0; //reset most recently accessed to 0
                        }
                        else {
                            (*it2.slot).access_ts++; //increment non-accessed timestamps
                        }
                    }
                }
            }

            if (!hit) {//load miss --> load into empty slot
                for (auto& it2 : it.sets) {
                    if ((*it2.slot).valid) { //increment load timestamp for valid blocks
                        (*it2.slot).load_ts++;
                        (*it2.slot).access_ts++;
                    }   
                    if (!(*it2.slot).valid && !hit) {
                        (*it2.slot).tag = tag;
                        (*it2.slot).valid = true;
                        it2.offset = offset;
                        (*it2.slot).load_ts = 0;
                        (*it2.slot).access_ts = 0;
                        hit = true;
                    }
                }
            }

            if (!hit) { //load miss and no empty slots --> LRU
                unsigned LRU = cache.blocksperset - 1;
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

    //do we need this? I think so right??? do we need to do LRU on this too? 
    if (!existing_ind) {//index does not exist --> load in 
        for (auto& it : sets_list) {
            for (auto& it2 : it.sets) {
                if (it.index == NULL && !(*it2.slot).valid && !existing_ind) { //empty 
                    (*it2.slot).tag = tag;
                    (*it2.slot).valid = true;//filled
                    it2.offset = offset;
                    it.index = index;
                    (*it2.slot).load_ts = 0;
                    (*it2.slot).access_ts = 0;
                    existing_ind = true;
                }
            }
        }
    }
}

//store (direct mapping)
void store_dm(unsigned address, struct Cache cache, bool wb, bool wa) { //wb true if write_back, false if write through; wa true if write_allocate, false if no write allocate
    unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
    unsigned index = get_index(address, cache.blocksperset, cache.numsets);
    unsigned offset = get_offset(address, cache.blocksperset);

    std::vector <struct Sets> sets_list = cache.sets_list;

    //bool existing_ind = false; //check for if index exists, if does not then needs to be added
    bool cache_hit = false; 
    
    for (auto& it : sets_list) {
        if (it.index == index) { //found index
            // existing_ind = true;
            for (auto& it2 : it.sets) {
                if ((*it2.slot).tag == tag) { //cache hit
                    cache_hit = true;
                    if (wb) { //write back
                        (*it2.slot).dirty = true;
                        //TO DO: increment write back counter; write to memory/cycle count stuff
                    }
                    else { //write through
                        //TO DO: increment write through counter; write to memory/cycle count stuff
                    }
                }

                else { //cache miss
                    if (wa) {//write allocate
                        (*it2.slot).tag = tag;
                        //TO DO: increment write allocate counter; write to memory/cycle count stuff
                    }
                    else {//no write allocate
                        //TO DO: increment no write allocate counter; write to memory/cycle count stuff
                    }
                }
            }
        }
    }
    // //do we need this? I think so right??? do we need to do LRU on this too? 
    // if (!existing_ind) {//index does not exist --> load in 
    //     for (auto& it : sets_list) {
    //         for (auto& it2 : it.sets) {
    //             if (!(*it2.slot).valid && !existing_ind) { //empty 
    //                 (*it2.slot).tag = tag;
    //                 (*it2.slot).valid = true;//filled
    //                 it2.offset = offset;
    //                 it.index = index;
    //                 existing_ind = true;
    //                 (*it2.slot).load_ts = 0;
    //                 (*it2.slot).access_ts = 0;
    //             }
    //         }
    //     }
    // }
}

//store (fully associative)
void store_fa(unsigned address, struct Cache cache, bool wb, bool wa) { //wb true if write_back, false if write through; wa true if write_allocate, false if no write allocate
    unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
    unsigned offset = get_offset(address, cache.blocksperset);

    std::vector <struct Sets> sets_list = cache.sets_list; //1 set

    bool hit = false; //cache hit checker
    uint32_t hit_access_ts = 0;//access_ts holder
    
    for (auto& it : sets_list) {//check if hit; 1 sets
        for (auto& it2 : it.sets) { //each sets has n blocks
            if ((*it2.slot).valid && (*it2.slot).tag == tag) { //load hit
                hit = true;
                hit_access_ts = (*it2.slot).access_ts; //get access timestamp of hit
            }
        }
    }

    if (hit) {//store hit, update access timestamps
        for (auto& it : sets_list) {
            for (auto& it2 : it.sets) {
                if ((*it2.slot).valid && (*it2.slot).access_ts <= hit_access_ts) { 
                    if ((*it2.slot).access_ts == hit_access_ts) {
                        (*it2.slot).access_ts = 0; //reset most recently accessed to 0
                        if (wb) { //write back
                            (*it2.slot).dirty = true;
                            //TO DO: increment write back counter; write to memory/cycle count stuff
                        }
                        else { //write through
                            //TO DO: increment write through counter; write to memory/cycle count stuff
                        }
                    }
                    else {
                        (*it2.slot).access_ts++; //increment non-accessed timestamps
                    }
                }
            }
        }
    }

    if (!hit) {//store miss
        if (wa) { //write allocate --> load to cache
            for (auto& it : sets_list) { //load into empty spot
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

            //TO DO: increment write allocate counter; write to memory/cycle count stuff
        }
        else {//no write allocate
            //TO DO: increment no write allocate counter; write to memory/cycle count stuff
        }
    }    
}

//store (set associative)
void store_sa(unsigned address, struct Cache cache, bool wb, bool wa) { //wb true if write_back, false if write through; wa true if write_allocate, false if no write allocate
    unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
    unsigned index = get_index(address, cache.blocksperset, cache.numsets);
    unsigned offset = get_offset(address, cache.blocksperset);

    std::vector <struct Sets> sets_list = cache.sets_list;

    //check if hit or miss
    bool hit = false; //check index exists
}

#endif