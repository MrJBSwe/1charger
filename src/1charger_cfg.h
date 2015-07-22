#ifndef _1CHARGER_CFG_H_
#define _1CHARGER_CFG_H_

#define  SOFTWARE_UART

// #define CALIBRATION
// #define LOG_PERF_MON    // log, number of timer interrupts


 #define  GA1A12S202        // light sensor on PA0
// #define  GA_LUX2WATT    // 2110 bytes, calculate lux to watt, light sensor


// Defines for UART initialization
#define TX_PIN		       PA7
#define TX_PORT 	       PORTA
#define TX_DDR	           DDRA


#define NINT_SEC           9687      // Calibration time, number of interrupts per second
#define TX_TIMERVALUE      12		 // Calibration UART timer, at 1 MHz, 9600 8n1

// Display

#define LX12864G_086
//#define NOKIA_5110

#define LX12864G_086_SPI   (3)
//#define SSD1306_SPI      (1)
//#define SSD1306_IIC      (2)
//#define NOKIA_5110_SPI   (0)

#define SH1106_I2C_ADDRESS 0x3C // 011110+SA0+RW - 0x3C or 0x3D
// Address for 128x32 is 0x3C
// Address for 128x64 is 0x3D (default) or 0x3C (if SA0 is grounded)

#define SSD1306_IIC_SCL	   PB5	    // SCL,	Pin 3 on SSD1306 Board
#define SSD1306_IIC_SDA	   PB6	    // SDA,	Pin 4 on SSD1306 Board
#define SSD1306_IIC_SA	   0x78	    // Slave address

#define SSD1306_IIC_PORT   PORTB
#define SSD1306_IIC_DDR    DDRB

#define SPI_DDR	           DDRB
#define SPI_PORT           PORTB
#define SPI_SCLK           PB0      // Clock
#define SPI_SDIN           PB1      // Data input (MOSI)   SDA
#define SPI_DC             PB2      // Data Control (0 = command, 1 = data)  RS= LX12864G
#define SPI_RST            PB4      // Reset

// CE = CS LX12864G
#define NOKIA_CE           PB3      // Chip enable (active low)
#define SSD1306_CE         PB5
#define LX12864G_086_CE    PB5

// Selected display type
#define DISP_TYPE          LX12864G_086_SPI
#define DISP_WIDTH         128
#define DISP_HEIGHT        64
#define DISP_NCOL_SMALL    21  // 128/6 small font

#define DIGITAL_WRITE_HIGH(PORT) PORTB |= (1 << PORT)
#define DIGITAL_WRITE_LOW(PORT)  PORTB &= ~(1 << PORT)

#endif // _1CHARGER_CFG_H_
