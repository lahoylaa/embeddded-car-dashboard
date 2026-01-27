/*  Author: Jacob Clarey
 *  Date: 10/7/2023
 *  Description: This is a header file for the ILI9341 driver.
 *  Notes:
 */

#ifndef _ILI9341_H_
#define _ILI9341_H_

#include "bitmap_typedefs.h"
#include <stm32f446xx.h>

/*****************************************************************************/
//                               USER DEFINES
/*****************************************************************************/

/***************************** COLOR DEFINITIONS *****************************/

#define BLACK  0x0000
#define BLUE   0x001F
#define RED    0xF800
//#define RED   0x001F
//#define BLUE    0xF800
#define GREEN  0x07E0
#define PURPLE RED | BLUE
#define YELLOW RED | GREEN
#define TEAL   BLUE | GREEN
#define WHITE  BLUE | GREEN | RED
#define GRAY   0x7308
#define NAVY   0x0007
#define MAROON 0xF000
#define PINK   0xF18F
#define JADE   0x0280

/*****************************************************************************/

/***************************** PINOUT DEFINITION *****************************/

#define LCD_RST_PORT GPIOC
#define LCD_RST      0U

#define LCD_CS_PORT GPIOC
#define LCD_CS      1U

#define LCD_RS_PORT GPIOC
#define LCD_RS      2U

#define LCD_WR_PORT GPIOC
#define LCD_WR      3U

#define LCD_RD_PORT GPIOC
#define LCD_RD      4U

#define LCD_D0_PORT GPIOC
#define LCD_D0      5U

#define LCD_D1_PORT GPIOC
#define LCD_D1      6U

#define LCD_D2_PORT GPIOC
#define LCD_D2      7U

#define LCD_D3_PORT GPIOC
#define LCD_D3      8U

#define LCD_D4_PORT GPIOC
#define LCD_D4      9U

#define LCD_D5_PORT GPIOC
#define LCD_D5      10U

#define LCD_D6_PORT GPIOC
#define LCD_D6      11U

#define LCD_D7_PORT GPIOC
#define LCD_D7      12U

#define SD_CS_PORT GPIOF
#define SD_CS      6U

#define SD_DI_PORT GPIOF
#define SD_DI      7U

#define SD_DO_PORT GPIOF
#define SD_DO      6U

#define SD_SCK_PORT GPIOF
#define SD_SCK      5U

/*****************************************************************************/

/**************************** DIMENSION DEFINITION ***************************/

#define TFT_HEIGHT 320
#define TFT_WIDTH  240

/*****************************************************************************/

/***************************** COMMAND DEFINITION ****************************/

#define ILI_NOP     0x00
#define ILI_SWRESET 0x01
#define ILI_RDDID   0xD3
#define ILI_RDDST   0x09

#define ILI_SLPIN  0x10
#define ILI_SLPOUT 0x11
#define ILI_PTLON  0x12
#define ILI_NORON  0x13

#define ILI_RDMODE     0x0A
#define ILI_RDMADCTL   0x0B
#define ILI_RDPIXFMT   0x0C
#define ILI_RDIMGFMT   0x0D
#define ILI_RDSELFDIAG 0x0F

#define ILI_INVOFF   0x20
#define ILI_INVON    0x21
#define ILI_GAMMASET 0x26
#define ILI_DISPOFF  0x28
#define ILI_DISPON   0x29

#define ILI_CASET 0x2A
#define ILI_PASET 0x2B
#define ILI_RAMWR 0x2C
#define ILI_RAMRD 0x2E

#define ILI_PTLAR  0x30
#define ILI_MADCTL 0x36
#define ILI_PIXFMT 0x3A

#define ILI_FRMCTR1 0xB1
#define ILI_FRMCTR2 0xB2
#define ILI_FRMCTR3 0xB3
#define ILI_INVCTR  0xB4
#define ILI_DFUNCTR 0xB6

#define ILI_PWCTR1 0xC0
#define ILI_PWCTR2 0xC1
#define ILI_PWCTR3 0xC2
#define ILI_PWCTR4 0xC3
#define ILI_PWCTR5 0xC4
#define ILI_VMCTR1 0xC5
#define ILI_VMCTR2 0xC7

