#include <avr/pgmspace.h>
#include "display.h"



/////////////////////////////////////////////
//
//  LCD operations
//  http://www.elcesiyejn.com/IC/All%20data%20modules/1444.zip
//
////////////////////////////////////////////






// GLCD FontName : Terminal6x8
// GLCD FontSize : 5 x 8
// https://github.com/jadonk/Energia/blob/master/examples/7.Display/LCD_5110_430/Terminal6.h

const uint8_t Terminal6x8[][5] PROGMEM =
{

    {0x00, 0x60, 0x60, 0x00, 0x00}, // Code for char .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // Code for char /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // Code for char 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // Code for char 1
    {0x62, 0x51, 0x49, 0x49, 0x46}, // Code for char 2
    {0x22, 0x49, 0x49, 0x49, 0x36}, // Code for char 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // Code for char 4
    {0x2F, 0x49, 0x49, 0x49, 0x31}, // Code for char 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // Code for char 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // Code for char 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // Code for char 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // Code for char 9
    {0x00, 0x6C, 0x6C, 0x00, 0x00}, // Code for char :

    {0x00, 0x00, 0x00, 0x00, 0x00}, // Code for char ;  space
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // Code for char <  A
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // Code for char  =  V
    {0x3F, 0x40, 0x3C, 0x40, 0x3F}, // Code for char >  W
    {0x7F, 0x04, 0x04, 0x78, 0x00}  // Code for char ?  h



#if 0
    {0x06, 0x09, 0x09, 0x06, 0x00} // Code for <Degrees>
#endif
};

// GLCD FontName : Terminal11x16
// GLCD FontSize : 11 x 16
// https://github.com/jadonk/Energia/blob/master/examples/7.Display/LCD_5110_430/Terminal12.h

