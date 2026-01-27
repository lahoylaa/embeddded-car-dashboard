/*  Author: Jacob Clarey
 *  Date: 10/7/2023
 *  Appended generic built in font- with adjustable size from ST7735
 *  Added DRAWSTRING, DRAWCHAR commands which don't require a specific font.
 *  Date:  10/11/2023 Dr. Krug
 *  Description: This is a source file for the ILI9341 driver.
 *  Notes:
 */

#include "iLI9341.h"
#include <stdlib.h>
#include <string.h>
#include "bitmap_typedefs.h"

// standard ascii 5x7 font
// originally from glcdfont.c from Adafruit project

static const uint8_t font_generic[1016] = {
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x3E, 0x5B, 0x4F, 0x5B, 0x3E,
  0x3E, 0x6B, 0x4F, 0x6B, 0x3E,
  0x1C, 0x3E, 0x7C, 0x3E, 0x1C,
  0x18, 0x3C, 0x7E, 0x3C, 0x18,
  0x1C, 0x57, 0x7D, 0x57, 0x1C,
  0x1C, 0x5E, 0x7F, 0x5E, 0x1C,
  0x00, 0x18, 0x3C, 0x18, 0x00,
  0xFF, 0xE7, 0xC3, 0xE7, 0xFF,
  0x00, 0x18, 0x24, 0x18, 0x00,
  0xFF, 0xE7, 0xDB, 0xE7, 0xFF,
  0x30, 0x48, 0x3A, 0x06, 0x0E,
  0x26, 0x29, 0x79, 0x29, 0x26,
  0x40, 0x7F, 0x05, 0x05, 0x07,
  0x40, 0x7F, 0x05, 0x25, 0x3F,
  0x5A, 0x3C, 0xE7, 0x3C, 0x5A,
  0x7F, 0x3E, 0x1C, 0x1C, 0x08,
  0x08, 0x1C, 0x1C, 0x3E, 0x7F,
  0x14, 0x22, 0x7F, 0x22, 0x14,
  0x5F, 0x5F, 0x00, 0x5F, 0x5F,
  0x06, 0x09, 0x7F, 0x01, 0x7F,
  0x00, 0x66, 0x89, 0x95, 0x6A,
  0x60, 0x60, 0x60, 0x60, 0x60,
  0x94, 0xA2, 0xFF, 0xA2, 0x94,
  0x08, 0x04, 0x7E, 0x04, 0x08,
  0x10, 0x20, 0x7E, 0x20, 0x10,
  0x08, 0x08, 0x2A, 0x1C, 0x08,
  0x08, 0x1C, 0x2A, 0x08, 0x08,
  0x1E, 0x10, 0x10, 0x10, 0x10,
  0x0C, 0x1E, 0x0C, 0x1E, 0x0C,
  0x30, 0x38, 0x3E, 0x38, 0x30,
  0x06, 0x0E, 0x3E, 0x0E, 0x06,
  0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x5F, 0x00, 0x00,
  0x00, 0x07, 0x00, 0x07, 0x00,
  0x14, 0x7F, 0x14, 0x7F, 0x14,
  0x24, 0x2A, 0x7F, 0x2A, 0x12,
  0x23, 0x13, 0x08, 0x64, 0x62,
  0x36, 0x49, 0x56, 0x20, 0x50,
  0x00, 0x08, 0x07, 0x03, 0x00,
  0x00, 0x1C, 0x22, 0x41, 0x00,
  0x00, 0x41, 0x22, 0x1C, 0x00,
  0x2A, 0x1C, 0x7F, 0x1C, 0x2A,
  0x08, 0x08, 0x3E, 0x08, 0x08,
  0x00, 0x80, 0x70, 0x30, 0x00,
  0x08, 0x08, 0x08, 0x08, 0x08,
  0x00, 0x00, 0x60, 0x60, 0x00,
  0x20, 0x10, 0x08, 0x04, 0x02,
  0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
  0x00, 0x42, 0x7F, 0x40, 0x00, // 1
  0x72, 0x49, 0x49, 0x49, 0x46, // 2
  0x21, 0x41, 0x49, 0x4D, 0x33, // 3
  0x18, 0x14, 0x12, 0x7F, 0x10, // 4
  0x27, 0x45, 0x45, 0x45, 0x39, // 5
  0x3C, 0x4A, 0x49, 0x49, 0x31, // 6
  0x41, 0x21, 0x11, 0x09, 0x07, // 7
  0x36, 0x49, 0x49, 0x49, 0x36, // 8
  0x46, 0x49, 0x49, 0x29, 0x1E, // 9
  0x00, 0x00, 0x14, 0x00, 0x00,
  0x00, 0x40, 0x34, 0x00, 0x00,
  0x00, 0x08, 0x14, 0x22, 0x41,
  0x14, 0x14, 0x14, 0x14, 0x14,
  0x00, 0x41, 0x22, 0x14, 0x08,
  0x02, 0x01, 0x59, 0x09, 0x06,
  0x3E, 0x41, 0x5D, 0x59, 0x4E,
  0x7C, 0x12, 0x11, 0x12, 0x7C, // A
  0x7F, 0x49, 0x49, 0x49, 0x36, // B
  0x3E, 0x41, 0x41, 0x41, 0x22, // C
  0x7F, 0x41, 0x41, 0x41, 0x3E, // D
  0x7F, 0x49, 0x49, 0x49, 0x41, // E
  0x7F, 0x09, 0x09, 0x09, 0x01, // F
  0x3E, 0x41, 0x41, 0x51, 0x73, // G
  0x7F, 0x08, 0x08, 0x08, 0x7F, // H
  0x00, 0x41, 0x7F, 0x41, 0x00, // I
  0x20, 0x40, 0x41, 0x3F, 0x01, // J
  0x7F, 0x08, 0x14, 0x22, 0x41, // K
  0x7F, 0x40, 0x40, 0x40, 0x40, // L
  0x7F, 0x02, 0x1C, 0x02, 0x7F, // M
  0x7F, 0x04, 0x08, 0x10, 0x7F, // N
  0x3E, 0x41, 0x41, 0x41, 0x3E, // O
  0x7F, 0x09, 0x09, 0x09, 0x06, // P
  0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
  0x7F, 0x09, 0x19, 0x29, 0x46, // R
  0x26, 0x49, 0x49, 0x49, 0x32, // S
  0x03, 0x01, 0x7F, 0x01, 0x03, // T
  0x3F, 0x40, 0x40, 0x40, 0x3F, // U
  0x1F, 0x20, 0x40, 0x20, 0x1F, // V
  0x3F, 0x40, 0x38, 0x40, 0x3F, // W
  0x63, 0x14, 0x08, 0x14, 0x63, // X
  0x03, 0x04, 0x78, 0x04, 0x03, // Y
  0x61, 0x59, 0x49, 0x4D, 0x43, // Z
  0x00, 0x7F, 0x41, 0x41, 0x41,
  0x02, 0x04, 0x08, 0x10, 0x20,
  0x00, 0x41, 0x41, 0x41, 0x7F,
  0x04, 0x02, 0x01, 0x02, 0x04,
  0x40, 0x40, 0x40, 0x40, 0x40,
  0x00, 0x03, 0x07, 0x08, 0x00,
  0x20, 0x54, 0x54, 0x78, 0x40, // a
  0x7F, 0x28, 0x44, 0x44, 0x38, // b
  0x38, 0x44, 0x44, 0x44, 0x28, // c
  0x38, 0x44, 0x44, 0x28, 0x7F, // d
  0x38, 0x54, 0x54, 0x54, 0x18, // e
  0x00, 0x08, 0x7E, 0x09, 0x02, // f
  0x18, 0xA4, 0xA4, 0x9C, 0x78, // g
  0x7F, 0x08, 0x04, 0x04, 0x78, // h
  0x00, 0x44, 0x7D, 0x40, 0x00, // i
  0x20, 0x40, 0x40, 0x3D, 0x00, // j
  0x7F, 0x10, 0x28, 0x44, 0x00, // k
  0x00, 0x41, 0x7F, 0x40, 0x00, // l
  0x7C, 0x04, 0x78, 0x04, 0x78, // m
  0x7C, 0x08, 0x04, 0x04, 0x78, // n
  0x38, 0x44, 0x44, 0x44, 0x38, // o
  0xFC, 0x18, 0x24, 0x24, 0x18, // p
  0x18, 0x24, 0x24, 0x18, 0xFC, // q
  0x7C, 0x08, 0x04, 0x04, 0x08, // r
  0x48, 0x54, 0x54, 0x54, 0x24, // s
  0x04, 0x04, 0x3F, 0x44, 0x24, // t
  0x3C, 0x40, 0x40, 0x20, 0x7C, // u
  0x1C, 0x20, 0x40, 0x20, 0x1C, // v
  0x3C, 0x40, 0x30, 0x40, 0x3C, // w
  0x44, 0x28, 0x10, 0x28, 0x44, // x
  0x4C, 0x90, 0x90, 0x90, 0x7C, // y
  0x44, 0x64, 0x54, 0x4C, 0x44, // z
  0x00, 0x08, 0x36, 0x41, 0x00,
  0x00, 0x00, 0x77, 0x00, 0x00,
  0x00, 0x41, 0x36, 0x08, 0x00,
  0x02, 0x01, 0x02, 0x04, 0x02,
  0x3C, 0x26, 0x23, 0x26, 0x3C,
  0x1E, 0xA1, 0xA1, 0x61, 0x12,
  0x3A, 0x40, 0x40, 0x20, 0x7A,
  0x38, 0x54, 0x54, 0x55, 0x59,
  0x21, 0x55, 0x55, 0x79, 0x41,
  0x21, 0x54, 0x54, 0x78, 0x41,
  0x21, 0x55, 0x54, 0x78, 0x40,
  0x20, 0x54, 0x55, 0x79, 0x40,
  0x0C, 0x1E, 0x52, 0x72, 0x12,
  0x39, 0x55, 0x55, 0x55, 0x59,
  0x39, 0x54, 0x54, 0x54, 0x59,
  0x39, 0x55, 0x54, 0x54, 0x58,
  0x00, 0x00, 0x45, 0x7C, 0x41,
  0x00, 0x02, 0x45, 0x7D, 0x42,
  0x00, 0x01, 0x45, 0x7C, 0x40,
  0xF0, 0x29, 0x24, 0x29, 0xF0,
  0xF0, 0x28, 0x25, 0x28, 0xF0,
  0x7C, 0x54, 0x55, 0x45, 0x00,
  0x20, 0x54, 0x54, 0x7C, 0x54,
  0x7C, 0x0A, 0x09, 0x7F, 0x49,
  0x32, 0x49, 0x49, 0x49, 0x32,
  0x32, 0x48, 0x48, 0x48, 0x32,
  0x32, 0x4A, 0x48, 0x48, 0x30,
  0x3A, 0x41, 0x41, 0x21, 0x7A,
  0x3A, 0x42, 0x40, 0x20, 0x78,
  0x00, 0x9D, 0xA0, 0xA0, 0x7D,
  0x39, 0x44, 0x44, 0x44, 0x39,
  0x3D, 0x40, 0x40, 0x40, 0x3D,
  0x3C, 0x24, 0xFF, 0x24, 0x24,
  0x48, 0x7E, 0x49, 0x43, 0x66,
  0x2B, 0x2F, 0xFC, 0x2F, 0x2B,
  0xFF, 0x09, 0x29, 0xF6, 0x20,
  0xC0, 0x88, 0x7E, 0x09, 0x03,
  0x20, 0x54, 0x54, 0x79, 0x41,
  0x00, 0x00, 0x44, 0x7D, 0x41,
  0x30, 0x48, 0x48, 0x4A, 0x32,
  0x38, 0x40, 0x40, 0x22, 0x7A,
  0x00, 0x7A, 0x0A, 0x0A, 0x72,
  0x7D, 0x0D, 0x19, 0x31, 0x7D,
  0x26, 0x29, 0x29, 0x2F, 0x28,
  0x26, 0x29, 0x29, 0x29, 0x26,
  0x30, 0x48, 0x4D, 0x40, 0x20,
  0x38, 0x08, 0x08, 0x08, 0x08,
  0x08, 0x08, 0x08, 0x08, 0x38,
  0x2F, 0x10, 0xC8, 0xAC, 0xBA,
  0x2F, 0x10, 0x28, 0x34, 0xFA,
  0x00, 0x00, 0x7B, 0x00, 0x00,
  0x08, 0x14, 0x2A, 0x14, 0x22,
  0x22, 0x14, 0x2A, 0x14, 0x08,
  0xAA, 0x00, 0x55, 0x00, 0xAA,
  0xAA, 0x55, 0xAA, 0x55, 0xAA,
  0x00, 0x00, 0x00, 0xFF, 0x00,
  0x10, 0x10, 0x10, 0xFF, 0x00,
  0x14, 0x14, 0x14, 0xFF, 0x00,
  0x10, 0x10, 0xFF, 0x00, 0xFF,
  0x10, 0x10, 0xF0, 0x10, 0xF0,
  0x14, 0x14, 0x14, 0xFC, 0x00,
  0x14, 0x14, 0xF7, 0x00, 0xFF,
  0x00, 0x00, 0xFF, 0x00, 0xFF,
  0x14, 0x14, 0xF4, 0x04, 0xFC,
  0x14, 0x14, 0x17, 0x10, 0x1F,
  0x10, 0x10, 0x1F, 0x10, 0x1F,
  0x14, 0x14, 0x14, 0x1F, 0x00,
  0x10, 0x10, 0x10, 0xF0, 0x00,
  0x00, 0x00, 0x00, 0x1F, 0x10,
  0x10, 0x10, 0x10, 0x1F, 0x10,
  0x10, 0x10, 0x10, 0xF0, 0x10,
  0x00, 0x00, 0x00, 0xFF, 0x10,
  0x10, 0x10, 0x10, 0x10, 0x10,
  0x10, 0x10, 0x10, 0xFF, 0x10,
  0x00, 0x00, 0x00, 0xFF, 0x14,
  0x00, 0x00, 0xFF, 0x00, 0xFF,
  0x00, 0x00, 0x1F, 0x10, 0x17,
  0x00, 0x00, 0xFC, 0x04, 0xF4,
  0x14, 0x14, 0x17, 0x10, 0x17,
  0x14, 0x14, 0xF4, 0x04, 0xF4,
  0x00, 0x00, 0xFF, 0x00, 0xF7,
  0x14, 0x14, 0x14, 0x14, 0x14,
  0x14, 0x14, 0xF7, 0x00, 0xF7,
  0x14, 0x14, 0x14, 0x17, 0x14,
  0x10, 0x10, 0x1F, 0x10, 0x1F,
  0x14, 0x14, 0x14, 0xF4, 0x14,
  0x10, 0x10, 0xF0, 0x10, 0xF0,
  0x00, 0x00, 0x1F, 0x10, 0x1F,
  0x00, 0x00, 0x00, 0x1F, 0x14,
  0x00, 0x00, 0x00, 0xFC, 0x14,
  0x00, 0x00, 0xF0, 0x10, 0xF0,
  0x10, 0x10, 0xFF, 0x10, 0xFF,
  0x14, 0x14, 0x14, 0xFF, 0x14,
  0x10, 0x10, 0x10, 0x1F, 0x00,
  0x00, 0x00, 0x00, 0xF0, 0x10,
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0,
  0xFF, 0xFF, 0xFF, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFF, 0xFF,
  0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
  0x38, 0x44, 0x44, 0x38, 0x44,
  0x7C, 0x2A, 0x2A, 0x3E, 0x14,
  0x7E, 0x02, 0x02, 0x06, 0x06,
  0x02, 0x7E, 0x02, 0x7E, 0x02,
  0x63, 0x55, 0x49, 0x41, 0x63,
  0x38, 0x44, 0x44, 0x3C, 0x04,
  0x40, 0x7E, 0x20, 0x1E, 0x20,
  0x06, 0x02, 0x7E, 0x02, 0x02,
  0x99, 0xA5, 0xE7, 0xA5, 0x99,
  0x1C, 0x2A, 0x49, 0x2A, 0x1C,
  0x4C, 0x72, 0x01, 0x72, 0x4C,
  0x30, 0x4A, 0x4D, 0x4D, 0x30,
  0x30, 0x48, 0x78, 0x48, 0x30,
  0xBC, 0x62, 0x5A, 0x46, 0x3D,
  0x3E, 0x49, 0x49, 0x49, 0x00,
  0x7E, 0x01, 0x01, 0x01, 0x7E,
  0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
  0x44, 0x44, 0x5F, 0x44, 0x44,
  0x40, 0x51, 0x4A, 0x44, 0x40,
  0x40, 0x44, 0x4A, 0x51, 0x40,
  0x00, 0x00, 0xFF, 0x01, 0x03,
  0xE0, 0x80, 0xFF, 0x00, 0x00,
  0x08, 0x08, 0x6B, 0x6B, 0x08,
  0x36, 0x12, 0x36, 0x24, 0x36,
  0x06, 0x0F, 0x09, 0x0F, 0x06,
  0x00, 0x00, 0x18, 0x18, 0x00,
  0x00, 0x00, 0x10, 0x10, 0x00,
  0x30, 0x40, 0xFF, 0x01, 0x01,
  0x00, 0x1F, 0x01, 0x01, 0x1E,
  0x00, 0x19, 0x1D, 0x17, 0x12,
  0x00, 0x3C, 0x3C, 0x3C, 0x3C,
  0x00, 0x00, 0x00, 0x00, 0x00,
};
static uint16_t ILI_TFTwidth   = TFT_WIDTH;
static uint16_t ILI_TFTheight  = TFT_HEIGHT;
static uint8_t ILI_Orientation = 0;


