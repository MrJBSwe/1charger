/*--------------------------------------------------------------------------*
* A small utility to add date & time to log files
*
* Log2plotly ver 0.95 24-Aug-2014 JB
*
* http://hackaday.io/project/1936-Multicharger
*
*---------------------------------------------------------------------------*/

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
using namespace std;


//
// File types
//

enum
{
    TYPE_TM_U_I_Wh_Ah_W        = 0,
    TYPE_TM_U_I_Wh_Ah_W_ADCLux
    /* add more here ... */
};

//
// Field index
//

enum
{
    IX_TIME,
    IX_U,
    IX_I,
    IX_Wh,
    IX_Ah,
    IX_W,
    IX_ADCLux
};


#define N_TOKENS     30
#define TXT_MAX_LEN  1024

int read_next_line( FILE* file, char* buff, int max_len );

int read_next_token( int& pos, const char* pSrc, char* pDest, int max_len );

int str2tm( bool bDateAndTime, const char* pSrc, struct tm* pTm, int& seconds );

int str2val(  const char* pSrc, double& val );

void adc_Lux2hWatt( int adc, double vRef, double panel_dm2, double panel_efficency, double& hWatt);

void plotly_path( const char* read_path, char* write_path );

int main(int argc, char* argv[] )
{
    FILE* rfile = 0;
    FILE* wfile = 0;

    double hWatt;
    int rel_seconds;
    int start_seconds;
    time_t sec_since_1970 = 0;
    char txt[TXT_MAX_LEN];
    char wtxt[TXT_MAX_LEN];
    char token[N_TOKENS][TXT_MAX_LEN];

    char tm_str[128];
    struct tm tm_start;

    struct tm* timeinfo;
    const char* rpath="";
    char  wpath[1024]= {'\0'};
    double val[N_TOKENS];
    int type=0;


    int pos=0;
    int t=0;


    if( argc<2   )
    {
        printf("input file ? \n" );
        return -1;
    }

    rpath=argv[1];

    rfile = fopen( rpath, "rb");


    if ( rfile == 0 )
    {
        printf("File not found %s\n",rpath);
        return -1;
    }

    plotly_path( rpath, wpath );

    wfile = fopen( wpath, "wb");

    if ( wfile == 0 )
    {
        printf("Can't write to %s\n",wpath);
        return -1;
    }



    while( read_next_line(  rfile, txt, TXT_MAX_LEN )>0 )
    {
        pos=0;
        if( read_next_token(  pos, txt, &token[0][0],TXT_MAX_LEN  ) )
        {
            t=0;
            if( token[0][0]=='#')                     // COMMENT
                continue;

            if( stricmp( &token[0][0],"date")==0 )    // DATE
            {
                read_next_token(  pos, txt, &token[1][0],TXT_MAX_LEN  );
                memset( &tm_start,0,sizeof(struct tm));
                str2tm( 1, &token[1][0], &tm_start, start_seconds );
                tm_start.tm_isdst = -1;
                start_seconds = mktime(&tm_start);
                printf("date=%s\n",&token[1][0] );
            }
            else if( stricmp(&token[0][0],"type")==0) // TYPE
            {

                printf("type=%s\n",&txt[pos] );
                if(stricmp(&txt[pos],"{hours:min:sec,U,I,Wh,Ah,W,ADC-LogLux}")==0)
                {
                    type=TYPE_TM_U_I_Wh_Ah_W_ADCLux;
                }

            }
            else if( stricmp(&token[0][0],"ver")==0)
            {
                read_next_token(  pos, txt, &token[1][0],TXT_MAX_LEN  );
                printf("ver=%s\n",&token[1][0]);
            }
            else
            {
                // Data

                if( str2tm( 0, &token[0][0], &tm_start,  rel_seconds ) )
                {
                    t=1;
                    sec_since_1970 = start_seconds+rel_seconds;
                    val[IX_TIME]   = sec_since_1970;
                    timeinfo       = localtime (  &sec_since_1970 );
                    sprintf(tm_str,"%d-%02d-%02d %02d:%02d:%02d",timeinfo->tm_year+1900,timeinfo->tm_mon+1,timeinfo->tm_mday,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);

                    while( read_next_token(  pos, txt, &token[t][0],TXT_MAX_LEN  ) )
                    {
                        str2val(  &token[t][0], val[t] );
                        ++t;
                    }

                    if( type==TYPE_TM_U_I_Wh_Ah_W_ADCLux)
                    {
                        adc_Lux2hWatt( val[IX_ADCLux], 3.3,      6.4     ,   0.16    ,  hWatt );
                        val[IX_ADCLux]=hWatt;
                    }


                    for( int j=1; j<t; ++j)
                    {
                        if( type==TYPE_TM_U_I_Wh_Ah_W_ADCLux )
                        {
                            // All measurements converted to double and then back to string
                            if( j==1 )
                                sprintf(wtxt,"%s,%.2f",tm_str,val[j] );
                            else
                                sprintf(wtxt,",%s,%.2f",tm_str,val[j] );
                        }
                        else
                        {
                            // Default
                            // All measurements strings copied ......
                             if( j==1 )
                                sprintf(wtxt,"%s,%s",tm_str,&token[j][0] );
                            else
                                sprintf(wtxt,",%s,%s",tm_str,&token[j][0] );
                        }


                        fwrite ( wtxt, strlen(wtxt), 1, wfile );
                    }

                    fwrite ( "\r\n", 2, 1, wfile );
                }
            }

        }



        // printf("%s\n",txt);
    }

    return 0;
}

