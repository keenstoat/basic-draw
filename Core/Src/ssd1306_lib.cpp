/*
 * ssd1306_lib.c
 *
 *  Created on: Dec 8, 2021
 *      Author: charles
 */

#include "ssd1306_lib.h"


Display::Display(I2C_HandleTypeDef * hi2c1, uint16_t devAddress) {
  i2cHandle = hi2c1;
  deviceAddress = devAddress;
}

uint8_t Display::readData() {
  return this->screen[(this->page)*128 + this->col];
}

void Display::writeData(uint8_t data[], uint8_t size) {

  uint8_t buffer[size + 1];
  buffer[0] = DATA_BYTE;
  for(int i = 0; i < size; i++) {
    buffer[i + 1] = data[i];
    this->screen[(this->page)*128 + this->col + i] = data[i];
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


void Display::init() {

  this->writeCommand(SET_DISPLAY_OFF);
  this->writeCommand(SET_NORMAL_DISPLAY);
  this->writeCommand(SET_ENTIRE_DISPLAY_ON_RESUME_RAM);
  this->writeCommand(SET_MEMORY_ADDRESSING_MODE, HORIZONTAL_ADDRESSING_MODE);
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



void Display::cleanAll() {
  setCoordinate(0, 0);
  uint8_t buffer[1] = {0x00};
  for(uint16_t i = 0; i < 1024; i++) {
    writeData(buffer, 1);
    this->screen[i] = 0x00;

  }
}

void Display::setCoordinate(int page, int col) {
  this->page = page;
  this->col = col;
  this->writeCommand(SET_COLUMN_ADDRESS, col, 127);
  this->writeCommand(SET_PAGE_ADDRESS, page, 7);
}

void Display::point(int x0, int y0) {

  uint8_t page = y0 / 8;
  this->setCoordinate(page, x0);
  uint8_t byteToRead = this->readData();

  uint8_t byteToWrite[1];
  byteToWrite[0] = (0x01 << (y0 % 8)) | byteToRead;

  this->writeData(byteToWrite, 1);

}
void Display::line(int x0, int y0, int x1, int y1) {

  int dx  = x1 - x0,
      dy  = y1 - y0,
      y   = y0,
      D = 2*dy - dx;

  for ( int x = x0; x <= x1; x++ )  {
    this->point(x, y);
    if ( D > 0 )  {
      y++;
      D = D - 2*dx;
    }
    D = D + 2*dy;
  }









//  this->point(x1, y1);
//
//  int m = (y1 - y0)/(x1 - x0);
//  signed int b = y0 - m*x0;
//
//
//  int yLeft = y0;
//  for(uint8_t x = x0; x < x1; x++) {
//
//    if(m < 1) {
//      this->point(x,  m*x + b);
//
//    } else {
//
//      const int yRight = m*(x + 1) + b;
//
//      int midPixels = yRight - yLeft - 1;
//
//      int midPixelsLeft = midPixels - midPixels/2;
//      int midPixelsRight = midPixels - midPixelsLeft;
//
//      this->point(x, yLeft);
//      for(int fill = 1; fill <= midPixelsLeft; fill++) {
//        yLeft += 1;
//        this->point(x, yLeft);
//      }
//      for(int fill = 1; fill <= midPixelsRight; fill++) {
//        yLeft += 1;
//        this->point(x + 1, yLeft);
//      }
//
//      yLeft = yRight;
//    }
//
//  }
}

void Display::text(std::string s, int page, int col) {

  this->setCoordinate(page, col);
  uint8_t charIndex = 0;
  for(int i = 0U; i < ((int) s.size()); i++) {
    charIndex = s[i] - 0x30;
    this->writeData(this->numberChars[charIndex], NUMBER_CHAR_SIZE);
  }


}


