#include QMK_KEYBOARD_H
#include "pimoroni_trackball.h"
#include "pointing_device.h"
#include "i2c_master.h"
// Standard layout

enum layer_names {
  _QWERTY,
  _SYM,
  _RAISE,
  _ADJUST,
  _FN,
  _MOUS
};

enum custom_keycodes {	
  QWERTY = SAFE_RANGE,
  COLEMAK,
  DVORAK,
  BALL_HUI,//cycles hue
  BALL_WHT,//cycles white
  BALL_DEC,//decreased color
  BALL_SCR,//scrolls
  BALL_NCL,//left click
  BALL_RCL,//right click
  BALL_MCL,//middle click
  BALL_BCL,//mouse button 4 
  BALL_FCL,//mouse button 5
  BALL_HUE, //hold + scroll ball up and down to cycle hue
  BALL_SAT,//hold + scroll ball up and down to cycle saturation
  BALL_VAL,//hold + scroll ball up and down to cycle value
  BALL_DRG,
};

#define SYM  MO(_SYM)
#define RAISE  MO(_RAISE)
#define ADJUST MO(_ADJUST)
#define FN MO(_FN)
#define MOUSE MO(_MOUS)
#define WIN LGUI(KC_TAB)
#define CTRLTB LCTL_T(KC_TAB)
#define LSCL LSFT_T(KC_CAPS)
#define AHK LALT(KC_PSCR)
#define SEARCH LGUI(KC_SPC)
#define ESCFN LT(_FN, KC_ESC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_QWERTY] = LAYOUT_all(
   ESCFN,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
   CTRLTB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,   BALL_MCL, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, 
   LSCL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   BALL_RCL, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
   WIN,     SEARCH,  KC_LALT, KC_LGUI, SYM,     KC_ENT, KC_NO,    KC_SPC,  RAISE,   BALL_DRG, BALL_BCL, BALL_FCL, KC_RALT
),

[_SYM] = LAYOUT_all(
  KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,          KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
  _______,  KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, _______,  AHK,    _______, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,
  _______, _______, _______, _______, _______, _______, _______, _______, S(KC_NUHS), S(KC_NUBS),_______, KC_UP, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   KC_LEFT , KC_DOWN, KC_RGHT
),

[_RAISE] = LAYOUT_all(
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  _______, _______, _______, _______, _______, _______, _______, _______, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
  _______, _______, _______, _______, _______, _______, _______, _______,  KC_NUHS, KC_NUBS, KC_VOLD, KC_MUTE,  KC_VOLU,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT
),

[_ADJUST] =  LAYOUT_all(
  _______, RESET,   _______, _______, BALL_VAL, RGB_TOG,          _______, _______, _______, _______, _______, KC_DEL,
  _______, _______, _______, _______, BALL_SAT, RGB_HUI, RGB_MOD, RGB_SAI, RGB_VAI, _______, _______, _______, _______,
  _______, _______, _______, BALL_WHT, BALL_HUE, RGB_HUD,RGB_RMOD,RGB_SAD, RGB_VAD, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______
),

[_FN] = LAYOUT_all(
  _______, KC_F1,   KC_F4,   KC_F7,   KC_F10,  KC_F13,           KC_F16,   KC_F19,  KC_F22, _______, _______, _______,
  _______, KC_F2,   KC_F5,   KC_F8,   KC_F11,  KC_F14,  _______, KC_F17,   KC_F20,  KC_F23, _______, _______, _______,
  _______, KC_F3,   KC_F6,   KC_F9,   KC_F12,  KC_F15,  _______, KC_F18,   KC_F21,  KC_F24, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

[_MOUS] =  LAYOUT_all(
  _______, RESET,   _______, _______, BALL_VAL, RGB_TOG,          _______, _______, _______, _______, _______, KC_DEL,
  _______, _______, _______, _______, BALL_SAT, RGB_HUI, RGB_MOD, RGB_SAI, RGB_VAI,  _______,_______, _______, _______,
  _______, _______, _______, BALL_WHT, BALL_HUE, RGB_HUD,RGB_RMOD,RGB_SAD, RGB_VAD, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, BALL_RCL, _______, _______, _______, _______, _______
)

};

#include "timer.h"
#include "math.h"
#include "quantum/quantum.h"


static bool hue_mode_enabled = 0;
static bool saturation_mode_enabled = 0;
static bool value_mode_enabled = 0;
/*static bool right_click_mode_enabled = 0;*/
static bool drag_mode_enabled = 0;
#define MOUSE_TIMEOUT 1000
#define TRACKBALL_TIMEOUT 5

#define SIGN(x) ((x > 0) - (x < 0))

// user config EEPROM stuff {{{
typedef union {
  uint32_t raw;
  struct {
	uint8_t tb_hue :8;
    uint8_t tb_value :8;
	uint8_t tb_saturation :8;
  };
} user_config_t;

user_config_t user_config;

static uint8_t tb_hue = 1;
static uint8_t tb_saturation = 255;
static uint8_t tb_value = 255;

void keyboard_post_init_user(void) {
  user_config.raw = eeconfig_read_user();
  tb_hue = user_config.tb_hue;
  tb_saturation = user_config.tb_saturation;
  tb_value = user_config.tb_value;
  trackball_set_hsv(tb_hue, tb_saturation,tb_value);
}