/*****************************************************************************/
//                          USER FUNCTION DEFINITIONS
/*****************************************************************************/
void SysTick_Init(void)
{
  SysTick->CTRL = 0;
  SysTick->LOAD = 0x00FFFFFF;
  SysTick->VAL  = 0;
  SysTick->CTRL |= 5;
}

void delayMS(uint16_t n)
{
  SysTick->LOAD = ((n * 16000) - 1);
  SysTick->VAL  = 0;
  while ((SysTick->CTRL & 0x00010000) == 0) {}
}

void delayMicroS(uint16_t n)
{
	SysTick->LOAD = ((n * 16) - 1);
	SysTick->VAL 	= 0;
	while ((SysTick->CTRL & 0x00010000) == 0) {}
}
//------------ST7735_DrawString------------
// String draw function.
// 16 rows (0 to 15) and 21 characters (0 to 20)
// Requires (11 + size*size*6*8) bytes of transmission for each character
// Input: x         columns from the left edge (0 to 20)
//        y         rows from the top edge (0 to 15)
//        pt        pointer to a null terminated string to be printed
//        textColor 16-bit color of the characters
// bgColor is Black and size is 1
// Output: number of characters printed
void DrawString(uint16_t x, uint16_t y, char *pt, int16_t textColor, int16_t backColor){
  uint32_t count = 0;
  while(*pt){
	  //DrawCharS(x*6, y*10, *pt, textColor, BLACK, 1);
	  DrawCharS(x*6, y*10, *pt, textColor, backColor, 2);
    pt++;
    x = x+1;
    count++;
  }
}
void Draw_String(uint16_t x, uint16_t y, char *str, uint16_t fore_color,
                 uint16_t back_color, const tFont *font)
{
  uint16_t temp_x    = x;
  uint16_t temp_y    = y;
  uint16_t currWidth = font->chars[(uint8_t)str[0] - 0x20U].image->width;
  uint8_t length     = strlen(str);
  Draw_Char(x, y, str[0], fore_color, back_color, font, 1);
  for (uint8_t i = 1; i < length; i++)
  {
    temp_x += currWidth;
    currWidth = font->chars[(uint8_t)str[i] - 0x20U].image->width;
    if ((temp_x + currWidth) >= ILI_TFTwidth)
    {
      temp_y += font->chars[0].image->height;
      temp_x = 0;
    }
    Draw_Char(temp_x, temp_y, str[i], fore_color, back_color, font, 1);
  }
}
void DrawCharS(int16_t x, int16_t y, char c, int16_t textColor, int16_t bgColor, uint8_t size){
  uint8_t line; // vertical column of pixels of character in font
  int32_t i, j;

  if((x >= TFT_WIDTH)            || // Clip right
     (y >= TFT_HEIGHT)           || // Clip bottom
     ((x + 5 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (i=0; i<6; i++ ) {
    if (i == 5)
      line = 0x0;
    else
      line = font_generic[(c*5)+i];
    for (j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          Draw_Pixel(x+i, y+j, textColor);
        else {  // big size
          Fill_Rect(x+(i*size), y+(j*size), size, size, textColor);
        }
      } else if (bgColor != textColor) {
        if (size == 1) // default size
          Draw_Pixel(x+i, y+j, bgColor);
        else {  // big size
          Fill_Rect(x+i*size, y+j*size, size, size, bgColor);
        }
      }
      line >>= 1;
    }
  }
}
void Draw_Char(uint16_t x, uint16_t y, char character, uint16_t fore_color,
               uint16_t back_color, const tFont *font, uint8_t is_bg)
{
  const tImage *img = NULL;

  for (uint8_t i = 0; i < font->length; i++)
  {
    if (font->chars[i].code == character)
    {
      img = font->chars[i].image;
      break;
    }
  }
  // No glyph (img) found, so return from this function
  if (img == NULL) { return; }
  if (is_bg)
  {
    uint16_t temp_x             = x;
    uint16_t temp_y             = y;
    uint16_t width              = 0;
    uint16_t height             = 0;
    uint8_t mask                = 0x80;
    uint8_t bit_counter         = 0;
    const uint8_t *img_data_ptr = (const uint8_t *)(img->data);
    uint8_t img_data            = 0;

    width  = img->width;
    height = img->height;

    // font bitmaps are stored in column major order (scanned from
    // left-to-right, not the conventional top-to-bottom) as font glyphs have
    // heigher height than width, this scanning saves some storage. So, we also
    // render in left-to-right manner.

    // Along x axis (width)
    for (int i = 0; i < width; i++)
    {
      // Along y axis (height)
      for (int j = 0; j < height; j++)
      {

        // load new data only when previous byte (or word, depends on
        // glyph->dataSize) is completely traversed by the mask bit_counter = 0
        // means glyph_data is completely traversed by the mask
        if (bit_counter == 0)
        {
          img_data    = *img_data_ptr++;
          bit_counter = img->dataSize;
        }
        // Decrement bit counter
        bit_counter--;

        // If pixel is blank
        if (img_data & mask) { Draw_Pixel(temp_x, temp_y, back_color); }
        else { Draw_Pixel(temp_x, temp_y, fore_color); }

        img_data <<= 1;
        temp_y++;
      }

      // New col starts. So, row is set to initial value and col is increased by
      // one
      temp_y = y;
      temp_x++;

      // Reset the bit counter cause we're moving to next column, so we start
      // with a new byte
      bit_counter = 0;
    }
  }
  else
  {
    //_ili_render_glyph(x, y, fore_color, back_color, img, 0);
    uint16_t width = 0, height = 0;

    uint16_t temp_x = x;
    uint16_t temp_y = y;

    uint8_t mask        = 0x80;
    uint8_t bit_counter = 0;

    const uint8_t *img_data_ptr = (const uint8_t *)(img->data);
    uint8_t img_data            = 0;

    width  = img->width;
    height = img->height;

    // font bitmaps are stored in column major order (scanned from
    // left-to-right, not the conventional top-to-bottom) as font glyphs have
    // heigher height than width, this scanning saves some storage. So, we also
    // render in left-to-right manner.

    // Along x axis (width)
    for (int i = 0; i < width; i++)
    {
      // Along y axis (height)
      for (int j = 0; j < height; j++)
      {

        // load new data only when previous byte (or word, depends on
        // glyph->dataSize) is completely traversed by the mask bit_counter = 0
        // means glyph_data is completely traversed by the mask
        if (bit_counter == 0)
        {
          img_data    = *img_data_ptr++;
          bit_counter = img->dataSize;
        }
        // Decrement bit counter
        bit_counter--;

        if (img_data & mask) { Draw_Pixel(temp_x, temp_y, back_color); }

        img_data <<= 1;
        temp_y++;
      }

      // New col starts. So, row is set to initial value and col is increased by
      // one
      temp_y = y;
      temp_x++;

      // Reset the bit counter cause we're moving to next column, so we start
      // with a new byte
      bit_counter = 0;
    }
  }
}

void Draw_Bitmap(uint16_t x, uint16_t y, const tImage16bit *bitmap)
{
	int16_t w;
	int16_t h;
	w=bitmap->width;
	h=bitmap->height;
  RESET_LCD_CS;
  int16_t skipC = 0;                      // non-zero if columns need to be skipped due to clipping
  int16_t originalWidth = bitmap->width;              // save this value; even if not all columns fit on the screen, the image is still this width in ROM
  int i = bitmap->width*(bitmap->height - 1);

  if((x >= ILI_TFTwidth) || ((y - bitmap->height + 1) >= ILI_TFTheight) || ((x + bitmap->width) <= 0) || (y < 0)){
    return;                             // image is totally off the screen, do nothing
  }
  if((bitmap->width> ILI_TFTwidth) || (bitmap->height > ILI_TFTheight)){    // image is too wide for the screen, do nothing
    //***This isn't necessarily a fatal error, but it makes the
    //following logic much more complicated, since you can have
    //an image that exceeds multiple boundaries and needs to be
    //clipped on more than one side.
    return;
  }
  if((x + w - 1) >= ILI_TFTwidth){            // image exceeds right of screen
    skipC = (x + w) - ILI_TFTwidth;           // skip cut off columns
    w = ILI_TFTwidth - x;
  }
  if((y - h + 1) < 0){                  // image exceeds top of screen
    i = i - (h - y - 1)*originalWidth;  // skip the last cut off rows
    h = y + 1;
  }
  if(x < 0){                            // image exceeds left of screen
    w = w + x;
    skipC = -1*x;                       // skip cut off columns
    i = i - x;                          // skip the first cut off columns
    x = 0;
  }
  if(y >= ILI_TFTheight){                     // image exceeds bottom of screen
    h= bitmap->height - (y - ILI_TFTheight + 1);
    y = ILI_TFTheight - 1;
  }

  Set_Address_Window(x, y-bitmap->height+1, x+bitmap->width-1, y);

  for(y=0; y<bitmap->height; y=y+1){
    for(x=0; x<bitmap->width; x=x+1){
                                        // send the top 8 bits
     // ILI_8Bit_Data((uint8_t)(bitmap[i] >> 8));
                                        // send the bottom 8 bits
     // ILI_8Bit_Data((uint8_t)(bitmap[i]);
			ILI_8Bit_Data((uint16_t)((bitmap->data[i]) & 0xFF00) >> 8);
      ILI_8Bit_Data((uint16_t)((bitmap->data[i]) & 0x00FF));
      i = i + 1;                        // go to the next pixel
    }
    i = i + skipC;
    i = i - 2*originalWidth;
  }

/*  uint16_t width = 0, height = 0;
  width  = bitmap->width;
  height = bitmap->height;

  uint32_t total_pixels = width * height;

  Set_Address_Window(x, y, x + width - 1, y + height - 1);

  for (uint32_t pixels = 0; pixels < total_pixels; pixels++)
  {
    ILI_8Bit_Data((uint16_t)((bitmap->data[pixels]) & 0xFF00) >> 8);
    ILI_8Bit_Data((uint16_t)((bitmap->data[pixels]) & 0x00FF));
	*/

  SET_LCD_CS;
}
// Send two bytes of data, most significant byte first
// Requires 2 bytes of transmission
void static pushColor(uint16_t color) {
  ILI_8Bit_Data((uint8_t)(color >> 8));
  ILI_8Bit_Data((uint8_t)color);
}


void Fill_Screen(uint16_t color)
{
  RESET_LCD_CS;
  Set_Address_Window(0, 0, ILI_TFTwidth, ILI_TFTheight);
  Fill_Color(color, (uint32_t)ILI_TFTwidth * (uint32_t)ILI_TFTheight);
  SET_LCD_CS;
}

void Draw_Line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
               uint8_t width, uint16_t color)
{
  RESET_LCD_CS;
  if (x0 == x1) // vertical line
  {
    if (y0 < y1) { Set_Address_Window(x0, y0, x0 + width - 1, y1); }
    else { Set_Address_Window(x0, y1, x0 + width - 1, y0); }

    Fill_Color(color, (uint32_t)width * (uint32_t)abs(y1 - y0 + 1));
  }
  else if (y0 == y1) // horizontal line
  {
    if (x0 < x1) { Set_Address_Window(x0, y0, x1, y0 + width - 1); }
    else { Set_Address_Window(x1, y0, x0, y0 + width - 1); }

    Fill_Color(color, (uint32_t)width * (uint32_t)abs(x1 - x0 + 1));
  }

  else
  {
    int16_t dx;
    int16_t dy;
    int8_t yi;
    int8_t xi;

    int16_t D;
    uint16_t y;
    uint16_t x;

    uint8_t pixels_per_point = width * width;
    uint8_t color_high       = (uint8_t)(color >> 8);
    uint8_t color_low        = (uint8_t)color;

    if (abs(y1 - y0) < abs(x1 - x0))
    {
      if (x0 > x1)
      {
        dx = x0 - x1;
        dy = y0 - y1;
        yi = 1;

        if (dy < 0)
        {
          yi = -1;
          dy = -dy;
        }

        D = 2 * dy - dx;
        y = y1;
        x = x1;

        while (x <= x0)
        {
          Set_Address_Window(x, y, x + width - 1, y + width - 1);
          // Drawing all the pixels of a single point

          for (uint8_t pixel_cnt = 0; pixel_cnt < pixels_per_point; pixel_cnt++)
          {
            ILI_8Bit_Data(color_high);
            ILI_8Bit_Data(color_low);
          }
          if (D > 0)
          {
            y = y + yi;
            D = D - 2 * dx;
          }
          D = D + 2 * dy;
          x++;
        }
      }
      else
      {
        dx = x1 - x0;
        dy = y1 - y0;
        yi = 1;
        if (dy < 0)
        {
          yi = -1;
          dy = -dy;
        }

        D = 2 * dy - dx;
        y = y0;
        x = x0;

        while (x <= x1)
        {
          Set_Address_Window(x, y, x + width - 1, y + width - 1);
          // Drawing all the pixels of a single point

          for (uint8_t pixel_cnt = 0; pixel_cnt < pixels_per_point; pixel_cnt++)
          {
            ILI_8Bit_Data(color_high);
            ILI_8Bit_Data(color_low);
          }
          if (D > 0)
          {
            y = y + yi;
            D = D - 2 * dx;
          }
          D = D + 2 * dy;
          x++;
        }
      }
    }

    else
    {
      if (y0 > y1)
      {
        dx = x0 - x1;
        dy = y0 - y1;
        xi = 1;

        if (dx < 0)
        {
          xi = -1;
          dx = -dx;
        }

        D = 2 * dx - dy;
        y = y1;
        x = x1;

        while (y <= y0)
        {
          Set_Address_Window(x, y, x + width - 1, y + width - 1);
          // Drawing all the pixels of a single point

          for (uint8_t pixel_cnt = 0; pixel_cnt < pixels_per_point; pixel_cnt++)
          {
            ILI_8Bit_Data(color_high);
            ILI_8Bit_Data(color_low);
          }
          if (D > 0)
          {
            x = x + xi;
            D = D - 2 * dy;
          }
          D = D + 2 * dx;
          y++;
        }
      }
      else
      {
        dx = x1 - x0;
        dy = y1 - y0;
        xi = 1;

        if (dx < 0)
        {
          xi = -1;
          dx = -dx;
        }

        D = 2 * dx - dy;
        y = y0;
        x = x0;

        while (y <= y1)
        {
          Set_Address_Window(x, y, x + width - 1, y + width - 1);
          // Drawing all the pixels of a single point

          for (uint8_t pixel_cnt = 0; pixel_cnt < pixels_per_point; pixel_cnt++)
          {
            ILI_8Bit_Data(color_high);
            ILI_8Bit_Data(color_low);
          }
          if (D > 0)
          {
            x = x + xi;
            D = D - 2 * dy;
          }
          D = D + 2 * dx;
          y++;
        }
      }
    }
  }
  SET_LCD_CS;
}
// Color the pixel at the given coordinates with the given color.
// Requires 13 bytes of transmission
// Input: x     horizontal position of the pixel, columns from the left edge
//               must be less than 240
//               0 is on the left, 238 is near the right
//        y     vertical position of the pixel, rows from the top edge
//               must be less than 320
//               319 is near the wires, 0 is the side opposite the wires
// Output: none
void Draw_Pixel(uint16_t x, uint16_t y, uint16_t color)
{
  RESET_LCD_CS;
  Set_Address_Window(x, y, x, y);
  pushColor(color);
  SET_LCD_CS;
}

void Invert_Display(void)
{
  static uint8_t inv_flag = 0;
  inv_flag ^= 1;
  RESET_LCD_CS;
  if (inv_flag) { ILI_8Bit_Command(ILI_INVON); }
  else { ILI_8Bit_Command(ILI_INVOFF); }
  SET_LCD_CS;
}
//------------ST7735_Color565------------
// Pass 8-bit (each) R,G,B and get back 16-bit packed color.
// Input: r red value
//        g green value
//        b blue value
// Output: 16-bit color
  uint16_t ST7735_Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((b & 0xF8) << 8) | ((g & 0xFC) << 3) | (r >> 3);
}
void Rotate_Display(uint8_t rotation)
{
  uint16_t temp_height = 320;
  uint16_t temp_width  = 240;
  RESET_LCD_CS;
  switch (rotation)
  {

  /* Portrait */
  case 0:
    ILI_8Bit_Command(ILI_MADCTL); // Memory Access Control
    ILI_8Bit_Data(0x48);          // MX: 1, MY: 0, MV: 0
    ILI_TFTheight   = temp_height;
    ILI_TFTwidth    = temp_width;
    ILI_Orientation = 0;
    break;

  /* Landscape */
  case 1:
    ILI_8Bit_Command(ILI_MADCTL); // Memory Access Control
    ILI_8Bit_Data(0x28);          // MX: 0, MY: 0, MV: 1
    ILI_TFTheight   = temp_width;
    ILI_TFTwidth    = temp_height;
    ILI_Orientation = 1;
    break;

  /* Portrait Inverted */
  case 2:
    ILI_8Bit_Command(ILI_MADCTL); // Memory Access Control
    ILI_8Bit_Data(0x88);          // MX: 0, MY: 1, MV: 0
    ILI_TFTheight   = temp_height;
    ILI_TFTwidth    = temp_width;
    ILI_Orientation = 2;
    break;

  /* Landscape Inverted */
  case 3:
    ILI_8Bit_Command(ILI_MADCTL); // Memory Access Control
    ILI_8Bit_Data(0xE8);          // MX: 1, MY: 1, MV: 1
    ILI_TFTheight   = temp_width;
    ILI_TFTwidth    = temp_height;
    ILI_Orientation = 3;
    break;

  /* Default to Portrait */
  default:
    ILI_8Bit_Command(ILI_MADCTL); // Memory Access Control
    ILI_8Bit_Data(0x48);          // MX: 1, MY: 0, MV: 0
    ILI_TFTheight   = temp_height;
    ILI_TFTwidth    = temp_width;
    ILI_Orientation = 0;
    break;
  }
  SET_LCD_CS;
}

