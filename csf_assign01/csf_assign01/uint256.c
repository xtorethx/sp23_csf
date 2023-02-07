#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "uint256.h"
#include <stdbool.h>


// Create a UInt256 value from a single uint64_t value.
// Only the least-significant 64 bits are initialized directly,
// all other bits are set to 0.
UInt256 uint256_create_from_u64(uint64_t val) {
  UInt256 result;
  result.data[0] = val;
  result.data[1] = 0;
  result.data[2] = 0;
  result.data[3] = 0;
  return result;
}


// Create a UInt256 value from an array of 4 uint64_t values.
// The element at index 0 is the least significant, and the element
// at index 3 is the most significant.
UInt256 uint256_create(const uint64_t data[4]) {
  UInt256 result;
  result.data[0] = data[0];
  result.data[1] = data[1];
  result.data[2] = data[2];
  result.data[3] = data[3];
  return result;
}


// Create a UInt256 value from a string of hexadecimal digits.
UInt256 uint256_create_from_hex(const char *hex) {
  UInt256 result;
  size_t len = strlen(hex);
  char hexcopy[65] = {0};
  hexcopy[64] = '\0';
  uint64_t data[5] = {0};
  data[5] = '\0';
  if (len < 64) {
      for (int i = 0; i < (int) (64-len); i++) {
          hexcopy[i] = '0';
      }
      for (int i = (64-len); i < 64; i++) {
          hexcopy[i] = *(hex+(i-(64-len)));
      }
      //std::cout << hexcopy << std::endl;  
  }
 
  else if (len > 64) {
      for (int i = (len - 64); i < (int) len; i++) {
          hexcopy[i-(len-64)] = *(hex+i);
      }
      //std::cout << hexcopy << std::endl;
  }


  else {
    for (int i = 0; i < 64; i++) {
      hexcopy[i] = *(hex + i);
    }
  }


  int arrind = 3;
 
  for (int i = 0; i < 64; i = i + 16) {
    char substr[17] = {0};
    substr[16] = '\0';
    char *end = &(substr[16]);
    for (int j = i; j < i + 16; j++) {
      substr[j%16] = *(hexcopy + j);
      //std::cout << *(hexcopy + j) << std::endl;
    }
    //std::cout << substr << std::endl;
    data[arrind] = strtoul(substr, &end, 16);
    //std::cout << data[arrind] << std::endl;
    arrind--;
  }
  result = uint256_create(data);
  return result;
}


int leadingzeroindex(char *hex) {
  int leadingzeroind = -1;
  int i = 0;
  while (i < (int) strlen(hex) && *(hex + i) == '0') {
      ++leadingzeroind;
      ++i;
  }
  if (leadingzeroind == -1) {
    return 0;
  }
  if (i == (int) strlen(hex)){
    return (strlen(hex) - 1);
  }
  return i;
}


// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex(UInt256 val) {
  char *hex = (char*) calloc(65, sizeof(char));
  hex[64] = '\0';
  int count = 0;
  int next = 0;
  for (int i = 3; i >=0; i--) {
    char substr[17] = {0};
    substr[16] = '\0';
    uint64_t data = val.data[i];
    sprintf(substr, "%016lx", data);
    int j = count*16;
    for (int k = j; k < j+ (int) strlen(substr); k++) {
      hex[next] = substr[k%16];
      next++;
    }
    count++;
  }
  int leadingzeroind = leadingzeroindex(hex);
  char *trunc = (char*) calloc((strlen(hex) - leadingzeroind + 1), sizeof(char));
  trunc[strlen(hex)-leadingzeroind] = '\0';
  for (int i = 0; i < (int) strlen(hex)-leadingzeroind; i++) {
    trunc[i] = hex[leadingzeroind+i];
  }
  free(hex);
  return trunc;
}


// Get 64 bits of data from a UInt256 value.
// Index 0 is the least significant 64 bits, index 3 is the most
// significant 64 bits.
uint64_t uint256_get_bits(UInt256 val, unsigned index) {
  uint64_t bits;
  bits = val.data[index];
  return bits;
}


