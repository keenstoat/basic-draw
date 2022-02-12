/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_MAIN_H
#define __USER_MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
/* ---------------------------------------------------------------------------*/
#define DISPLAY_DEV_ADDRESS     0x78 //specific to the OLED display

#define GREEN_LED_PORT GPIOD
#define GREEN_LED_PIN  GPIO_PIN_12

#define ORANGE_LED_PORT GPIOD
#define ORANGE_LED_PIN  GPIO_PIN_13

#define RED_LED_PORT GPIOD
#define RED_LED_PIN GPIO_PIN_14

#define BLUE_LED_PORT GPIOD
#define BLUE_LED_PIN GPIO_PIN_15

#define PAINT_STATUS_INPUT_PORT GPIOA
#define PAINT_STATUS_INPUT_PIN GPIO_PIN_4

#define ERASE_STATUS_INPUT_PORT GPIOA
#define ERASE_STATUS_INPUT_PIN GPIO_PIN_5

#define ON GPIO_PIN_SET
#define OFF GPIO_PIN_RESET

#define FILL 1
#define CLEAR 0

#define ADC_CENTER 130
#define CENTER_THOLD 10

void user_main(void);

void moveCursor(void);






/* ---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __USER_MAIN_H */
