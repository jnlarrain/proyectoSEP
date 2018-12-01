/** 
 * LCD driver for controller st7735.h / 1.8 TFT DISPLAY /
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.01.2016
 * @file        st7735.h
 * @tested      AVR Atmega16
 * @inspiration http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              https://github.com/adafruit/Adafruit-ST7735-Library
 *              http://w8bh.net/avr/AvrTFT.pdf
 *
 */
#include <avr/pgmspace.h>

#ifndef __ST7735_H__
#define __ST7735_H__

  #define DELAY   0x80
  
  #define NOP     0x00
  #define SWRESET 0x01
  #define RDDID   0x04
  #define RDDST   0x09

  #define SLPIN   0x10
  #define SLPOUT  0x11
  #define PTLON   0x12
  #define NORON   0x13

  #define INVOFF  0x20
  #define INVON   0x21
  #define DISPOFF 0x28
  #define DISPON  0x29
  #define RAMRD   0x2E
  #define CASET   0x2A
  #define RASET   0x2B
  #define RAMWR   0x2C

  #define PTLAR   0x30
  #define MADCTL  0x36
  #define COLMOD  0x3A

  #define FRMCTR1 0xB1
  #define FRMCTR2 0xB2
  #define FRMCTR3 0xB3
  #define INVCTR  0xB4
  #define DISSET5 0xB6

  #define PWCTR1  0xC0
  #define PWCTR2  0xC1
  #define PWCTR3  0xC2
  #define PWCTR4  0xC3
  #define PWCTR5  0xC4
  #define VMCTR1  0xC5

  #define RDID1   0xDA
  #define RDID2   0xDB
  #define RDID3   0xDC
  #define RDID4   0xDD

  #define GMCTRP1 0xE0
  #define GMCTRN1 0xE1

  #define PWCTR6  0xFC

  // MV = 0 in MADCTL
  // max columns
  #define MAX_X 162
  // max rows
  #define MAX_Y 132
  // columns max counter
  #define SIZE_X MAX_X - 1
  // rows max counter
  #define SIZE_Y MAX_Y - 1
  // whole pixels
  #define CACHE_SIZE_MEM (MAX_X * MAX_Y)
  // number of columns for chars
  #define CHARS_COLS_LEN 5
  // number of rows for chars
  #define CHARS_ROWS_LEN 8

  /** @const Command list ST7735B */
  extern const uint8_t INIT_ST7735B[];

  /** @const Characters */
  extern const uint8_t CHARACTERS[][CHARS_COLS_LEN];

  /** @enum Font sizes */
  typedef enum {
    // normal font size: 1x high & 1x wide
    X1 = 0x00,
    // bigger font size: 2x higher & 1x wide
    X2 = 0x01,
    // the biggest font size: font 2x higher & 2x wider
    // 0x0A is set cause offset 5 position to right only for
    //      this case and no offset for previous cases X1, X2
    //      when draw the characters of string in DrawString()
    X3 = 0x0A
  } ESizes;

  void St7735Init(void);

  void St7735Commands(const uint8_t *commands);

  uint8_t Data8BitsSend(uint8_t);

  uint8_t Data16BitsSend(uint16_t);

  uint8_t SetWindow(uint8_t, uint8_t, uint8_t, uint8_t);

  char SetPosition(uint8_t, uint8_t);

  uint8_t SetPartialArea(uint8_t, uint8_t);

  void SendColor565(uint16_t, uint16_t);

  void DrawPixel(uint8_t, uint8_t, uint16_t);

  char DrawChar(char, uint16_t, ESizes);

  void DrawString(char*, uint16_t, ESizes);

  char DrawLine(uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);

  void DrawLineHorizontal(uint8_t, uint8_t, uint8_t, uint16_t);

  void DrawLineVertical(uint8_t, uint8_t, uint8_t, uint16_t);

  void ClearScreen(uint16_t);

  void UpdateScreen(void);

  void DelayMs(uint8_t);

  void drawPixelRGB(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
  
#endif
