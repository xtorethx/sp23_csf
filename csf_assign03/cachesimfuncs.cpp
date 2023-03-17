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

//read memory access trace from standard input
void read() {
}

//write to standard output
void write() {
}

//load
void load() {
    //if hit --> do nothing
    //if miss --> check if empty
    //if empty --> write
    //if not empty --> LRU
}

//store
void store() {
    //if hit
    //if write-through --> call write through
    //if write-back --> call write-back

    //if miss
    //if write-allocate --> call write allocate
    //if no-write-allocate --> call no-write-allocate
}

//write-through
void write_through(){

}

void write_back(){

}

void write_allocate(){

}

void no_write_allocate(){

}

#endif