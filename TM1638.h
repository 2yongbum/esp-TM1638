#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

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

  unsigned char key_scan();
  unsigned char tm1638_rd_data();
  void tm1638_wr_data(unsigned char data);
  void  tm1638_wr_str_16();
public:
  TM1638(uint8_t stb, uint8_t clk, uint8_t dio);
  unsigned char getKey();
  void display(const uint8_t (&number)[8], uint8_t dots = 0, uint8_t leds = 0);
};