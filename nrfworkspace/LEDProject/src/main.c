/** @file
 *
 * @defgroup ble_sdk_app_hrs_eval_main main.c
 * @{
 * @ingroup ble_sdk_app_hrs_eval
 * @brief Main file for Heart Rate Service Sample Application for nRF51822 evaluation board
 *
 * This file contains the source code for a sample application using the Heart Rate service
 * (and also Battery and Device Information services) for the nRF51822 evaluation board (PCA10001).
 * This application uses the @ref ble_sdk_lib_conn_params module.
 */

#include <stdint.h>
#include <string.h>
#include "nordic_common.h"
#include "nrf.h"
#include "app_error.h"
#include "nrf51_bitfields.h"
#include "ble.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_bas.h"
#include "ble_dis.h"
#include "ble_conn_params.h"
#include "ble_eval_board_pins.h"
#include "ble_stack_handler.h"
#include "app_timer.h"
#include "nrf_gpio.h"
#include "battery.h"
#include "ble_bondmngr.h"
#include "app_gpiote.h"
#include "app_button.h"
#include "ble_radio_notification.h"
#include "ble_flash.h"
#include "ble_debug_assert_handler.h"

#include "services/ble_led_program.h"
#include "head_unit.h"
#include "global.h"
#include "light_ws2812_cortex.h"
#include "LEDVM.h"
#include "led_strip.h"
#include "buttons.h"

#define BONDMNGR_DELETE_BUTTON_PIN_NO        HR_DEC_BUTTON_PIN_NO                      /**< Button used for deleting all bonded masters during startup. */


#define BATTERY_LEVEL_MEAS_INTERVAL          APP_TIMER_TICKS(2000, APP_TIMER_PRESCALER)/**< Battery level measurement interval (ticks). */

#define APP_GPIOTE_MAX_USERS                 1                                         /**< Maximum number of users of the GPIOTE handler. */

#define BUTTON_DETECTION_DELAY               APP_TIMER_TICKS(50, APP_TIMER_PRESCALER)  /**< Delay from a GPIOTE event until a button is reported as pushed (in number of timer ticks). */

#define MIN_CONN_INTERVAL                    MSEC_TO_UNITS(20, UNIT_1_25_MS)           /**< Minimum acceptable connection interval (0.5 seconds). */
#define MAX_CONN_INTERVAL                    MSEC_TO_UNITS(50, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (1 second). */
#define SLAVE_LATENCY                        0                                         /**< Slave latency. */
#define CONN_SUP_TIMEOUT                     MSEC_TO_UNITS(4000, UNIT_10_MS)           /**< Connection supervisory timeout (4 seconds). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY       APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)/**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY        APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)/**< Time between each call to sd_ble_gap_conn_param_update after the first (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT         3                                         /**< Number of attempts before giving up the connection parameter negotiation. */

#define SEC_PARAM_TIMEOUT                    30                                        /**< Timeout for Pairing Request or Security Request (in seconds). */
#define SEC_PARAM_BOND                       1                                         /**< Perform bonding. */
#define SEC_PARAM_MITM                       0                                         /**< Man In The Middle protection not required. */
#define SEC_PARAM_IO_CAPABILITIES            BLE_GAP_IO_CAPS_NONE                      /**< No I/O capabilities. */
#define SEC_PARAM_OOB                        1                                         /**< Out Of Band data available. */
#define SEC_PARAM_MIN_KEY_SIZE               7                                         /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE               16                                        /**< Maximum encryption key size. */

#define FLASH_PAGE_SYS_ATTR                  (BLE_FLASH_PAGE_END - 3)                  /**< Flash page used for bond manager system attribute information. */
#define FLASH_PAGE_BOND                      (BLE_FLASH_PAGE_END - 1)                  /**< Flash page used for bond manager bonding information. */

#define DEAD_BEEF                            0xDEADBEEF                                /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

