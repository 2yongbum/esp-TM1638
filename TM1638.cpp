#include "TM1638.h"
#define   TM1638_SET_DATA_KEY_RD    0x42
#define   TM1638_SET_DATA_INC_WR    0x40
#define   TM1638_SET_ADDR_0         0xC0
#define   TM1638_SET_DISP_B14       0x8f    // brightness 14/16

constexpr unsigned char ASC2SEG_TBL[256] = {
    0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,   // 0x00 .. 0x07
    0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,   // 0x08 .. 0x0f
    0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,   // 0x10 .. 0x17
    0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,   // 0x18 .. 0x1f
    0b00000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b11010010, 0b00001000, 0b01100011,   // 0x20 .. 0x27
    0b00111001, 0b00001111, 0b10000000, 0b10000000, 0b10000000, 0b01000000, 0b10000000, 0b10000000,   // 0x28 .. 0x2f
    0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111,   // 0x30 .. 0x37
    0b01111111, 0b01101111, 0b00001001, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,   // 0x38 .. 0x3f
    0b10000000, 0b01110111, 0b01111100, 0b00111001, 0b01011110, 0b01111001, 0b01110001, 0b00111101,   // 0x40 .. 0x47
    0b01110110, 0b00110000, 0b00011110, 0b01110101, 0b00111000, 0b01010101, 0b01010100, 0b00111111,   // 0x48 .. 0x4f
    0b01110011, 0b01100111, 0b01010000, 0b01101101, 0b01111000, 0b00111110, 0b00111100, 0b00011101,   // 0x50 .. 0x57
    0b00110110, 0b01101110, 0b01001001, 0b00111001, 0b10000000, 0b00001111, 0b10000000, 0b00001000,   // 0x58 .. 0x5f
    0b10000000, 0b01110111, 0b01111100, 0b01011000, 0b01011110, 0b01111001, 0b01110001, 0b01101111,   // 0x60 .. 0x67
    0b01110100, 0b00010000, 0b00001100, 0b01110101, 0b00111000, 0b01010101, 0b01010100, 0b01011100,   // 0x68 .. 0x6f
    0b01110011, 0b01100111, 0b01010000, 0b01101101, 0b01111000, 0b00011100, 0b00111100, 0b00011101,   // 0x70 .. 0x77
    0b00110110, 0b01101110, 0b01001001, 0b00111001, 0b10000000, 0b00001111, 0b01000000, 0b00001000,   // 0x78 .. 0x7f

    0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,   // 0x80 .. 0x87
    0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,   // 0x88 .. 0x8f
    0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,   // 0x90 .. 0x97
    0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,   // 0x98 .. 0x9f
    0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b11010010, 0b10001000, 0b11100011,   // 0xa0 .. 0xa7
    0b10111001, 0b10001111, 0b10000000, 0b10000000, 0b10000000, 0b11000000, 0b10000000, 0b10000000,   // 0xa8 .. 0xaf
    0b10111111, 0b10000110, 0b11011011, 0b11001111, 0b11100110, 0b11101101, 0b11111101, 0b10000111,   // 0xb0 .. 0xb7
    0b11111111, 0b11101111, 0b10001001, 0b10000000, 0b10000000, 0b10000000, 0b10000000, 0b10000000,   // 0xb8 .. 0xbf
    0b10000000, 0b11110111, 0b11111100, 0b10111001, 0b11011110, 0b11111001, 0b11110001, 0b10101111,   // 0xc0 .. 0xc7
    0b11110110, 0b10110000, 0b10011110, 0b11110101, 0b10111000, 0b11010101, 0b11010100, 0b11111101,   // 0xc8 .. 0xcf
    0b11110011, 0b11100111, 0b11010000, 0b11101101, 0b11111000, 0b10111110, 0b10111100, 0b10011101,   // 0xd0 .. 0xd7
    0b10110110, 0b11101110, 0b11001001, 0b10111001, 0b10000000, 0b10001111, 0b10000000, 0b10001000,   // 0xd8 .. 0xdf
    0b10000000, 0b11110111, 0b11111100, 0b11011000, 0b11011110, 0b11111001, 0b11110001, 0b11101111,   // 0xe0 .. 0xe7
    0b11110100, 0b10010000, 0b10001100, 0b11110101, 0b10111000, 0b11010101, 0b11010100, 0b11011100,   // 0xe8 .. 0xef
    0b11110011, 0b11100111, 0b11010000, 0b11101101, 0b11111000, 0b10011100, 0b10111100, 0b10011101,   // 0xf0 .. 0xf7
    0b10110110, 0b11101110, 0b11001001, 0b10111001, 0b10000000, 0b10001111, 0b11000000, 0b10001000    // 0xf8 .. 0xff
};

TM1638::TM1638(uint8_t stb, uint8_t clk, uint8_t dio) :
  stb(stb), clk(clk), dio(dio) {
}

