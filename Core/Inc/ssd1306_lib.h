/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SSD1306_LIB_H
#define __SSD1306_LIB_H

//#ifdef __cplusplus
// extern "C" {
//#endif
/*----------------------------------------------------------------------------*/


#include "stm32f4xx_hal.h"
#include <string>

// SSD1306 characteristics and commands definitions

#define COLS 128
#define ROWS 64
#define PAGES 8



/* =========== CONTROL BYTE =============================
 * ======================================================
 *
 * A control byte defines if the next bytes are commands or data.
 * It consists of Co and D/C# bits following by six "0" as [Co, D/C, 0, 0, 0, 0, 0, 0] (MSB -> LSB)
 *
 * - Co: if 0 the transmission of the following information will contain data bytes only
 * - D/C#:
 *      if 0: the next byte is a command.
 *      if 1: the next byte is data to be written to GDDRAM. The GDDRAM column address pointer will be increased by one
 *      automatically after each data write.
 */

#define COMMAND_BYTE 0x00
#define DATA_BYTE 0x40

/* =========== FUNDAMENTAL COMMANDS =====================
 * ======================================================
 */
/* SET_CONTRAST_CONTROL
 * This is a two byte command.
 * Send command data as:
 * data[1] = SET_CONTRAST_CONTROL
 * data[2] = <contrast value. Range: 0 - 255d >
*/
#define SET_CONTRAST_CONTROL 0x81
#define DEFAULT_CONTRAST_VALUE 0x7F //Value set on reset (RESET)

// Sets display pixels as defined in RAM content (RESET)
#define SET_ENTIRE_DISPLAY_ON_RESUME_RAM 0xA4

// Sets all pixels ON in the display. RAM is not modified.
#define SET_ENTIRE_DISPLAY_ON_IGNORE_RAM 0xA5

#define SET_NORMAL_DISPLAY 0xA6   // 0 in RAM = pixel OFF (RESET)
#define SET_INVERSE_DISPLAY 0XA7  // 0 in RAM = pixel ON


#define SET_DISPLAY_OFF 0xAE // This is sleep mode (RESET)
#define SET_DISPLAY_ON 0xAF


/* =========== ADDRESS SETTING COMMANDS =====================
 * ==========================================================
 */

/* SET_MEMORY_ADDRESSING_MODE ------
 * This is a two byte command. Send data as:
 * data = [IS_COMMAND_BYTE, SET_MEMORY_ADDRESSING_MODE, <horizontal, vertical, page> ]
 */
#define SET_MEMORY_ADDRESSING_MODE 0x20
#define HORIZONTAL_ADDRESSING_MODE 0x00
#define VERTICAL_ADDRESSING_MODE 0x01
#define PAGE_ADDRESSING_MODE 0x10

/* SET_COLUMN_ADDRESS ------------
 * This is a three byte command. Send command data as:
 * data[1] = SET_COLUMN_ADDRESS
 * data[2] = <column start address. Range: 0 - 127d >
 * data[3] = <column end address. Range: 0 - 127d >
 * NOTE: This command is only for horizontal or vertical addressing mode.
 */
#define SET_COLUMN_ADDRESS 0x21

/* SET_PAGE_ADDRESS ------------
 * This is a three byte command. Send command data as:
 * data[1] = SET_PAGE_ADDRESS
 * data[2] = <page start address. Range: 0 - 7d >
 * data[3] = <page end address. Range: 0 - 7d >
 * NOTE: This command is only for horizontal or vertical addressing mode
 */
#define SET_PAGE_ADDRESS 0x22



/* SET_PAGE_START_ADDRESS_x ------------
 * This is a different command for each page option.
 * NOTE: This command is only for page addressing mode
 */
#define SET_PAGE_START_ADDRESS_0 0xB0
#define SET_PAGE_START_ADDRESS_1 0xB1
#define SET_PAGE_START_ADDRESS_2 0xB2
#define SET_PAGE_START_ADDRESS_3 0xB3
#define SET_PAGE_START_ADDRESS_4 0xB4
#define SET_PAGE_START_ADDRESS_5 0xB5
#define SET_PAGE_START_ADDRESS_6 0xB6
#define SET_PAGE_START_ADDRESS_7 0xB7



/* =========== HARDWARE CONFIGURATION COMMANDS =====================
 * ==========================================================
 */

