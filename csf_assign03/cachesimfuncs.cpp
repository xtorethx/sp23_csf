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

struct Slot{
    uint32_t tag;
    bool valid; // true if filled, false by default (unfilled)
    uint32_t load_ts, access_ts;
    bool dirty;
};

struct Block{
    uint32_t offset;
    Slot * slot;
};

struct Sets {
    std::vector <struct Block> blocks; //in direct mapping/fully associative, size of 1 
    uint32_t index; //non-applicable in fully associative
    uint32_t filled; //number of valid blocks in set
};

struct Cache{
    std::vector <struct Sets> block_list; // list of sets, each set corresponds to one index
    unsigned numsets; // first param
    unsigned blocksperset; // second param
    unsigned bytesperblock; // third param
    unsigned total_loads;
    unsigned total_stores;
    unsigned load_hits;
    unsigned load_misses;
    unsigned store_hits;
    unsigned store_misses;
    unsigned total_cycles;
};

struct Cache buildCache(unsigned numsets, unsigned blocksperset, unsigned bytesperblock) {
    struct Slot slot;
    struct Block create_set;
    struct Sets create_sets;
    struct Cache cache;
    
    std::vector <struct Block> sets;
    std::vector <struct Sets> block_list;

    for (unsigned i = 0; i < numsets; i++) { //initialize each set
        for (unsigned j = 0; j < blocksperset; j++) { //initialize blocks in each set
            //initializing a slot
            slot.valid = false;
            slot.dirty = false;
            slot.tag = NULL;
            slot.load_ts = NULL;
            slot.access_ts = NULL;

            create_set.offset = j;
            create_set.slot = &slot;
            sets.push_back(create_set); //add block (set) to sets
        }
        create_sets.blocks = sets;
        create_sets.index = i;
        create_sets.filled = 0;
        block_list.push_back(create_sets);
        sets.clear(); //empty vector to use for next block
    }
    cache.block_list = block_list;
    cache.numsets = numsets;
    cache.blocksperset = blocksperset;
    cache.bytesperblock = bytesperblock;
    cache.total_loads = 0;
    cache.total_stores = 0;
    cache.load_hits = 0;
    cache.load_misses = 0;
    cache.store_hits = 0;
    cache.total_cycles = 0;

    return cache;
}

/*
 * Return unsigned int corresponding to tag
 *
 * Parameters:
 *   address - unsigned
 *   blocksperset - unsigned
 *   numsets - unsigned
 *
 * Returns:
 *   unsigned int
 */
unsigned get_tag(unsigned address, unsigned blocksperset, unsigned numsets) {
    unsigned blockbits = log2(blocksperset) + 1;
    unsigned setbits = log2(numsets) + 1;
    return address >> (blockbits + setbits);
}

/*
 * Return unsigned int corresponding to index
 *
 * Parameters:
 *   address - unsigned
 *   blocksperset - unsigned
 *   numsets - unsigned
 *
 * Returns:
 *   unsigned int
 */
unsigned get_index(unsigned address, unsigned blocksperset, unsigned numsets) {
    unsigned blockbits = log2(blocksperset) + 1;
    unsigned setbits = log2(numsets) + 1;
    unsigned index = address << (32 - (blockbits + setbits));
    index = index >> blockbits;
    return index;
}

/*
 * Return unsigned int corresponding to offset
 *
 * Parameters:
 *   address - unsigned
 *   blocksperset - unsigned
 *   numsets - unsigned
 *
 * Returns:
 *   unsigned int
 */
unsigned get_offset(unsigned address, unsigned blocksperset) {
    unsigned blockbits = log2(blocksperset) + 1;
    return address >> (32 - blockbits);
}


/*
 * read memory access trace from standard input
 * memory address is tag index offset
 * given: num sets, num blocks, num bytes/ block (= block size)
 *
 * Parameters:
 *   ls - char
 *   memaddress - char array
 *   tmp - char
 */