void Display_Init(void)
{
  GPIO_PinMode_Setup();

  SET_LCD_RST;
  delayMS(50);
  RESET_LCD_RST;
  delayMS(50);
  SET_LCD_RST;

  RESET_LCD_CS;
  ILI_8Bit_Command(0xEF);
  ILI_8Bit_Data(0x03);
  ILI_8Bit_Data(0x80);
  ILI_8Bit_Data(0x02);

  ILI_8Bit_Command(0xCF);
  ILI_8Bit_Data(0x00);
  ILI_8Bit_Data(0XC1);
  ILI_8Bit_Data(0X30);

  ILI_8Bit_Command(0xED);
  ILI_8Bit_Data(0x64);
  ILI_8Bit_Data(0x03);
  ILI_8Bit_Data(0X12);
  ILI_8Bit_Data(0X81);

  ILI_8Bit_Command(0xE8);
  ILI_8Bit_Data(0x85);
  ILI_8Bit_Data(0x00);
  ILI_8Bit_Data(0x78);

  ILI_8Bit_Command(0xCB);
  ILI_8Bit_Data(0x39);
  ILI_8Bit_Data(0x2C);
  ILI_8Bit_Data(0x00);
  ILI_8Bit_Data(0x34);
  ILI_8Bit_Data(0x02);

  ILI_8Bit_Command(0xF7);
  ILI_8Bit_Data(0x20);

  ILI_8Bit_Command(0xEA);
  ILI_8Bit_Data(0x00);
  ILI_8Bit_Data(0x00);

  ILI_8Bit_Command(ILI_PWCTR1); // Power control
  ILI_8Bit_Data(0x23);          // VRH[5:0]

  ILI_8Bit_Command(ILI_PWCTR2); // Power control
  ILI_8Bit_Data(0x10);          // SAP[2:0];BT[3:0]

  ILI_8Bit_Command(ILI_VMCTR1); // VCM control
  ILI_8Bit_Data(0x3e);
  ILI_8Bit_Data(0x28);

  ILI_8Bit_Command(ILI_VMCTR2); // VCM control2
  ILI_8Bit_Data(0x86);          //--

  ILI_8Bit_Command(ILI_MADCTL); // Memory Access Control
  ILI_8Bit_Data(0x48);          // i.e. Rotate_Display(0);

  ILI_8Bit_Command(ILI_PIXFMT);
  ILI_8Bit_Data(0x55);

  ILI_8Bit_Command(ILI_FRMCTR1);
  ILI_8Bit_Data(0x00);
  ILI_8Bit_Data(0x13); // 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz

  ILI_8Bit_Command(ILI_DFUNCTR); // Display Function Control
  ILI_8Bit_Data(0x08);
  ILI_8Bit_Data(0x82);
  ILI_8Bit_Data(0x27);

  ILI_8Bit_Command(0xF2); // 3Gamma Function Disable
  ILI_8Bit_Data(0x00);

  ILI_8Bit_Command(ILI_GAMMASET); // Gamma curve selected
  ILI_8Bit_Data(0x01);

  ILI_8Bit_Command(ILI_GMCTRP1); // Set Gamma Positive
  ILI_8Bit_Data(0x0F);
  ILI_8Bit_Data(0x31);
  ILI_8Bit_Data(0x2B);
  ILI_8Bit_Data(0x0C);
  ILI_8Bit_Data(0x0E);
  ILI_8Bit_Data(0x08);
  ILI_8Bit_Data(0x4E);
  ILI_8Bit_Data(0xF1);
  ILI_8Bit_Data(0x37);
  ILI_8Bit_Data(0x07);
  ILI_8Bit_Data(0x10);
  ILI_8Bit_Data(0x03);
  ILI_8Bit_Data(0x0E);
  ILI_8Bit_Data(0x09);
  ILI_8Bit_Data(0x00);

  ILI_8Bit_Command(ILI_GMCTRN1); // Set Gamma Negative
  ILI_8Bit_Data(0x00);
  ILI_8Bit_Data(0x0E);
  ILI_8Bit_Data(0x14);
  ILI_8Bit_Data(0x03);
  ILI_8Bit_Data(0x11);
  ILI_8Bit_Data(0x07);
  ILI_8Bit_Data(0x31);
  ILI_8Bit_Data(0xC1);
  ILI_8Bit_Data(0x48);
  ILI_8Bit_Data(0x08);
  ILI_8Bit_Data(0x0F);
  ILI_8Bit_Data(0x0C);
  ILI_8Bit_Data(0x31);
  ILI_8Bit_Data(0x36);
  ILI_8Bit_Data(0x0F);

  ILI_8Bit_Command(ILI_SLPOUT); // Wake Up Display
  delayMS(50);
  ILI_8Bit_Command(ILI_DISPON); // Display On
  delayMS(50);
  SET_LCD_CS;

  Fill_Screen(BLACK); // Begin black screen, Portrait mode
  Rotate_Display(0);
}

