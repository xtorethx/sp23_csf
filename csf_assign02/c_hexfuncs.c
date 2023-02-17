// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

// TODO:s add function implementations here
// Read up to 16 bytes from standard input into data_buf.
// Returns the number of characters read.
unsigned hex_read(char data_buf[]){
    unsigned ret = 0;
    unsigned tmp = 1;
    
    while (tmp > 0) {
        tmp = read(0, data_buf, 16);
        ret = ret + tmp;
    }
    return ret;
}

// Write given nul-terminated string to standard output.
void hex_write_string(const char s[]){
    while(*s != 0) {
        write(1, s, sizeof(*s));
        s++;
    }
}

// Format an unsigned value as an offset string consisting of exactly 8
// hex digits.  The formatted offset is stored in sbuf, which must
// have enough room for a string of length 8.
void hex_format_offset(unsigned offset, char sbuf[]){
    
    unsigned rm;
    
    for (int i = 0; i < 8; i++) {
        rm = offset % 16;
        if (rm < 10) {
            rm = rm + 48;
        } else {
            rm = rm + 87;
        }

        sbuf[i] = rm;
        offset = offset / 16;
    }
    
    char temp;
        for (int i = 0; i < 4; i++)  
    {  
        // temp variable use to temporary hold the string  
        temp = sbuf[i];  
        sbuf[i] = sbuf[8 - i - 1];  
        sbuf[8 - i - 1] = temp;  
    } 
    
    sbuf[8] = '\0';
}

char int_to_hexchar(int dec) {
    char cov[16] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    cov[15] = '\0';
    return cov[dec-1];
}

// Format a byte value (in the range 0-255) as string consisting
// of two hex digits.  The string is stored in sbuf.
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]){
    int dec;
    dec = (unsigned int)byteval;
    char div, mod;
    
    div = int_to_hexchar(dec/16); 
    mod = int_to_hexchar(dec%16); 

    sbuf[0] = div;
    sbuf[1] = mod;
    
    //printf("%s", sbuf);
}

// Convert a byte value (in the range 0-255) to a printable character
// value.  If byteval is already a printable character, it is returned
// unmodified.  If byteval is not a printable character, then the
// ASCII code for '.' should be returned.
char hex_to_printable(unsigned char byteval){
    int dec;
    
    dec = (int)(byteval);

    if (dec >= 32 && dec <= 126) {
        return byteval;
    }
    return '.';
}