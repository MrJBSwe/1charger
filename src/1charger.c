/*--------------------------------------------------------------------------*
*
* 1Charger ver 0.96 31-Aug-2014 JB
*
* http://hackaday.io/project/1936-1Charger
*
*---------------------------------------------------------------------------*
*
*
* This code is inspired by :
*
* http://flashlightwiki.com/AVR_Drivers
*
*
*--------------------------------------------------------------------------*/

#include <avr/io.h>

#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/pgmspace.h>
#include <math.h>

#include "1charger_cfg.h"
#include "display.h"
#include "tinywatt.h"
#include "strutils.h"
#include "tx_log.h"


// ADC prescaler defines
#define PRESCALE_2   (1<<ADPS0)
#define PRESCALE_4   (1<<ADPS1)
#define PRESCALE_8   ((1<<ADPS1) | (1<<ADPS0) )
#define PRESCALE_16  (1<<ADPS2)
#define PRESCALE_32  ((1<<ADPS2) | (1<<ADPS0))
#define PRESCALE_64  ((1<<ADPS2) | (1<<ADPS1))
#define PRESCALE_128 ((1<<ADPS2) | (1<<ADPS1)| (1<<ADPS0))


/////////////////////////////////////////////
//
//  Utils
//
////////////////////////////////////////////



void simple_memset( uint8_t* arr, uint8_t val, uint8_t sz );

void round_value( uint32_t* pVal, uint32_t val );

/////////////////////////////////////////////
//
// Software UART
//
// http://martinsant.net/?page_id=1689
//
////////////////////////////////////////////




#ifdef SOFTWARE_UART
static  TxSerial        s_tx    = { (1<<TX_PIN),(uint8_t)~(1<<TX_PIN),0,0,TX_IDLE,0,'\0'}; // UART
#else
static  TxSerial        s_tx    = { 0,0 }; // UART
#endif


/** Interrupt vector
 *
 * This is an interrupt service routine for the TIMER0 COMPA vector.
 * It is used by software UART and time keeping.
 * The UART state is kept in s_tx
 *
 */

ISR(TIMER0_COMPA_vect)
{

    if ( s_tx.ctr0 == 0xFF )
        ++s_tx.ctr1;

    ++s_tx.ctr0;

#ifdef SOFTWARE_UART
    switch (s_tx.state)
    {
    case TX_TRANSMIT:
        //Fase 1: Shift 1 bit
        if ( (s_tx.data & 0x01) == 0x01 )  //1 in lsb s_tx_data
            TX_PORT |=  s_tx.bit_msk;   //Pull up, high level
        else                               //0 in lsb s_tx_data
            TX_PORT  &= s_tx.bit_clr;   //Pull down, low level

        s_tx.data >>= 1;                   //Shift away sent bit
        //Fase 2: Incement Bit Counter and Compare
        ++s_tx.bit_cnt;                    //Increment
        if (s_tx.bit_cnt < 8)              //Not done yet
            s_tx.state = TX_TRANSMIT;      //Repeat transmit state next bit
        else                               //Done with data
            s_tx.state = TX_STOPBIT;       //Stopbit is the last bit

        return;

    case TX_STARTBIT:                      //TX_STARTBIT MODE (1 bit)
        TX_PORT  &= s_tx.bit_clr;       //Pull down, low level
        s_tx.state 	= TX_TRANSMIT;         //Next state is transmit
        break;                             //Wait 1 bit length
    //##########
    //Wait 1 bit length
    //##########
    case TX_STOPBIT:
        //Fase 3: Stopbit, put line to high level again
        TX_PORT |= s_tx.bit_msk;        //Pull up, High level
        //Prepare for next time
        s_tx.bit_cnt  = 0;                 //Reset bitcounter
        s_tx.state    = TX_IDLE;

        return;

    case TX_IDLE:                           //TX_IDLE mode, wait for startbit
        break;                              //Wait 1 bit length
    //##########
    default:	//Any exceptions or whatever
        s_tx.state 	=	TX_STOPBIT;
        break;
    }
#endif

    return;
}






/////////////////////////////////////////////
//
// Display
//
////////////////////////////////////////////



/** Update display
 *
 *  Display
 *  Volt
 *  Amp
 *  Wh    Ah
 *  watt  time (passed since start)
 *
 * @param pwt pointer to TinyWatt
 * @param pmsg pointer to text buffer
 */

void update_display( DispVal* pDisp, uint8_t* pmsg, uint8_t ix1,uint8_t ix2, uint8_t disp_type );

