// Unit tests for hex functions
// These tests should work for both your C implementations and your
// assembly language implementations

#include <stdio.h>
#include <stdlib.h>
#include "tctest.h"
#include "hexfuncs.h"

// test fixture object
typedef struct {
  char test_data_1[16];
} TestObjs;

// setup function (to create the test fixture)
TestObjs *setup(void) {
  TestObjs *objs = malloc(sizeof(TestObjs));
  strcpy(objs->test_data_1, "Hello, world!\n");
  return objs;
}

// cleanup function (to destroy the test fixture)
void cleanup(TestObjs *objs) {
  free(objs);
}

// Prototypes for test functions
void testFormatOffset(TestObjs *objs);
void testFormatOffset1(TestObjs *objs);
void testFormatOffset2(TestObjs *objs);
void testFormatByteAsHex(TestObjs *objs);
void testFormatByteAsHex1(TestObjs *objs);
void testFormatByteAsHex2(TestObjs *objs);
void testFormatByteAsHex3(TestObjs *objs);
void testFormatByteAsHex4(TestObjs *objs);
void testFormatByteAsHex5(TestObjs *objs);
void testFormatByteAsHex6(TestObjs *objs);
void testFormatByteAsHex7(TestObjs *objs);
void testFormatByteAsHex8(TestObjs *objs);
void testFormatByteAsHex9(TestObjs *objs);
void testFormatByteAsHex10(TestObjs *objs);
void testFormatByteAsHex11(TestObjs *objs);
void testFormatByteAsHex12(TestObjs *objs);
void testFormatByteAsHex13(TestObjs *objs);
void testHexToPrintable(TestObjs *objs);
void testHexToPrintable1(TestObjs *objs);

int main(int argc, char **argv) {
  if (argc > 1) {
    tctest_testname_to_execute = argv[1];
  }

  TEST_INIT();
  TEST(testFormatOffset);
  TEST(testFormatOffset1);
  TEST(testFormatOffset2);
  TEST(testFormatByteAsHex);
  TEST(testFormatByteAsHex1);
  TEST(testFormatByteAsHex2);
  TEST(testFormatByteAsHex3);
  TEST(testFormatByteAsHex4);
  TEST(testFormatByteAsHex5);
  TEST(testFormatByteAsHex6);
  TEST(testFormatByteAsHex7);
  TEST(testFormatByteAsHex8);
  TEST(testFormatByteAsHex9);
  TEST(testFormatByteAsHex10);
  TEST(testFormatByteAsHex11);
  TEST(testFormatByteAsHex12);
  TEST(testFormatByteAsHex13);
  TEST(testHexToPrintable);
  TEST(testHexToPrintable1);

  TEST_FINI();

  return 0;
}

void testFormatOffset(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  hex_format_offset(0x00000001u, buf);
  ASSERT(0 == strcmp(buf, "00000001"));

  hex_format_offset(0xabcd1234u, buf);
  ASSERT(0 == strcmp(buf, "abcd1234"));
}

void testFormatOffset1(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  // test zeros
  hex_format_offset(0xffffffffu, buf);
  ASSERT(0 == strcmp(buf, "ffffffff"));

  hex_format_offset(0x00000000u, buf);
  ASSERT(0 == strcmp(buf, "00000000"));

    hex_format_offset(0, buf);
  ASSERT(0 == strcmp(buf, "00000000"));
}

void testFormatOffset2(TestObjs *objs) {
  (void) objs; // suppress warning about unused parameter
  char buf[16];
  // test max
  hex_format_offset(0xffffffffu, buf);
  ASSERT(0 == strcmp(buf, "ffffffff"));

  hex_format_offset(4294967295, buf);
  ASSERT(0 == strcmp(buf, "ffffffff"));
}

//'H'
void testFormatByteAsHex(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[0], buf);
  ASSERT(0 == strcmp(buf, "48"));
}

//'e'
void testFormatByteAsHex1(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[1], buf);
  ASSERT(0 == strcmp(buf, "65"));
}

//'l'
void testFormatByteAsHex2(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[2], buf);
  ASSERT(0 == strcmp(buf, "6c"));
}

//'l'
void testFormatByteAsHex3(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[3], buf);
  ASSERT(0 == strcmp(buf, "6c"));
}

//'o'
void testFormatByteAsHex4(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[4], buf);
  ASSERT(0 == strcmp(buf, "6f"));
}

//','
void testFormatByteAsHex5(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[5], buf);
  ASSERT(0 == strcmp(buf, "2c"));
}

//' '
void testFormatByteAsHex6(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[6], buf);
  ASSERT(0 == strcmp(buf, "20"));
}

//'w'
void testFormatByteAsHex7(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[7], buf);
  ASSERT(0 == strcmp(buf, "77"));
}

//'o'
void testFormatByteAsHex8(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[8], buf);
  ASSERT(0 == strcmp(buf, "6f"));
}

//'r'
void testFormatByteAsHex9(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[9], buf);
  ASSERT(0 == strcmp(buf, "72"));
}

//'l'
void testFormatByteAsHex10(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[10], buf);
  ASSERT(0 == strcmp(buf, "6c"));
}

//'d'
void testFormatByteAsHex11(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[11], buf);
  ASSERT(0 == strcmp(buf, "64"));
}

//'!'
void testFormatByteAsHex12(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[12], buf);
  ASSERT(0 == strcmp(buf, "21"));
}

//'\n'
void testFormatByteAsHex13(TestObjs *objs) {
  char buf[16];
  hex_format_byte_as_hex(objs->test_data_1[13], buf);
  ASSERT(0 == strcmp(buf, "0a"));
}

void testHexToPrintable(TestObjs *objs) {
  ASSERT('H' == hex_to_printable(objs->test_data_1[0]));
  ASSERT('.' == hex_to_printable(objs->test_data_1[13]));
}

void testHexToPrintable1(TestObjs *objs) {
  // non-printable characters
  ASSERT('.' == hex_to_printable(0));
  ASSERT('.' == hex_to_printable(31));
  ASSERT('.' == hex_to_printable(128));
}