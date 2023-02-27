/*
 * C implementation of hexdump functions
 * D. Zheng   H. Qu
 * dzheng12@jhu.edu   hqu6@jhu.edu
 */

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

/*
 * Return character corresponding to hex digit
 *
 * Parameters:
 *   dec - integer
 *
 * Returns:
 *   a character
 */
char int_to_hexchar(int dec) {
    if(dec == 0){
        return '0';
    }
    char cov[16] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    cov[15] = '\0';
    return cov[dec-1];
}

/*
 * Read up to 16 bytes from standard input into data_buf.
 * Returns the number of characters read.
 *
 * Parameters:
 *   data_buf - character array
 *
 * Returns:
 *   number of characters read in from standard input
 */
unsigned hex_read(char data_buf[]){
    return read(0, data_buf, 16);
}

/*
 * Write given nul-terminated string to standard output.
 *
 * Parameters:
 *   s - character array that contains a string
 */
void hex_write_string(const char s[]){
    while(*s != 0) {
        write(1, s, sizeof(*s));
        s++;
    }
}

/*
 * Format an unsigned value as an offset string consisting of exactly 8
 * hex digits.  The formatted offset is stored in sbuf, which must
 * have enough room for a string of length 8.
 *
 * Parameters:
 *   offset - unsigned integer
 *   sbuf - char array that contains a 8 char string
 */
void hex_format_offset(unsigned offset, char sbuf[]){
    
    unsigned rm;
    // get char representation
    int count = 7;
    for (int i = 0; i < 8; i++) {
        rm = offset % 16;
        sbuf[count] = int_to_hexchar(rm);
        offset = offset / 16;
        count--;
    }
    
    // null terminate the string
    sbuf[8] = 0;
}


/*
 * Format a byte value (in the range 0-255) as string consisting
 * of two hex digits.  The string is stored in sbuf.
 *
 * Parameters:
 *   byteval - unsigned char
 *   sbuf - character array that contains a string
 */
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]){
    int dec;
    dec = (unsigned int)byteval;
    char div, mod;
    
    div = int_to_hexchar(dec/16); 
    mod = int_to_hexchar(dec%16); 

    sbuf[0] = div;
    sbuf[1] = mod;
    sbuf[2] = 0;
}

/*
 * Convert a byte value (in the range 0-255) to a printable character
 * value.  If byteval is already a printable character, it is returned
 * unmodified.  If byteval is not a printable character, then the
 * ASCII code for '.' should be returned.
 *
 * Parameters:
 *   byteval - unsigned char
 *
 * Returns:
 *   ascii representation of byteval or '.' if not printable
 */
char hex_to_printable(unsigned char byteval){
    int dec;
    
    dec = (int)(byteval);

    if (dec >= 32 && dec <= 126) {
        return byteval;
    }
    // not printable
    return '.';
}