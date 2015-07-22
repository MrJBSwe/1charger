#include "strutils.h"
#include "tinywatt.h"

/////////////////////////////////////////////
//
// ADC operations
//
////////////////////////////////////////////

/** ADC select
 *
 * Setup ADMUX,ADCSRB,ADCSRA according to pAD
 * Discarde the first three conversions
 *
 * @param pAD adc settings
 */

inline void adc_select( ADval* pAD )
{
    uint8_t n=0;

    ADMUX  = pAD->admux_msk;
    ADCSRB = pAD->adcsrb_msk;
    ADCSRA = pAD->adcsra_msk;

    // Discarde first conversion

    for ( n=0; n<3; ++n )
    {
        ADCSRA |= (1 << ADSC);
        while (ADCSRA & (1 << ADSC));
        pAD->adc = ADCL;
        pAD->adc |= ADCH << 8;
    }
}

/** ADC read
 *
 *  Setup ADC and read nTimes result in adc_sum, nread
 *
 * @param pAD adc settings
 */

void INLINE_ADC adc_read( ADval* pAD, uint8_t nTimes  )
{
    int32_t  i   = 0;

    adc_select( pAD );

    while ( i<nTimes )
    {

        ADCSRA |= (1<<ADSC);
        while (ADCSRA & (1 << ADSC));
        {
        }

        pAD->adc = ADCL;
        pAD->adc |= ADCH << 8;


        pAD->adc_sum  = pAD->adc_sum +(uint32_t)pAD->adc;
        ++pAD->nread;

        ++i;
    }
}

/** Calc value
 *
 * Calc value from the ADC by using calibration line
 *
 * @param pAD adc
 */

void
adc_calc_value(  ADval* pAD )
{

    if ( pAD->adc<pAD->calib.dz )
    {
        pAD->v=0;
        return;
    }

    if ( pAD->calib.k2 != 0 )
        pAD->v=pAD->calib.k1*pAD->adc/pAD->calib.k2+pAD->calib.m;
}




/////////////////////////////////////////////
//
// Wattmeter
//
////////////////////////////////////////////

/** Uppdate wattmeter
 *
 * Add measured averaged watt and ampere during one second
 *
 * @param pMeter pointer to destination
 * @param pDVal pointer to display values
 * @param watt_sec
 * @param amp_sec
 */

void watt_update(  WattMeter* pMeter, DVal* pDVal, uint32_t watt_sec, uint32_t amp_sec )
{
    ++pMeter->nsec;
    pMeter->watt_sec = pMeter->watt_sec+watt_sec;
    pMeter->amp_sec  = pMeter->amp_sec+amp_sec;

    if ( pMeter->nsec > 59 )
    {
        pMeter->nsec     = 0;
        pMeter->watt_min = pMeter->watt_min+pMeter->watt_sec/(uint32_t)60;
        pMeter->amp_min  = pMeter->amp_min+pMeter->amp_sec/(uint32_t)60;

        ++pMeter->nmin;
        if ( pMeter->nmin >59 )
        {
            pMeter->watt_min  = (pMeter->watt_min)/(uint32_t)60;
            pMeter->watt_hour = pMeter->watt_hour+pMeter->watt_min;

            pMeter->amp_min   = (pMeter->amp_min)/(uint32_t)60;
            pMeter->amp_hour  = pMeter->amp_hour+pMeter->amp_min;
            pMeter->watt_min  = 0;
            pMeter->amp_min   = 0;
            pMeter->nmin      = 0;
            ++pMeter->nhour;
        }

        pMeter->watt_sec  = 0;
        pMeter->amp_sec   = 0;
    }


    pDVal->wh = pMeter->watt_hour+(pMeter->watt_min*(uint32_t)60+pMeter->watt_sec)/(uint32_t)3600;
    pDVal->wh = ( pDVal->wh+50)/(uint32_t)100;

    pDVal->ah = pMeter->amp_hour+(pMeter->amp_min*(uint32_t)60+pMeter->amp_sec)/(uint32_t)3600;
    pDVal->ah = ( pDVal->ah+5)/(uint32_t)10;

}

/** Measure watt
 *
 * Measure volt and amp in nPair
 *
 * Auto select range
 * U[0] 0-12.9V else U[1]
 * I[0] 0- 0.8A else I[1]
 *
 * For each pair measure volt nUAdc times
 * For each pair measure ampere nIAdc times
 *
 * @param pwt pointer to TinyWatt
 * @param nPair
 * @param nUAdc measure Volt nUAdc times
 * @param nIAdc measure Ampere nIAdc times
 */
