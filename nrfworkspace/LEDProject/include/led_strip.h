#ifndef LED_STRIP_H
#define LED_STRIP_H

typedef struct {
	uint8_t sync;
	uint8_t brightness;
	uint8_t count;
	uint8_t pixels[3*255];
} LEDStrip;

#endif
