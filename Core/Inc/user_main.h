/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_MAIN_H
#define __USER_MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
/* ---------------------------------------------------------------------------*/

#define DISPLAY_STATUS_LED_PORT GPIOD
#define DISPLAY_STATUS_LED_PIN  GPIO_PIN_12
#define DISPLAY_DEV_ADDRESS     0x78

#define ADC_MAX 206
#define ADC_CENTER 130
#define ADC_MIN 46

#define FILL 1
#define CLEAR 0

void user_main(void);

void calculateBallXY(void);






/* ---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __USER_MAIN_H */
