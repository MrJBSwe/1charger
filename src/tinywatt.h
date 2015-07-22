#ifndef _TINYWATT_H_
#define _TINYWATT_H_

#include <avr/io.h>

#include "1charger_cfg.h"

/////////////////////////////////////////////
//
// Wattmeter
//
////////////////////////////////////////////



#define  INLINE_ADC     // faster ADC but cost more memory

// Measurement ranges U(volt) I(current)
#define U_AD_0     0
#define U_AD_1     1
#define U_AD_SZ    2

#define I_AD_0     0
#define I_AD_1     1
#define I_AD_SZ    2



#define DISP_TM       (0)
#define DISP_TM_STR   (0)
#define DISP_U        (1)
#define DISP_U_STR    (12)
#define DISP_I        (2)
#define DISP_I_STR    (24)
#define DISP_Wh       (3)
#define DISP_Wh_STR   (36)
#define DISP_Ah       (4)
#define DISP_Ah_STR   (48)
#define DISP_W        (5)
#define DISP_W_STR    (60)
#define DISP_Lux      (6)
#define DISP_Lux_STR  (72)
#define DISP_PERF     (7)
#define DISP_PERF_STR (84)

//#ifdef LOG_PERF_MON
#define DISP_SZ   (8)
//#else
//   #define DISP_SZ   (7)
//#endif

#define DISP_STR_SZ  (12)
#define DISP_BUFF_SZ (DISP_STR_SZ*DISP_SZ+5)

typedef struct
{
    uint8_t   sz[DISP_SZ];
    uint16_t  val[DISP_SZ];
} DispVal;


// Current values to be displayed
typedef struct
{
    uint32_t  volt;
    uint32_t  amp;
    uint32_t  watt;
    uint32_t  ah;
    uint32_t  wh;
} DVal;

typedef struct
{
    uint8_t  nsec;
    uint8_t  nmin;
    uint16_t nhour;

    uint32_t amp_sec;
    uint32_t amp_min;
    uint32_t amp_hour;

    uint32_t watt_sec;
    uint32_t watt_min;
    uint32_t watt_hour;
} WattMeter;

// Calibration line
typedef struct
{
    uint32_t k1;
    uint32_t k2;
    uint32_t m;
    uint16_t dz;  /* dead zone */
} CaLine;


typedef struct
{
    uint32_t    v;          /* adc converted in to fix-point value (volt,amp ) */
    uint16_t    adc;        /* current adc        */
    uint32_t    adc_sum;    /* accumulated sum of adc values */
    uint16_t    nread;      /* number of adc read */
    uint8_t     adcsra_msk; /* prescale           */
    uint8_t     adcsrb_msk; /* gain and mux05     */
    uint8_t     admux_msk;  /* mux0-4 and ref     */
    CaLine      calib;
} ADval;

typedef struct
{
    uint32_t u;
    uint16_t u_nread;
    uint32_t i;
    uint16_t i_nread;
    uint32_t w;
    uint16_t w_nread;
} ADSum;

typedef struct
{
    uint8_t  u_ix;
    ADval    u[U_AD_SZ];

    uint8_t  i_ix;
    ADval    i[I_AD_SZ];

#ifdef GA1A12S202
    ADval    light;
#endif

    ADSum    sum;
} Measurement;

typedef struct
{

    Measurement ad;
    WattMeter   meter;
    DVal        dval;

    uint16_t    dbg_tm;
} TinyWatt;




void  INLINE_ADC adc_read( ADval* pAD, uint8_t nTimes  );

void  adc_calc_value(  ADval* pAD );

void  watt_update(  WattMeter* pMeter, DVal* pDVal, uint32_t watt_sec, uint32_t amp_sec );

void  watt_measure( TinyWatt* pwt, uint8_t nPair, uint8_t nUAdc, uint8_t nIAdc  );

void  watt_2string( TinyWatt* ptw, DispVal* pDisp, uint8_t* pmsg, uint8_t ix1, uint8_t ix2 );

#endif // _TINYWATT_H_