static ble_gap_sec_params_t m_sec_params;
/**< Security requirements for this application. */
static ble_gap_adv_params_t m_adv_params;
/**< Parameters to be passed to the stack when starting advertising. */
ble_bas_t bas;
/**< Structure used to identify the battery service. */
ble_led_program_t led_program;
static app_timer_id_t m_battery_timer_id;




static void ble_evt_dispatch(ble_evt_t *p_ble_evt);

static void update_led_strips(bool radioActive);

/*****************************************************************************
 * Error Handling Functions
 *****************************************************************************/


/**@brief Function for error handling, which is called when an error has occurred. 
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze 
 *          how your product is supposed to react in case of error.
 *
 * @param[in] error_code  Error code supplied to the handler.
 * @param[in] line_num    Line number where the handler is called.
 * @param[in] p_file_name Pointer to the file name. 
 */
void app_error_handler(uint32_t error_code, uint32_t line_num, const uint8_t *p_file_name) {
    // This call can be used for debug purposes during development of an application.
    // @note CAUTION: Activating this code will write the stack to flash on an error.
    //                This function should NOT be used in a final product.
    //                It is intended STRICTLY for development/debugging purposes.
    //                The flash write will happen EVEN if the radio is active, thus interrupting
    //                any communication.
    //                Use with care. Un-comment the line below to use.
    // ble_debug_assert_handler(error_code, line_num, p_file_name);

    // On assert, the system can only recover with a reset.
    NVIC_SystemReset();
}