void GPIO_PinMode_Setup(void)
{
  RCC->AHB1ENR |= 1 | 2 | 4;

  /* Clear pin modes */
  LCD_RST_PORT->MODER &= ~(3 << (LCD_RST * 2));
  LCD_CS_PORT->MODER &= ~(3 << (LCD_CS * 2));
  LCD_RS_PORT->MODER &= ~(3 << (LCD_RS * 2));
  LCD_WR_PORT->MODER &= ~(3 << (LCD_WR * 2));
  LCD_RD_PORT->MODER &= ~(3 << (LCD_RD * 2));
  LCD_D0_PORT->MODER &= ~(3 << (LCD_D0 * 2));
  LCD_D1_PORT->MODER &= ~(3 << (LCD_D1 * 2));
  LCD_D2_PORT->MODER &= ~(3 << (LCD_D2 * 2));
  LCD_D3_PORT->MODER &= ~(3 << (LCD_D3 * 2));
  LCD_D4_PORT->MODER &= ~(3 << (LCD_D4 * 2));
  LCD_D5_PORT->MODER &= ~(3 << (LCD_D5 * 2));
  LCD_D6_PORT->MODER &= ~(3 << (LCD_D6 * 2));
  LCD_D7_PORT->MODER &= ~(3 << (LCD_D7 * 2));

  /* Set pin modes to output */
  LCD_RST_PORT->MODER |= (1 << (LCD_RST * 2));
  LCD_CS_PORT->MODER |= (1 << (LCD_CS * 2));
  LCD_RS_PORT->MODER |= (1 << (LCD_RS * 2));
  LCD_WR_PORT->MODER |= (1 << (LCD_WR * 2));
  LCD_RD_PORT->MODER |= (1 << (LCD_RD * 2));
  LCD_D0_PORT->MODER |= (1 << (LCD_D0 * 2));
  LCD_D1_PORT->MODER |= (1 << (LCD_D1 * 2));
  LCD_D2_PORT->MODER |= (1 << (LCD_D2 * 2));
  LCD_D3_PORT->MODER |= (1 << (LCD_D3 * 2));
  LCD_D4_PORT->MODER |= (1 << (LCD_D4 * 2));
  LCD_D5_PORT->MODER |= (1 << (LCD_D5 * 2));
  LCD_D6_PORT->MODER |= (1 << (LCD_D6 * 2));
  LCD_D7_PORT->MODER |= (1 << (LCD_D7 * 2));

  /* Pull control pins to HIGH (idle state) */
  SET_LCD_CS;
  SET_LCD_RS;
  SET_LCD_WR;
  SET_LCD_RD;
}

