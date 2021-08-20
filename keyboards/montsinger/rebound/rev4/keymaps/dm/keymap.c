#include QMK_KEYBOARD_H
// Standard layout



enum layer_names {
  _QWERTY,
  _LOWER,
  _RAISE,
  _ADJUST,
  _FN
};

enum custom_keycodes {	
  QWERTY = SAFE_RANGE,
  COLEMAK,
  DVORAK,
};

#define LOWER  MO(_LOWER)
#define RAISE  MO(_RAISE)
#define ADJUST MO(_ADJUST)
#define FN MO(_FN)
#define WIN LGUI(KC_TAB)
#define CTRLTB LCTL_T(KC_TAB)
#define LSCL LSFT_T(KC_CAPS)
#define AHK LALT(KC_PSCR)
#define SEARCH LGUI(KC_SPC)
#define ESCFN LT(_FN, KC_ESC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_QWERTY] = LAYOUT_all(
   ESCFN,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,            KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC,
   CTRLTB,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,   KC_PGUP, KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
   LSCL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,   KC_PGDN, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT ,
   WIN,     SEARCH,  KC_LALT, KC_LGUI, LOWER,   KC_ENT, KC_MUTE, KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
),

[_LOWER] = LAYOUT_all(
  KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,          KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
  KC_DEL,  KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, _______,  AHK,    _______, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,
  _______, _______, _______, _______, _______, _______, _______, _______, S(KC_NUHS), S(KC_NUBS),_______, KC_UP, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,   KC_LEFT , KC_DOWN, KC_RGHT
),

[_RAISE] = LAYOUT_all(
  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,             KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  KC_DEL,  KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  _______, KC_F18,  KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,
  _______, KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  _______, KC_F24,  KC_NUHS, KC_NUBS, _______, _______,   _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_MPRV, KC_MPLY, KC_MNXT
),

[_ADJUST] =  LAYOUT_all(
  _______, RESET,   _______, _______, _______, RGB_TOG,          _______, _______, _______, _______, _______, KC_DEL,
  _______, _______, _______, _______, _______, RGB_HUI, RGB_MOD, RGB_SAI, RGB_VAI,  _______, _______,  _______, _______,
  _______, _______, _______, _______, _______, RGB_HUD, RGB_RMOD, RGB_SAD, RGB_VAD, _______, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
),

[_FN] = LAYOUT_all(
  _______, KC_F1,   KC_F4,   KC_F7,   KC_F10,  KC_F13,           KC_F16,   KC_F19,  KC_F22, _______, _______, _______,
  _______, KC_F2,   KC_F5,   KC_F8,   KC_F11,  KC_F14,  _______, KC_F17,   KC_F20,  KC_F23, _______, _______, _______,
  _______, KC_F3,   KC_F6,   KC_F9,   KC_F12,  KC_F15,  _______, KC_F18,   KC_F21,  KC_F24, _______, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
)

};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
             }
             break;
       
    }
    return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
   switch(get_highest_layer(layer_state)){
case _QWERTY:
    if (clockwise) {
      tap_code16(KC_VOLD);
    } else {
      tap_code16(KC_VOLU);
    }
   break;
 case _LOWER:
    if (clockwise) {
      tap_code16(KC_PGUP);
    } else {
      tap_code16(KC_PGDN);
    }
	 break;
  case _RAISE:
	if (clockwise) {
		tap_code16(RGB_HUI);
	} else {
		tap_code16(RGB_HUD);
	}
	break;
   }
   return true;
  
}