uint8_t      s_msg[DISP_BUFF_SZ]= {'\0'}; // string buffer
uint8_t      s_row[32]          = {'\0'}; // string buffer

int main(void)
{
    DispVal    disp;
    TinyWatt   tw;
    ADSum      sum;
//   uint8_t    lcd_cnt=0;
    uint8_t    pw=0;


    const uint8_t ctr1_sec = (uint8_t)((uint16_t)NINT_SEC/(uint16_t)256);
    const uint8_t ctr0_sec = (uint8_t)((uint16_t)NINT_SEC-(uint16_t)ctr1_sec*(uint16_t)256);

    PORTB |= (1 << PB0);
    DDRB  |= (1 << PB0);
    PORTB |= (1 << PB1);
    DDRB  |= (1 << PB1);
    PORTB |= (1 << PB2);
    DDRB  |= (1 << PB2);
    PORTB |= (1 << PB3);
    DDRB  |= (1 << PB3);

    PORTB |= (1 << PB4);
    DDRB  |= (1 << PB4);

// Not used
    PORTB |= (1 << PB5);
    DDRB  |= (1 << PB5);
    PORTB |= (1 << PB6);
    DDRB  |= (1 << PB6);

    DDRB |= (1<<PB5); //PB5 (OC1D) is an output



    DIGITAL_WRITE_LOW(SPI_RST);
    delay_nms(2);
    DIGITAL_WRITE_HIGH(SPI_RST);
    delay_nms(2);

    display_init( DISP_TYPE, DISP_HEIGHT );
    display_fillscreen( 0,  DISP_TYPE );


    simple_memset( s_msg,0,DISP_BUFF_SZ);
    simple_memset( (uint8_t*)&tw,0,sizeof(TinyWatt));
    simple_memset( (uint8_t*)&sum,0,sizeof(ADSum));
    simple_memset( (uint8_t*)&disp,0,sizeof(DispVal));



    /*
       Calibration
       v=adc*k1/k2+m
    */


// U[0]  Vref= 1.1V, Gain x1, Range 13.2V,   ADC clock 125kHz, pin PA4
    tw.ad.u[U_AD_0].calib.k1       = 10000;
    tw.ad.u[U_AD_0].calib.k2       = 772;
    tw.ad.u[U_AD_0].calib.m        = 8;
    tw.ad.u[U_AD_0].calib.dz       = 0; /* dead zone => 0 */
    tw.ad.u[U_AD_0].admux_msk      = ((1<<MUX1)|(1<<MUX0)|(1<<REFS1));
    tw.ad.u[U_AD_0].adcsrb_msk     = 0;
    tw.ad.u[U_AD_0].adcsra_msk     = PRESCALE_8|(1<<ADEN);

// U[1] Vref= 2.56, Gain x1, Range 30.9V, ADC clock 125kHz, pin PA4
    tw.ad.u[U_AD_1].calib.k1       = 16000;
    tw.ad.u[U_AD_1].calib.k2       = 530;
    tw.ad.u[U_AD_1].calib.m        = 42;
    tw.ad.u[U_AD_1].calib.dz       = 0; /* dead zone => 0 */
    tw.ad.u[U_AD_1].admux_msk      = ((1<<MUX1)|(1<<MUX0)|(1<<REFS1));
    tw.ad.u[U_AD_1].adcsrb_msk     = (1<<REFS2);
    tw.ad.u[U_AD_1].adcsra_msk     = PRESCALE_8|(1<<ADEN);


// I[0] Vref= 1.1, Differential gain x30,  range 1.6A, ADC clock 250kHz, pin PA 5-6
    tw.ad.i[I_AD_0].calib.k1       = 1110;
    tw.ad.i[I_AD_0].calib.k2       = 692;
    tw.ad.i[I_AD_0].calib.m        = 16;
    tw.ad.i[I_AD_0].calib.dz       = 4; /* dead zone => 0 */
    tw.ad.i[I_AD_0].admux_msk      = ((1<<MUX3)|(1<<MUX2)|(1<<REFS1));
    tw.ad.i[I_AD_0].adcsrb_msk     = (1<<MUX5)|(1<<GSEL);
    tw.ad.i[I_AD_0].adcsra_msk     = PRESCALE_4|(1<<ADEN);

// I[1] Vref= 2.56,Differential gain x20, range 6.2A, ADC clock 250kHz, pin PA 5-6
    tw.ad.i[I_AD_1].calib.k1       = 4690;
    tw.ad.i[I_AD_1].calib.k2       = 778;
    tw.ad.i[I_AD_1].calib.m        = 30;
    tw.ad.i[I_AD_1].calib.dz       = 5; /* dead zone => 0 */
    tw.ad.i[I_AD_1].admux_msk      = (1<<MUX3)|(1<<MUX2)|(1<<REFS1);
    tw.ad.i[I_AD_1].adcsrb_msk     = (1<<MUX5)|(1<<REFS2);
    tw.ad.i[I_AD_1].adcsra_msk     = PRESCALE_4|(1<<ADEN);

#ifdef GA1A12S202
//  Vref= 3.2V, Gain x1,     ADC clock 125kHz, pin PA0
    tw.ad.light.calib.k1           = 0;
    tw.ad.light.calib.k2           = 0;
    tw.ad.light.calib.m            = 0;
    tw.ad.light.calib.dz           = 0; /* dead zone => 0 */
    tw.ad.light.admux_msk          = 0;
    tw.ad.light.adcsrb_msk         = 0;
    tw.ad.light.adcsra_msk         = PRESCALE_8|(1<<ADEN);
#endif

    // set whole PORTB initially to output
    DDRB = 0xff;
    DDRA |= (1<<PA7);

    tx_init_suart( TX_TIMERVALUE );
    sei();

    // Not used
//   DDRA   |= (1<<PA0);
//   PORTA  |= (1<<PA0);

    DDRA   |= (1<<PA1);
    PORTA  |= (1<<PA1);
    DDRA   |= (1<<PA2);
    PORTA  |= (1<<PA2);
    DDRA   |= (1<<PA3);
    PORTA  |= (1<<PA3);




#ifdef SOFTWARE_UART
//    tx_log_info( &s_tx, &tw, s_msg );
#endif



//interrupt counter
    s_tx.ctr0 = 0;
    s_tx.ctr1 = 0;

    tw.dval.ah         = 0;
    tw.dval.wh         = 0;
    tw.meter.amp_hour  = 0;
    tw.meter.amp_min   = 0;
    tw.meter.watt_hour = 0;
    tw.meter.watt_min  = 0;

    tw.meter.nhour     = 0;
    tw.meter.nmin      = 0;

    tw.ad.u_ix         = 0;
    tw.ad.i_ix         = 0;


#ifndef CALIBRATION
    uint8_t j;

    while (1)
    {
#ifdef GA1A12S202
        tw.ad.light.adc_sum = 0;
        tw.ad.light.nread   = 0;
#endif

        for ( j=0; j<2; ++j ) /* Update display 2 times per sec */
        {
            if( pw>64)
                pw=0;
            else
                ++pw;

            OCR1D = pw;

            tw.ad.sum.w       = 0;
            tw.ad.sum.w_nread = 0;
            tw.ad.sum.u       = 0;
            tw.ad.sum.u_nread = 0;
            tw.ad.sum.i       = 0;
            tw.ad.sum.i_nread = 0;


            watt_measure( &tw,6/*pair*/, 25/*n volt*/, 100/*n amp*/ );


            sum.u   = sum.u+tw.ad.sum.u;
            sum.i   = sum.i+tw.ad.sum.i;
            sum.w   = sum.w+tw.ad.sum.w;

            sum.u_nread = sum.u_nread+tw.ad.sum.u_nread;
            sum.i_nread = sum.i_nread+tw.ad.sum.i_nread;
            sum.w_nread = sum.w_nread+tw.ad.sum.w_nread;


            tw.dval.watt   = sum.w/(uint32_t)sum.w_nread;
            tw.dval.volt   = sum.u/(uint32_t)sum.u_nread;
            tw.dval.amp    = sum.i/(uint32_t)sum.i_nread;

            /*

            The LCD is connected to the PCB by means of a conductive rubber strip; this sometimes causes contact problems and a black screen
            http://www.avdweb.nl/arduino/hardware-interfacing/nokia-5110-lcd.html
                        ++lcd_cnt;
                        if( lcd_cnt>5)
                        {

                            LCD_init();
                            LCD_clear();
                            lcd_cnt = 0;
                        }
            */

#if    0
            ++lcd_cnt;
            if( lcd_cnt>100)
            {
                display_init( DISP_TYPE, 64 );
                display_fillscreen( 0,  DISP_TYPE );
                lcd_cnt = 0;
            }
#endif

            if ( j==1 )
                watt_update(  &tw.meter, &tw.dval, tw.dval.watt, tw.dval.amp );

            round_value( &tw.dval.volt, tw.dval.volt );
            round_value( &tw.dval.amp, tw.dval.amp );

            if ( j==0 )
            {
                watt_2string( &tw, &disp, s_msg,DISP_U, DISP_I);
                update_display(  &disp, s_msg,  DISP_U,DISP_I,DISP_TYPE  );
            }
            else
            {
                watt_2string(  &tw, &disp, s_msg,DISP_TM, DISP_PERF );
                update_display(  &disp, s_msg,DISP_U,DISP_Lux,DISP_TYPE  );
            }


            sum.u       = tw.ad.sum.u;
            sum.i       = tw.ad.sum.i;
            sum.w       = tw.ad.sum.w;
            sum.u_nread = tw.ad.sum.u_nread;
            sum.i_nread = tw.ad.sum.i_nread;
            sum.w_nread = tw.ad.sum.w_nread;
        }
#ifdef GA1A12S202
        tw.ad.light.adc =  (uint16_t)(tw.ad.light.adc_sum/(uint32_t)tw.ad.light.nread);
#endif

#ifdef SOFTWARE_UART
        tx_log( &s_tx, &disp, s_msg );
        tw.dbg_tm=s_tx.ctr1*256+s_tx.ctr0; // Performance monitor

#endif


        // wait until one second is complete
        while ( s_tx.ctr1<ctr1_sec  ) {};
        while ( s_tx.ctr0<ctr0_sec  ) {};

        //clear interrupt counter
        s_tx.ctr0 = 0;
        s_tx.ctr0 = 0;
        s_tx.ctr1 = 0;
        s_tx.ctr1 = 0;

    }

#else /* CALIBRATION */
    uint8_t n=0;

    uint8_t i=0;
    uint8_t t,tt,d;
    while (1)
    {

        n=0;

        for ( t=0; t<2; ++t )
        {
            d=0;
            for ( i=0; i<U_AD_SZ; ++i )
            {

                tw.ad.u[i].v     =0;
                tw.ad.u[i].adc   =0;
                tw.ad.u[i].adc_sum   =0;
                tw.ad.u[i].nread =0;
                adc_read( &tw.ad.u[i],200 );

                if ( tw.ad.u[i].nread>0 )
                    tw.ad.u[i].adc = (uint16_t)(tw.ad.u[i].adc_sum/(uint32_t)tw.ad.u[i].nread);
                else
                    tw.ad.u[i].adc = 0;

                adc_calc_value( &tw.ad.u[i] );
                round_value( &tw.ad.u[i].v, tw.ad.u[i].v );

                n=0;
                uint16_2string(  (uint16_t)tw.ad.u[i].v, &n, s_msg, 1,0);
                s_msg[n] = ';';
                ++n;
                uint16_2string(  (uint16_t)tw.ad.u[i].adc, &n, s_msg, 0,0);
                s_msg[n] = ';';
                ++n;
                s_msg[n] = ';';
                ++n;

                s_msg[n] = '\0';
                display_write_str(0,d,s_msg,0, SSD1306_SPI);
                ++d;
            }


            for ( i=0; i<I_AD_SZ; ++i )
            {

                tw.ad.i[i].v         =0;
                tw.ad.i[i].adc       =0;
                tw.ad.i[i].adc_sum   =0;
                tw.ad.i[i].nread     =0;

                adc_read( &tw.ad.i[i],200  );
                if ( tw.ad.i[i].nread>0 )
                    tw.ad.i[i].adc = (uint16_t)(tw.ad.i[i].adc_sum/(uint32_t)tw.ad.i[i].nread);
                else
                    tw.ad.i[i].adc=0;

                adc_calc_value(  &tw.ad.i[i] );
                round_value( &tw.ad.i[i].v, tw.ad.i[i].v );

                n=0;
                uint16_2string(  (uint16_t)tw.ad.i[i].v, &n, s_msg, 1,0);
                s_msg[n] = ';';
                ++n;
                uint16_2string(  (uint16_t)tw.ad.i[i].adc, &n, s_msg, 0,0);
                s_msg[n] = ';';
                ++n;
                s_msg[n] = ';';
                ++n;

                s_msg[n] = '\0';
                display_write_str(0,d,s_msg,0 , SSD1306_SPI);
                ++d;

            }
        }

        watt_update(  &tw.meter, &tw.dval, tw.dval.watt, tw.dval.amp );


        n=0;
        // Time
        uint16_2string( tw.meter.nhour, &n,s_msg, 0, 2 );
        s_msg[n] = ':';
        ++n;
        uint16_2string( tw.meter.nmin, &n, s_msg, 0,2 );
        s_msg[n] = ':';
        ++n;
        uint16_2string(  tw.meter.nsec, &n, s_msg, 0,2 );
        s_msg[n] = ';';
        ++n;


        tw.dbg_tm=s_tx.ctr1*256+s_tx.ctr0; // performance monitor
        uint16_2string(  (uint16_t)tw.dbg_tm, &n, s_msg, 0,0);
        s_msg[n] = '\0';
        display_write_str(0,d,s_msg,0 , SSD1306_SPI);

        tx_kalib_log( &s_tx, &tw, s_msg  );

        // wait for until one second is complete
        while ( s_tx.ctr1<ctr1_sec  ) {};
        while ( s_tx.ctr0<ctr0_sec  ) {};


        s_tx.ctr0 = 0;
        s_tx.ctr0 = 0;
        s_tx.ctr1 = 0;
        s_tx.ctr1 = 0;
    }

#endif

    return 0;
}