void ILI_8Bit_Command(uint8_t command)
{
  RESET_LCD_RS; // RS->0 for Command

  RESET_LCD_D0; // Clear Previous Command
  RESET_LCD_D1;
  RESET_LCD_D2;
  RESET_LCD_D3;
  RESET_LCD_D4;
  RESET_LCD_D5;
  RESET_LCD_D6;
  RESET_LCD_D7;

  /* Set New Command */
  LCD_D0_PORT->ODR |= ((command & 0x1) >> 0) << LCD_D0;
  LCD_D1_PORT->ODR |= ((command & 0x2) >> 1) << LCD_D1;
  LCD_D2_PORT->ODR |= ((command & 0x4) >> 2) << LCD_D2;
  LCD_D3_PORT->ODR |= ((command & 0x8) >> 3) << LCD_D3;
  LCD_D4_PORT->ODR |= ((command & 0x10) >> 4) << LCD_D4;
  LCD_D5_PORT->ODR |= ((command & 0x20) >> 5) << LCD_D5;
  LCD_D6_PORT->ODR |= ((command & 0x40) >> 6) << LCD_D6;
  LCD_D7_PORT->ODR |= ((command & 0x80) >> 7) << LCD_D7;
	delayMicroS(1);
  RESET_LCD_WR; // Pulse Write Pin
  SET_LCD_WR;
}