const uint8_t Terminal11x16[][22] PROGMEM =
{

    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x38,0x00,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // Code for char .
    {0x00,0x18,0x00,0x1C,0x00,0x0E,0x00,0x07,0x80,0x03,0xC0,0x01,0xE0,0x00,0x70,0x00,0x38,0x00,0x1C,0x00,0x0E,0x00}, // Code for char /
    {0xF8,0x07,0xFE,0x1F,0x06,0x1E,0x03,0x33,0x83,0x31,0xC3,0x30,0x63,0x30,0x33,0x30,0x1E,0x18,0xFE,0x1F,0xF8,0x07}, // Code for char 0
    {0x00,0x00,0x00,0x00,0x0C,0x30,0x0C,0x30,0x0E,0x30,0xFF,0x3F,0xFF,0x3F,0x00,0x30,0x00,0x30,0x00,0x30,0x00,0x00}, // Code for char 1
    {0x1C,0x30,0x1E,0x38,0x07,0x3C,0x03,0x3E,0x03,0x37,0x83,0x33,0xC3,0x31,0xE3,0x30,0x77,0x30,0x3E,0x30,0x1C,0x30}, // Code for char 2
    {0x0C,0x0C,0x0E,0x1C,0x07,0x38,0xC3,0x30,0xC3,0x30,0xC3,0x30,0xC3,0x30,0xC3,0x30,0xE7,0x39,0x7E,0x1F,0x3C,0x0E}, // Code for char 3
    {0xC0,0x03,0xE0,0x03,0x70,0x03,0x38,0x03,0x1C,0x03,0x0E,0x03,0x07,0x03,0xFF,0x3F,0xFF,0x3F,0x00,0x03,0x00,0x03}, // Code for char 4
    {0x3F,0x0C,0x7F,0x1C,0x63,0x38,0x63,0x30,0x63,0x30,0x63,0x30,0x63,0x30,0x63,0x30,0xE3,0x38,0xC3,0x1F,0x83,0x0F}, // Code for char 5
    {0xC0,0x0F,0xF0,0x1F,0xF8,0x39,0xDC,0x30,0xCE,0x30,0xC7,0x30,0xC3,0x30,0xC3,0x30,0xC3,0x39,0x80,0x1F,0x00,0x0F}, // Code for char 6
    {0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x30,0x03,0x3C,0x03,0x0F,0xC3,0x03,0xF3,0x00,0x3F,0x00,0x0F,0x00,0x03,0x00}, // Code for char 7
    {0x00,0x0F,0xBC,0x1F,0xFE,0x39,0xE7,0x30,0xC3,0x30,0xC3,0x30,0xC3,0x30,0xE7,0x30,0xFE,0x39,0xBC,0x1F,0x00,0x0F}, // Code for char 8
    {0x3C,0x00,0x7E,0x00,0xE7,0x30,0xC3,0x30,0xC3,0x30,0xC3,0x38,0xC3,0x1C,0xC3,0x0E,0xE7,0x07,0xFE,0x03,0xFC,0x00}, // Code for char 9
    {0x00,0x00,0x00,0x00,0x00,0x00,0x70,0x1C,0x70,0x1C,0x70,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // Code for char :

    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // Code for char ; space
    {0x00,0x38,0x00,0x3F,0xE0,0x07,0xFC,0x06,0x1F,0x06,0x1F,0x06,0xFC,0x06,0xE0,0x07,0x00,0x3F,0x00,0x38,0x00,0x00}, // Code for char < A
    {0x07,0x00,0x3F,0x00,0xF8,0x01,0xC0,0x0F,0x00,0x3E,0x00,0x3E,0xC0,0x0F,0xF8,0x01,0x3F,0x00,0x07,0x00,0x00,0x00}, // Code for char = V


    {0xFF,0x3F,0xFF,0x3F,0x00,0x1C,0x00,0x06,0x80,0x03,0x80,0x03,0x00,0x06,0x00,0x1C,0xFF,0x3F,0xFF,0x3F,0x00,0x00}, // Code for char > W
    {0xFF,0x3F,0xFF,0x3F,0xC0,0x00,0x60,0x00,0x60,0x00,0x60,0x00,0xE0,0x00,0xC0,0x3F,0x80,0x3F,0x00,0x00,0x00,0x00}  // Code for char ? h
#if 0
    {0x00,0x00,0x18,0x00,0x3C,0x00,0x66,0x00,0x66,0x00,0x3C,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}  // Code for <Degrees>
#endif

};

/** Delay  milliseconds
 *
 * Simple delay routine
 *
 *
 * @param nms number of milliseconds to wait.
 */

#define WAIT_INNER (F_CPU / 6000)

void delay_nms(uint16_t  nms)
{
    for (uint16_t outer=0; outer<nms; outer++)
    {
        for (uint16_t inner=0; inner<WAIT_INNER; inner++)
        {
            asm volatile(
                "  nop    \n\t"
            );
        }
    }
}



/** Write byte to LCD
 *
 * Write byte/command to LCD
 *
 *
 * @param data
 * @param command
 */

void spi_write_byte(uint8_t data, uint8_t command )
{
    uint8_t i;

    if (command == 0)
        DIGITAL_WRITE_LOW(SPI_DC);
    else
        DIGITAL_WRITE_HIGH(SPI_DC);

    for (i=0; i<8; i++)
    {
        if (data&0x80)
            DIGITAL_WRITE_HIGH(SPI_SDIN);
        else
            DIGITAL_WRITE_LOW(SPI_SDIN);

        DIGITAL_WRITE_LOW(SPI_SCLK);
        data = data << 1;
        DIGITAL_WRITE_HIGH(SPI_SCLK);
    }
}



/*
    NOKIA
    Before you do any communication with the device it needs to be reset.
    The data-sheet is very specific on how this occurs; the RESET line must
    be LOW when power stabilizes and must be pulled high within 100ms of
    power on - this means that the initialization process must occur very
    early on in the program. Once the device has been reset you can start
    sending commands and data to it over the SPI bus.
*/


void display_init( uint8_t disp_type, uint8_t height )
{

#if 0
    if( disp_type == NOKIA_5110_SPI )
        DIGITAL_WRITE_LOW(NOKIA_CE);


    if( disp_type == SSD1306_SPI )
        DIGITAL_WRITE_LOW(SSD1306_CE);

    if( disp_type == LX12864G_086_SPI )
        DIGITAL_WRITE_LOW(LX12864G_086_CE);
#endif // 0


    delay_nms(1);

#ifdef SSD1306_SPI
    if( disp_type == SSD1306_SPI  )
    {
        spi_write_byte(SSD1306_DISPLAY_OFF_AE, 0);        // display off
        spi_write_byte(SSD1306_NORMALDISPLAY_A6, 0);      // This command sets the display to be either normal or inverse.
        spi_write_byte(SSD1306_SETLOWCOLUMN_00, 0);       // set low column address
        spi_write_byte(SSD1306_SETHIGHCOLUMN_10, 0);      // set high column address
        spi_write_byte(SSD1306_SETDISPLAYCLOCKDIV_D5, 0); // set display clock divide ratio/oscillator frequency
        spi_write_byte(0x80, 0);                          // set divide ratio, Set Clock as 100 Frames/Sec

        spi_write_byte(SSD1306_SETMULTIPLEX_A8, 0);
        if( height == 32 )
            spi_write_byte(0x1F, 0); // set multiplex ratio(1 to 32)
        else
            spi_write_byte(0x3F, 0); // set multiplex ratio(1 to 64)

        spi_write_byte(SSD1306_SETDISPLAYOFFSET_D3, 0);   // set display offset	Shift Mapping RAM Counter (0x00~0x3F)
        spi_write_byte(0x00, 0);                          // not offset
        spi_write_byte(SSD1306_SETSTART_LINE_40| 0x0, 0); // set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
        spi_write_byte(SSD1306_CHARGEPUMP_8D, 0);         // set Charge Pump enable/disable
        spi_write_byte(0x14, 0);                          // (0x10) disable == SSD1306_EXTERNALVCC
        spi_write_byte(SSD1306_MEMORYMODE_20, 0);         // Set Page Addressing Mode (0x00/0x01/0x02)
        spi_write_byte(0x02, 0);                          // 0x0 act like ks0108
        spi_write_byte(SSD1306_SEGREMAP_A1, 0);           // A1h, X[0]=1b: column address 127 is mapped to SEG0
        // SH1106_command(SH1106_SEGREMAP | 0x1);
        spi_write_byte(SSD1306_COMSCANDEC_C8, 0);         // C8h, X[3]=1b: remapped mode. Scan from COM[N-1] to COM0

        spi_write_byte(SSD1306_SETCOMPINS_DA, 0);         // set com pins hardware configuration
        if( height == 32 )
            spi_write_byte(0x02, 0);
        else
            spi_write_byte(0x12, 0);

        spi_write_byte(SSD1306_SETCONTRAST_81, 0);        // set contrast control register 2 bytes cmd
        spi_write_byte(0xCF, 0);                          // 1-256 Set SEG Output Current Brightness
        spi_write_byte(SSD1306_SETPRECHARGE_D9, 0);       // set pre-charge period
        spi_write_byte(0xF1, 0);                          // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
        spi_write_byte(SSD1306_SETVCOMDETECT_DB, 0);      // adjusts the VCOMH regulator output.
        spi_write_byte(0x40, 0);                          // VCOM Deselect Level

        spi_write_byte(SSD1306_DISPLAYALLON_RESUME_A4, 0); // set DC-DC enable
        spi_write_byte(SSD1306_NORMALDISPLAY_A6, 0);       //

        spi_write_byte(SSD1306_DISPLAY_ON_AF, 0);          // turn on oled panel

    }
#endif
#ifdef  NOKIA_5110
    if( disp_type == NOKIA_5110_SPI )
    {
        /* Nokia
           After the reset is performed the device is set to the following;.

           Power down mode
           Address counter set to (0, 0)
           Display blank
           Horizontal addressing mode
           Temperature control set to 0
           Bias system set to 0 (Mux rate 1:100
           HV generator is off
           RAM contents undefined
        */
        spi_write_byte(0x21, 0 );	// set LCD mode
        spi_write_byte(0xBE, 0 );	// set bias voltage
        spi_write_byte(0x06, 0 );	// temperature correction
        spi_write_byte(0x13, 0 );	// 1:48
        spi_write_byte(0x20, 0 );	// use bias command
        display_fillscreen( 0,  NOKIA_5110_SPI);
        spi_write_byte(0x0c, 0 );	// set LCD mode,display normally
    }
#endif // NOKIA_5110

#ifdef LX12864G_086

    if( disp_type == LX12864G_086_SPI )
    {
        /*
            http://www.mikrocontroller.net/attachment/252515/JLX12864G-086.pdf
        */
        spi_write_byte(0xe2, 0 ); /* Soft Reset */
        delay_nms(5);
        spi_write_byte(0x2c, 0 ); /* boost 1 */
        delay_nms(5);
        spi_write_byte(0x2e, 0 ); /* boost 2 */
        delay_nms(5);
        spi_write_byte(0x2f, 0 ); /* boost 2 */
        delay_nms(5);
        spi_write_byte(0xa6, 0 ); /*  General: Positive Display */

        /*  bottom 0XC8: reverse scanning order: from bottom to top*/
        spi_write_byte(0xc8, 0 );

        /* 0xA0: General: column address from left to right */
        /** 0xA1: Reverse: column address from right to left */
        spi_write_byte(0xa0, 0 );

        /*
        Internal voltage resistance
        Select the internal resistance ratio (Rb / Ra): it can be understood as a crude
        Adjust the contrast value. The settable range: 0x20 ~ 0x27,
        */
        spi_write_byte(0x23, 0 ); /* coarse contrast, you can set the range of 0x20 ~ 0x27 */
        /*
        Order 0x81 is not changed, one instruction can be set below Fan
        Wai is: 0x00 ~ 0x3F, the greater the value, the more concentrated contrast,
        The smaller the light
        */
        spi_write_byte(0x81, 0 ); /* trim Contrast */
        spi_write_byte(0x28, 0 ); /* 0x1a, trimming the value of contrast, you can set the range of 0x00 ~ 0x3f */

        spi_write_byte(0xa2, 0 ); /* 1/9 ratio bias (bias)  0XA3: BIAS = 1/7 */
        spi_write_byte(0x40, 0 ); /** start line: first line */

        // 0XAE OFF
        spi_write_byte(0xaf, 0 ); /* open display */
    }
#endif // LX12864G_086

#if 0
    if( disp_type == NOKIA_5110_SPI )
        DIGITAL_WRITE_HIGH(NOKIA_CE);

    if( disp_type == SSD1306_SPI )
        DIGITAL_WRITE_HIGH(SSD1306_CE);

    if( disp_type == LX12864G_086_SPI )
        DIGITAL_WRITE_HIGH(LX12864G_086_CE);
#endif
}


void display_fillscreen( uint8_t fill_Data, uint8_t disp_type )
{
    uint16_t m;
    uint8_t  n;

#if 0
    if( disp_type == NOKIA_5110_SPI )
        DIGITAL_WRITE_LOW(NOKIA_CE);


    if( disp_type == SSD1306_SPI )
        DIGITAL_WRITE_LOW(SSD1306_CE);

    if( disp_type == LX12864G_086_SPI )
        DIGITAL_WRITE_LOW(LX12864G_086_CE);

    delay_nms(1);
#endif


#ifdef SSD1306_SPI
    if( disp_type == SSD1306_SPI  )
    {
        for(m=0; m<8; m++)
        {
            spi_write_byte(0xb0+m,0);	//page0-page1
            spi_write_byte(0x00,0);		//low column start address
            spi_write_byte(0x10,0);		//high column start address

            for(n=0; n<128; n++)
            {
                spi_write_byte(fill_Data,1 );
            }
        }
    }
#endif

#ifdef NOKIA_5110
    if( disp_type == NOKIA_5110_SPI )
    {
        spi_write_byte(0x0c, 0);
        spi_write_byte(0x80, 0);

        for (m=0; m<504; m++)
        {
            spi_write_byte(fill_Data, 1);
        }
    }
#endif // NOKIA_5110

#ifdef LX12864G_086
    if( disp_type == LX12864G_086_SPI )
    {
        for(m=0; m<9; m++)
        {
            spi_write_byte(0xb0+m, 0);
            spi_write_byte(0x10, 0);
            spi_write_byte(0x00, 0);
            for(n=0; n<132; n++)
            {
                spi_write_byte(fill_Data, 1);
            }
        }
    }
#endif
#if 0
    if( disp_type == NOKIA_5110_SPI )
        DIGITAL_WRITE_HIGH(NOKIA_CE);

    if( disp_type == SSD1306_SPI )
        DIGITAL_WRITE_HIGH(SSD1306_CE);

    if( disp_type == LX12864G_086_SPI )
        DIGITAL_WRITE_HIGH(LX12864G_086_CE);
#endif
}

void display_setpos(uint8_t x, uint8_t y, uint8_t disp_type)
{
#ifdef SSD1306_SPI
    if( disp_type == SSD1306_SPI  )
    {
        spi_write_byte(0x00,0);	//write command
        spi_write_byte(0xb0+y,0);
        spi_write_byte(((x&0xf0)>>4)|0x10,0); // |0x10
        spi_write_byte((x&0x0f)|0x01,0); // |0x01
    }
#endif

#ifdef NOKIA_5110
    if( disp_type == NOKIA_5110_SPI )
    {
        spi_write_byte(0x40 | y, 0);		// column
        spi_write_byte(0x80 | x, 0);        // row
    }
#endif

#ifdef LX12864G_086
    if( disp_type == LX12864G_086_SPI )
    {
        // x+4 for reveresed
        spi_write_byte(0xb0+y,0);              /*Set the page address. 8 lines per page. 64 line screen is divided into eight pages. We usually say the first one, the LCD driver IC in the first */
        spi_write_byte(((x>>4)&0x0f)+0x10,0);  /* Set column address high */
        spi_write_byte(  x&0x0f,0);       /* Set column address low */
    }
#endif
}

/** Write small font char to LCD
 *
 * Write character starting at ASCII 46  = '.'
 * supported "./0123456789:;<=>?"
 * where ';' = space
 *       '<' = A
 *       '=' = V
 *       '>' = W
 *       '?' = h
 * font = Terminal6x8
 *
 * @param c
 */

void display_write_char(uint8_t c )
{
    uint8_t i=0;
    const uint8_t* chdata = &Terminal6x8[c-46][0];

    for (i=0; i<5; ++i)
    {
        spi_write_byte(pgm_read_byte_near(chdata),1 );
        ++chdata;
    }
    spi_write_byte(0,1 );
}

/** Write large font char to LCD
 *
 * Write character starting at ASCII 46  = '.'
 * supported "./0123456789:;<="
 * where ';' = space
 *       '<' = A
 *       '=' = V
 * font = Terminal11x16
 *
 * @param c
 * @param ix  0=first half 1=second half
 */

void display_large_char(uint8_t c, uint8_t ix )
{
    uint8_t i=0;
    const uint8_t* chdata = &Terminal11x16[c-46][0];

    if ( ix )
        chdata = &Terminal11x16[c-46][1];


    for (i=0; i<11; ++i)
    {
        spi_write_byte(pgm_read_byte_near(chdata),1 );
        chdata+=2;
    }
    spi_write_byte(0,1 );


}

/** Write string to LCD
 *
 * Write string at column Y and row X
 *
 *
 * @param Y      column
 * @param X      row
 * @param pStr   string pointer
 * @param bLarge 0=normal else large
 */


void display_write_str(uint8_t x, uint8_t y, uint8_t* pStr, uint8_t bLarge, uint8_t disp_type )
{
    uint8_t* p=pStr;
#if 0
    if( disp_type == NOKIA_5110_SPI )
        DIGITAL_WRITE_LOW(NOKIA_CE);


    if( disp_type == SSD1306_SPI )
        DIGITAL_WRITE_LOW(SSD1306_CE);

    if( disp_type == LX12864G_086_SPI )
        DIGITAL_WRITE_LOW(LX12864G_086_CE);

#endif
    display_setpos(x,y,disp_type);

    while (*p)
    {
        if ( bLarge )
            display_large_char(*p,0 );
        else
            display_write_char(*p );
        p++;
    }

    if ( bLarge )
    {
        p=pStr;
        display_setpos(x,y+1,disp_type );

        while (*p)
        {
            display_large_char(*p,1);
            p++;
        }
    }
#if 0
    if( disp_type == NOKIA_5110_SPI )
        DIGITAL_WRITE_HIGH(NOKIA_CE);

    if( disp_type == SSD1306_SPI )
        DIGITAL_WRITE_HIGH(SSD1306_CE);

    if( disp_type == LX12864G_086_SPI )
        DIGITAL_WRITE_LOW(LX12864G_086_CE);
#endif
}

#ifdef SSD1306_IIC

// ----------------------------------------------------------------------------

// Some code based on "IIC_wtihout_ACK" by http://www.14blog.com/archives/1358

void ssd1306_init(void)
{
    SSD1306_IIC_DDR |= (1 << SSD1306_IIC_SDA);	// Set port as output
    SSD1306_IIC_DDR |= (1 << SSD1306_IIC_SCL);	// Set port as output

    ssd1306_send_command(0xAE); // display off
    ssd1306_send_command(0x00); // Set Memory Addressing Mode
    ssd1306_send_command(0x10); // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    ssd1306_send_command(0x40); // Set Page Start Address for Page Addressing Mode,0-7
    ssd1306_send_command(0x81); // Set COM Output Scan Direction
    ssd1306_send_command(0xCF); // ---set low column address
    ssd1306_send_command(0xA1); // ---set high column address
    ssd1306_send_command(0xC8); // --set start line address
    ssd1306_send_command(0xA6); // --set contrast control register
    ssd1306_send_command(0xA8);
    //ssd1306_send_command(0x3F); // --set segment re-map 0 to 127
    ssd1306_send_command(0x1F);
    ssd1306_send_command(0xD3); // --set normal display
    ssd1306_send_command(0x00); // --set multiplex ratio(1 to 64)
    ssd1306_send_command(0xD5); //
    ssd1306_send_command(0x80); // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    ssd1306_send_command(0xD9); // -set display offset
    ssd1306_send_command(0xF1); // -not offset
    ssd1306_send_command(0xDA); // --set display clock divide ratio/oscillator frequency
    // ssd1306_send_command(0x12); // --set divide ratio
    ssd1306_send_command(0x02); // --set divide ratio
    ssd1306_send_command(0xDB); // --set pre-charge period
    ssd1306_send_command(0x40); //
    ssd1306_send_command(0x20); // --set com pins hardware configuration
    ssd1306_send_command(0x02);
    ssd1306_send_command(0x8D); // --set vcomh
    ssd1306_send_command(0x14); // 0x20,0.77xVcc
    ssd1306_send_command(0xA4); // --set DC-DC enable
    ssd1306_send_command(0xA6); //
    ssd1306_send_command(0xAF); // --turn on oled panel
}




void ssd1306_xfer_start(void)
{
    DIGITAL_WRITE_HIGH(SSD1306_IIC_SCL);	// Set to HIGH
    DIGITAL_WRITE_HIGH(SSD1306_IIC_SDA);	// Set to HIGH
    DIGITAL_WRITE_LOW(SSD1306_IIC_SDA);		// Set to LOW
    DIGITAL_WRITE_LOW(SSD1306_IIC_SCL);		// Set to LOW
}

void ssd1306_xfer_stop(void)
{
    DIGITAL_WRITE_LOW(SSD1306_IIC_SCL);		// Set to LOW
    DIGITAL_WRITE_LOW(SSD1306_IIC_SDA);		// Set to LOW
    DIGITAL_WRITE_HIGH(SSD1306_IIC_SCL);	// Set to HIGH
    DIGITAL_WRITE_HIGH(SSD1306_IIC_SDA);	// Set to HIGH
}

void ssd1306_iic_send_byte(uint8_t byte)
{
    uint8_t i;
    for(i=0; i<8; i++)
    {
        if((byte << i) & 0x80)
            DIGITAL_WRITE_HIGH(SSD1306_IIC_SDA);
        else
            DIGITAL_WRITE_LOW(SSD1306_IIC_SDA);

        DIGITAL_WRITE_HIGH(SSD1306_IIC_SCL);
        DIGITAL_WRITE_LOW(SSD1306_IIC_SCL);
    }
    DIGITAL_WRITE_HIGH(SSD1306_IIC_SDA);
    DIGITAL_WRITE_HIGH(SSD1306_IIC_SCL);
    DIGITAL_WRITE_LOW(SSD1306_IIC_SCL);
}



void ssd1306_send_command(uint8_t command)
{
    ssd1306_xfer_start();
    ssd1306_iic_send_byte(SSD1306_IIC_SA);  // Slave address, SA0=0
    ssd1306_iic_send_byte(0x00);	// write command
    ssd1306_iic_send_byte(command);
    ssd1306_xfer_stop();
}

void ssd1306_send_data_start(void)
{
    ssd1306_xfer_start();
    ssd1306_iic_send_byte(SSD1306_IIC_SA);
    ssd1306_iic_send_byte(0x40);	//write data
}

void ssd1306_send_data_stop(void)
{
    ssd1306_xfer_stop();
}

void ssd1306_setpos(uint8_t x, uint8_t y)
{
    ssd1306_xfer_start();
    ssd1306_iic_send_byte(SSD1306_IIC_SA);  //Slave address,SA0=0
    ssd1306_iic_send_byte(0x00);	//write command

    ssd1306_iic_send_byte(0xb0+y);
    ssd1306_iic_send_byte(((x&0xf0)>>4)|0x10); // |0x10
    ssd1306_iic_send_byte((x&0x0f)|0x01); // |0x01

    ssd1306_xfer_stop();
}


void ssd1306_fillscreen(uint8_t fill_Data)
{
    uint8_t m,n;
    for(m=0; m<8; m++)
    {
        ssd1306_send_command(0xb0+m);	//page0-page1
        ssd1306_send_command(0x00);		//low column start address
        ssd1306_send_command(0x10);		//high column start address
        ssd1306_send_data_start();
        for(n=0; n<128; n++)
        {
            ssd1306_iic_send_byte(fill_Data);
        }
        ssd1306_send_data_stop();
    }
}
void ssd1306_write_char(uint8_t c)
{
    uint8_t i=0;
    const uint8_t* chdata = &Terminal6x8[c-46][0];

    for (i=0; i<5; ++i)
    {
        ssd1306_iic_send_byte(pgm_read_byte_near(chdata));
        ++chdata;
    }
    ssd1306_iic_send_byte(0);
}

void ssd1306_large_char(uint8_t c, uint8_t ix)
{
    uint8_t i=0;
    const uint8_t* chdata = &Terminal11x16[c-46][0];

    if ( ix )
        chdata = &Terminal11x16[c-46][1];


    for (i=0; i<11; ++i)
    {
        ssd1306_iic_send_byte(pgm_read_byte_near(chdata) );
        chdata+=2;
    }
    ssd1306_iic_send_byte(0);


}

void ssd1306_write_str(uint8_t x, uint8_t y, uint8_t* pStr, uint8_t bLarge )
{
    uint8_t* p=pStr;
    ssd1306_setpos(x,y);
    ssd1306_send_data_start();
    while (*p)
    {
        if ( bLarge )
            ssd1306_large_char(*p,0);
        else
            ssd1306_write_char(*p);
        p++;
    }
    ssd1306_send_data_stop();
    if ( bLarge )
    {
        p=pStr;
        ssd1306_setpos(x,y+1);
        ssd1306_send_data_start();
        while (*p)
        {
            ssd1306_large_char(*p,1);
            p++;
        }
        ssd1306_send_data_stop();
    }
}




#endif
