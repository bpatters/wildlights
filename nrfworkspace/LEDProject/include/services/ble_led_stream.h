#ifndef BLE_LED_STREAM_H
#define BLE_LED_STREAM_H
#include <stdint.h>
#include <ble.h>
#include "led_strip.h"

//D4DCxxxx-83CD-4027-A5F3-D9D172A33130
#define BLE_LED_STREAM_BASE_UUID {{0x30,0x31,0xA3,0x72,0xD1,0xD9,0xF3,0xA5,0x27,0x40,0xCD,0x83,0x00,0x00,0xDC,0xD4}}
#define BLE_LED_STREAM_SERVICE_UUID {{0x30,0x31,0xA3,0x72,0xD1,0xD9,0xF3,0xA5,0x27,0x40,0xCD,0x83,0x77,0x77,0xDC,0xD4}}

#define BLE_LED_STREAM_SERVICE 0x7777
#define BLE_LED_STREAM_CHAR    0x7778
#define BLE_LED_STREAM_READY_CHAR     0x7779

typedef struct _ble_led_stream_t ble_led_stream_t;
typedef struct _led_stream_packet_header_t led_stream_packet_header_t;
typedef struct _ble_led_stream_init_t ble_stream_init_t;
typedef struct _ble_led_stream_packet_t ble_led_stream_packet_t;

/**
 * Called when the Server sends us a new operation to perform.
 * @param p_ble_led_stream the packet stream instance
 * @param p_data the data packet received, based upon the type parameter cast it to the correct packet type.
 */
uint32_t ble_led_stream_init(struct _ble_led_stream_t* p_led_stream, const struct _ble_led_stream_init_t* p_led_stream_init);
void ble_led_stream_on_ble_evt(struct _ble_led_stream_t* p_led_stream, ble_evt_t* p_ble_evt);

/*** BLE init information ***/
typedef struct _ble_led_stream_init_t {
	ble_srv_cccd_security_mode_t    led_packet_char_attr_md;     /**< Initial security level for led packet characteristics attribute */
	uint8_t                         led_strip_size;
	LEDStrip                        *led_strip; // location of the pixel strip
} ble_led_stream_init_t;




typedef struct _ble_led_stream_packet_t {
	uint8_t offsetHigh;
	uint8_t offsetLow;
	uint8_t data[18];
} ble_led_stream_packet_t;

typedef struct _ble_led_stream_t {
	uint16_t						service_handle;
	ble_gatts_char_handles_t		stream_handles;
	ble_gatts_char_handles_t		sync_handles;
	uint8_t							uuid_type;
	uint16_t						conn_handle;
	uint8_t                         led_strip_size;
	LEDStrip                        *led_strip;  // pixel strip to write to
	ble_led_stream_packet_t         packet_buffer;
} ble_led_stream_t;
/*******************/

#endif