void ILI_8Bit_Data(uint8_t data)
{
  SET_LCD_RS; // RS->1 for Data

  RESET_LCD_D0; // Clear Previous Data
  RESET_LCD_D1;
  RESET_LCD_D2;
  RESET_LCD_D3;
  RESET_LCD_D4;
  RESET_LCD_D5;
  RESET_LCD_D6;
  RESET_LCD_D7;

  /* Set New Data */
  LCD_D0_PORT->BSRR |= ((data & 0x1) >> 0) << LCD_D0;
  LCD_D1_PORT->BSRR |= ((data & 0x2) >> 1) << LCD_D1;
  LCD_D2_PORT->BSRR |= ((data & 0x4) >> 2) << LCD_D2;
  LCD_D3_PORT->BSRR |= ((data & 0x8) >> 3) << LCD_D3;
  LCD_D4_PORT->BSRR |= ((data & 0x10) >> 4) << LCD_D4;
  LCD_D5_PORT->BSRR |= ((data & 0x20) >> 5) << LCD_D5;
  LCD_D6_PORT->BSRR |= ((data & 0x40) >> 6) << LCD_D6;
  LCD_D7_PORT->BSRR |= ((data & 0x80) >> 7) << LCD_D7;
	delayMicroS(5);
  RESET_LCD_WR; // Pulse Write pin
  SET_LCD_WR;
}