/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze 
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name) {
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error) {
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for handling a Bond Manager error.
 *
 * @param[in]   nrf_error   Error code containing information about what went wrong.
 */
static void bond_manager_error_handler(uint32_t nrf_error) {
    APP_ERROR_HANDLER(nrf_error);
}


/*****************************************************************************
 * Static Timeout Handling Functions
 *****************************************************************************/

/**@brief Function for handling the Battery measurement timer timeout.
 *
 * @details This function will be called each time the battery level measurement timer expires.
 *          This function will start the ADC.
 *
 * @param[in]   p_context   Pointer used for passing some arbitrary information (context) from the
 *                          app_start_timer() call to the timeout handler.
 */
static void battery_level_meas_timeout_handler(void *p_context) {
    UNUSED_PARAMETER(p_context);
    battery_start();
}

/*****************************************************************************
 * Static Initialization Functions
 *****************************************************************************/

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
static void timers_init(void) {
    uint32_t err_code;

    // Initialize timer module
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, false);

    // Create timers
    err_code = app_timer_create(&m_battery_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                battery_level_meas_timeout_handler);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for the GAP initialization.
 *
 * @details This function shall be used to setup all the necessary GAP (Generic Access Profile)
 *          parameters of the device. It also sets the permissions and appearance.
 */
static void gap_params_init(void) {
    uint32_t err_code;
    ble_gap_conn_params_t gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode, (const uint8_t *) DEVICE_NAME, strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 */
static void advertising_init(void) {
    uint32_t err_code;
    ble_advdata_t advdata;
    uint8_t flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    ble_uuid_t adv_uuids[] =
            {
                    {BLE_LED_PROGRAM_SERVICE, BLE_UUID_TYPE_BLE},
            };

    // Build and set advertising data
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = true;
    advdata.flags.size = sizeof(flags);
    advdata.flags.p_data = &flags;
    advdata.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    advdata.uuids_complete.p_uuids = adv_uuids;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);

    // Initialize advertising parameters (used when starting advertising)
    memset(&m_adv_params, 0, sizeof(m_adv_params));

    m_adv_params.type = BLE_GAP_ADV_TYPE_ADV_IND;
    m_adv_params.p_peer_addr = NULL;                           // Undirected advertisement
    m_adv_params.fp = BLE_GAP_ADV_FP_ANY;
    m_adv_params.interval = APP_ADV_INTERVAL;
    m_adv_params.timeout = APP_ADV_TIMEOUT_IN_SECONDS;
}


/**@brief Function for initializing the services that will be used by the application.
 *
 * @details Initialize the Heart Rate, Battery and Device Information services.
 */
static void services_init(void) {
    uint32_t err_code;
    ble_bas_init_t bas_init;
    ble_dis_init_t dis_init;
    ble_led_program_init_t led_program_init;

    // Initialize Battery Service
    memset(&bas_init, 0, sizeof(bas_init));

    // Here the sec level for the Battery Service can be changed/increased.
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&bas_init.battery_level_char_attr_md.write_perm);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init.battery_level_report_read_perm);

    bas_init.evt_handler = NULL;
    bas_init.support_notification = true;
    bas_init.p_report_ref = NULL;
    bas_init.initial_batt_level = 100;

    err_code = ble_bas_init(&bas, &bas_init);
    APP_ERROR_CHECK(err_code);

    // Initialize Device Information Service
    memset(&dis_init, 0, sizeof(dis_init));

    ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, MANUFACTURER_NAME);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dis_init.dis_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&dis_init.dis_attr_md.write_perm);

    err_code = ble_dis_init(&dis_init);
    APP_ERROR_CHECK(err_code);


    // Initialize LED Streaming service
    memset(&led_program, 0, sizeof(led_program));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&led_program_init.led_program_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&led_program_init.led_program_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&led_program_init.led_program_char_attr_md.write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&led_program_init.led_stream_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&led_program_init.led_stream_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&led_program_init.led_stream_char_attr_md.write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&led_program_init.led_stream_sync_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&led_program_init.led_stream_sync_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&led_program_init.led_stream_sync_char_attr_md.write_perm);
    led_program_init.led_program = &head_unit.program;

    err_code = ble_led_program_init(&led_program, &led_program_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the security parameters.
 */
static void sec_params_init(void) {
    m_sec_params.timeout = SEC_PARAM_TIMEOUT;
    m_sec_params.bond = SEC_PARAM_BOND;
    m_sec_params.mitm = SEC_PARAM_MITM;
    m_sec_params.io_caps = SEC_PARAM_IO_CAPABILITIES;
    m_sec_params.oob = SEC_PARAM_OOB;
    m_sec_params.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
    m_sec_params.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
}


/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void) {
    uint32_t err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.disconnect_on_fail = true;
    cp_init.evt_handler = NULL;
    cp_init.error_handler = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for the Bond Manager initialization.
 */
static void bond_manager_init(void) {
    uint32_t err_code;
    ble_bondmngr_init_t bond_init_data;


    // Initialize the Bond Manager
    bond_init_data.flash_page_num_bond = FLASH_PAGE_BOND;
    bond_init_data.flash_page_num_sys_attr = FLASH_PAGE_SYS_ATTR;
    bond_init_data.evt_handler = NULL;
    bond_init_data.error_handler = bond_manager_error_handler;
    bond_init_data.bonds_delete = false;

    err_code = ble_bondmngr_init(&bond_init_data);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Radio Notification event.
 */
static void radio_notification_init(void) {
    uint32_t err_code;

    err_code = ble_radio_notification_init(NRF_APP_PRIORITY_HIGH,
                                           NRF_RADIO_NOTIFICATION_DISTANCE_4560US,
                                           update_led_strips);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void) {
    BLE_STACK_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_SYNTH_250_PPM,
                           128,
                           ble_evt_dispatch,
                           false);
}


/*****************************************************************************
 * Static Start Functions
 *****************************************************************************/

/**@brief Function for starting the application timers.
 */
static void application_timers_start(void) {
    uint32_t err_code;

    // Start application timers
    err_code = app_timer_start(m_battery_timer_id, BATTERY_LEVEL_MEAS_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for starting advertising.
 */
static void advertising_start(void) {
    uint32_t err_code;

    err_code = sd_ble_gap_adv_start(&m_adv_params);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for putting the chip in System OFF Mode
 */
static void system_off_mode_enter(void) {
    uint32_t err_code;

    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
}


/*****************************************************************************
 * Static Event Handling Functions
 *****************************************************************************/

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
static void on_ble_evt(ble_evt_t *p_ble_evt) {
    uint32_t err_code = NRF_SUCCESS;
    static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;

    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:

            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

            if ((head_unit.current_mode == MODE_RUN_ON_DISCONNECT) && (head_unit.program.state == STATE_RUNNING)) {
                head_unit_set_program_idle(&head_unit);
            }
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            // Since we are not in a connection and have not started advertising, store bonds
            err_code = ble_bondmngr_bonded_masters_store();
            APP_ERROR_CHECK(err_code);

            // on disconnect restart advertising
            //	system_off_mode_enter();
            advertising_start();
            // if we get disconnected check transition state
            if (head_unit.current_mode == MODE_RUN_ON_DISCONNECT) {
                head_unit_set_program_idle(&head_unit);
            }
            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            err_code = sd_ble_gap_sec_params_reply(m_conn_handle,
                                                   BLE_GAP_SEC_STATUS_SUCCESS,
                                                   &m_sec_params);
            break;

        case BLE_GAP_EVT_TIMEOUT:
            if (p_ble_evt->evt.gap_evt.params.timeout.src == BLE_GAP_TIMEOUT_SRC_ADVERTISEMENT) {
                // we want to always be advertising when not connected. Power be damned
                advertising_start();
                //system_off_mode_enter();
            }
            break;

        default:
            break;
    }

    APP_ERROR_CHECK(err_code);
}


/**@brief Function for dispatching a BLE stack event to all modules with a BLE stack event handler.
 *
 * @details This function is called from the BLE Stack event interrupt handler after a BLE stack
 *          event has been received.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 */
static void ble_evt_dispatch(ble_evt_t *p_ble_evt) {
    ble_bondmngr_on_ble_evt(p_ble_evt);
    ble_bas_on_ble_evt(&bas, p_ble_evt);
    ble_led_program_on_ble_evt(&led_program, p_ble_evt);
    ble_conn_params_on_ble_evt(p_ble_evt);
    on_ble_evt(p_ble_evt);
}


static void update_led_strips(bool radio_active) {
    if (false == radio_active) {
        if (head_unit.program.strips[0].sync) {
            ws2812_sendPixels(LED1_PIN, head_unit.program.strips[0].pixels, head_unit.program.strips[0].count * 3);
            head_unit.program.strips[0].sync = 0;
        }
    }
}

static void power_on() {
    // Actually start advertising
    advertising_start();
}

static void power_off() {
//    sd_ble_gap_adv_stop();
}


/*****************************************************************************
 * Main Function
 *****************************************************************************/

/**@brief Function for the application main entry.
 */
int main(void) {

    timers_init();
    bond_manager_init();
    ble_stack_init();
    radio_notification_init();
    // Initialize Bluetooth Stack parameters
    gap_params_init();
    services_init();
    advertising_init();
    conn_params_init();
    sec_params_init();
    APP_GPIOTE_INIT(1);
    buttons_init();

    advertising_start();

    head_unit_init();
    application_timers_start();



    while (true) {
        // if we are transitioning to a new state handle it appropriately here
        if (head_unit.transition_to_mode != head_unit.current_mode) {
            head_unit_mode_t last_mode = head_unit.current_mode;
            head_unit_process_transition(&head_unit);
            switch(head_unit.current_mode) {
                case MODE_OFF:
                    power_off();
                    break;
                case MODE_IDLE:
                    // we just came out of low power mode so start advertising
                    if (last_mode == MODE_OFF) {
                        power_on();
                    }
                    break;
                default:
                    break;
            }
        } else {
            if (head_unit.current_mode == MODE_RUN) {
                switch (head_unit.program.state) {
                    case STATE_RUNNING:
                        runProgram(&head_unit.program);
                        break;
                }
            }
        }

        // we always wait for sync if not set this allows transitions to play animations
        while (head_unit.program.strips[0].sync) {
        }
    }

}

/**
 * @}
 */
