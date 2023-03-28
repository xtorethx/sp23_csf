// main:
// in while
//     get address, ls
//     if ls = l
//         call load method in funcs.cpp
//     if ls = s
//         call store method in funcs.cpp

// funcs:

// load method:
//     increment number of loads
//     currentset = get the right set

//     for blocks in currentset
//         if address found
//             increment load hit
//     if address not found
//         incremeent load miss
//         do other load miss stuff


// cache struct: have a vector of sets
// cache set: have a vector of blocks