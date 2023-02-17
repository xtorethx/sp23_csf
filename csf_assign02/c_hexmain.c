// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  unsigned counter;
  char data_buf[16];
  char sbuf[9];
  char hexchar[3];
  char colon[] = ":";
  char space[] = " ";
  char twospace[] = "  ";
  char newline[] = "\n";
  unsigned offset = 0;
  char printable[17];
  printable[16] = 0;

  counter = hex_read(data_buf);
  while (counter != 0) {
    hex_format_offset(offset, sbuf);
    hex_write_string(sbuf);
    hex_write_string(colon);
    hex_write_string(space);
    for (unsigned i = 0; i < 16; i++) {
      if (i >= counter) {
        hex_write_string(twospace);
        hex_write_string(space);
        printable[i] = 0;
      } else {
        hex_format_byte_as_hex(data_buf[i], hexchar);
        hex_write_string(hexchar);
        hex_write_string(space);
        printable[i] = hex_to_printable(data_buf[i]);
      }
    }
    hex_write_string(space);
    hex_write_string(printable);
    hex_write_string(newline);
    offset = offset + counter;
    counter = hex_read(data_buf);
  }
}
