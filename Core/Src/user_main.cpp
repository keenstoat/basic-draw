#include "user_main.h"
#include "main.h"
#include "ssd1306_lib.h"

//operational module handles
extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c2;
extern TIM_HandleTypeDef htim2;

// OLED display object
Display oled(&hi2c2, DISPLAY_DEV_ADDRESS);

int posX = 0;
int posY = 0;
int adcValY = 0;
int adcValX = 0;
uint8_t adcValueArray[2] = { 0, 0 };

void user_main(void) {

  // IF display is ready - LED ORANGE ON
  if (HAL_I2C_IsDeviceReady(&hi2c2, DISPLAY_DEV_ADDRESS, 10, 10) == HAL_OK) {
    HAL_GPIO_WritePin(ORANGE_LED_PORT, ORANGE_LED_PIN, ON);
  }

  // IF ADC is started ok - LED GREEN ON
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adcValueArray, 2) == HAL_OK) {
    HAL_GPIO_WritePin(GREEN_LED_PORT, GREEN_LED_PIN, ON);
  }

  // start the screen refresh cycle
  HAL_TIM_Base_Start_IT(&htim2);

  // initialize screen
  oled.init();
  oled.clear();
  oled.reDraw();
  oled.drawCursor(posX, posY, FILL);

  while (1) {

    moveCursor();
    HAL_Delay(1);

  }
}

uint8_t getVelocity(int8_t adcValue) {

  // get abs()
  adcValue = adcValue < 0 ? -adcValue : adcValue;
  if (adcValue >= 80) {
    return 7;
  } else if (adcValue >= 60) {
    return 4;
  } else if (adcValue >= 30) {
    return 2;
  } else {
    return 1;
  }
}

void moveCursor(void) {

  // joystick value range:
  // large - Y and X: [00, 130, 255]

  // with offset correction (substract ADC_CENTER):
  // Y and X: [-130, 0, 125]

  // normalize the ADC value to take 0 as center of joystick
  adcValY = adcValueArray[0] - ADC_CENTER;
  adcValX = adcValueArray[1] - ADC_CENTER;

  HAL_GPIO_WritePin(BLUE_LED_PORT, BLUE_LED_PIN, OFF);
  HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, OFF);
  // clear the current cursor if not painting
  if (HAL_GPIO_ReadPin(PAINT_STATUS_INPUT_PORT, PAINT_STATUS_INPUT_PIN)) {

    HAL_GPIO_WritePin(BLUE_LED_PORT, BLUE_LED_PIN, ON);
    oled.drawSolid(posX, posY, FILL);

  } else if (HAL_GPIO_ReadPin(ERASE_STATUS_INPUT_PORT, ERASE_STATUS_INPUT_PIN)) {

    HAL_GPIO_WritePin(RED_LED_PORT, RED_LED_PIN, ON);
    oled.drawSolid(posX, posY, CLEAR);

  }

  oled.drawCursor(posX, posY, CLEAR);

  // The joystick has X movement inverted
  // so calculation is inverted
  if (adcValX < -CENTER_THOLD) { // joystick moves right
    posX += getVelocity(adcValX);
    posX = posX > 120 ? 120 : posX;
  } else if (adcValX > CENTER_THOLD) { // joystick moves left
    posX -= getVelocity(adcValX);
    posX = posX < 0 ? 0 : posX;
  }

  // The joystick has Y movement normal but...
  // In display, Y coordinates increase down
  // so if ADC Y value goes up, the ball should go down
  if (adcValY > CENTER_THOLD) { // joystick moves up
    posY -= getVelocity(adcValY);
    posY = posY < 0 ? 0 : posY;
  } else if (adcValY < -CENTER_THOLD) {
    posY += getVelocity(adcValY);
    posY = posY > 56 ? 56 : posY;
  }
  oled.drawCursor(posX, posY, FILL);
//  printf("%d - %d\n", posX, posY);

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  oled.reDraw();
}