void eeconfig_init_user(void) {
  user_config.raw = 0;
  user_config.tb_hue = 80;
  user_config.tb_saturation = 80;
  user_config.tb_value = 80;
  eeconfig_update_user(user_config.raw);
}

void matrix_init_user() {
    trackball_init();
}

void suspend_power_down_user(void) {
    trackball_set_brightness(0);
    /* trackball_sleep(); */
}

__attribute__((weak)) void pointing_device_init(void) { trackball_set_rgbw(0,0,0,tb_value); }

void update_member(int8_t* member, int16_t* offset) {//{{{
    if (*offset > 127) {
        *member = 127;
        *offset -= 127;
    } else if (*offset < -127) {
        *member = -127;
        *offset += 127;
    } else {
        *member = *offset;
        *offset = 0;
    }
}//}}}

static int16_t x_offset = 0;
static int16_t y_offset = 0;
static int16_t v_offset = 0;
static int16_t h_offset = 0;
static int16_t tb_timer = 0;
void pointing_device_task() {
    report_mouse_t mouse = pointing_device_get_report();

    if (trackball_get_interrupt() && (!tb_timer || timer_elapsed(tb_timer) > TRACKBALL_TIMEOUT)) {
        tb_timer = timer_read() | 1;

        trackball_state_t state = trackball_get_state();

        uint8_t mods;
        if (state.x || state.y) {
            mods = get_mods();
        }

        /*if (state.button_triggered && (right_click_mode_enabled == 1)) {
            if(state.button_down) {
                mouse.buttons |= MOUSE_BTN2;
            } else {
                mouse.buttons &= ~MOUSE_BTN2;
            }
            pointing_device_set_report(mouse);
		pointing_device_send();
		} else*/ if (state.button_triggered && (drag_mode_enabled == 1)) {
			if (state.button_down){
			mouse.buttons |= MOUSE_BTN1;
			pointing_device_set_report(mouse);
            pointing_device_send();}
		} else if (state.button_triggered) {
			if(state.button_down) {
				mouse.buttons |= MOUSE_BTN1;
			} else {
				mouse.buttons &= ~MOUSE_BTN1;
			}
			pointing_device_set_report(mouse);
            pointing_device_send();
        } else {
			// on the ADJUST layer with BALL_VAL held, roll ball downwards to change trackball value
            if (layer_state_is(_ADJUST) && value_mode_enabled == 1) {
                tb_value += state.y * 2;
                trackball_set_hsv(tb_hue, tb_saturation, tb_value | 1);
			// on the ADJUST layer with BALL_SAT held, roll ball downwards to change trackball saturation
			} else if (layer_state_is(_ADJUST) && saturation_mode_enabled == 1) {
				tb_saturation += state.y * 2;
                trackball_set_hsv(tb_hue, tb_saturation | 1, tb_value);
			// on the ADJUST layer, or with BALL_HUE held, roll ball downwards to change trackball hue
			} else if (layer_state_is(_ADJUST) || hue_mode_enabled == 1) {
				tb_hue += state.y;
                trackball_set_hsv(tb_hue | 1, tb_saturation, tb_value);
			// on the NUM layer, trackball behaves as vertical scroll
            } else if (layer_state_is(_RAISE) || layer_state_is(_FN)) {
                h_offset += (state.x);
                v_offset -= (state.y);
            } else if ((state.x || state.y) && !state.button_down) {

			
                uint8_t scale = 3;
                if (mods & MOD_MASK_CTRL) scale = 2;
                x_offset += state.x * state.x * SIGN(state.x) * scale;
                y_offset += state.y * state.y * SIGN(state.y) * scale;

            } 	
        }

       

    }

    while (x_offset || y_offset || h_offset || v_offset) {
        update_member(&mouse.x, &x_offset);
        update_member(&mouse.y, &y_offset);

        update_member(&mouse.v, &v_offset);
        update_member(&mouse.h, &h_offset);

        pointing_device_set_report(mouse);
        pointing_device_send();
    }
}

   



layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _SYM, _RAISE, _ADJUST);
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
             }
             break;
			 case BALL_HUE:
        if (record->event.pressed) {
        hue_mode_enabled = 1;
        } else {
        hue_mode_enabled = 0;
        }
        break;
	case BALL_SAT:
		if (record->event.pressed) {
        saturation_mode_enabled = 1;
        } else {
        saturation_mode_enabled = 0;
        }
        break;
	case BALL_VAL:
		if (record->event.pressed) {
        value_mode_enabled = 1;
        } else {
        value_mode_enabled = 0;
        }
        break;
	/*case BALL_RCL:
	if (record->event.pressed) {
        right_click_mode_enabled = 1;
        } else {
        right_click_mode_enabled = 0;
        }
        break;*/
	case BALL_DRG:
	if (record->event.pressed) {
        drag_mode_enabled = 1;
        } else {
        drag_mode_enabled = 0;
        }
        break;
		
	case BALL_MCL:
		record->event.pressed?register_code(KC_BTN3):unregister_code(KC_BTN3);
		break;
	case BALL_RCL:
		record->event.pressed?register_code(KC_BTN2):unregister_code(KC_BTN2);
		break;
	case BALL_BCL:
		record->event.pressed?register_code(KC_BTN4):unregister_code(KC_BTN4);
		break;
	case BALL_FCL:
		record->event.pressed?register_code(KC_BTN5):unregister_code(KC_BTN5);
		break;
		

    }


    return true;
	
	  
	  
}/*}}}*/




  
  