#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "1charger_cfg.h"

/////////////////////////////////////////////
//
//  LCD operations
//  http://thegaragelab.com/posts/nokia-5110-lcd-displays-on-the-attiny.html
//  http://www.elcesiyejn.com/IC/All%20data%20modules/1444.zip
//
//
////////////////////////////////////////////

//Resolution:   84 x 48
#define NOKIA_SMALL_COL    (14)   // 84/6

//Resolution:   128 x 64
#define SSD1306_SMALL_COL  (21)  // 128/6


#define SSD1306_DISPLAY_OFF_AE         0xAE
#define SSD1306_DISPLAY_ON_AF          0xAF
#define SSD1306_SETLOWCOLUMN_00        0x00
#define SSD1306_SETHIGHCOLUMN_10       0x10
#define SSD1306_SETSTART_LINE_40       0x40
#define SSD1306_SETCONTRAST_81         0x81
#define SSD1306_SEGREMAP_A1            0xA1
#define SSD1306_COMSCANDEC_C8          0xC8
#define SSD1306_NORMALDISPLAY_A6       0xA6
#define SSD1306_SETMULTIPLEX_A8        0xA8
#define SSD1306_SETDISPLAYOFFSET_D3    0xD3
#define SSD1306_SETDISPLAYCLOCKDIV_D5  0xD5
#define SSD1306_SETPRECHARGE_D9        0xD9
#define SSD1306_SETCOMPINS_DA          0xDA
#define SSD1306_SETVCOMDETECT_DB       0xDB
#define SSD1306_MEMORYMODE_20          0x20
#define SSD1306_CHARGEPUMP_8D          0x8D
#define SSD1306_DISPLAYALLON_RESUME_A4 0xA4




void delay_nms(uint16_t  nms);

void spi_write_byte( uint8_t data, uint8_t command  );

void display_init( uint8_t disp_type, uint8_t height );
void display_fillscreen( uint8_t fill_Data, uint8_t disp_type );
void display_setpos( uint8_t x, uint8_t y, uint8_t disp_type);
void display_write_str(uint8_t x, uint8_t y, uint8_t* pStr, uint8_t bLarge, uint8_t disp_type );


// Some code based on "IIC_wtihout_ACK" by http://www.14blog.com/archives/1358
// http://tinusaur.org/2014/08/29/c-library-for-attiny85-to-work-with-ssd1306-controlled-oled-display/

#if SSD1306_IIC
void ssd1306_init(void);
void ssd1306_xfer_start(void);
void ssd1306_xfer_stop(void);
void ssd1306_send_byte(uint8_t byte);
void ssd1306_send_command(uint8_t command);
void ssd1306_send_data_start(void);
void ssd1306_send_data_stop(void);
void ssd1306_setpos(uint8_t x, uint8_t y);
void ssd1306_fillscreen(uint8_t fill_Data);
void ssd1306_large_char(uint8_t c, uint8_t ix);
void ssd1306_write_str(uint8_t x, uint8_t y, uint8_t* pStr, uint8_t bLarge);
#endif


#endif // _DISPLAY_H_