void Fill_Rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  /* Error checking for Rectangle */
  if ((x >= ILI_TFTwidth) || (y >= ILI_TFTheight) || (w == 0) || (h == 0))
    return;

  /* Cropping if Off Screen */
  if (x + w - 1 >= ILI_TFTwidth) w = ILI_TFTwidth - x;
  if (y + h - 1 >= ILI_TFTheight) h = ILI_TFTheight - y;

  RESET_LCD_CS;
  Set_Address_Window(x, y, x + w - 1, y + h - 1);
  Fill_Color(color, (uint32_t)w * (uint32_t)h);
  SET_LCD_CS;
}

void Set_Address_Window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
  ILI_8Bit_Command(ILI_CASET);

  ILI_8Bit_Data((uint8_t)(x0 >> 8));
  ILI_8Bit_Data((uint8_t)x0);
  ILI_8Bit_Data((uint8_t)(x1 >> 8));
  ILI_8Bit_Data((uint8_t)x1);

  ILI_8Bit_Command(ILI_PASET);
  ILI_8Bit_Data((uint8_t)(y0 >> 8));
  ILI_8Bit_Data((uint8_t)y0);
  ILI_8Bit_Data((uint8_t)(y1 >> 8));
  ILI_8Bit_Data((uint8_t)y1);

  ILI_8Bit_Command(ILI_RAMWR);
}