void TM1638::setup() {
  pinMode(stb, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(dio, OUTPUT);

  digitalWrite(stb, HIGH);
  digitalWrite(clk, HIGH);
  digitalWrite(dio, LOW);
}

unsigned char TM1638::getKey() {
  key_evt = key_scan();
  unsigned char key = key_evt;
  key_evt = 0;
  return key;
}

void TM1638::display(const uint8_t (&number)[8], uint8_t dots, uint8_t leds) {
  for (int i=0; i<8; i++) {
    disp_buf[i*2] = ASC2SEG_TBL[number[i] + 0x30];
    disp_buf[i*2] += dots & (0x80 >> i) ? 0x80 : 0;
    disp_buf[i*2+1] = leds & (0x80 >> i) ? 1 : 0;
  }
  tm1638_wr_str_16();
}

void TM1638::display(const char* chars) {
  uint32_t j = 0;
  uint32_t i = 0;
  for (i=0; i < strlen(chars); i++) {
    unsigned char ch = chars[i];
    if (i != 0) {
      if (ch == '.') {
        disp_buf[(j-1)*2] += 0x80;
        continue;
      }
    }
    disp_buf[j*2] = ASC2SEG_TBL[ch];
    if (++j > 8) {
      break;
    }
  }
  tm1638_wr_str_16();
}
void TM1638::display(uint32_t number) {
  uint8_t n[8] = {0};
  n[7] = number % 10;
  n[6] = number / 10 % 10;
  n[5] = number / 100 % 10;
  n[4] = number / 1000 % 10;
  n[3] = number / 10000 % 10;
  n[2] = number / 100000 % 10;
  n[1] = number / 1000000 % 10;
  n[0] = number / 10000000 % 10;
  display(n);
}

void TM1638::led(const uint8_t leds) {
  for (int i=0; i<8; i++) {
    disp_buf[i*2+1] = leds & (0x80 >> i) ? 1 : 0;
  }
  tm1638_wr_str_16();
}

void TM1638::dot(const uint8_t dots) {
  for (int i=0; i<8; i++) {
    disp_buf[i*2] += dots & (0x80 >> i) ? 0x80 : 0;
  }
  tm1638_wr_str_16();
}

// scan keys on display board
// this function will be called at every 10 msec
// key debouncing time = 80 msec
unsigned char TM1638::key_scan()
{
  int   i;
  unsigned char   rd_byte[4];
  unsigned char   tmp_key = 0;
  unsigned char   key_ret = 0;
  
  // 1. set data CMD write
  digitalWrite(stb, LOW);
  delayMicroseconds(1);
  tm1638_wr_data(TM1638_SET_DATA_KEY_RD);              // set data cmd, key read
  delayMicroseconds(2);

  // 2. read 4 bytes key data
  for (i=0; i<4; i++)
  {
    rd_byte[i] = tm1638_rd_data();
  }

  delayMicroseconds(1);
  digitalWrite(stb, HIGH);

  tmp_key = rd_byte[0] | (rd_byte[1] << 1) | (rd_byte[2] << 2) | (rd_byte[3] << 3);

  if (tmp_key)                  // any key pressed
  {
    if (pre_key == tmp_key)     // key continuously pressed
    {
      if (key_cnt < 8)
        key_cnt++;
    }
    else                        // idle --> key pressed
    {
      pre_key = tmp_key;
      key_cnt = 1;
    }
  }
  else                          // key released or idle
  {
    if (pre_key != 0)
    {
      if (key_cnt >= 8)         // valid key released
      {
        key_ret = pre_key;
        pre_key = 0;
        key_cnt = 0;
      }
      else
      {
        pre_key = 0;
        key_cnt = 0;
      }
    }
  }
  if (key_ret)
  {
    Serial.print("[key_scan] key_data = 0x");
    Serial.println(key_ret, HEX);
  }
  return  key_ret;  
}

// reade 1 byte data from DIO pin serially, and make clock signal
// serial data = LSB first
unsigned char TM1638::tm1638_rd_data()
{
  unsigned char   rd_data;
  int             i;

  pinMode(dio, INPUT_PULLUP);
  rd_data = 0;
  
  for (i=0; i<8; i++)
  {
    digitalWrite(clk, LOW);
    delayMicroseconds(1);
    digitalWrite(clk, HIGH);

    if (digitalRead(dio))
      rd_data |= 0x80;
      
    delayMicroseconds(1);

    if (i < 7)
      rd_data = (rd_data >> 1);
  }
  pinMode(dio, OUTPUT);
  return rd_data;
}

// write 1 byte data to DIO pin serially, with clock signal
// serial data = LSB first
void  TM1638::tm1638_wr_data(unsigned char data)
{
  unsigned char   wr_data;
  int             i;

  wr_data = data;
  
  for (i=0; i<8; i++)
  {
    if (wr_data & 0x01)
      digitalWrite(dio, HIGH);
    else
      digitalWrite(dio, LOW);

    digitalWrite(clk, LOW);
    delayMicroseconds(1);
    digitalWrite(clk, HIGH);
    delayMicroseconds(1);

    wr_data >>= 1;
  }
}
// write 16 characters to TM1638
void  TM1638::tm1638_wr_str_16()
{
  int   i;
  
  // ... set data CMD write
  digitalWrite(stb, LOW);
  delayMicroseconds(1);
  tm1638_wr_data(TM1638_SET_DATA_INC_WR);              // set data cmd
  //delayMicroseconds(1);
  digitalWrite(stb, HIGH);
  delayMicroseconds(1);
  
  // ... set address CMD write
  digitalWrite(stb, LOW);
  delayMicroseconds(1);
  tm1638_wr_data(TM1638_SET_ADDR_0);                  // set address cmd
  //delayMicroseconds(1);

  // 16 byte data write
  for (i=0; i<16; i++)
  {
    tm1638_wr_data(disp_buf[i]);                    // write data
    //delayMicroseconds(1);
  }
  digitalWrite(stb, HIGH);
  delayMicroseconds(1);

  // ... set display CMD write
  digitalWrite(stb, LOW);
  delayMicroseconds(1);
  tm1638_wr_data(TM1638_SET_DISP_B14);                // set display cmd
  //delayMicroseconds(1);
  digitalWrite(stb, HIGH);
  //delayMicroseconds(1);
}