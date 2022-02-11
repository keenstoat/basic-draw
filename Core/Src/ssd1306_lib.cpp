
#include "ssd1306_lib.h"


Display::Display(I2C_HandleTypeDef * hi2c1, uint16_t devAddress) {
  i2cHandle = hi2c1;
  deviceAddress = devAddress;
}

/* I2C I/O level functions
 * these handle the i/o through i2c
 */

#define BUFFER_SIZE 8
void Display::writeData(uint8_t data[], uint16_t size) {

//  uint8_t buffer[BUFFER_SIZE + 1];
//  buffer[0] = DATA_BYTE;
//  for(int i = 0; i < BUFFER_SIZE; i++) {
//    buffer[i + 1] = data[i];
//  }



  uint8_t buffer[size + 1];
  buffer[0] = DATA_BYTE;
  for(int i = 0; i < size; i++) {
    buffer[i + 1] = data[i];
  }
  HAL_I2C_Master_Transmit(i2cHandle, deviceAddress, buffer, size + 1, 10);
}

void Display::writeCommand(uint8_t commandName) {

  uint8_t buffer[2] = {COMMAND_BYTE, commandName};
  HAL_I2C_Master_Transmit(i2cHandle, deviceAddress, buffer, 2, 10);
}

void Display::writeCommand(uint8_t commandName, uint8_t data1) {

  uint8_t buffer[3] = {COMMAND_BYTE, commandName, data1};
  HAL_I2C_Master_Transmit(i2cHandle, deviceAddress, buffer, 3, 10);
}

void Display::writeCommand(uint8_t commandName, uint8_t data1, uint8_t data2) {

  uint8_t buffer[4] = {COMMAND_BYTE, commandName, data1, data2};
  HAL_I2C_Master_Transmit(i2cHandle, deviceAddress, buffer, 4, 10);
}


/* OLED Display level functions
 * these handle specific instructions to interact with the display
 * through it's instruction set
 * */

void Display::init() {

  this->writeCommand(SET_DISPLAY_OFF);
  this->writeCommand(SET_NORMAL_DISPLAY);
  this->writeCommand(SET_ENTIRE_DISPLAY_ON_RESUME_RAM);
  this->writeCommand(SET_MEMORY_ADDRESSING_MODE, HORIZONTAL_ADDRESSING_MODE);
  this->writeCommand(SET_CLOCK_FREQUENCY, FREQUENCY_DIVIDE_RATIO);
  this->writeCommand(SET_COLUMN_ADDRESS, 0, 127);
  this->writeCommand(SET_PAGE_ADDRESS, 0, 7);
  this->writeCommand(SET_DISPLAY_START_LINE_0);
  this->writeCommand(SET_SEGMENT_ORDER_INVERSE);
  this->writeCommand(SET_COM_ORDER_INVERSE);
  this->writeCommand(SET_CHARGE_PUMP_SETTING, ENABLE_CHARGE_PUMP);
  this->writeCommand(SET_DISPLAY_ON);
  HAL_Delay(100);
  this->writeCommand(SET_CONTRAST_CONTROL, DEFAULT_CONTRAST_VALUE);

}

void Display::setCoordinate(int page, int col) {
  this->page = page;
  this->col = col;
  this->writeCommand(SET_COLUMN_ADDRESS, col, 127);
  this->writeCommand(SET_PAGE_ADDRESS, page, 7);
}

void Display::reDraw() {
  setCoordinate(0, 0);
//  writeData(this->screen, 1024);

  uint8_t subArray[64];
  for(int page = 0; page < 16; page++) {
    for(int i = 0; i < 64; i++) {
        subArray[i] = this->screen[page*64 + i];
    }
    writeData(subArray, 64);
  }
}


/* Screen and draw functions
 * These require a call to reDraw() to be applied
 *
 * */

void Display::clear() {
  for(uint16_t i = 0; i < 1024; i++) {
    this->screen[i] = 0x00;
  }
}


void Display::fill() {
  for(uint16_t i = 0; i < 1024; i++) {
    this->screen[i] = 0xFF;
  }
}


void Display::drawBlock(int x, int y, int color) {

  uint8_t block[8] = {0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C};

  // limit coordinates to fit block in screen
  if(x < 0 || x > 120) {
    return;
  }
  if(y < 0 || y > 56) {
    return ;
  }

  int page = y / 8;
  int lowerPageBits = y % 8;
  int upperPageBits = 8 - lowerPageBits;

  // set the upper page bits
  int initPos = page * COLS + x;
  for(int i = 0; i < 8; i++) {
    this->screen[initPos++] = color > 0 ? block[i] << lowerPageBits : 0x00;
  }
  // set the lower page bits
  if(lowerPageBits > 0) {
    initPos = (page + 1) * COLS + x;
    for(int i = 0; i < 8; i++) {
      this->screen[initPos++] = color > 0 ? block[i] >> upperPageBits : 0x00;
    }
  }

}


//void Display::text(std::string s, int x, int y) {
//
//  uint8_t charIndex = 0;
//  for(int i = 0; i < ((int) s.size()); i++) {
//    charIndex = s[i] - 0x30;
//
//    this->writeData(this->numberChars[charIndex], NUMBER_CHAR_SIZE);
//  }
//}



//
void Display::cleanAll() {
  this->setCoordinate(0, 0);
  uint8_t buffer[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  for(uint16_t i = 0; i < 128; i++) {
    writeData(buffer, 8);
//    this->screen[i] = 0x00;
  }
}

//uint8_t Display::readData() {
//  return this->screen[(this->page)*128 + this->col];
//}

//void Display::writeData(uint8_t data[], uint8_t size) {
//
//  uint8_t buffer[size + 1];
//  buffer[0] = DATA_BYTE;
//  for(int i = 0; i < size; i++) {
//    buffer[i + 1] = data[i];
//    this->screen[(this->page)*128 + this->col + i] = data[i];
//  }
//  HAL_I2C_Master_Transmit(i2cHandle, deviceAddress, buffer, size + 1, 10);
//}

//void Display::text(std::string s, int page, int col) {
//
//  this->setCoordinate(page, col);
//  uint8_t charIndex = 0;
//  for(int i = 0U; i < ((int) s.size()); i++) {
//    charIndex = s[i] - 0x30;
//    this->writeData(this->numberChars[charIndex], NUMBER_CHAR_SIZE);
//  }
//}