void plotly_path( const char* read_path, char* write_path )
{
    int n=0;
    int p=0;
    int i=0;
    int len=0;

    len=strlen(read_path);

    n=0;
    for( i=0; i<len; ++i)
    {
        if( p==0 && read_path[i]=='.' )
        {
            write_path[n]='p';
            p=1;
            ++n;
        }
        write_path[n]=read_path[i];
        ++n;
    }

    if( p==0 )
    {
        write_path[n]='p';
        ++n;
    }
    write_path[n]='\0';
}



void adc_Lux2hWatt( int adc, double vRef, double panel_dm2, double panel_efficency, double& hWatt )
{
    /*
    double logLux = k_calib*vRef*(double)adc/ ((double)1024*(double)0.68);
    hWatt         = (pow(10,logLux)*(double)0.000079)*panel_dm2*panel_efficency;
    */

    double logLux = (3.3/vRef)*5.0*adc  / 1024.0;  // Adafruit 3.3v = 10^5 lux
  //  double logLux = vRef*(double)adc*3.2/ ((double)1024*(double)0.68);
    hWatt         = pow(10, logLux)*(double)0.000079;
    hWatt         = hWatt*panel_dm2*panel_efficency;
}

int
str2val(  const char* pSrc, double& val )
{
    if( strlen(pSrc)<1 )
        return 0;

    val = atof(pSrc);

    return 1;
}

int
str2tm( bool bDateAndTime, const char* pSrc, struct tm* pTm, int& seconds )
{
    int len=strlen(pSrc);
    int n=0;
    int j=0;
    int i=0;
    char txt[6][64];

    for( i=0; i<6; ++i )
        txt[i][0]='\0';


    if(len<8 || len>20 )
        return 0; // bad string


    if(bDateAndTime==0)
        n=3;

    i=0;
    while(pSrc[i]==' ')
    {
        ++i;
    }

    for( i; i<len; ++i)
    {
        if( pSrc[i]==':' || pSrc[i]=='-' || (n==2 && pSrc[i]==' ') )
        {
            ++n;
            if(n>=6)
                break;
            j=0;
        }
        else if( pSrc[i]==' ' )
        {
            continue;
        }
        else
        {
            txt[n][j]=pSrc[i];
            txt[n][j+1]='\0';
            ++j;
        }
    }

    if( n!=5 )
        return 0;

    if(bDateAndTime )
    {
        pTm->tm_year = atoi(&txt[0][0]) - 1900;
        pTm->tm_mon = atoi(&txt[1][0]) - 1;
        pTm->tm_mday = atoi(&txt[2][0]);
        pTm->tm_hour = atoi(&txt[3][0]);
        pTm->tm_min =  atoi(&txt[4][0]);
        pTm->tm_sec = atoi(&txt[5][0]);
    }
    else
    {
        seconds= atoi(&txt[3][0])*3600+atoi(&txt[4][0])*60+atoi(&txt[5][0]);
    }

    return 1;

}


int
read_next_token( int& pos, const char* pSrc, char* pDest, int max_len )
{

    int len=0;

    pDest[0] = '\0';

    if(pSrc[pos]=='\0'  )
        return 0;

    len=strlen( &pSrc[pos]);

    while (  pSrc[pos] == '\n' || pSrc[pos]=='\r' || pSrc[pos]==' '  )
    {
        ++pos;
    }

    for (int n=0; n<len; ++n )
    {
        if(pSrc[pos]=='\0'  )
        {
            break;
        }


        if( pSrc[pos]==',' ||  pSrc[pos]=='=' )
        {
            ++(pos);
            break;
        }

        pDest[n] = pSrc[pos];
        pDest[n+1]='\0';
        ++(pos);
    }

    return strlen(pDest);
}


int
read_next_line( FILE* file, char* buff, int max_len )
{
    int n=0;
    int i=0;
    int res=0;

    res = fgetc( file );

    while ( (res == '\n' || res=='\r' ) && res != EOF)
    {
        res=fgetc( file );
    }

    for ( i=0; i<max_len; ++i )
    {
        if ( res == EOF )
        {
            buff[n] = '\0';
            return -1;
        }
        else
        {
            if ( res == '\n' || res=='\r'  )
            {
                break;
            }
            else
            {
                buff[n] = (char)res;
                ++n;
            }
        }
        res = fgetc( file );
    }

    buff[n] = '\0';

    return n;
}

