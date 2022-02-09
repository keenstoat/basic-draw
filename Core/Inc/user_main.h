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

void user_main(void);








/* ---------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* __USER_MAIN_H */
