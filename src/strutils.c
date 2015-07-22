#include "strutils.h"

/////////////////////////////////////////////
//
// String operations
//
////////////////////////////////////////////

static const uint16_t c_i16[5]   = { 10000,
                                     1000,
                                     100,
                                     10,
                                     1
                                   };


/** Convert uint16_t to string
 *
 *
 * @param v value to convert
 * @param n insert position in string buffer
 * @param str pointer to string
 * @param bDec  0=integer else fixpoint two decimals
 */
void uint16_2string(  uint16_t v, uint8_t* n, uint8_t* str, uint8_t bDec, uint8_t width )
{
    uint8_t i;
    uint8_t w=0;
    // uint16_t d=0;
    // uint16_t val=v;

    if ( width>0 )
    {
        w= 5-width;
    }


    if ( v == 0 && bDec==0 )
    {
        for ( i=0; i<width; ++i )
        {
            str[*n] = '0';
            ++(*n);
        }

        return;
    }

    for (i=0; i<5; ++i)
    {
        if ( v<c_i16[i]  )
        {
            if ( bDec && i>1 )
            {
                if ( bDec && i==3)
                {
                    str[*n]='.';
                    ++(*n);
                }

                str[*n]= '0';
                ++(*n);
            }
            if ( width )
            {

                if ( i>=w )
                {
                    str[*n]= '0';
                    ++(*n);
                }
            }
            continue;
        }

        if ( bDec && i==3)
        {
            str[*n]='.';
            ++(*n);
        }
        //  d=((uint8_t)(val/c_i16[i]));
        //  str[*n]= (uint8_t)(d+48);
        //  val=val-d*c_i16[i];
        str[*n]=((v/c_i16[i])%10)+48;
        ++(*n);
    }
}




#ifdef UINT32_STR

static const uint32_t c_i32[10]   = {1000000000,
                                     100000000,
                                     10000000,
                                     1000000,
                                     100000,
                                     10000,
                                     1000,
                                     100,
                                     10,
                                     1
                                    };

/** Convert uint32_t to string
 *
 *
 * @param v value to convert
 * @param n insert position in string buffer
 * @param str pointer to string
 * @param bDec  0=integer else fixpoint two decimals
 */
void uint32_2string(  uint32_t v, uint8_t* n, uint8_t* str, uint8_t bDec )
{
    uint8_t i;

    if ( v == 0 && bDec==0 )
    {
        str[*n] = '0';
        ++(*n);

        return;
    }

    for (i=0; i<10; ++i)
    {
        if ( v<c_i32[i]  )
        {
            if ( bDec && c_i32[i]<=100 )
            {
                if ( bDec && c_i32[i]==10 )
                {
                    str[*n]='.';
                    ++(*n);
                }

                str[*n]= '0';
                ++(*n);
            }

            continue;
        }

        if ( bDec && c_i32[i]==10 )
        {
            str[*n]='.';
            ++(*n);
        }

        str[*n]=((v/c_i32[i])%10)+48;
        ++(*n);
    }
}
#endif


void make_row_str( const uint8_t* str1, uint8_t npos, const uint8_t* str2, uint8_t* dest_str, uint8_t row_sz )
{
    uint8_t n=0;
    uint8_t* p1=(uint8_t*)str1;
    uint8_t* p2=dest_str;

    while( *p1 && (*p1) != ';' )
    {
        *p2=*p1;
        ++p1;
        ++p2;
        ++n;
    }

    while(n<npos)
    {
        *p2=';';
        ++p2;
        ++n;
    }
    p1=(uint8_t*)str2;
    while( *p1 )
    {
        *p2=*p1;
        ++p1;
        ++p2;
        ++n;
    }
    while(n<row_sz)
    {
        *p2=';';
        ++p2;
        ++n;
    }
    *p2='\0';
    dest_str[row_sz]='\0';
}
