#ifndef CACHESIMFUNCS_H
#define CACHESIMFUNCS_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "cachesimfuncs.h"

//TO DO: Write Functions
struct Slot{
    uint32_t tag;
    bool valid;
    uint32_t load_ts, access_ts;
};

struct Set{
    std::vector <Slot> slots;
    std::map <uint32_t, Slot *> index; //used to map tags to valid blocks, which valid blocks have been stored in set, key is tag value is pointer to corresponding slot
};

struct Cache{
    std::vector <Set> sets;
};

//possible functions/helper functions:
//initialize slot
//cache hit
//cache miss
//seq_search: find valid block in set with matching tag (boolean function)
//

#endif