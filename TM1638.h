#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#define LED1 0x80
#define LED2 0x40
#define LED3 0x20
#define LED4 0x10
#define LED5 0x08
#define LED6 0x04
#define LED7 0x02
#define LED8 0x01

#define S1 0x01
#define S2 0x02
#define S3 0x04
#define S4 0x08
#define S5 0x10
#define S6 0x20
#define S7 0x40
#define S8 0x80

class TM1638
{
  uint8_t stb;
  uint8_t clk;
  uint8_t dio;
  unsigned char key_evt = 0;
  uint32_t keyCnt[8] = {0};
  unsigned char disp_buf[16] = {0};
  unsigned char   pre_key = 0;
  unsigned char   key_buf = 0;
  unsigned char   key_cnt = 0;
  uint32_t scroll_pos = 0;

  unsigned char key_scan();
  unsigned char tm1638_rd_data();
  void tm1638_wr_data(unsigned char data);
  void  tm1638_wr_str_16();

public:
  TM1638(uint8_t stb, uint8_t clk, uint8_t dio);
  void setup();
  unsigned char getKey();
  void display(const uint8_t (&number)[8], uint8_t dots = 0, uint8_t leds = 0);
  void display(const char* chars);
  void display(uint32_t number);
  void led(const uint8_t leds);
  void dot(const uint8_t dots);
  void led_fill_r(uint8_t num);
};