void watt_measure( TinyWatt* pwt, uint8_t nPair, uint8_t nUAdc, uint8_t nIAdc  )
{
    uint8_t  j=0;
    uint8_t  i=0;

    uint32_t w=0;
    uint32_t u=0;


    for ( j=0; j<nPair; j++ )
    {
        u=0;

        i = pwt->ad.u_ix;
        pwt->ad.u[i].v        = 0;
        pwt->ad.u[i].adc      = 0;
        pwt->ad.u[i].adc_sum  = 0;
        pwt->ad.u[i].nread    = 0;

        adc_read( &pwt->ad.u[i], nUAdc  );

        if ( pwt->ad.u[i].nread>0 )
        {
            pwt->ad.u[i].adc = (uint16_t)(pwt->ad.u[i].adc_sum/(uint32_t)pwt->ad.u[i].nread);

            if ( (i==0 && pwt->ad.u[i].adc<(uint16_t)1000) || (i==1 && pwt->ad.u[i].adc>(uint16_t)280) )
            {
                adc_calc_value( &pwt->ad.u[i] );
                pwt->ad.sum.u = pwt->ad.sum.u+pwt->ad.u[i].v*(uint32_t)pwt->ad.u[i].nread;

                u=pwt->ad.u[i].v;
                pwt->ad.sum.u_nread=pwt->ad.sum.u_nread+pwt->ad.u[i].nread;
            }
        }

        if ( i==0 && pwt->ad.u[i].adc>(uint16_t)900 )
            pwt->ad.u_ix=1;
        else if ( i==1 && pwt->ad.u[i].adc<(uint16_t)300 )
            pwt->ad.u_ix=0;



        i = pwt->ad.i_ix;

        pwt->ad.i[i].v       = 0;
        pwt->ad.i[i].adc     = 0;
        pwt->ad.i[i].adc_sum = 0;
        pwt->ad.i[i].nread   = 0;

        adc_read( &pwt->ad.i[i], nIAdc  );

        if ( pwt->ad.i[i].nread>0 )
        {
            pwt->ad.i[i].adc = (uint16_t)(pwt->ad.i[i].adc_sum/(uint32_t)pwt->ad.i[i].nread);

            if ( (i==0 && pwt->ad.i[i].adc<(uint16_t)500) || (i==1 && pwt->ad.i[i].adc>(uint16_t)50) )
            {
                adc_calc_value( &pwt->ad.i[i] );
                pwt->ad.sum.i   = pwt->ad.sum.i   + pwt->ad.i[i].v*(uint32_t)pwt->ad.i[i].nread;
                pwt->ad.sum.i_nread = pwt->ad.sum.i_nread + pwt->ad.i[i].nread;

                if ( u>0 )
                {
                    w = u*pwt->ad.i[i].v;
                    w = (w+50)/(uint32_t)100;
                    pwt->ad.sum.w +=  w;
                    ++pwt->ad.sum.w_nread;
                }
            }
        }

        if ( i==0 && pwt->ad.i[i].adc>(uint16_t)400 )
            pwt->ad.i_ix=1;
        else if ( i==1 && pwt->ad.i[i].adc<(uint16_t)100 )
            pwt->ad.i_ix=0;

#ifdef GA1A12S202
        adc_read( &pwt->ad.light, 5 );
#endif

    }
}


void watt_2string( TinyWatt* ptw, DispVal* pDisp, uint8_t* pmsg, uint8_t ix1, uint8_t ix2 )
{
    uint8_t i=0;
    uint8_t n=0;
    uint8_t* p=0;

    pDisp->val[DISP_U]   = (uint16_t)ptw->dval.volt;
    pDisp->val[DISP_I]   = (uint16_t)ptw->dval.amp;

    if( ix2>DISP_I)
    {
        uint32_t w=ptw->dval.watt/(uint32_t)100;
        pDisp->val[DISP_W]   = (uint16_t)w;
        pDisp->val[DISP_Wh]  = (uint16_t)ptw->dval.wh;
        pDisp->val[DISP_Ah]  = (uint16_t)ptw->dval.ah;
        pDisp->val[DISP_Lux] = (uint16_t)ptw->ad.light.adc;
//#ifdef LOG_PERF_MON
        pDisp->val[DISP_PERF] = (uint16_t)ptw->dbg_tm;
//#endif
    }

    for(i=ix1; i<=ix2; i++)
    {
        n=0;
        p=&pmsg[i*12];

        if( i== DISP_TM)
        {
            // Time
            uint16_2string( ptw->meter.nhour, &n, pmsg, 0,2 );
            pmsg[n] = ':';
            ++n;
            uint16_2string( ptw->meter.nmin, &n, pmsg, 0,2 );
            pmsg[n] = ':';
            ++n;
            uint16_2string(   ptw->meter.nsec, &n, pmsg, 0,2 );
            pDisp->sz[0]=n;
        }
        else if( i>=DISP_Lux)
            uint16_2string(   pDisp->val[i], &n, p, 0,0 );
        else
            uint16_2string(    pDisp->val[i], &n, p, 1,0 );

        pDisp->sz[i]=n;

        p[n] = '\0';
    }
}

#ifdef GA1A12S202
#ifdef GA_LUX2WATT
inline void adc_Lux2hWatt( ADval* pAD )
{
    /*
        This is just experimental, I'm planning a lookup table with calibrated values

        hW/m2

        LogLux at 3.2V  => 3.2/0.68 (68K load resistor, Adafruit )

        https://learn.adafruit.com/adafruit-ga1a12s202-log-scale-analog-light-sensor/overview
        http://bccp.berkeley.edu/o/Academy/workshop_09/pdfs/InverseSquareLawPresentation.pdf

        There is no simple conversion...it depends on the wavelength or color of the tw.ad.light.
        However, for the SUN, there is an approximate conversion of 0.0079 W/m2 per Lux
    */
    double logLux = (double)3.2*(double)pAD->adc/ ((double)1024*(double)0.68);
    pAD->v        = (uint16_t)(pow(10,logLux)*(double)0.079);
    pAD->v        = (pAD->v+5)/(uint16_t)10;
}
#endif
#endif