#define ILI_RDID1 0xDA
#define ILI_RDID2 0xDB
#define ILI_RDID3 0xDC
#define ILI_RDID4 0xDD

#define ILI_GMCTRP1 0xE0
#define ILI_GMCTRN1 0xE1

/*****************************************************************************/

/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
//                                 USER MACROS
/*****************************************************************************/

/****************************** SET/RESET MACROS *****************************/

#define SET_LCD_RST LCD_RST_PORT->BSRR |= 1U << LCD_RST
#define SET_LCD_CS  LCD_CS_PORT->BSRR |= 1U << LCD_CS
#define SET_LCD_RS  LCD_RS_PORT->BSRR |= 1U << LCD_RS
#define SET_LCD_WR  LCD_WR_PORT->BSRR |= 1U << LCD_WR
#define SET_LCD_RD  LCD_RD_PORT->BSRR |= 1U << LCD_RD
#define SET_LCD_D0  LCD_D0_PORT->BSRR |= 1U << LCD_D0
#define SET_LCD_D1  LCD_D1_PORT->BSRR |= 1U << LCD_D1
#define SET_LCD_D2  LCD_D2_PORT->BSRR |= 1U << LCD_D2
#define SET_LCD_D3  LCD_D3_PORT->BSRR |= 1U << LCD_D3
#define SET_LCD_D4  LCD_D4_PORT->BSRR |= 1U << LCD_D4
#define SET_LCD_D5  LCD_D5_PORT->BSRR |= 1U << LCD_D5
#define SET_LCD_D6  LCD_D6_PORT->BSRR |= 1U << LCD_D6
#define SET_LCD_D7  LCD_D7_PORT->BSRR |= 1U << LCD_D7

#define RESET_LCD_RST LCD_RST_PORT->BSRR |= 1U << (LCD_RST + 16)
#define RESET_LCD_CS  LCD_CS_PORT->BSRR |= 1U << (LCD_CS + 16)
#define RESET_LCD_RS  LCD_RS_PORT->BSRR |= 1U << (LCD_RS + 16)
#define RESET_LCD_WR  LCD_WR_PORT->BSRR |= 1U << (LCD_WR + 16)
#define RESET_LCD_RD  LCD_RD_PORT->BSRR |= 1U << (LCD_RD + 16)
#define RESET_LCD_D0  LCD_D0_PORT->BSRR |= 1U << (LCD_D0 + 16)
#define RESET_LCD_D1  LCD_D1_PORT->BSRR |= 1U << (LCD_D1 + 16)
#define RESET_LCD_D2  LCD_D2_PORT->BSRR |= 1U << (LCD_D2 + 16)
#define RESET_LCD_D3  LCD_D3_PORT->BSRR |= 1U << (LCD_D3 + 16)
#define RESET_LCD_D4  LCD_D4_PORT->BSRR |= 1U << (LCD_D4 + 16)
#define RESET_LCD_D5  LCD_D5_PORT->BSRR |= 1U << (LCD_D5 + 16)
#define RESET_LCD_D6  LCD_D6_PORT->BSRR |= 1U << (LCD_D6 + 16)
#define RESET_LCD_D7  LCD_D7_PORT->BSRR |= 1U << (LCD_D7 + 16)

/*****************************************************************************/

/*****************************************************************************/

/*****************************************************************************/
/*****************************************************************************/

/*****************************************************************************/
//                           USER FUNCTION PROTOTYPE
/*****************************************************************************/
/// @brief Initializes SysTick timer for delays.
void SysTick_Init(void);

/// @brief Delays a given number of milliseconds.
/// @param n is number of milliseconds to delay.
void delayMS(uint16_t n);

void delayMicroS(uint16_t n);

/// @brief Set an area for drawing on the display.
/// @param x1 is start column address.
/// @param y1 is start row address.
/// @param x2 is end column address.
/// @param y2 is end row address.
void Set_Address_Window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

/// @brief Fills number of pixels with a color.
/// @brief Note: Call Set_Address_Window() before calling this function.
/// @param color is 16-bit BGR565 color value.
/// @param len is 32-bit number of pixels.
void Fill_Color(uint16_t color, uint32_t len);

/// @brief Draw a line from (x0,y0) to (x1,y1).
/// @param x0 is start column address.
/// @param y0 is start row address.
/// @param x1 is end column address.
/// @param y1 is end row address.
/// @param width is thickness of the line.
/// @param color is 16-bit BGR565 color of the line.
void Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
               uint8_t width, uint16_t color);

