// this is the style you want to emulate.
// This is the canonical layout file for the Quantum project. If you want to add another keyboard,

#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QW 0
#define _RS 1
#define _LW 2

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_QW] = LAYOUT( /* Qwerty */
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P    ,
    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    LCTL_T(KC_SCLN) ,
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                      KC_N,    KC_M,    KC_COMM, KC_DOT,  LSFT_T(KC_SLSH) ,
    KC_ESC,  KC_TAB,  KC_LGUI,  KC_LSFT, KC_BSPC,  KC_LCTL, KC_LALT, KC_SPC,  LT(_RS, KC_MINS), LT(_LW, KC_EQL), LGUI_T(KC_QUOT), KC_ENT
  ),

  /*
   *  !       @     up     {    }        ||     pgup    7     8     9    *
   *  #     left   down  right  $        ||     pgdn    4     5     6    +
   *  [       ]      (     )    &        ||       `     1     2     3    \
   * lower  insert super shift bksp ctrl || alt space   fn    .     0    =
   */
  [_RS] = LAYOUT( /* [> RAISE <] */
                 KC_7,   KC_8, KC_9,    KC_GRAVE, KC_PERC,                   KC_EXLM, KC_LBRC, KC_AMPR,  KC_RBRC, LSFT(KC_NUBS) ,
                 KC_4,   KC_5, KC_6,    KC_HASH, KC_MINS,                   KC_NUHS, KC_LPRN, KC_AT,    KC_RPRN, LSFT(KC_SCLN) ,
                 KC_1,   KC_2, KC_3,    KC_EQL,  KC_PLUS,                   KC_NUBS, KC_LCBR, KC_ASTR,  KC_RCBR, LSFT(KC_NUHS) ,
                 KC_DOT, KC_0, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, MO(_RS), KC_CIRC, KC_DLR, TG(_RS)  ),
  /*
   * insert home   up  end   pgup       ||      up     F7    F8    F9   F10
   *  del   left  down right pgdn       ||     down    F4    F5    F6   F11
   *       volup             reset      ||             F1    F2    F3   F12
   *       voldn  super shift bksp ctrl || alt space   L0  prtsc scroll pause
   */
  [_LW] = LAYOUT( /* [> LOWER <] */
    KC_INS,  KC_HOME, KC_UP,   KC_END,  KC_PGUP,                 KC_F1,  KC_F2,  KC_F3, KC_F4, KC_F5  ,
    KC_DELT, KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN,                 KC_F6,  KC_F7,  KC_F8, KC_F9, KC_F10  ,
    KC_MUTE, KC_VOLD, KC_VOLU, KC_BRID,   KC_BRIU,                   KC_F11, KC_F12, KC_F13, KC_F14, KC_PSCR  ,
    RESET,   KC_SLCK, KC_NO, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_NO, MO(_LW), KC_HASH, TG(_LW) )
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt) {
  // MACRODOWN only works in this function
  switch(id) {
    case 0:
      if (record->event.pressed) {
        register_code(KC_RSFT);
      } else {
        unregister_code(KC_RSFT);
      }
    break;
  }
  return MACRO_NONE;
};
