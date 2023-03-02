/*
 * Create a program that takes in input and produces a hexdump of input
 * D. Zheng   H. Qu
 * dzheng12@jhu.edu   hqu6@jhu.edu
 */

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  char colon[] = ":";
  char space[] = " ";
  char twospace[] = "  ";
  char newline[] = "\n";
  
  unsigned counter;
  char data_buf[17];
  char sbuf[9];
  char hexchar[3];
  unsigned offset = 0;
  char printable[17];
  printable[16] = 0;

  counter = hex_read(data_buf);
  while (counter != 0) {
    hex_format_offset(offset, sbuf); //00000000
    hex_write_string(sbuf); //write in "00000000"
    hex_write_string(colon); //write in ":"
    hex_write_string(space); //write in " "
    for (unsigned i = 0; i < 16; i++) { //
      if (i >= counter) { //after ASCII values are read in
        hex_write_string(twospace); //write in "  "
        hex_write_string(space); //write in " "
        printable[i] = 0; //null terminated
      } else {
        hex_format_byte_as_hex(data_buf[i], hexchar); //54
        hex_write_string(hexchar); //write in 54
        hex_write_string(space); //write in " "
        printable[i] = hex_to_printable(data_buf[i]); //convert to T
      }
    }
    hex_write_string(space); //write in " "
    hex_write_string(printable); // write in print string
    hex_write_string(newline); // line break
    offset = offset + counter; //update num char read
    counter = hex_read(data_buf); //next 16 characters
  }
}