//check if adding 2 uint64_t values results in overflow
int check_overflow(uint64_t top, uint64_t bot, uint64_t carry) {
  uint64_t sum = top + bot;
  uint64_t sum2 = sum + carry;
  // write a test for 0 + max + 1
  // not less than top
  if (sum < top) {
    return 1;
  } else if (sum2 < sum) {
    return 1;
  } else {
    return 0;
  }
}


// Compute the sum of two UInt256 values.
UInt256 uint256_add(UInt256 left, UInt256 right) {
  UInt256 sum;
  uint64_t carry = 0UL;
  for (int i = 0; i < 4; i++) {
    sum.data[i] = left.data[i] + right.data[i] + carry;


    if (check_overflow(left.data[i], right.data[i], carry)) {
      carry = 1UL;
    } else {
      carry = 0UL;
    }
  }


  return sum;
}


// invert the bits of a Uint256 value and add 1 to negate
UInt256 negate(UInt256 num) {
  UInt256 negated;




  for (int i = 0; i < 4; i++) {
    negated.data[i] = ~num.data[i];
  }
 


  negated = uint256_add(negated, uint256_create_from_u64(1UL));


  return negated;
}
// Compute the difference of two UInt256 values.
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  UInt256 result;


  result = uint256_add(left, negate(right));
  // printf("%lu\n", result.data[0]);
  // printf("%lu\n", result.data[1]);
  // printf("%lu\n", result.data[2]);
  // printf("%lu\n", result.data[3]);
  return result;
}


UInt256 shift_n_chunks (UInt256 num, int n) {
  UInt256 new;
  new = uint256_create_from_u64(0UL);


  for (int i = n; i < 4; i++) {
    new.data[i] = num.data[i - n];
  }


  return new;
}


bool uint256_bit_is_set(UInt256 val, unsigned index) {
  int num_chunks = 0;
  unsigned tmp = index;
  while (tmp >= 64) {
    num_chunks++;
    tmp = tmp - 64;
  }


  return (val.data[num_chunks] & (1UL << tmp));
}


// uint64_t set_zeros(uint64_t val, unsigned start, unsigned end) {
//   uint64_t new = val;
//   for (unsigned i = start; i < end + 1; i++) {
//     new &= ~(1 << i);
//   }


//   return new;
// }


UInt256 uint256_leftshift(UInt256 val, unsigned shift) {
  UInt256 new_val;
  int num_chunks = shift / 64;
  unsigned tmp = shift % 64;
  uint64_t left = 0UL;
  uint64_t right = 0UL;


  new_val = shift_n_chunks(val, num_chunks);


  if (tmp == 0) {
    return new_val;
  }
 
  right = new_val.data[num_chunks] >> (64 - tmp);
  // right = set_zeros(right, tmp, 63);
  left = new_val.data[num_chunks] << tmp;
  // left = set_zeros(left, 0, tmp - 1);
  new_val.data[num_chunks] = left;
  for (int i = num_chunks + 1; i < 4; i++) {
    left = new_val.data[i] << tmp;
    // left = set_zeros(left, 0, tmp - 1);
    new_val.data[i] = left + right;
    right = new_val.data[i] >> (64 - tmp);
    // right = set_zeros(right, tmp, 63);
  }


  return new_val;
}


// Compute the product of two UInt256 values.
UInt256 uint256_mul(UInt256 left, UInt256 right) {
  UInt256 product;
  product = uint256_create_from_u64(0UL);


  for (int i = 0; i < 256; i++) {
    if (uint256_bit_is_set(left, i)) {
      product = uint256_add(product, uint256_leftshift(right, i));
    }
   
  }


  // printf("%lu\n", product.data[0]);
  // printf("%lu\n", product.data[1]);
  // printf("%lu\n", product.data[2]);
  // printf("%lu\n", product.data[3]);


  return product;
}


