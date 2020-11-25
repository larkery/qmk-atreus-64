/* Copyright 2019 %YOUR_NAME%
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
#include "analog.h"
#include "pointing_device.h"
#include "timer.h"

#define X_PIN 1
#define Y_PIN 0

bool mouse_is_scrolling = false;

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
                      SCROLL_MOUSE = SAFE_RANGE,
                      LEFT_MOUSE,
                      RIGHT_MOUSE,
                      MID_MOUSE,
                      SCROLL_MOUSE_LOCK
};

#define KC_EX LSFT(KC_1)
#define KC_DQ LSFT(KC_2)
#define KC_PD LSFT(KC_3)
#define KC_DL LSFT(KC_4)
#define KC_PC LSFT(KC_5)
#define KC_HT LSFT(KC_6)
#define KC_AM LSFT(KC_7)
#define KC_ST LSFT(KC_8)
#define KC_LP LSFT(KC_9)
#define KC_RP LSFT(KC_0)
#define KC_LC LSFT(KC_LBRC)
#define KC_RC LSFT(KC_RBRC)
#define KC_TL LSFT(KC_NUHS)
#define KC_PP LSFT(KC_NUBS)
#define KC_US LSFT(KC_MINS)
#define KC_CL LSFT(KC_SCLN)
#define KC_PL LSFT(KC_EQL)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT( /* Base */
               KC_GRAVE,          KC_LEFT, KC_RIGHT, KC_UP,  KC_DOWN, KC_PGUP, KC_PGDN, KC_HOME, KC_END,  KC_BSPC,
               KC_Q,            KC_W,    KC_E,     KC_R,   KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
               KC_A,            KC_S,    KC_D,     KC_F,   KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    LCTL_T(KC_SCLN),
               KC_Z,            KC_X,    KC_C,     KC_V,   KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
               LT(2, KC_ESC), KC_NUHS, KC_NUBS,  KC_EQL, KC_LSFT, KC_SPC,  TT(1),   KC_MINS, KC_QUOT, KC_ENT,

               KC_LCTL, KC_LGUI, KC_TAB, KC_LALT
  ),

  //punctuation not in L0
  // ( ) { }
  // ^ $ ! "
  // £ % & *
  // ~ | _ + :
  // DEL INS HOME END
  [1] = LAYOUT(KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,  KC_F6,   KC_F7,   KC_F8,  KC_F9,   KC_F10,
               KC_7,    KC_8,    KC_9,    KC_F11,  KC_F12, KC_EX,   KC_LBRC, KC_AM,  KC_RBRC, KC_PP,
               KC_4,    KC_5,    KC_6,    KC_PD,   KC_PC,  KC_NUHS, KC_LP,   KC_DQ,  KC_RP,   KC_CL,
               KC_1,    KC_2,    KC_3,    KC_MINS, KC_PL,  KC_NUBS, KC_LC,   KC_ST,  KC_RC,   KC_TL,
               KC_DOT,  KC_0,    KC_HOME, KC_END,  KC_TRNS, KC_INS,  KC_TRNS, KC_HT, KC_DL,   KC_TRNS,
               KC_TRNS, KC_TRNS, KC_DEL, KC_TRNS
               ),

  [2] = LAYOUT(KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS,
               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, SCROLL_MOUSE, SCROLL_MOUSE_LOCK, KC_TRNS, KC_TRNS, KC_TRNS,
               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, LEFT_MOUSE,   RIGHT_MOUSE,       KC_TRNS, KC_TRNS, KC_TRNS,
               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, MID_MOUSE,    KC_TRNS,           KC_TRNS, KC_TRNS, KC_TRNS,
               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,      KC_TRNS,           KC_TRNS, KC_TRNS, RESET,
               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
               ),
};

void handle_mouse_button(bool state, uint8_t button) {
    report_mouse_t report = pointing_device_get_report();

    if (state) {
        report.buttons |= button;
    } else {
        report.buttons &= ~button;
    }

    pointing_device_set_report(report);
    pointing_device_send();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case LEFT_MOUSE:
        handle_mouse_button(record->event.pressed, 0b1);
        break;
    case MID_MOUSE:
        handle_mouse_button(record->event.pressed, 0b100);
        break;
    case RIGHT_MOUSE:
        handle_mouse_button(record->event.pressed, 0b10);
        break;
    case SCROLL_MOUSE:
        mouse_is_scrolling = !mouse_is_scrolling;
        break;
    case SCROLL_MOUSE_LOCK:
        if (record->event.pressed) {
            mouse_is_scrolling = !mouse_is_scrolling;
        }
        break;
    }
    return true;
}

void pointing_device_init(void) {
    timer_init();
}


float ranged_pin(uint8_t pin,
                 uint16_t min_val,
                 uint16_t dead_low, uint16_t dead_high,
                 uint16_t max_val
                 ) {
    uint16_t value = analogRead(pin);
    float x;
    if (value < dead_low) {
        x = ((float)(dead_low - value)) / (dead_low - min_val);
        return -x;
    } else if (value > dead_high) {
        x = ((float)(value - dead_high)) / (max_val - dead_high);
        return x;
    } else {
        return 0;
    }
}

uint16_t last_report_x, last_report_y;
bool zap;

void pointing_device_task(void) {
    float x_val = ranged_pin(X_PIN,
                             200, 520, 562, 885);
    float y_val = ranged_pin(Y_PIN,
                             120, 476, 522, 880);

    if (x_val == 0 && y_val == 0) {
        zap = true;
        return;
    } else if (zap) {
        zap = false;
        last_report_x = last_report_y = timer_read();
    }

    uint16_t delta_tx = timer_elapsed(last_report_x);
    uint16_t delta_ty = timer_elapsed(last_report_y);

    float magnitude = sqrt(x_val * x_val + y_val * y_val);
    magnitude = pow(magnitude+0.3, 2);

    x_val = x_val * magnitude;
    y_val = y_val * magnitude;

    float rate_scale = mouse_is_scrolling ? 0.1 : 2;

    float x_rate = rate_scale * x_val * delta_tx;
    float y_rate = rate_scale * y_val * delta_ty;

    if (fabs(x_rate) > 1 || fabs(y_rate) > 1) {
        report_mouse_t report = pointing_device_get_report();

        if (mouse_is_scrolling) {
            report.h = (int) x_rate;
            report.v = - (int) y_rate;
            report.x = report.y = 0;
        } else {
            report.x = (int) x_rate;
            report.y = (int) y_rate;
            report.v = report.h = 0;
        }

        uint16_t now = timer_read();
        if ((int)x_rate) last_report_x = now;
        if ((int)y_rate) last_report_y = now;

        pointing_device_set_report(report);
        pointing_device_send();
    }
}

void matrix_init_user(void) {

}

void matrix_scan_user(void) {

}

void led_set_user(uint8_t usb_led) {

}
