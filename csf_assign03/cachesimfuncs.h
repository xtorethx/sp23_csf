#ifndef CACHESIMFUNCS_H
#define CACHESIMFUNCS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdio>
#include <algorithm>
#include <cmath>

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

struct Cache buildCache(unsigned numsets, unsigned blocksperset, unsigned bytesperblock);

unsigned get_tag(unsigned address, unsigned blocksperset, unsigned numsets);

unsigned get_index(unsigned address, unsigned blocksperset, unsigned numsets);

unsigned get_offset(unsigned address, unsigned blocksperset);

int read(struct Cache cache, char ls, char memaddress[], char tmp);

void write(struct Cache cache);

unsigned hex_to_dec(char hex_unformated[]);

void load_dm(unsigned address, struct Cache cache);

void update_access_ts_fa(std::vector <struct Sets> sets_list, uint32_t hit_access_ts);

bool load_empty_fa(std::vector <struct Sets> sets_list, bool hit, unsigned offset, unsigned tag);

void evict_block_LRU_fa(std::vector <struct Sets> sets_list, bool hit, unsigned offset, unsigned LRU, unsigned tag);

void load_fa(unsigned address, struct Cache cache);

void load_sa(unsigned address, struct Cache cache);

void store_dm(unsigned address, struct Cache cache, bool wb, bool wa);

void update_access_ts_write_fa(std::vector <struct Sets> sets_list, uint32_t hit_access_ts, bool wb);

void store_fa(unsigned address, struct Cache cache, bool wb, bool wa);

void store_sa(unsigned address, struct Cache cache, bool wb, bool wa);


#endif