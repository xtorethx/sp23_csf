#ifndef CACHESIMFUNCS_H
#define CACHESIMFUNCS_H

#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include "cachesimfuncs.h"

//README
//the error is not that something is doubling up
//error is not in read reading in wrong chunks
    //changed read for mem address to simplify but should not be an issue
//not that the offset, index, tag are spitting out junk
    //using bit shift for get offset resulted in junk when shifting sometimes
    //junk, aka, very big numbers
//works for small cases like input, read03, write01, input2
//errors with big files like gcc
//error for basic 1 4 4 case with gcc but smaller margin of error vs
//bigger cache like 256 4 16


struct Block{
    //uint32_t offset;
    uint32_t tag;
    bool valid; // true if filled, false by default (unfilled)
    uint32_t load_ts, access_ts;
    bool dirty;
};

struct Sets {
    std::vector <struct Block> blocks; //in direct mapping/fully associative, size of 1 
    //uint32_t index; //non-applicable in fully associative
    uint32_t filled; //number of valid blocks in set
    unsigned access_ts_counter; //$
    unsigned load_ts_counter; //$
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
    struct Block create_set;
    struct Sets create_sets;
    struct Cache cache;
    
    std::vector <struct Block> sets;
    std::vector <struct Sets> block_list;

    for (unsigned i = 0; i < numsets; i++) { //initialize each set
        for (unsigned j = 0; j < blocksperset; j++) { //initialize blocks in each set
            //initializing a slot
            create_set.valid = false;
            create_set.dirty = false;
            create_set.tag = 0;
            create_set.load_ts = 0;
            create_set.access_ts = 0;
            //create_set.offset = j;
            sets.push_back(create_set); //add block (set) to sets
        }
        create_sets.blocks = sets;
        //create_sets.index = i;
        create_sets.filled = 0;
        create_sets.access_ts_counter = 0; //$
        create_sets.load_ts_counter = 0; //$
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
    cache.store_misses = 0;
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
    unsigned blockbits = log2(blocksperset);
    unsigned setbits = log2(numsets);
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
    if (numsets == 1) {
        return 0;
    }
    unsigned blockbits = log2(blocksperset);
    unsigned setbits = log2(numsets);
    unsigned index = address << (32 - (blockbits + setbits));
    index = index >> (32 - setbits);
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
    if (blocksperset == 1) {
        return 0;
    }
    unsigned blockbits = log2(blocksperset);
    return address & unsigned(pow(2, blockbits) - 1);
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
    scanf(" %s ", tmp);
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

//hex to dec function 
unsigned hex_to_dec(char hex_unformated[]) {
    return std::stoul (hex_unformated,nullptr,16);
}

std::tuple<struct Block, bool, bool> find_block(struct Cache &cache, unsigned address, bool lru) {
    unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
    unsigned index = get_index(address, cache.blocksperset, cache.numsets);

    bool hit = false;
    bool found_empty = false;

    std::vector <struct Sets> &block_list = cache.block_list;
    std::vector<struct Block> &blocks = block_list.at(index).blocks;

    unsigned next_empty = 0;
    unsigned counter = 0;

    block_list.at(index).access_ts_counter++;

    unsigned evict_ts = 0;
    int evict_index = 0;

    if (lru) {
        evict_ts = blocks.at(0).access_ts;
    } else {
        evict_ts = blocks.at(0).load_ts;
    }

    for (auto& it : blocks) {
        // find evict block
        if(lru && it.access_ts < evict_ts) {
            evict_ts = it.access_ts;
            evict_index = counter;//index of least recent in
        } else if (!lru && it.load_ts < evict_ts) {
            evict_ts = it.load_ts;
            evict_index = counter;//index of first loaded
        }

        if (!(it.valid) && !(found_empty)) {//index of empty block
            next_empty = counter;
            found_empty = true;
        }
        if (it.valid && it.tag == tag) { //load hit
            it.access_ts = block_list.at(index).access_ts_counter;
            return std::make_tuple(it, true, false);
        }
        counter++;
    }
    if (!hit) {//iterated through cache and could not find, load miss 
        if (!found_empty) {//no more empty spots, evict
            return std::make_tuple(blocks.at(evict_index), false, false);
        }
        else {//empty spot exists, find and fill
            return std::make_tuple(blocks.at(next_empty), false, true);
        }
    }
    //should never reach here
    return std::make_tuple(blocks.at(0), false, false);
}

/*
 * load (set_length = number of blocks per each set/index)
 *
 * Parameters:
 *   address - unsigned
 *   cache - Cache struct
 */
void load(unsigned address, struct Cache &cache, bool lru) {
    // unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
    // unsigned index = get_index(address, cache.blocksperset, cache.numsets);
    // unsigned offset = get_offset(address, cache.blocksperset);
    // std::vector <struct Sets> &block_list = cache.block_list;

    // //$uint32_t hit_access_ts = 0;//access_ts holder
    // bool hit = false; //check for if hit, false if miss
    // //$bool find_ind = false;
    // //$unsigned it_ind;
    // unsigned next_empty = 0;
    // unsigned counter = 0; 
    // bool found_empty = false;

    // std::vector<struct Block> &blocks = block_list.at(index).blocks;
    // block_list.at(index).load_ts_counter++;
    // block_list.at(index).access_ts_counter++;

    // unsigned lru_ts = blocks.at(0).access_ts;
    // unsigned fifo_ts = blocks.at(0).load_ts;
    // int lru_index = 0;
    // int fifo_index = 0;
    // for (auto& it : blocks) {
    //     //$it_ind = it.index;

    //     if(it.access_ts < lru_ts) {
    //         lru_ts = it.access_ts;
    //         lru_index = counter;//index of least recent in
    //     }
    //     if(it.load_ts < fifo_ts) {
    //         fifo_ts = it.load_ts;
    //         fifo_index = counter;//index of first loaded
    //     }
    //     if (!(it.valid) && !(found_empty)) {//index of empty block
    //         next_empty = counter;
    //         found_empty = true;
    //     }
    //     if (it.valid && it.tag == tag) { //load hit
    //         hit = true;
    //         it.access_ts = block_list.at(index).access_ts_counter;//update access ts
    //         cache.load_hits++;//increment load hits
    //         cache.total_cycles++;//increment cycle count
    //         //$hit_access_ts = (*it2.slot).access_ts; //get access timestamp of hit
    //     }
    //     counter++;
    // }

    // if (!hit) {//iterated through cache and could not find, load miss 
    //     cache.load_misses++;
    //     cache.total_cycles = cache.total_cycles + (cache.bytesperblock/4) * 100;

    //     if (!found_empty) {//no more empty spots, evict
    //         if (lru) {
    //             //$unsigned LRU = cache.blocksperset - 1;
    //             //$evict_block_LRU(index, block_list, hit, offset, LRU, tag);
    //             blocks.at(lru_index).tag = tag;
    //             //blocks.at(lru_index).offset = offset;
    //             //block_list.at(index).access_ts_counter++;
    //             blocks.at(lru_index).access_ts = block_list.at(index).access_ts_counter;
    //             blocks.at(lru_index).load_ts = block_list.at(index).load_ts_counter;//update load ts
    //             //blocks.at(mr).access_ts = block_list.at(index).access_ts_counter;//update access ts
    //             //$it.tag = tag;
    //             cache.total_cycles = cache.total_cycles + (cache.bytesperblock/4) * 100;
    //         } else {
    //             blocks.at(fifo_index).tag = tag;
    //             //blocks.at(fifo_index).offset = offset;
    //             blocks.at(fifo_index).access_ts = block_list.at(index).access_ts_counter;
    //             blocks.at(fifo_index).load_ts = block_list.at(index).load_ts_counter;
    //             cache.total_cycles = cache.total_cycles + (cache.bytesperblock/4) * 100;
    //         }
    //     }
    //     else {//empty spot exists, find and fill
    //         blocks.at(next_empty).tag = tag;
    //         //blocks.at(lru_index).offset = offset;
    //         //blocks.at(next_empty).offset = offset; 
    //         blocks.at(next_empty).valid = true; 
    //         block_list.at(index).filled++;
    //         blocks.at(next_empty).access_ts = block_list.at(index).access_ts_counter;
    //         blocks.at(next_empty).load_ts = block_list.at(index).load_ts_counter;
    //     }
    // }

    std::tuple<struct Block, bool, bool> found = find_block(cache, address, lru);
    unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
    unsigned index = get_index(address, cache.blocksperset, cache.numsets);

    std::vector <struct Sets> &block_list = cache.block_list;
    block_list.at(index).access_ts_counter++;
    block_list.at(index).load_ts_counter++;

    if (std::get<1>(found)) {
        cache.load_hits++;
        cache.total_cycles++;
    } else {
        cache.load_misses++;
        //write into cache from mem
        cache.total_cycles = cache.total_cycles + (cache.bytesperblock/4) * 100;
        std::get<0>(found).tag = tag;
        std::get<0>(found).valid = true;
        std::get<0>(found).load_ts = block_list.at(index).load_ts_counter;
        std::get<0>(found).access_ts = block_list.at(index).access_ts_counter;
        if (std::get<2>(found)) {
            block_list.at(index).filled++;
        } else {
            // evict, write in mem from cache
            cache.total_cycles = cache.total_cycles + (cache.bytesperblock/4) * 100;
        }
    }
}

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
void store(unsigned address, struct Cache &cache, bool wb, bool wa, bool lru) { 
    unsigned tag = get_tag(address, cache.blocksperset, cache.numsets);
    unsigned index = get_index(address, cache.blocksperset, cache.numsets);
    //unsigned offset = get_offset(address, cache.blocksperset);

    std::vector <struct Sets> &block_list = cache.block_list;
    block_list.at(index).access_ts_counter++;

    bool hit = false;
    //uint32_t hit_access_ts = 0;//access_ts holder
    //unsigned it_ind;
    //unsigned access_ind;
    // unsigned next_empty;
    unsigned counter = 0; 

    std::vector<struct Block> &blocks = block_list.at(index).blocks;
    
    for (auto& it : blocks) {
        counter++;

        // if(it.access_ts == block_list.at(index).access_ts_counter) {
        //     access_ind = counter;
        // }
        // if (!(it.valid)) {
        //     next_empty = counter;
        // }
        if (it.valid && it.tag == tag) { //store hit
            hit = true;
            cache.store_hits++;
            cache.total_cycles++;
            it.access_ts = block_list.at(index).access_ts_counter;//update access ts
            if (wb) { //write back
                it.dirty = true;
                //TO DO: increment write back counter; write to memory/cycle count stuff
            }
            else { //write through
                //TO DO: increment write through counter; write to memory/cycle count stuff
                cache.total_cycles = cache.total_cycles + (cache.bytesperblock/4) * 100;
            }
        }
    }
    if (!hit) { //store miss
        cache.store_misses++;    
        if (wa) {//write allocate
            //$(*it2.slot).tag = tag;
            cache.total_cycles = cache.total_cycles + (cache.bytesperblock/4) * 100;
            unsigned total_cycles = cache.total_cycles;
            load(address, cache, lru);
            cache.load_misses--;
            cache.total_cycles = total_cycles;
            //TO DO: increment write allocate counter; write to memory/cycle count stuff
        }
        else {//no write allocate
            //TO DO: increment no write allocate counter; write to memory/cycle count stuff
        }
    }
}

#endif