/* SET_DISPLAY_START_LINE_x ------------
 * This is a different command for each line to start.
 * If start line == 0: RAM line 0 == COM0
 * If start line == 1: RAM line 1 == COM0
 * And so on, basically shifts the display upwards
 */
#define SET_DISPLAY_START_LINE_0 0x40
#define SET_DISPLAY_START_LINE_1 0x40


/* Segment is a vertical line in the display. Segment lines go from 0 to 127
 * */
#define SET_SEGMENT_ORDER_DEFAULT 0xA0 // segment 0 is mapped to column 0
#define SET_SEGMENT_ORDER_INVERSE 0xA1 // segment 0 is mapped to column 127

/* COM is a horizontal line in the display. COM lines go from 0 to 63
 * */
#define SET_COM_ORDER_DEFAULT 0xC0 // COM 0 is mapped to row 0
#define SET_COM_ORDER_INVERSE 0xC8 // COM 0 is mapped to row 63

/* The charge pump must be enabled before display is on (SET_DISPLAY_ON)
 * */
#define SET_CHARGE_PUMP_SETTING 0x8D
#define DISABLE_CHARGE_PUMP 0x10
#define ENABLE_CHARGE_PUMP 0x14




/* Sets the display internal clock prescaler and frequency.
 * the prescaler is mentioned as divide ration in the DS.
 * This is a two byte command. Send command data as:
 * data[1] = SET_CLOCK_FREQUENCY
 * data[2] = FREQUENCY_DIVIDE_RATIO
 */
#define SET_CLOCK_FREQUENCY 0xD5
#define FREQUENCY_DIVIDE_RATIO 0xF0

/* =========== UTILITY FUNCTIONS =====================
 * ==========================================================
 */

class Display {

  private:
    I2C_HandleTypeDef * i2cHandle;
    uint16_t deviceAddress;
    uint8_t screen[1024];
    uint8_t cscreen[1024];
    uint8_t page = 0;
    uint8_t col = 0;

#define NUMBER_CHAR_SIZE 6
    const uint8_t numberChars[11][NUMBER_CHAR_SIZE] = {
      //{0x0, 0x3E, 0x41, 0x41, 0x3E, 0x0}, // 0
      {0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0}, // 0
      {0x0, 0x00, 0x41, 0x7F, 0x40, 0x0}, // 1
      {0x0, 0x62, 0x51, 0x49, 0x46, 0x0}, // 2
      {0x0, 0x22, 0x41, 0x49, 0x36, 0x0}, // 3
      {0x0, 0x0F, 0x08, 0x08, 0x7F, 0x0}, // 4
      {0x0, 0x47, 0x45, 0x45, 0x39, 0x0}, // 5
      {0x0, 0x3E, 0x49, 0x49, 0x32, 0x0}, // 6
      {0x0, 0x03, 0x71, 0x09, 0x07, 0x0}, // 7
      {0x0, 0x36, 0x49, 0x49, 0x36, 0x0}, // 8
      {0x0, 0x26, 0x49, 0x49, 0x3E, 0x0},  // 9
      {0x0, 0x00, 0x63, 0x63, 0x00, 0x0}  // :
    };

    const uint8_t font2[3][6] = {
      {0x70, 0x1E, 0x11, 0x1E, 0x70, 0x0}, // A
      {0x7F, 0x49, 0x49, 0x49, 0x36, 0x0}, // B
      {0x3E, 0x41, 0x41, 0x41, 0x22, 0x0} // C
    };


  public:
    Display(I2C_HandleTypeDef *, uint16_t);

    void writeData(uint8_t [], uint16_t);
    void writeCommand(uint8_t);
    void writeCommand(uint8_t, uint8_t);
    void writeCommand(uint8_t, uint8_t, uint8_t);

    void init(void);
    void setCoordinate(int, int);
    void reDraw(void);

    void clear(void);
    void fill(void);

    void drawBlock(int, int, int, uint8_t [], uint8_t []);
    void drawSolid(int, int, int);
    void drawCursor(int, int, int);

//    void point(int, int);
//    void line(int, int, int, int);
//    void text(std::string, int, int);
};





/* ---------------------------------------------------------------------------*/
//#ifdef __cplusplus
//}
//#endif

#endif /* __SSD1306_LIB_H */

