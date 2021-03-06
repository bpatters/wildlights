#include <stddef.h>
#include <ble_srv_common.h>
#include <nordic_common.h>
#include <ble_types.h>
#include <string.h>
#include "services/ble_led_program.h"
#include "LEDVM.h"

static void on_connect(ble_led_program_t * p_led_program, ble_evt_t * p_ble_evt) {
	p_led_program->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

static void on_disconnect(ble_led_program_t * p_led_program, ble_evt_t * p_ble_evt) {
	UNUSED_PARAMETER(p_ble_evt);
	p_led_program->conn_handle = BLE_CONN_HANDLE_INVALID;
}

static void on_write(ble_led_program_t * p_led_program, ble_evt_t * p_ble_evt) {
	ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

	if ( p_evt_write->handle == p_led_program->stream_handles.value_handle)
	{
		ble_led_instruction_packet_t *packet = (ble_led_instruction_packet_t *) p_evt_write->data;
		// save previous values
		uint8_t brightness = p_led_program->led_program->strips[0].brightness;
		uint8_t count = p_led_program->led_program->strips[0].count;
		uint16_t oldpc, pc = 0;

		// run instruction(s)
		p_led_program->led_program->strips[0].brightness = packet->brightness;
		p_led_program->led_program->strips[0].count = packet->pixel_count;
		p_led_program->led_program->strips[0].sync = false; // disable sync until done
		int8_t icount = 0;
		do {
			//limit instruction count to 10 for safety
			icount++;
			oldpc = pc;
			runSingleInstruction(p_led_program->led_program, &pc, packet->inst);
			// execute until we hit an INST_END and pc doesn't but limit to a max of 10 executions
		} while (oldpc != pc && icount < 10);

		// restore values
		p_led_program->led_program->strips[0].brightness = brightness;
		p_led_program->led_program->strips[0].count = count;
	} else if ( p_evt_write->handle == p_led_program->program_handles.value_handle) {
		ble_led_program_packet_t *packet = (ble_led_program_packet_t *) p_evt_write->data;
		uint16_t offset = (((uint16_t)packet->offsetHigh & 0xFF) <<8) + (packet->offsetLow & 0xFF);
		if ( (offset >= 0) &&
				(offset < sizeof(LEDProgram)) &&
				((offset+p_evt_write->len -2) < (sizeof(LEDProgram)) )
		){
			memcpy(((uint8_t *)p_led_program->led_program)+offset, packet->data, p_evt_write->len-2);
		}
	}
}

void ble_led_program_on_ble_evt(ble_led_program_t* p_led_program, ble_evt_t* p_ble_evt)
{
	switch (p_ble_evt->header.evt_id)
	{
	case BLE_GAP_EVT_CONNECTED:
		on_connect(p_led_program, p_ble_evt);
		break;

	case BLE_GAP_EVT_DISCONNECTED:
		on_disconnect(p_led_program, p_ble_evt);
		break;

	case BLE_GATTS_EVT_WRITE:
		on_write(p_led_program, p_ble_evt);
		break;
	default:
		break;
	}
}

/**@brief Function for adding the LED Sync Stte characteristic.
 *
 * @param[in]   p_led_program        LED Stream Service structure.
 * @param[in]   p_led_program_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t ble_led_stream_sync_state_char_add(ble_led_program_t * p_led_program, const ble_led_program_init_t * p_led_program_init)
{
	uint32_t            err_code;
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t    attr_char_value;
	ble_uuid_t          ble_uuid;
	ble_gatts_attr_md_t attr_md;

	// Add Battery Level characteristic
	memset(&cccd_md, 0, sizeof(cccd_md));

	// According to BAS_SPEC_V10, the read operation on cccd should be possible without
	// authentication.
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	cccd_md.write_perm= p_led_program_init->led_stream_sync_char_attr_md.cccd_write_perm;
	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.read  = 1;
	char_md.char_props.notify = 1;
	char_md.p_char_user_desc  = (uint8_t *)"Sync State";
	char_md.p_char_pf         = NULL;
	char_md.p_user_desc_md    = NULL;
	char_md.p_cccd_md         = &cccd_md;
	char_md.p_sccd_md         = NULL;

	ble_uuid.type = p_led_program->uuid_type;
	ble_uuid.uuid = BLE_LED_STREAM_SYNC_CHAR;

	memset(&attr_md, 0, sizeof(attr_md));

	attr_md.read_perm  = p_led_program_init->led_stream_sync_char_attr_md.read_perm;
	attr_md.write_perm  = p_led_program_init->led_stream_sync_char_attr_md.write_perm;
	attr_md.vloc       = BLE_GATTS_VLOC_USER;
	attr_md.rd_auth    = 0;
	attr_md.wr_auth    = 0;
	attr_md.vlen       = 0;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid       = &ble_uuid;
	attr_char_value.p_attr_md    = &attr_md;
	attr_char_value.init_len     = sizeof(uint8_t);
	attr_char_value.init_offs    = 0;
	attr_char_value.max_len      = sizeof(uint8_t);
	attr_char_value.p_value      = (uint8_t *)&p_led_program->led_program->strips[0].sync;


	err_code = sd_ble_gatts_characteristic_add(p_led_program->service_handle, &char_md,
			&attr_char_value,
			&p_led_program->sync_handles);
	if (err_code != NRF_SUCCESS)
	{
		return err_code;
	}

	return NRF_SUCCESS;
}

/**@brief Function for adding the LED Stream characteristic.
 *
 * @param[in]   p_led_program        LED Stream Service structure.
 * @param[in]   p_led_program_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t ble_led_stream_char_add(ble_led_program_t * p_led_program, const ble_led_program_init_t * p_led_program_init)
{
	uint32_t            err_code;
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t    attr_char_value;
	ble_uuid_t          ble_uuid;
	ble_gatts_attr_md_t attr_md;

	// Add Battery Level characteristic
	memset(&cccd_md, 0, sizeof(cccd_md));

	// According to BAS_SPEC_V10, the read operation on cccd should be possible without
	// authentication.
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	cccd_md.write_perm = p_led_program_init->led_stream_char_attr_md.cccd_write_perm;
	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.read = 0;
	char_md.char_props.write = 1;
	char_md.p_char_user_desc  = (uint8_t *)"";
	char_md.p_char_pf         = NULL;
	char_md.p_user_desc_md    = NULL;
	char_md.p_cccd_md         = &cccd_md;
	char_md.p_sccd_md         = NULL;

	ble_uuid.type = p_led_program->uuid_type;
	ble_uuid.uuid = BLE_LED_STREAM_CHAR;

	memset(&attr_md, 0, sizeof(attr_md));

	attr_md.read_perm = p_led_program_init->led_stream_char_attr_md.read_perm;
	attr_md.write_perm = p_led_program_init->led_stream_char_attr_md.write_perm;
	attr_md.vloc       = BLE_GATTS_VLOC_USER;
	attr_md.rd_auth    = 0;
	attr_md.wr_auth    = 0;
	attr_md.vlen       = 1;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid       = &ble_uuid;
	attr_char_value.p_attr_md    = &attr_md;
	attr_char_value.init_len     = sizeof(uint8_t);
	attr_char_value.init_offs    = 0;
	attr_char_value.max_len      = sizeof(ble_led_program_packet_t);
	attr_char_value.p_value      = (uint8_t *)&p_led_program->stream_buffer;


	err_code = sd_ble_gatts_characteristic_add(p_led_program->service_handle, &char_md,
			&attr_char_value,
			&p_led_program->stream_handles);
	if (err_code != NRF_SUCCESS)
	{
		return err_code;
	}

	return NRF_SUCCESS;
}

/**@brief Function for adding the LED Program characteristic.
 *
 * @param[in]   p_led_program        LED Program Service structure.
 * @param[in]   p_led_program_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t ble_led_program_char_add(ble_led_program_t * p_led_program, const ble_led_program_init_t * p_led_program_init)
{
	uint32_t            err_code;
	ble_gatts_char_md_t char_md;
	ble_gatts_attr_md_t cccd_md;
	ble_gatts_attr_t    attr_char_value;
	ble_uuid_t          ble_uuid;
	ble_gatts_attr_md_t attr_md;

	// Add Battery Level characteristic
	memset(&cccd_md, 0, sizeof(cccd_md));

	// According to BAS_SPEC_V10, the read operation on cccd should be possible without
	// authentication.
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	cccd_md.write_perm = p_led_program_init->led_stream_char_attr_md.cccd_write_perm;
	cccd_md.vloc = BLE_GATTS_VLOC_STACK;

	memset(&char_md, 0, sizeof(char_md));

	char_md.char_props.write = 1;
	char_md.char_props.read  = 0;
	char_md.p_char_user_desc  = (uint8_t *)"";
	char_md.p_char_pf         = NULL;
	char_md.p_user_desc_md    = NULL;
	char_md.p_cccd_md         = &cccd_md;
	char_md.p_sccd_md         = NULL;

	ble_uuid.type = p_led_program->uuid_type;
	ble_uuid.uuid = BLE_LED_PROGRAM_CHAR;

	memset(&attr_md, 0, sizeof(attr_md));

	attr_md.read_perm  = p_led_program_init->led_program_char_attr_md.read_perm;
	attr_md.write_perm = p_led_program_init->led_program_char_attr_md.write_perm;
	attr_md.vloc       = BLE_GATTS_VLOC_USER;
	attr_md.rd_auth    = 0;
	attr_md.wr_auth    = 0;
	attr_md.vlen       = 1;

	memset(&attr_char_value, 0, sizeof(attr_char_value));

	attr_char_value.p_uuid       = &ble_uuid;
	attr_char_value.p_attr_md    = &attr_md;
	attr_char_value.init_len     = sizeof(uint8_t);
	attr_char_value.init_offs    = 0;
	attr_char_value.max_len      = sizeof(ble_led_program_packet_t);
	attr_char_value.p_value      = (uint8_t *)&p_led_program->program_buffer;


	err_code = sd_ble_gatts_characteristic_add(p_led_program->service_handle, &char_md,
			&attr_char_value,
			&p_led_program->program_handles);
	if (err_code != NRF_SUCCESS)
	{
		return err_code;
	}

	return NRF_SUCCESS;
}


uint32_t ble_led_program_init(ble_led_program_t * p_led_program, const ble_led_program_init_t * p_led_program_init)
{
	uint32_t   err_code;
	ble_uuid128_t base_uuid = BLE_LED_PROGRAM_BASE_UUID;

	err_code = sd_ble_uuid_vs_add(&base_uuid, &p_led_program->uuid_type);
	if (err_code != NRF_SUCCESS) {
		return err_code;
	}

	ble_uuid_t ble_uuid;
	ble_uuid.type = p_led_program->uuid_type;
	ble_uuid.uuid = BLE_LED_PROGRAM_SERVICE;

	// Initialize service structure
	p_led_program->led_program       = p_led_program_init->led_program;
	p_led_program->conn_handle       = BLE_CONN_HANDLE_INVALID;

	// Add service
	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_led_program->service_handle);
	if (err_code != NRF_SUCCESS)
	{
		return err_code;
	}

	// Add battery level characteristic
	err_code = ble_led_program_char_add(p_led_program, p_led_program_init);
	if (err_code != NRF_SUCCESS)
	{
		return err_code;
	}

	err_code = ble_led_stream_char_add(p_led_program, p_led_program_init);
	if (err_code != NRF_SUCCESS)
	{
		return err_code;
	}
	return ble_led_stream_sync_state_char_add(p_led_program, p_led_program_init);
}