/// @brief Draw a rectangle without filling it.
/// @param x is start column address.
/// @param y is start row address.
/// @param w is width of rectangle.
/// @param h is height of rectangle.
void Draw_Box(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/// @brief Inverts the colors on the display.
void Invert_Display(void);

/// @brief Rotate display to an orientation. 0: Portrait. 1: Landscape.
/// @brief 2: Portrait Inverted. 3: Landscape Inverted.
/// @param rotation Values 0, 1, 2, 3. Else, default to Portrait.
void Rotate_Display(uint8_t rotation);

/// @brief Initialize the display driver.
void Display_Init(void);

/// @brief Sets up the GPIO pin modes of the display pins.
void GPIO_PinMode_Setup(void);

/// @brief Sends an 8-bit command to the display.
/// @param command is the command to send.
void ILI_8Bit_Command(uint8_t command);

/// @brief Sends an 8-bit data to the display.
/// @param data is the data to send.
void ILI_8Bit_Data(uint8_t data);

/// @brief Fills a rectangular area with color.
/// @param x is start col address.
/// @param y is start row address.
/// @param w is width of rectangle.
/// @param h is height of rectangle.
/// @param color is 16-bit BGR565 color.
void Fill_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);

/// @brief Fills the entire display with color.
/// @param color is 16-bit BGR565 color.
void Fill_Screen(uint16_t color);

/// @brief Writes a string on the display.
/// @param x is start col address.
/// @param y is start row address.
/// @param str is pointer to the string to be drawn.
/// @param fore_color is 16-bit RGB565 foreground color.
/// @param back_color is 16-bit RGB565 background color.
/// @param font is pointer to the font of the string.
void Draw_String(uint16_t x, uint16_t y, char *str, uint16_t fore_color,
                 uint16_t back_color, const tFont *font);
// String draw function.
// 16 rows (0 to 15) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
// Input: x         columns from the left edge (0 to 20)
//        y         rows from the top edge (0 to 15)
//        pt        pointer to a null terminated string to be printed
//        textColor 16-bit color of the characters
// bgColor is Black and size is 1
// Output: number of characters printed
void DrawString(uint16_t x, uint16_t y, char *pt, int16_t textColor,int16_t backColor);
/// @brief Write a character at a given position.
/// @param x is top left col address.
/// @param y is top left row address.
/// @param character is the ASCII character to be drawn.
/// @param fore_color is foreground color.
/// @param back_color is background color.
/// @param font is pointer to the font of the character.
/// @param is_bg is flag if character has background (1), or not (0).
void Draw_Char(uint16_t x, uint16_t y, char character, uint16_t fore_color,
               uint16_t back_color, const tFont *font, uint8_t is_bg);

/// @brief Display a bitmap image on the screen.
/// @param x is start col address.
/// @param y is start row address.
/// @param bitmap is pointer to the image data to be drawn.
void Draw_Bitmap(uint16_t x, uint16_t y, const tImage16bit *bitmap);

/// @brief Draw a pixel at a coord x,y with color.
/// @param x is start col address.
/// @param y is start row address.
void Draw_Pixel(uint16_t x, uint16_t y, uint16_t color);
/*****************************************************************************/
/*****************************************************************************/
//------------DrawCharS------------
// Simple character draw function.  This is the same function from
// Adafruit_GFX.c but adapted for this processor.  However, each call
// to DrawPixel() calls setAddrWindow(), which needs to send
// many extra data and commands.  If the background color is the same
// as the text color, no background will be printed, and text can be
// drawn right over existing images without covering them with a box.
// Requires (11 + 2*size*size)*6*8 bytes of transmission (image fully on screen; textcolor != bgColor)
// Input: x         horizontal position of the top left corner of the character, columns from the left edge
//        y         vertical position of the top left corner of the character, rows from the top edge
//        c         character to be printed
//        textColor 16-bit color of the character 0-255
//        bgColor   16-bit color of the background 0-255
//        size      number of pixels per character pixel (e.g. size==2 prints each pixel of font as 2x2 square)
// Output: none
void DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size);

#endif // _ILI9341_H_

/* EOF */
