// #define TEST_MODE
// #define ENABLE_EEPROM_MEMORY

#define INPUT_ADDR 0x20
#define OUTPUT_ADDR 0x21
#define EEPROM_ADDR 0x50

#define N_STEP_INTERVALS 4
#define N_STOPS 7
#define N_BUTTONS 6
#define N_DIALS 3
#define N_MODES 3

#define ENLARGER_PIN 3
#define BUZZER_PIN 4
#define DISPLAY_CS_PIN 10

#define MAGIC_NUMBER 1989

#define STEP_INTERVALS {0.25, 0.33, 0.5, 1}
#define STOPS {-3, -2, -1, 0, 1, 2, 3}
#define MODE_PINS {14, 15, 0}
#define STEP_DELTA_PINS {8, 9, 10, 11}
#define PRINT_STOP_PINS {12, 13, 14, 15, 0, 1, 2}
#define DIAL_PINS {{9,8},{11,10},{13,12}}
#define BUTTON_PINS {1, 2, 3, 4, 5, 6}