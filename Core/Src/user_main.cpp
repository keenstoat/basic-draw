#include "user_main.h"
#include "main.h"
#include "ssd1306_lib.h"

// handles
extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c2;
extern TIM_HandleTypeDef htim2;

Display oled(&hi2c2, DISPLAY_DEV_ADDRESS);

int posX = 0;
int posY = 0;
int adcValY = 0;
int adcValX = 0;
uint8_t adcValueArray[2] = {0 ,0};
int cursorMode = NONE;


void user_main(void){

  // IF display is ready for I2C - LED GREEN ON
  if(HAL_I2C_IsDeviceReady(&hi2c2, DISPLAY_DEV_ADDRESS, 10, 10) == HAL_OK){
    HAL_GPIO_WritePin(DISPLAY_STATUS_LED_PORT, DISPLAY_STATUS_LED_PIN, GPIO_PIN_SET);
  }
  oled.init();
  oled.clear();
  oled.reDraw();

  HAL_TIM_Base_Start_IT(&htim2);
  // IF ADC is started ok - LED BLUE ON
  if(HAL_ADC_Start_DMA(&hadc1, (uint32_t *) adcValueArray, 2) == HAL_OK) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
  }

  oled.drawBlock(posX, posY, 0);

  while(1) {

    // toggles ORANGE LED
//    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);

    calculateBallPosition();

//    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_Delay(1);

  }
}

uint8_t getSteps(int adcValue) {

  // get abs()
  adcValue = adcValue < 0 ? -adcValue : adcValue;
  if(adcValue >= 80) {
    return 5;
  }else if(adcValue >= 60) {
    return 4;
  } else if(adcValue >= 40) {
    return 3;
  } else if(adcValue >= 10) {
    return 2;
  } else {
    return 1;
  }
}

void calculateBallPosition(void) {

#define ADC_CENTER 130
#define CENTER_THOLD 3

  // joystick value range:
  // small - Y and X: [45, 130, 206]
  // large - Y and X: [00, 130, 255]

  // with offset correction (substract ADC_CENTER):
  // Y and X: [-130, 0, 125]

//  printf("%d - %d\n", adcValueArray[1], adcValueArray[0]);

  // normalize the ADC value to take 0 as center of joystick
  adcValY = adcValueArray[0] - ADC_CENTER;
  adcValX = adcValueArray[1] - ADC_CENTER;
//  printf("%d - %d\n", adcValX, adcValY);

  if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
    oled.drawBlock(posX, posY, 0);
  } else {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
  }
  // The joystick has X movement inverted
  // so calculation is inverted
  if(adcValX < -CENTER_THOLD) { // joystick moves right
    posX += getSteps(adcValX);
    posX = posX > 120 ? 120 : posX;
  } else if (adcValX > CENTER_THOLD) { // joystick moves left
    posX -= getSteps(adcValX);
    posX = posX < 0 ? 0 : posX;
  }

  // The joystick has Y movement normal but...
  // In display, Y coordinates increase down
  // so if ADC Y value goes up, the ball should go down
  if(adcValY > CENTER_THOLD ) { // joystick moves up
    posY -= getSteps(adcValY);
    posY = posY < 0 ? 0 : posY;
  } else if (adcValY < -CENTER_THOLD) {
    posY += getSteps(adcValY);
    posY = posY > 56 ? 56 : posY;
  }
  oled.drawBlock(posX, posY, FILL);
//  printf("%d - %d\n", posX, posY);

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // toggles RED LED
//  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
  oled.reDraw();
//  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  // ORANGE LED
  HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
  cursorMode = cursorMode != NONE ? FILL : NONE;
}




