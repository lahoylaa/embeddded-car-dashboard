/*  Author: Jacob Clarey
 *  Date: 10/7/2023
 *  Description: This is a header file for use with the ILI9341 driver.
 *  It is used to define bitmap structures for fonts and images.
 *  Notes:
 */

#ifndef _BITMAP_TYPEDEFS_H_
#define _BITMAP_TYPEDEFS_H_

#include <stdint.h>

/// @brief A structure for an 8-bit mapped glyph.
/// @param data is pointer to glyph data.
/// @param width is width of glyph.
/// @param height is height of glyph.
/// @param dataSize is size of glyph data array.
typedef struct
{
  const uint8_t *data;
  uint16_t width;
  uint16_t height;
  uint8_t dataSize;
} tImage;

/// @brief A structure for a bitmap image.
/// @param data is pointer to the image.
/// @param width is width of image.
/// @param height is height of image.
/// @param dataSize is size of bitmap array.
typedef struct
{
  const uint16_t *data;
  uint16_t width;
  uint16_t height;
  uint32_t dataSize;
} tImage16bit;

/// @brief A structure for a font character.
/// @param code is ASCII code of character.
/// @param image is pointer to bitmap image of character.
typedef struct
{
  long int code;
  const tImage *image;
} tChar;

/// @brief A structure for a font.
/// @param length is number of characters in font
/// @param chars is characters in font.
typedef struct
{
  int length;
  const tChar *chars;
} tFont;

#endif // _BITMAP_TYPEDEFS_H_

/* EOF */