int read(struct Cache &cache, char &ls, char memaddress[], char tmp[]) {
    if (scanf(" %c", &ls) == EOF) {
        return -1;
    }
    scanf("%10s", memaddress);
    scanf(" %s", tmp);
    if (ls == 'l') {
        cache.total_loads++;
    } else if (ls == 's') {
        cache.total_stores++;
    }
    return 0;
}

/*
 * write to standard output
 *
 * Parameters:
 *   
 */
void write(struct Cache cache) {
    std::cout << "Total loads: " << cache.total_loads << std::endl;
    std::cout << "Total stores: " << cache.total_stores << std::endl;
    std::cout << "Load hits: " << cache.load_hits << std::endl;
    std::cout << "Load misses: " << cache.load_misses << std::endl;
    std::cout << "Store hits: " << cache.store_hits << std::endl;
    std::cout << "Store misses: " << cache.store_misses << std::endl;
    std::cout << "Total cycles: " << cache.total_cycles << std::endl;
}

//char to int helper function
int hexchar_to_int(char hex) {
    char hexchar[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    for (int i = 0; i < 16; i++) {
        if (hexchar[i] == hex) {
            return i;
        }
    }
    return 0;
}

//hex to dec function 
unsigned hex_to_dec(char hex_unformated[]) {
    char hex[8];
    for (int i = 2; i < 10; i++) {
        hex[i-2] = hex_unformated[i];
    }
    unsigned sum = 0;
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

/*
 * update access timestamp 
 *
 * Parameters:
 *   sets_list - vector of type Sets struct
 *   hit_access_ts - uint32_t
 */
void update_access_ts(unsigned index, std::vector <struct Sets> sets_list, uint32_t hit_access_ts, bool store, bool wb) {
    for (auto& it : sets_list) {
        if (it.index == index){
            for (auto& it2 : it.blocks) {
                if ((*it2.slot).valid && (*it2.slot).access_ts <= hit_access_ts) { 
                    if ((*it2.slot).access_ts == hit_access_ts) {
                        (*it2.slot).access_ts = 0; //reset most recently accessed to 0
                            if (store) {
                                if (wb) { //write back
                                (*it2.slot).dirty = true;
                                //TO DO: increment write back counter; write to memory/cycle count stuff
                            }
                            else { //write through
                                //TO DO: increment write through counter; write to memory/cycle count stuff
                            }
                        }
                    }
                    else {
                        (*it2.slot).access_ts++; //increment non-accessed timestamps
                    }
                }
            }
        }
    }
}

/*
 * evict least recently used for fully associative structure
 *
 * Parameters:
 *   sets_list - vector of type Sets struct
 *   hit - bool
 *   offset - unsigned int
 *   LRU - unsigned int
 *   tag - unsigned int
 */
void evict_block_LRU(unsigned index, std::vector <struct Sets> &block_list, bool hit, unsigned offset, unsigned LRU, unsigned tag) {
    for (auto& it : block_list) {
        if (it.index == index){
            for (auto& it2 : it.blocks) {
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

/*
 * load (set_length = number of blocks per each set/index)
 *
 * Parameters:
 *   address - unsigned
 *   cache - Cache struct
 */
void load(unsigned address, struct Cache &cache) {
    unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
    unsigned index = get_index(address, cache.blocksperset, cache.numsets);
    unsigned offset = get_offset(address, cache.blocksperset);
    
    std::vector <struct Sets> block_list = cache.block_list;

    uint32_t hit_access_ts = 0;//access_ts holder
    bool hit = false; //check for if hit, false if miss
    
    for (auto& it : block_list) {
        if (it.index == index || cache.numsets == 1) { //found index, numsets = 1 check for fully associative
            for (auto& it2 : it.blocks) {
                if ((*it2.slot).tag == tag && !hit) { //load hit
                    hit = true;
                    hit_access_ts = (*it2.slot).access_ts; //get access timestamp of hit
                }
            }

            if (hit) { //hit, update access timestamps
                update_access_ts(index, block_list, hit_access_ts, false, false);
                cache.load_hits++;
                cache.total_cycles++;
            }

            if (!hit) {//iterated through cache and could not find, load miss 
                cache.load_misses++;
                cache.total_cycles = cache.total_cycles + (4/cache.bytesperblock) * 100;

                if (it.filled == cache.blocksperset) {//no more empty spots, evict
                    unsigned LRU = cache.blocksperset - 1;
                    evict_block_LRU(index, block_list, hit, offset, LRU, tag);
                    cache.total_cycles = cache.total_cycles + (4/cache.bytesperblock) * 100;
                }

                else {//empty spot exists, find and fill
                    for (auto& it2 : it.blocks) {
                        if (!(*it2.slot).valid && !hit) {//empty slot
                            hit = true;
                            it2.offset = offset;
                            (*it2.slot).tag = tag;
                            (*it2.slot).valid = true;
                            (*it2.slot).load_ts = 0;
                            (*it2.slot).access_ts = 0;
                            it.filled++;
                        }
                        else if ((*it2.slot).valid) {
                            (*it2.slot).load_ts++;
                            (*it2.slot).access_ts++;
                        }
                    }
                }
            }
        }
    }
}



// /*
//  * load into empty slot for fully associative structure, returns true if empty slot exists
//  *
//  * Parameters:
//  *   sets_list - vector of type Sets struct
//  *   hit - bool
//  *   offset - unsigned int
//  *   tag - unsigned int
//  *
//  * Returns:
//  *   bool
//  */
// bool load_empty_fa(std::vector <struct Sets> &block_list, bool hit, unsigned offset, unsigned tag) {
//     for (auto& it : block_list) {
//         for (auto& it2 : it.blocks) {
//             if ((*it2.slot).valid) { //increment load timestamp for valid blocks
//                 (*it2.slot).load_ts++;
//                 (*it2.slot).access_ts++;
//             }
//             if (!(*it2.slot).valid && !hit) { //empty
//                 (*it2.slot).tag = tag;
//                 (*it2.slot).valid = true;
//                 it2.offset = offset;
//                 (*it2.slot).load_ts = 0;
//                 (*it2.slot).access_ts = 0;
//                 return true;
//             }
//         }
//     }
//     return false;
// }



// /*
//  * loading fully associative
//  *
//  * Parameters:
//  *   address - unsigned int
//  *   cache - Cache struct
//  */
// void load_fa(unsigned address, struct Cache &cache) {
//     unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
//     unsigned offset = get_offset(address, cache.blocksperset);
//     std::vector <struct Sets> block_list = cache.block_list; //1 set
//     bool hit = false; //cache hit checker
//     uint32_t hit_access_ts = 0;//access_ts holder
//     for (auto& it : block_list) {//check if hit; 1 sets
//         for (auto& it2 : it.blocks) { //each sets has n blocks
//             if ((*it2.slot).valid && (*it2.slot).tag == tag) { //load hit
//                 hit = true;
//                 hit_access_ts = (*it2.slot).access_ts; //get access timestamp of hit
//             }
//         }
//     }
//     if (hit) {//cache hit, update access timestamps
//         update_access_ts_fa(block_list, hit_access_ts);
//         cache.load_hits++;
//         cache.total_cycles++;
//     }
//     if (!hit) {//load miss --> load in to empty slot
//         hit = load_empty_fa(block_list, hit, offset, tag);
//         cache.load_misses++;
//         cache.total_cycles = cache.total_cycles + (4/cache.bytesperblock) * 100;
//     }
//     if (!hit) { //load miss and no empty slots --> LRU
//         //evict block --> LRU
//         unsigned LRU = cache.blocksperset - 1;
//         evict_block_LRU_fa(block_list, hit, offset, LRU, tag);
//         cache.total_cycles = cache.total_cycles + (4/cache.bytesperblock) * 100;
//     }
// }

// /*
//  * loading set associative
//  *
//  * Parameters:
//  *   address - unsigned int
//  *   cache - Cache struct
//  */
// void load_sa(unsigned address, struct Cache &cache) {
//     unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
//     unsigned index = get_index(address, cache.blocksperset, cache.numsets);
//     unsigned offset = get_offset(address, cache.blocksperset);
    
//     std::vector <struct Sets> block_list = cache.block_list;

//     //bool existing_ind = false; //check for if index exists, if does not then needs to be added
//     bool hit = false;
//     uint32_t hit_access_ts = 0;//access_ts holder
    
//     for (auto& it : block_list) { //n sets
//         if (it.index == index) { //found index
//             //existing_ind = true;
//             for (auto& it2 : it.blocks) { //1 block
//                 if ((*it2.slot).valid && (*it2.slot).tag == tag) { //load hit
//                     hit = true;
//                     hit_access_ts = (*it2.slot).access_ts; //get access timestamp of hit
//                 }
//             }

//             if (hit) {//cache hit, update access timestamps
//                 for (auto& it2 : it.blocks) {
//                     if ((*it2.slot).valid && (*it2.slot).access_ts <= hit_access_ts) { 
//                         if ((*it2.slot).access_ts == hit_access_ts) {
//                         (*it2.slot).access_ts = 0; //reset most recently accessed to 0
//                         }
//                         else {
//                             (*it2.slot).access_ts++; //increment non-accessed timestamps
//                         }
//                     }
//                 }
//                 cache.load_hits++;
//                 cache.total_cycles++;
//             }

//             if (!hit) {//load miss --> load into empty slot
//                 for (auto& it2 : it.blocks) {
//                     if ((*it2.slot).valid) { //increment load timestamp for valid blocks
//                         (*it2.slot).load_ts++;
//                         (*it2.slot).access_ts++;
//                     }   
//                     if (!(*it2.slot).valid && !hit) {
//                         (*it2.slot).tag = tag;
//                         (*it2.slot).valid = true;
//                         it2.offset = offset;
//                         (*it2.slot).load_ts = 0;
//                         (*it2.slot).access_ts = 0;
//                         hit = true;
//                     }
//                 }
//                 cache.load_misses++;
//                 cache.total_cycles = cache.total_cycles + (4/cache.bytesperblock) * 100;
//             }

//             if (!hit) { //load miss and no empty slots --> LRU
//                 unsigned LRU = cache.blocksperset - 1;
//                 for (auto& it2 : it.blocks) {
//                     if ((*it2.slot).valid && (*it2.slot).access_ts != LRU) { //increment non-LRU filled blocks
//                         (*it2.slot).load_ts++;
//                         (*it2.slot).access_ts++;
//                     }
//                     if ((*it2.slot).valid && (*it2.slot).access_ts == LRU && !hit) { //locate LRU
//                         (*it2.slot).tag = tag;
//                         it2.offset = offset;
//                         (*it2.slot).load_ts = 0;
//                         (*it2.slot).access_ts = 0;
//                         hit = true;
//                     }
//                 }
//                 cache.total_cycles = cache.total_cycles + (4/cache.bytesperblock) * 100;
//             }
//         }
//     }
// }

/*
 * direct mapping store
 * wb true if write_back, false if write through; wa true if write_allocate, false if no write allocate
 *
 * Parameters:
 *   address - unsigned int
 *   cache - Cache struct
 *   wb - bool
 *   wa - bool
 */
void store(unsigned address, struct Cache &cache, bool wb, bool wa) { 
    unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
    unsigned index = get_index(address, cache.blocksperset, cache.numsets);
    unsigned offset = get_offset(address, cache.blocksperset);

    std::vector <struct Sets> block_list = cache.block_list;

    bool hit = false; 
    uint32_t hit_access_ts = 0;//access_ts holder
    
    for (auto& it : block_list) {
        if (it.index == index || cache.numsets == 1) { //found index
            // existing_ind = true;
            for (auto& it2 : it.blocks) {
                if ((*it2.slot).valid && (*it2.slot).tag == tag) { //cache hit
                    hit = true;
                    hit_access_ts = (*it2.slot).access_ts; //get access timestamp of hit
                    cache.store_hits++;
                    cache.total_cycles++;
                    if (wb) { //write back
                        (*it2.slot).dirty = true;
                        //TO DO: increment write back counter; write to memory/cycle count stuff
                    }
                    else { //write through
                        //TO DO: increment write through counter; write to memory/cycle count stuff
                    }
                }
                if (hit) {
                    update_access_ts(block_list, hit_access_ts, true, wb);
                    cache.store_hits++;
                    cache.total_cycles++;
                }
                else { //cache miss
                    cache.store_misses++;    
                    if (wa) {//write allocate
                        (*it2.slot).tag = tag;
                        cache.total_cycles = cache.total_cycles + (4/cache.bytesperblock) * 100;
                        load(address, cache);
                        //TO DO: increment write allocate counter; write to memory/cycle count stuff
                    }
                    else {//no write allocate
                        //TO DO: increment no write allocate counter; write to memory/cycle count stuff
                    }
                }
            }
        }
    }
}

// /*
//  * update access timestamp and write hit behavior for fully associative
//  *
//  * Parameters:
//  *   sets_list - vector of Sets struct
//  *   hit_access_ts - uint32_t
//  *   wb - bool
//  */
// void update_access_ts_write_fa(std::vector <struct Sets> &block_list, uint32_t hit_access_ts, bool wb) {
//     for (auto& it : block_list) {
//         for (auto& it2 : it.blocks) {
//             if ((*it2.slot).valid && (*it2.slot).access_ts <= hit_access_ts) { 
//                 if ((*it2.slot).access_ts == hit_access_ts) {
//                     (*it2.slot).access_ts = 0; //reset most recently accessed to 0
//                     if (wb) { //write back
//                         (*it2.slot).dirty = true;
//                         //TO DO: increment write back counter; write to memory/cycle count stuff
//                     }
//                     else { //write through
//                         //TO DO: increment write through counter; write to memory/cycle count stuff
//                     }
//                 }
//                 else {
//                     (*it2.slot).access_ts++; //increment non-accessed timestamps
//                 }
//             }
//         }
//     }
// }

// void update_access_ts_fa(std::vector <struct Sets> sets_list, uint32_t hit_access_ts) {
//     for (auto& it : sets_list) {
//         for (auto& it2 : it.blocks) {
//             if ((*it2.slot).valid && (*it2.slot).access_ts <= hit_access_ts) { 
//                 if ((*it2.slot).access_ts == hit_access_ts) {
//                     (*it2.slot).access_ts = 0; //reset most recently accessed to 0
//                 }
//                 else {
//                     (*it2.slot).access_ts++; //increment non-accessed timestamps
//                 }
//             }
//         }
//     }
// }

// /*
//  * storing fully associative cache
//  * wb true if write_back, false if write through; wa true if write_allocate, false if no write allocate
//  *
//  * Parameters:
//  *   address - unsigned int
//  *   cache - Cache struct
//  *   wb - bool
//  *   wa - bool
//  */
// void store_fa(unsigned address, struct Cache &cache, bool wb, bool wa) { 
//     unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
//     unsigned offset = get_offset(address, cache.blocksperset);
//     std::vector <struct Sets> block_list = cache.block_list; //1 set
//     bool hit = false; //cache hit checker
//     uint32_t hit_access_ts = 0;//access_ts holder
//     for (auto& it : block_list) {//check if hit; 1 sets
//         for (auto& it2 : it.blocks) { //each sets has n blocks
//             if ((*it2.slot).valid && (*it2.slot).tag == tag) { //load hit
//                 hit = true;
//                 hit_access_ts = (*it2.slot).access_ts; //get access timestamp of hit
//             }
//         }
//     }
//     if (hit) {//store hit, update access timestamps
//         update_access_ts_write_fa(block_list, hit_access_ts, wb);
//         cache.store_hits++;
//         cache.total_cycles++;
//     }
//     if (!hit) {//store miss
//         if (wa) { //write allocate --> load to cache
//             cache.store_misses++;
//             cache.total_cycles = cache.total_cycles + (4/cache.bytesperblock) * 100;
//             hit = load_empty_fa(block_list, hit, offset, tag);
//             if (!hit) { //load miss and no empty slots --> LRU
//                 //evict block --> LRU
//                 unsigned LRU = cache.blocksperset - 1;
//                 evict_block_LRU_fa(block_list, hit, offset, LRU, tag);
//             }
//             //TO DO: increment write allocate counter; write to memory/cycle count stuff
//         }
//         else {//no write allocate
//             //TO DO: increment no write allocate counter; write to memory/cycle count stuff
//         }
//     }    
// }

// /*
//  * storing set associative cache
//  * wb true if write_back, false if write through; wa true if write_allocate, false if no write allocate
//  *
//  * Parameters:
//  *   address - unsigned int
//  *   cache - Cache struct
//  *   wb - bool
//  *   wa - bool
//  */
// void store_sa(unsigned address, struct Cache &cache, bool wb, bool wa) { 
//     unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
//     unsigned index = get_index(address, cache.blocksperset, cache.numsets);
//     unsigned offset = get_offset(address, cache.blocksperset);
    
//     std::vector <struct Sets> sets_list = cache.block_list;

//     //bool existing_ind = false; //check for if index exists, if does not then needs to be added
//     bool hit = false;
//     uint32_t hit_access_ts = 0;//access_ts holder
    
//     for (auto& it : sets_list) { //n sets
//         if (it.index == index) { //found index
//             //existing_ind = true;
//             for (auto& it2 : it.blocks) { //1 block
//                 if ((*it2.slot).valid && (*it2.slot).tag == tag) { //load hit
//                     hit = true;
//                     hit_access_ts = (*it2.slot).access_ts; //get access timestamp of hit
//                 }
//             }

//             if (hit) {//cache hit, update access timestamps
//                 for (auto& it2 : it.blocks) {
//                     if ((*it2.slot).valid && (*it2.slot).access_ts <= hit_access_ts) { 
//                         if ((*it2.slot).access_ts == hit_access_ts) {
//                             (*it2.slot).access_ts = 0; //reset most recently accessed to 0
//                             if (wb) { //write back
//                                 (*it2.slot).dirty = true;
//                                 //TO DO: increment write back counter; write to memory/cycle count stuff
//                             }
//                             else { //write through
//                                 //TO DO: increment write through counter; write to memory/cycle count stuff
//                             }
//                         }
//                         else {
//                             (*it2.slot).access_ts++; //increment non-accessed timestamps
//                         }
//                     }
//                 }
//             }

//             if (!hit) {//store miss 
//                 if (wa) { //write allocate --> load to cache
//                     for (auto& it2 : it.blocks) {
//                         if ((*it2.slot).valid) { //increment load timestamp for valid blocks
//                             (*it2.slot).load_ts++;
//                             (*it2.slot).access_ts++;
//                         }   
//                         if (!(*it2.slot).valid && !hit) {
//                             (*it2.slot).tag = tag;
//                             (*it2.slot).valid = true;
//                             it2.offset = offset;
//                             (*it2.slot).load_ts = 0;
//                             (*it2.slot).access_ts = 0;
//                             hit = true;
//                         }
//                     }
//                      if (!hit) { //store miss and no empty slots --> LRU
//                         unsigned LRU = cache.blocksperset - 1;
//                         for (auto& it2 : it.blocks) {
//                             if ((*it2.slot).valid && (*it2.slot).access_ts != LRU) { //increment non-LRU filled blocks
//                                 (*it2.slot).load_ts++;
//                                 (*it2.slot).access_ts++;
//                             }
//                             if ((*it2.slot).valid && (*it2.slot).access_ts == LRU && !hit) { //locate LRU
//                                 (*it2.slot).tag = tag;
//                                 it2.offset = offset;
//                                 (*it2.slot).load_ts = 0;
//                                 (*it2.slot).access_ts = 0;
//                                 hit = true;
//                             }
//                         }
//                     }
                    
//                     //TO DO: increment write allocate counter; write to memory/cycle count stuff
//                 }
//                 else {//no write allocate
//                         //TO DO: increment no write allocate counter; write to memory/cycle count stuff
//                 }
//             }
//         }
//     }
// }

#endif