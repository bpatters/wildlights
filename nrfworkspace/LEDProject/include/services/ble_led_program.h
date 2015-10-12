#ifndef BLE_LED_STREAM_H
#define BLE_LED_STREAM_H
#include <stdint.h>
#include <ble.h>
#include "led_program.h"

//D4DCxxxx-83CD-4027-A5F3-D9D172A33130
#define BLE_LED_PROGRAM_BASE_UUID 	 {{0x30,0x31,0xA3,0x72,0xD1,0xD9,0xF3,0xA5,0x27,0x40,0xCD,0x83,0x00,0x00,0xDC,0xD4}}
#define BLE_LED_PROGRAM_SERVICE_UUID {{0x30,0x31,0xA3,0x72,0xD1,0xD9,0xF3,0xA5,0x27,0x40,0xCD,0x83,0x77,0x77,0xDC,0xD4}}

#define BLE_LED_PROGRAM_SERVICE 	0x7777
#define BLE_LED_STREAM_CHAR    		0x7778
#define BLE_LED_STREAM_SYNC_CHAR    0x7779
#define BLE_LED_PROGRAM_CHAR   		0x7780

typedef struct _ble_led_program_t ble_led_program_t;
typedef struct _led_program_packet_header_t led_program_packet_header_t;
typedef struct _ble_led_program_init_t ble_program_init_t;
typedef struct _ble_led_program_packet_t ble_led_program_packet_t;

/**
 * Called when the Server sends us a new operation to perform.
 * @param p_ble_led_program the packet program instance
 * @param p_data the data packet received, based upon the type parameter cast it to the correct packet type.
 */
uint32_t ble_led_program_init(struct _ble_led_program_t* p_led_program, const struct _ble_led_program_init_t* p_led_program_init);
void ble_led_program_on_ble_evt(struct _ble_led_program_t* p_led_program, ble_evt_t* p_ble_evt);

/*** BLE init information ***/
typedef struct _ble_led_program_init_t {
	ble_srv_cccd_security_mode_t    led_program_char_attr_md;     	/**< Initial security level for led program characteristics attribute */
	ble_srv_cccd_security_mode_t    led_stream_char_attr_md;     	/**< Initial security level for led stream characteristics attribute */
	ble_srv_cccd_security_mode_t    led_stream_sync_char_attr_md;   /**< Initial security level for led stream characteristics attribute */
	LEDProgram                      *led_program; // led_program pointer
} ble_led_program_init_t;


/*
 * Packet allowing you to run a single instruction
 * against the specified strip length and brightness level.
 */
typedef struct _ble_led_instruction_packet_t {
	uint8_t pixel_count;
	uint8_t brightness;
	uint8_t inst[18];
} ble_led_instruction_packet_t;


typedef struct _ble_led_program_packet_t {
	uint8_t offsetHigh;
	uint8_t offsetLow;
	uint8_t data[18];
} ble_led_program_packet_t;

typedef struct _ble_led_program_t {
	uint16_t						service_handle;
	ble_gatts_char_handles_t		program_handles;
	ble_gatts_char_handles_t		stream_handles;
	ble_gatts_char_handles_t		sync_handles;
	uint8_t							uuid_type;
	uint16_t						conn_handle;
	LEDProgram                      *led_program;  // pixel strip to write to
	LEDStrip                        *led_strip;
	ble_led_program_packet_t         stream_buffer;
	ble_led_program_packet_t         program_buffer;
} ble_led_program_t;
/*******************/

#endif
