#include "nrf_gpio.h"
#include "app_gpiote.h"
#include "buttons.h"
#include "time.h"
#include "app_timer.h"
#include "head_unit.h"

#define IS_POWER_SET(x) (x & (1 << POWER_PIN))

static app_gpiote_user_id_t           m_gpiote_user_id;
static uint32_t                       m_active_low_states_mask;    /**< Mask representing buttons which are active low. */
static uint32_t                       m_active_high_states_mask;   /**< Mask representing buttons which are active high. */

static uint32_t  last_event;
static uint32_t  last_power_press_ticks;
static uint32_t  last_power_release_ticks;

static bool has_millis_elapsed(uint32_t millis, uint32_t start_tick, uint32_t end_tick) {
    uint32_t diff_ticks;
    app_timer_cnt_diff_compute(end_tick, start_tick, &diff_ticks);
    return (diff_ticks > (millis*TICKS_PER_MILLIS));
}

static void on_short_press_then_release() {
    if (head_unit.current_mode == MODE_RUN) {
        head_unit.transition_to_mode = MODE_IDLE;
    } else if (head_unit.current_mode == MODE_IDLE) {
        head_unit.transition_to_mode = MODE_RUN;
    }
}

static void on_long_press_then_release() {
    if (head_unit.current_mode == MODE_OFF) {
        head_unit.transition_to_mode = MODE_IDLE;
    } else {
        head_unit.transition_to_mode = MODE_OFF;
    }
}

static void buttons_event_handler(uint32_t event_pins_low_to_high, uint32_t event_pins_high_to_low) {
    uint32_t current_event;
    app_timer_cnt_get(&current_event);

    // debounce
    if (!has_millis_elapsed(10, last_event, current_event )) {
        last_event = current_event;
        return;
    }
    last_event = current_event;

    if (IS_POWER_SET(event_pins_high_to_low)) {
        app_timer_cnt_get(&last_power_press_ticks);
    } else {
        /** simple indicator of a release for debugging
        push_color(&ledProgram, 0, 255,255,255);
        ledProgram.strips[0].sync = 1;
        **/
        app_timer_cnt_get(&last_power_release_ticks);

        if (has_millis_elapsed(1000, last_power_press_ticks, last_power_release_ticks)) {
            on_long_press_then_release();
        } else {
            on_short_press_then_release();
        }

    }
}

/**
 * Initialize our buttons for usage
 */
void buttons_init() {
    // configure the power button
    // power button has never been pressed
    last_power_press_ticks = 0;
    last_power_release_ticks = 0;
    nrf_gpio_cfg_input(POWER_PIN, GPIO_PIN_CNF_PULL_Pullup);

    // We want to know when power button goes HIGH and when it goes low
    m_active_low_states_mask = 1 << POWER_PIN;
    m_active_high_states_mask = 1 << POWER_PIN;

   app_gpiote_user_register(&m_gpiote_user_id,
                            m_active_high_states_mask,
                            m_active_low_states_mask,
                            buttons_event_handler);
    // buttons are enabled by default
    buttons_enable();
}

/**
 * Enable the buttons
 */
uint32_t buttons_enable(void)
{
    return app_gpiote_user_enable(m_gpiote_user_id);
}


/**
 * disable all buttons
 */
uint32_t buttons_disable(void)
{
    return app_gpiote_user_disable(m_gpiote_user_id);
}
