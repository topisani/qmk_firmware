/* Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum layers{
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
};


enum custom_keycodes {
    MIDI_CC16D = SAFE_RANGE,
    MIDI_CC16U,
    MIDI_CC17D,
    MIDI_CC17U,
    MIDI_CC18D,
    MIDI_CC18U,
    MIDI_CC19D,
    MIDI_CC19U,
};

static uint8_t midi_values[4] = {0};

extern MidiDevice midi_device;

static void send_cc(int idx, int diff) {
  int newval =  midi_values[idx] + diff;
  if (newval > 127 ) newval = 127;
  if (newval < 0) newval = 0;
  
  midi_values[idx] = newval;
  midi_send_cc(&midi_device, midi_config.channel, 16 + idx, midi_values[idx]);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    int inc = 4;
    if (keyboard_report->mods & (MOD_BIT_LSHIFT | MOD_BIT_RSHIFT)) {
      inc = 1;
    }
    if (record->event.pressed) {
      switch (keycode) {
        case MIDI_CC16D: send_cc(0, -inc); break;
        case MIDI_CC16U: send_cc(0, inc); break;
        case MIDI_CC17D: send_cc(1, -inc); break;
        case MIDI_CC17U: send_cc(1, inc); break;
        case MIDI_CC18D: send_cc(2, -inc); break;
        case MIDI_CC18U: send_cc(2, inc); break;
        case MIDI_CC19D: send_cc(3, -inc); break;
        case MIDI_CC19U: send_cc(3, inc); break;
      }
    }
    return true;
};

static void cc_callback(MidiDevice* dev, uint8_t channel, uint8_t cc, uint8_t val) {
  if (cc >= 16 && cc < 20) {
    midi_values[cc - 16] = val;
  }
}

void keyboard_post_init_user(void) {
  midi_register_cc_callback(&midi_device, cc_callback);
  rgblight_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
  rgblight_sethsv_noeeprom(HSV_WHITE);
}

void led_set_user(uint8_t usb_led) {
  if (!(usb_led & (1<<USB_LED_NUM_LOCK))) {
    tap_code(KC_NUM_LOCK);
  }
}

#define RA(X) ALGR(KC_ ## X)

enum {
  DA_LT = KC_NUBS,
  DA_GT = S(KC_NUBS),
  DA_TLD = ALGR(KC_RBRC),
};

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

#define TOPISANI_LAYOUT_BASE(FN) LAYOUT_92_iso(                                                                                                                \
      KC_LGUI, KC_GRV,  KC_F1,    KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,    KC_F12,   KC_INS,  KC_DEL,  _______, \
      KC_P1,  KC_ESC,  KC_1,     KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,   KC_EQL,   KC_BSPC,          KC_PGUP, \
      KC_P2,  KC_TAB,  KC_Q,     KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,   KC_RBRC,                    KC_PGDN, \
      KC_P3,  KC_LCTL, KC_A,     KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,   KC_NUHS,  KC_ENT,           KC_HOME, \
      KC_P4,  KC_LSFT, KC_NUBS,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,             KC_RSFT, KC_UP,            \
      KC_P5,  MO(FN),  KC_LCTL,  KC_LGUI, KC_LALT,          KC_SPC,                    LT(FN, KC_SPC),   KC_RALT, MO(FN),    KC_RCTL,  KC_LEFT, KC_DOWN, KC_RGHT  \
  )
  
#define TOPISANI_LAYOUT_FN(BASE)  LAYOUT_92_iso(                                                                                                                   \
      RGB_TOG, _______, KC_BRID,  KC_BRIU, KC_TASK, KC_FLXP, RGB_VAD, RGB_VAI, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD,   KC_VOLU,  _______, _______, RGB_TOG, \
      KC_P6,  KC_ESC,  RA(1),    RA(2),   RA(3),   RA(4),   RA(5),   RA(6),   RA(7),   RA(8),   RA(9),   RA(0),   RA(MINS),  RA(EQL),  KC_DEL,           _______, \
    	KC_P7,  _______, _______,  _______, _______, _______, _______, _______, _______, _______, _______, KC_PSCR, _______,   RA(RBRC),                   _______, \
    	KC_P8,  _______, _______,  _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______,   _______,  _______,          KC_END, \
    	KC_P9,  _______, RA(NUBS), _______, _______, _______, _______, _______, _______, _______, DA_LT,   DA_GT,   DA_TLD,              _______, _______,          \
    	KC_P0,  KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,          _______,                   KC_TRNS,          KC_TRNS, KC_TRNS,   KC_TRNS,  _______, _______, _______) \

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[MAC_BASE] = TOPISANI_LAYOUT_BASE(MAC_FN),
	[MAC_FN] = TOPISANI_LAYOUT_FN(MAC_BASE),
	[WIN_BASE] = TOPISANI_LAYOUT_BASE(WIN_FN),
	[WIN_FN] = TOPISANI_LAYOUT_FN(WIN_BASE),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [MAC_BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [MAC_FN]   = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI), ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [WIN_BASE] = { ENCODER_CCW_CW(MIDI_CC16D, MIDI_CC16U), ENCODER_CCW_CW(MIDI_CC17D, MIDI_CC17U) },
    [WIN_FN]   = { ENCODER_CCW_CW(MIDI_CC18D, MIDI_CC18U), ENCODER_CCW_CW(MIDI_CC19D, MIDI_CC19U) }
};
#endif // ENCODER_MAP_ENABLE

