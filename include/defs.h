#define TEST_MODE
// #define ENABLE_EEPROM_MEMORY

#define INPUT_ADDR 0x20
#define OUTPUT_ADDR 0x21
#define EEPROM_ADDR 0x50

#define N_STEP_INTERVALS 4
#define N_STOPS 7
#define N_BUTTONS 6
#define N_DIALS 3
#define N_MODES 3

#define STEP_DELTA_BASE_PIN 0
#define PRINT_STOP_BASE_PIN 5
#define ENLARGER_PIN 11
#define BUZZER_PIN 12
#define PRINT_STOP_LEDS 7
#define STEP_DELTA_LEDS 4
#define BUTTONS_BASE_PIN 10
#define DIALS_BASE_PIN 0
#define DISPLAY_CS_PIN 10

#define MAGIC_NUMBER 1989

#define STEP_INTERVALS {0.25, 0.33, 0.5, 1}
#define STOPS {-3, -2, -1, 0, 1, 2, 3}
#define MODE_PINS {7, 8, 9}