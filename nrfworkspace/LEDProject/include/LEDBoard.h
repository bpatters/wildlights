#define LEDS_NUMBER    0

#define LEDS_MASK      (0)
// Defines which LEDs are lit when the signal is low. In this example,
// all LEDs are lit.
#define LEDS_INV_MASK  LEDS_MASK
// Defines the user buttons. In this example, there are no user buttons.
#define BUTTONS_NUMBER 0
#define BUTTONS_MASK   0x00000000
// Defines the UART connection with J-Link.
#define RX_PIN_NUMBER  11
#define TX_PIN_NUMBER  9
#define CTS_PIN_NUMBER 10
#define RTS_PIN_NUMBER 8
#define HWFC           false
