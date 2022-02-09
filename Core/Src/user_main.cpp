#include "user_main.h"
#include "main.h"
#include "ssd1306_lib.h"

extern ADC_HandleTypeDef hadc1;

extern I2C_HandleTypeDef hi2c2;

extern TIM_HandleTypeDef htim2;

Display oled(&hi2c2, DISPLAY_DEV_ADDRESS);
//int index = 0;
int offset = 1;
int pos = 64;
int adcVal = 0;


void user_main(void){



  if(HAL_I2C_IsDeviceReady(&hi2c2, DISPLAY_DEV_ADDRESS, 10, 10) == HAL_OK){
    HAL_GPIO_WritePin(DISPLAY_STATUS_LED_PORT, DISPLAY_STATUS_LED_PIN, GPIO_PIN_SET);
  }
  oled.init();
  oled.cleanAll();
//  oled.clear();
//  oled.reDraw();

  HAL_TIM_Base_Start_IT(&htim2);

  HAL_ADC_Start(&hadc1);
  // joystick values: [206, 43]
  // mid values: ~129


  oled.drawBlock(pos, 1);

  while(1) {

    adcVal = HAL_ADC_GetValue(&hadc1);

    oled.drawBlock(pos, 0);
    if(adcVal < 100) {
      pos -= offset;
      pos = pos < 0 ? 0 : pos;
    } else if (adcVal > 138) {
      pos += offset;
      pos = pos > 120 ? 120 : pos;
    }
    oled.drawBlock(pos, 1);

//    printf("ADC:: %d\n", adcVal);

    HAL_Delay(1);

  }
}





void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//  adcVal = HAL_ADC_GetValue(&hadc1);
//
//  oled.drawBlock(pos, 0);
//  if(adcVal < 100) {
//    pos -= offset;
//    pos = pos < 0 ? 0 : pos;
//  } else if (adcVal > 138) {
//    pos += offset;
//    pos = pos > 120 ? 120 : pos;
//  }
//  oled.drawBlock(pos, 1);




  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
  oled.reDraw();
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);

//  printf("POS %d\n", pos);
}