void Fill_Color(uint16_t color, uint32_t len)
{
  /* This draws using 8x8 squares of the image at a time */
  uint16_t blocks = (uint16_t)(len / 64); // 64 pixels/block
  uint8_t pass_count;
  uint8_t color_high = color >> 8;
  uint8_t color_low  = color;

  // Write first pixel
  ILI_8Bit_Data(color_high);
  ILI_8Bit_Data(color_low);
  len--;

  // If High Color and Low Color are the same,
  // Pulse Write pin to send the previous data
  if (color_high == color_low)
  {
    while (blocks--)
    {
      // pass count = number of blocks / pixels per pass = 64 / 4
      pass_count = 16;
      while (pass_count--)
      {
        RESET_LCD_WR;
        SET_LCD_WR;
        RESET_LCD_WR;
        SET_LCD_WR;
        RESET_LCD_WR;
        SET_LCD_WR;
        RESET_LCD_WR;
        SET_LCD_WR;
        RESET_LCD_WR;
        SET_LCD_WR;
        RESET_LCD_WR;
        SET_LCD_WR;
        RESET_LCD_WR;
        SET_LCD_WR;
        RESET_LCD_WR;
        SET_LCD_WR;
      }
    }
    // Fill any remaining pixels (1 to 64)
    pass_count = len & 63;
    while (pass_count--)
    {
      RESET_LCD_WR;
      SET_LCD_WR;

      RESET_LCD_WR;
      SET_LCD_WR;
    }
  }
  else
  {
    while (blocks--)
    {
      pass_count = 16;
      while (pass_count--)
      {
        /* Send the 4 pixels per Pass */
        ILI_8Bit_Data(color_high);
        ILI_8Bit_Data(color_low);
        ILI_8Bit_Data(color_high);
        ILI_8Bit_Data(color_low);
        ILI_8Bit_Data(color_high);
        ILI_8Bit_Data(color_low);
        ILI_8Bit_Data(color_high);
        ILI_8Bit_Data(color_low);
      }
    }
    pass_count = len & 63;
    while (pass_count--)
    {
      // write here the remaining data
      ILI_8Bit_Data(color_high);
      ILI_8Bit_Data(color_low);
    }
  }
}

/*****************************************************************************/
/*****************************************************************************/

/* EOF */
