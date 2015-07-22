#ifndef _STR_UTILS_H_
#define _STR_UTILS_H_


#include <avr/io.h>

/////////////////////////////////////////////
//
// String operations
//
////////////////////////////////////////////

// #define UINT32_STR



void uint16_2string(  uint16_t v, uint8_t* n, uint8_t* str, uint8_t bDec, uint8_t width );

void make_row_str(  const uint8_t* str1, uint8_t npos, const uint8_t* str2, uint8_t* dest_str, uint8_t row_sz );

#ifdef UINT32_STR
void uint32_2string(  uint32_t v, uint8_t* n, uint8_t* str, uint8_t bDec );
#endif


#endif // _STR_UTILS_H_
