#include "user_main.h"
#include "main.h"
#include "ssd1306_lib.h"

// handles
extern ADC_HandleTypeDef hadc1;
extern I2C_HandleTypeDef hi2c2;
extern TIM_HandleTypeDef htim2;

Display oled(&hi2c2, DISPLAY_DEV_ADDRESS);

int step = 3;
int posX = 60;
int posY = 28;
int adcValY = 0;
int adcValX = 0;
uint32_t adcValueArray[2] = {0 ,0};

void user_main(void){



  if(HAL_I2C_IsDeviceReady(&hi2c2, DISPLAY_DEV_ADDRESS, 10, 10) == HAL_OK){
    HAL_GPIO_WritePin(DISPLAY_STATUS_LED_PORT, DISPLAY_STATUS_LED_PIN, GPIO_PIN_SET);
  }
  oled.init();
  oled.cleanAll();
//  oled.clear();
//  oled.reDraw();

  HAL_TIM_Base_Start_IT(&htim2);
  if(HAL_ADC_Start_DMA(&hadc1, adcValueArray, 2) != HAL_OK) {
    Error_Handler();
  }

  oled.drawBlock(posX, posY, FILL);

  while(1) {

    // toggles ORANGE LED
//    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);

    calculateBallXY();

//    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_Delay(1);

  }
}


void calculateBallXY(void) {

  // joystick values:
  // Y: [206, 129, 45]
  // X: [206, 129, 45]

  adcValY = adcValueArray[0];
  adcValX = adcValueArray[1];

  oled.drawBlock(posX, posY, CLEAR);

  if(adcValX < ADC_CENTER - 30) {
    posX -= step;
    posX = posX < 0 ? 0 : posX;
  } else if (adcValX > ADC_CENTER + 10) {
    posX += step;
    posX = posX > 120 ? 120 : posX;
  }

  // In display, Y coordinates increase down
  // so if ADC Y value goes up, the ball should go down
  if(adcValY > ADC_CENTER + 10  ) {
    posY -= step;
    posY = posY < 0 ? 0 : posY;
  } else if (adcValY < ADC_CENTER - 30) {
    posY += step;
    posY = posY > 56 ? 56 : posY;
  }
  oled.drawBlock(posX, posY, 1);
  printf("%d - %d\n", posX, posY);

}







void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

  // toggles RED LED
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);

  oled.reDraw();

  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);





//  printf("POS %d\n", pos);
}