/** Simple memset
 *
 * Set sz number of bytes with val in uint8_t_arr
 *
 *
 * @param arr pointer to destination array
 * @param val value to set
 * @param sz  number of bytes
 */

void
simple_memset( uint8_t* arr, uint8_t val, uint8_t sz )
{
    uint8_t i=0;

    for ( i=0; i<sz; ++i )
    {
        arr[i]=val;
    }
}






/** Update Display
 *
 *  Display
 *  Volt
 *  Amp
 *  Wh    Ah
 *  watt  time (passed since start)
 *
 * @param pwt pointer to TinyWatt
 * @param pmsg pointer to text buffer
 */

void update_display(   DispVal* pDisp, uint8_t* pmsg, uint8_t ix1,uint8_t ix2, uint8_t disp_type )
{
    uint8_t i=0;
    uint8_t n;
    uint8_t* p;


    // Add unit
    for(i=ix1; i<=ix2; i++)
    {
        n=pDisp->sz[i];
        p=&pmsg[i*DISP_STR_SZ];

        if( i>0 && n>6)
            n=6; // 999.99
        else if ( n>=DISP_STR_SZ)
            n=DISP_STR_SZ-1;

        switch(i)
        {
        case DISP_U:
            p[n] = '=';
            ++n;
            p[n]  = ';';
            ++n;
            break;
        case DISP_I:
            p[n] = '<';
            ++n;
            p[n]  = ';';
            ++n;
            break;
        case DISP_Wh:
            p[n] = '>';
            ++n;
            p[n] = '?';
            ++n;
            p[n]=';';
            ++n;
            break;
        case DISP_Ah:
            p[n] = '<';
            ++n;
            p[n] = '?';
            ++n;
            p[n]=';';
            ++n;
            break;
        case DISP_W:
            p[n] = '>';
            ++n;
            p[n]  = ';';
            ++n;
            break;
        }

        p[n] = '\0';
    }

    // LX12864G_086_SPI
#ifdef   LX12864G_086
    spi_write_byte(0xa6, 0 ); /*  General: Positive Display */
    spi_write_byte(0xc8, 0 );
    spi_write_byte(0xa0, 0 );
#endif


    display_write_str(0,0, &pmsg[DISP_U_STR] ,1 ,disp_type);
    display_write_str(0,2, &pmsg[DISP_I_STR] ,1, disp_type);

    if( ix2>DISP_I)
    {
        make_row_str( &pmsg[DISP_Ah_STR],8, &pmsg[DISP_Wh_STR], s_row, DISP_NCOL_SMALL);
        display_write_str(0,4, s_row ,0,disp_type);
#ifdef LOG_PERF_MON
        make_row_str(&pmsg[DISP_W_STR] ,8, &pmsg[DISP_PERF_STR], s_row, DISP_NCOL_SMALL);
#else
        make_row_str(&pmsg[DISP_W_STR] ,8, &pmsg[DISP_Lux_STR], s_row, DISP_NCOL_SMALL);
#endif
        display_write_str(0,5, s_row ,0,disp_type);
    }

}



/** Round value
 *
 *  Round value
 *
 * @param pVal pointer to destination
 * @param val
 */

void
round_value( uint32_t* pVal, uint32_t val )
{
    *pVal = (val+5)/(uint32_t)10;
}

