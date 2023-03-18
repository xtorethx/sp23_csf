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

//int to cache 
struct Slot int_to_cache() {
    return NULL;
}

//add Slot to Set
void slot_to_set() {
}

//add Set to Cache
void set_to_cache() {
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