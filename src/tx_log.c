#include <util/delay.h>
#include "strutils.h"
#include "tx_log.h"






/** Init software UART
 *
 * Set up pins and registers and prescaler to 8
 *
 *
 * @param tm timer value ( Set Output Compare A  )
 */
void tx_init_suart( uint8_t tm )
{
    /* PORT is set here */


    TX_DDR	|=	(1<<TX_PIN);	//As output
    TX_PORT	|=	(1<<TX_PIN);	//With pullup

    /* Timer is set here, CTC mode, and your prescaler 8 */
    // Timer 0

    TCCR0A = (1<<CTC0);	    //CTC mode
    TCCR0B = (1<<CS01);		//Prescaler 8
    OCR0A  = tm; 	        //Set Output Compare A 		(See header file)
    TIMSK  = (1<<OCIE0A);	//Enable Output Compare A interrupt

    //Enable interrupt
}
#ifdef  SOFTWARE_UART
/** Write a single char to TX_PIN
 *
 * Wait for TX to finish and then send data, return when all data bits is sent
 *
 * @param pTx  pointer to TxSerial that keep uart status
 * @param data byte that is to be sent
 */



void tx_putchar( TxSerial* pTx, uint8_t data)
{
    // wait for TX to finish
    // When routine is not idle, wait for it to become idle
    while (pTx->state != TX_IDLE)
    {
        _delay_us(1);
    }

    pTx->data   = data;		    //Put data ready
    pTx->state  = TX_STARTBIT;	//Set state to startbit

    while (pTx->state != TX_IDLE)
    {
        _delay_us(1);
    }
}

/** Write string to UART
 *
 * putchar until end of string '\0' is reached
 *
 *
 * @param pTx pointer to TxSerial that keeps uart status
 */

void tx_putstring(TxSerial* pTx, const uint8_t* p)
{
    uint8_t i = 0;

    while (p[i] != '\0')
    {
        tx_putchar( pTx, p[i]);
        ++i;
    }
}

#if 0
void tx_log_info( TxSerial* pTx, TinyWatt* ptw, uint8_t* pmsg )
{
    uint8_t    n=0;
    uint8_t    j;

    tx_putstring(pTx, (const uint8_t*)"# 1Charger\r\n");
    tx_putstring(pTx, (const uint8_t*)"#\r\n");

    for ( j=0; j<U_AD_SZ; ++j )
    {
        n=0;
        pmsg[n]='#';
        ++n;
        pmsg[n]=' ';
        ++n;
        pmsg[n]='U';
        ++n;
        pmsg[n]='[';
        ++n;
        uint16_2string( j, &n, pmsg, 0,0 );
        pmsg[n]=']';
        ++n;
        pmsg[n]='{';
        ++n;
        pmsg[n]='k';
        ++n;
        pmsg[n]='1';
        ++n;
        pmsg[n]='=';
        ++n;

        uint16_2string(  (uint16_t)ptw->ad.u[j].calib.k1, &n, pmsg, 0,0);
        pmsg[n]=' ';
        ++n;

        pmsg[n]='k';
        ++n;
        pmsg[n]='2';
        ++n;
        pmsg[n]='=';
        ++n;
        uint16_2string(  (uint16_t)ptw->ad.u[j].calib.k2, &n, pmsg, 0,0);
        pmsg[n]=' ';
        ++n;

        pmsg[n]='m';
        ++n;
        pmsg[n]='=';
        ++n;
        uint16_2string(  (uint16_t)ptw->ad.u[j].calib.m, &n, pmsg, 0,0);
        pmsg[n]='}';
        ++n;
        pmsg[n] = '\r';
        ++n;
        pmsg[n] = '\n';
        ++n;
        pmsg[n] = '\0';
        tx_putstring(pTx,  pmsg);

    }


    for ( j=0; j<I_AD_SZ; ++j )
    {
        n=0;
        pmsg[n]='#';
        ++n;
        pmsg[n]=' ';
        ++n;
        pmsg[n]='I';
        ++n;
        pmsg[n]='[';
        ++n;
        uint16_2string( j, &n, pmsg, 0,0 );
        pmsg[n]=']';
        ++n;
        pmsg[n]='{';
        ++n;
        pmsg[n]='k';
        ++n;
        pmsg[n]='1';
        ++n;
        pmsg[n]='=';
        ++n;

        uint16_2string(  (uint16_t)ptw->ad.i[j].calib.k1, &n, pmsg, 0,0);
        pmsg[n]=' ';
        ++n;

        pmsg[n]='k';
        ++n;
        pmsg[n]='2';
        ++n;
        pmsg[n]='=';
        ++n;
        uint16_2string(  (uint16_t)ptw->ad.i[j].calib.k2, &n, pmsg, 0,0);
        pmsg[n]=' ';
        ++n;

        pmsg[n]='m';
        ++n;
        pmsg[n]='=';
        ++n;
        uint16_2string(  (uint16_t)ptw->ad.i[j].calib.m, &n, pmsg, 0,0);
        pmsg[n]='}';
        ++n;
        pmsg[n] = '\r';
        ++n;
        pmsg[n] = '\n';
        ++n;
        pmsg[n] = '\0';
        tx_putstring(pTx,  pmsg);

    }
    tx_putstring(pTx, (const uint8_t*)"#\r\n");
    tx_putstring(pTx, (const uint8_t*)"#\r\n");
    tx_putstring(pTx, (const uint8_t*)"\r\n");

    tx_putstring(pTx, (const uint8_t*)"ver=0.95\r\n");
#ifndef CALIBRATION
#ifdef  GA1A12S202
    tx_putstring(pTx, (const uint8_t*)"type={hours:min:sec,U,I,Wh,Ah,W,ADC-LogLux}\r\n");
#else
    tx_putstring(pTx, (const uint8_t*)"type={hours:min:sec,U,I,Wh,Ah,W}\r\n");
#endif
#endif
    tx_putstring(pTx,(const uint8_t*)"date=1971-01-01 00:00:00\r\n" );
    tx_putstring(pTx, (const uint8_t*)"\r\n");
    tx_putstring(pTx, (const uint8_t*)"\r\n");

    while (pTx->state != TX_IDLE)
    {
        _delay_us(1);
    }
}
#endif



