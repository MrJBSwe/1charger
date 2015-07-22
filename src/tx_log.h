#ifndef _TX_LOG_H_
#define _TX_LOG_H_

#include "tinywatt.h"



/////////////////////////////////////////////
//
// LOG serial port
//
// http://martinsant.net/?page_id=1689
//
////////////////////////////////////////////


#define TX_IDLE		0
#define TX_STARTBIT	1
#define TX_TRANSMIT	2
#define TX_STOPBIT 	3

#define CTC0        0

typedef struct
{
#ifdef SOFTWARE_UART
    uint8_t            bit_msk;
    uint8_t            bit_clr;
    volatile uint8_t   state;
    uint8_t            bit_cnt;
    volatile uint8_t   data;
#endif
    // interrupt counter, number of interrupts = ctr1*256+ctr0
    volatile uint8_t   ctr0;
    volatile uint8_t   ctr1;
} TxSerial;

#ifdef SOFTWARE_UART

void tx_init_suart( uint8_t tm );

void tx_on_timer( TxSerial* pTx );

void tx_putchar( TxSerial* pTx, uint8_t data);

void tx_putstring(TxSerial* pTx, const uint8_t* p);


// void tx_log_info( TxSerial* pTx, TinyWatt* ptw, uint8_t* pmsg );
#ifdef CALIBRATION
void tx_kalib_log( TxSerial* pTx, TinyWatt* ptw, uint8_t* pmsg );
#else
void tx_log(  TxSerial* pTx, DispVal* pDisp, uint8_t* pmsg );

#endif
#endif


#endif // _TX_LOG_H_