#ifdef CALIBRATION
void
tx_kalib_log( TxSerial* pTx, TinyWatt* ptw, uint8_t* pmsg )
{
    uint8_t n=0;
    uint8_t i=0;

    pmsg[n] = 'U';
    ++n;
    pmsg[n] = '{';
    ++n;
    for ( i=0; i<U_AD_SZ; ++i )
    {
        if ( i>0 )
        {
            pmsg[n] = ' ';
            ++n;
        }
        uint16_2string(  (uint16_t)ptw->ad.u[i].v, &n, pmsg, 1,0 );
        pmsg[n] = ',';
        ++n;
        uint16_2string(  ptw->ad.u[i].adc, &n, pmsg, 0,0 );
    }
    pmsg[n] = '}';
    ++n;

    pmsg[n] = ' ';
    ++n;

    pmsg[n] = 'I';
    ++n;
    pmsg[n] = '{';
    ++n;
    for ( i=0; i<I_AD_SZ; ++i )
    {
        if ( i>0 )
        {
            pmsg[n] = ' ';
            ++n;
        }
        uint16_2string(  (uint16_t)ptw->ad.i[i].v, &n, pmsg, 1,0 );
        pmsg[n] = ',';
        ++n;
        uint16_2string(  ptw->ad.i[i].adc, &n, pmsg, 0,0 );
    }
    pmsg[n] = '}';
    ++n;

    pmsg[n] = '\r';
    ++n;
    pmsg[n] = '\n';
    ++n;
    pmsg[n] = '\0';

    tx_putstring( pTx, pmsg );
}
#else

void
tx_log( TxSerial* pTx, DispVal* pDisp, uint8_t* pmsg  )
{
    /* hours:min:sec,U,I,Wh,Ah,W */

    uint8_t n=0;
    uint8_t i=0;
    uint8_t* p=0;


    for( i=0; i<(DISP_SZ-1); i++ )
    {
        n=pDisp->sz[i];


        p=&pmsg[i*DISP_STR_SZ];
        p[n]=',';
        ++n;
        p[n]='\0';
    }

    i=DISP_SZ-1;
    n=pDisp->sz[i];
    p=&pmsg[i*DISP_STR_SZ];
    p[n] = '\r';
    ++n;
    p[n] = '\n';
    ++n;
    p[n] = '\0';

    for( i=0; i<DISP_SZ; ++i )
        tx_putstring( pTx, &pmsg[i*DISP_STR_SZ] );
}

#endif
#endif
