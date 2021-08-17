/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>
char wpm_str[10];

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  RSFT_T(KC_ESC),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LALT,   MO(2),  KC_SPC,     KC_ENT,   MO(1), KC_RGUI
                                      //`--------------------------'  `--------------------------'

  ),

  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_F1,   KC_F2,  KC_F3,   KC_F4,   KC_F5,   KC_F6,                      KC_LEFT, KC_DOWN, KC_UP,  KC_RIGHT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LALT, MO(3),  KC_SPC,     KC_ENT, _______, KC_RGUI
                                      //`--------------------------'  `--------------------------'
  ),

  [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LEAD, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY, XXXXXXX,                      KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LALT,   _______,  KC_SPC,     KC_ENT, MO(3), KC_RGUI
                                      //`--------------------------'  `--------------------------'
  ),

  [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        RESET, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, RGB_SPI, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LALT, _______,  KC_SPC,     KC_ENT, _______, KC_RGUI
                                      //`--------------------------'  `--------------------------'
  )
};

#ifdef OLED_DRIVER_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

#    define L_BASE 0
#    define L_LOWER 2
#    define L_RAISE 4
#    define L_ADJUST 8

/* KEYBOARD PET START */

/* settings */
#    define MIN_WALK_SPEED 10
#    define MIN_RUN_SPEED 40

/* advanced settings */
#    define ANIM_FRAME_DURATION 200  // how long each frame lasts in ms
#    define ANIM_SIZE 96             // number of bytes in array. If you change sprites, minimize for adequate firmware size. max is 1024

/* timers */
uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;

/* current frame */
uint8_t current_frame = 0;

/* status variables */
int   current_wpm = 0;
led_t led_usb_state;

bool isSneaking = false;
bool isJumping  = false;
bool showedJump = true;

/* logic */
static void render_luna(int LUNA_X, int LUNA_Y) {
    /* Sit */
    static const char PROGMEM sit[2][ANIM_SIZE] = {/* 'sit1', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x68, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   },

                                                   /* 'sit2', 32x22px */
                                                   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x1c, 0x02, 0x05, 0x02, 0x24, 0x04, 0x04, 0x02, 0xa9, 0x1e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x90, 0x08, 0x18, 0x60, 0x10, 0x08, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x0e, 0x82, 0x7c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x0c, 0x10, 0x10, 0x20, 0x20, 0x20, 0x28, 0x3e, 0x1c, 0x20, 0x20, 0x3e, 0x0f, 0x11, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}};

    /* Walk */
    static const char PROGMEM walk[2][ANIM_SIZE] = {/* 'walk1', 32x22px */
                                                    {
                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x10, 0x90, 0x90, 0x90, 0xa0, 0xc0, 0x80, 0x80, 0x80, 0x70, 0x08, 0x14, 0x08, 0x90, 0x10, 0x10, 0x08, 0xa4, 0x78, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x08, 0xfc, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x18, 0xea, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x03, 0x06, 0x18, 0x20, 0x20, 0x3c, 0x0c, 0x12, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    },

                                                    /* 'walk2', 32x22px */
                                                    {
                                                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x20, 0x20, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x28, 0x10, 0x20, 0x20, 0x20, 0x10, 0x48, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0xf8, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x10, 0x30, 0xd5, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x02, 0x1c, 0x14, 0x08, 0x10, 0x20, 0x2c, 0x32, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    }};

    /* Run */
    static const char PROGMEM run[2][ANIM_SIZE] = {/* 'run1', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0x00, 0xe0, 0x10, 0x08, 0x08, 0xc8, 0xb0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x40, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0xc4, 0xa4, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x58, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x09, 0x04, 0x04, 0x04, 0x04, 0x02, 0x03, 0x02, 0x01, 0x01, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   },

                                                   /* 'run2', 32x22px */
                                                   {
                                                       0x00, 0x00, 0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x78, 0x28, 0x08, 0x10, 0x20, 0x30, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x10, 0xb0, 0x50, 0x55, 0x20, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x1e, 0x20, 0x20, 0x18, 0x0c, 0x14, 0x1e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   }};

    /* Bark */
    static const char PROGMEM bark[2][ANIM_SIZE] = {/* 'bark1', 32x22px */
                                                    {
                                                        0x00, 0xc0, 0x20, 0x10, 0xd0, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x3c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc8, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    },

                                                    /* 'bark2', 32x22px */
                                                    {
                                                        0x00, 0xe0, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x40, 0x40, 0x2c, 0x14, 0x04, 0x08, 0x90, 0x18, 0x04, 0x08, 0xb0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x08, 0x10, 0x11, 0xf9, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x48, 0x28, 0x2a, 0x10, 0x0f, 0x20, 0x4a, 0x09, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x0c, 0x10, 0x20, 0x28, 0x37, 0x02, 0x02, 0x04, 0x08, 0x10, 0x26, 0x2b, 0x32, 0x04, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                    }};

    /* Sneak */
    static const char PROGMEM sneak[2][ANIM_SIZE] = {/* 'sneak1', 32x22px */
                                                     {
                                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x40, 0x80, 0x00, 0x80, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1e, 0x21, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x04, 0x04, 0x04, 0x03, 0x01, 0x00, 0x00, 0x09, 0x01, 0x80, 0x80, 0xab, 0x04, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1c, 0x20, 0x20, 0x3c, 0x0f, 0x11, 0x1f, 0x02, 0x06, 0x18, 0x20, 0x20, 0x38, 0x08, 0x10, 0x18, 0x04, 0x04, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00,
                                                     },

                                                     /* 'sneak2', 32x22px */
                                                     {
                                                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0xa0, 0x20, 0x40, 0x80, 0xc0, 0x20, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3e, 0x41, 0xf0, 0x04, 0x02, 0x02, 0x02, 0x03, 0x02, 0x02, 0x02, 0x04, 0x04, 0x02, 0x01, 0x00, 0x00, 0x00, 0x04, 0x00, 0x40, 0x40, 0x55, 0x82, 0x7c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x20, 0x30, 0x0c, 0x02, 0x05, 0x09, 0x12, 0x1e, 0x04, 0x18, 0x10, 0x08, 0x10, 0x20, 0x28, 0x34, 0x06, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                     }};

    /* animation */
    void animate_luna(void) {
        /* jump */
        if (isJumping || !showedJump) {
            /* clear */
            oled_set_cursor(LUNA_X, LUNA_Y + 2);
            oled_write("     ", false);

            oled_set_cursor(LUNA_X, LUNA_Y - 1);

            showedJump = true;
        } else {
            /* clear */
            oled_set_cursor(LUNA_X, LUNA_Y - 1);
            oled_write("     ", false);

            oled_set_cursor(LUNA_X, LUNA_Y);
        }

        /* switch frame */
        current_frame = (current_frame + 1) % 2;

        /* current status */
        if (led_usb_state.caps_lock) {
            oled_write_raw_P(bark[abs(1 - current_frame)], ANIM_SIZE);

        } else if (isSneaking) {
            oled_write_raw_P(sneak[abs(1 - current_frame)], ANIM_SIZE);

        } else if (current_wpm <= MIN_WALK_SPEED) {
            oled_write_raw_P(sit[abs(1 - current_frame)], ANIM_SIZE);

        } else if (current_wpm <= MIN_RUN_SPEED) {
            oled_write_raw_P(walk[abs(1 - current_frame)], ANIM_SIZE);

        } else {
            oled_write_raw_P(run[abs(1 - current_frame)], ANIM_SIZE);
        }
    }

    /* animation timer */
    if (timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
        anim_timer = timer_read32();
        animate_luna();
    }

    /* this fixes the screen on and off bug */
    if (current_wpm > 0) {
        oled_on();
        anim_sleep = timer_read32();
    } else if (timer_elapsed32(anim_sleep) > OLED_TIMEOUT) {
        /* clear */
        oled_set_cursor(0, 0);
        oled_write("                                                                                                    ", false);
        oled_off();
        oled_set_cursor(LUNA_X, LUNA_Y);
    }
}

/* KEYBOARD PET END */

/* BONGO CAT START */

#    define IDLE_FRAMES 5
#    define TAP_FRAMES 2
#    define FRAME_DURATION 200         // Number of ms between frames
#    define WIDTH OLED_DISPLAY_HEIGHT  // 32 px for OLED_ROTATION_270

static uint16_t const left_base[] PROGMEM = {192,   33343, 33375, 33407, 33439, 33471, 33502, 33534, 33566, 33598, 33630, 33661, 33693, 33725, 33757, 33789, 33820, 33852, 33884, 33916, 33948, 33979, 34011, 34043, 34075, 34107, 34138, 34170, 34202, 34234, 34266, 34297, 34327, 34328, 34329, 34345, 34346, 34347, 34348, 34356, 34357, 34358, 34361, 34376, 34381, 34382, 34383, 34386, 34387, 34393, 34407, 34416, 34417, 34424, 34439, 34456, 34472, 34488, 34504, 34520, 34537, 34552, 34569, 34584, 34600, 34617, 34631, 34649, 34662, 34682, 34694, 34714, 34725, 34747, 34757, 34767, 34768, 34779, 34789, 34799, 34800, 34811, 34821, 34843, 34852, 34870, 34875, 34884, 34901, 34903, 34904, 34905, 34906, 34916, 34928, 34933, 34948,
                                             34961, 34965, 34979, 34993, 34996, 35010, 35024, 35028, 35041, 35056, 35060, 35072, 35088, 35092, 35104, 35119, 35120, 35123, 35137, 35155, 35170, 35171, 35180, 35181, 35187, 35204, 35205, 35212, 35213, 35219, 35238, 35251, 35271, 35282, 35304, 35314, 35336, 35347, 35369, 35379, 35402, 35412, 35435, 35444, 35468, 35476, 35501, 35508, 35534, 35540, 35567, 35568, 35572, 35600, 35601, 35602, 35603, 35632, 35664, 35695, 35727, 35759, 35791, 35822, 35854, 35886, 35918, 35949, 35981, 36013, 36045, 36076, 36108, 36140, 36172, 36203, 36235, 36267, 36299, 36331, 36362, 36394, 36426, 36458, 36490, 36521, 36553, 36585, 36617, 36648, 36680, 36712, 36744, 36775, 36807, 36839};

static void render_array(uint16_t const *frame) {
    // First array element is its size
    uint16_t const size = pgm_read_word(&(frame[0]));
    for (uint16_t i = size; i > 0; --i) {
        uint16_t cur_px = pgm_read_word(&(frame[i]));
        // Get pixel state bit
        bool const on = (cur_px & (1 << 15)) >> 15;
        // Remove pixel state bit
        cur_px &= ~(1UL << 15);
        oled_write_pixel(cur_px % WIDTH, cur_px / WIDTH, on);
    }
}

static void render_frames(uint16_t const *base, uint16_t const *diff) {
    render_array(base);
    render_array(diff);
}

static void render_cat_idle(void) {
    // Idle frames pixel differences
    static uint16_t const  left_idle_diff_0[] PROGMEM  = {0};
    static uint16_t const  left_idle_diff_1[] PROGMEM  = {105, 1577, 34349, 1608, 34377, 1613, 34384, 1618, 34388, 1639, 34408, 1648, 34418, 1671, 34440, 1704, 34473, 1736, 34505, 1769, 34538, 1801, 34570, 1832, 34601, 1863, 34632, 1894, 34663, 1926, 34695, 1957, 34726, 1989, 34758, 1999, 34769, 2021, 34790, 2031, 34801, 2053, 34822, 2084, 34853, 2116, 34885, 2148, 34917, 2160, 34929, 2180, 34949, 2193, 34962, 2211, 34980, 2225, 34994, 2242, 35011, 2256, 35025, 2273, 35042, 2288, 35057, 2304, 35073, 2320, 35089, 2336, 35105, 2351, 35121, 2369, 35138, 2402, 35172, 2412, 35182, 2436, 35206, 2444, 35214, 2470, 35239, 2503, 35272, 2536, 35305, 2568, 35337, 2601, 35370, 2634, 35403, 2667, 35436, 2700, 35469, 2733, 35502, 2766, 35535, 2799};
    static uint16_t const  left_idle_diff_2[] PROGMEM  = {31, 34326, 1577, 1578, 34355, 1590, 1608, 34378, 1615, 1619, 1639, 34409, 34415, 1671, 34441, 1704, 34473, 1736, 34505, 2211, 34980, 2242, 35011, 2273, 35043, 2304, 35074, 2336, 35106, 2369, 35138, 2402};
    static uint16_t const  left_idle_diff_3[] PROGMEM  = {21, 1577, 1608, 34377, 1639, 34408, 1671, 34440, 1704, 34473, 1736, 34505, 2242, 35011, 2273, 35042, 2304, 35073, 2336, 35105, 2403, 35203};
    static uint16_t const *left_idle_diff[IDLE_FRAMES] = {left_idle_diff_0, left_idle_diff_0, left_idle_diff_1, left_idle_diff_2, left_idle_diff_3};

    static uint8_t current_frame = 0;
    current_frame                = (current_frame + 1 > IDLE_FRAMES - 1) ? 0 : current_frame + 1;

    render_frames(left_base, left_idle_diff[current_frame]);
}

static void render_cat_prep(void) {
    // Prep frames pixel differences
    static uint16_t const  left_prep_diff_0[] PROGMEM = {82, 34515, 34516, 34545, 34546, 34576, 34578, 34607, 34611, 34612, 34615, 1849, 34639, 34640, 34643, 34644, 34647, 1881, 34671, 34674, 34679, 1914, 34704, 34705, 34710, 34711, 1946, 34738, 34739, 34740, 34741, 34742, 1979, 34774, 2011, 34806, 2043, 34838, 2075, 2107, 2135, 2136, 2137, 2138, 35341, 35342, 35343, 35346, 2579, 35371, 35372, 35378, 2611, 35401, 35410, 2644, 35432, 35442, 2676, 35464, 35469, 35473, 2708, 35496, 35498, 35500, 35505, 2740, 35528, 2766, 35537, 2772, 35561, 35563, 2804, 35594, 35595, 35596, 35597, 35598, 2833, 2834, 2835};
    static uint16_t const *left_prep_diff[]           = {left_prep_diff_0};

    render_frames(left_base, left_prep_diff[0]);
}

static void render_cat_tap(void) {
    // Tap frames pixel differences
    static uint16_t const  left_tap_diff_0[] PROGMEM = {156,   34515, 34516, 34545, 34546, 34576, 34578, 34607, 34611, 34612, 34615, 1849,  34639, 34640, 34643, 34644, 34647, 1881,  34671, 34674, 34679, 1914,  34704, 34705, 34710, 1946,  34738, 34739, 34740, 34741, 34742, 1979,  34774, 2011,  34806, 2043,  34838, 2075,  2107,  2135,  2136,  2137,  2138,  35523, 35524, 35525, 35526, 35527, 35528, 35529, 35555, 35556, 35557, 35558, 35559, 35560, 35561, 35562, 35587, 35588, 35589, 35590, 35591, 35592, 35593, 35594, 35619, 35620, 35621, 35622, 35623, 35624, 35666, 35667, 35668, 35669, 35691, 35692, 35693,
                                                       35698, 35699, 35700, 35701, 35702, 35722, 35723, 35724, 35725, 35730, 35731, 35732, 35733, 35734, 35735, 35753, 35754, 35755, 35756, 35757, 35762, 35763, 35764, 35765, 35766, 35767, 35768, 35784, 35785, 35786, 35787, 35788, 35794, 35795, 35796, 35797, 35798, 35799, 35800, 35801, 35815, 35816, 35817, 35818, 35819, 35820, 35826, 35827, 35828, 35829, 35830, 35831, 35832, 35833, 35847, 35848, 35849, 35850, 35851, 35852, 35858, 35859, 35860, 35861, 35862, 35863, 35880, 35881, 35882, 35883, 35890, 35891, 35892, 35893, 35914, 35915, 35922, 35923};
    static uint16_t const  left_tap_diff_1[] PROGMEM = {72, 34363, 34364, 34365, 34366, 34395, 34396, 34397, 34398, 34427, 34428, 34429, 34430, 34459, 34460, 34461, 34462, 34491, 34492, 34493, 34494, 34523, 34524, 34525, 34526, 34555, 34556, 34557, 34588, 34589, 34847, 34879, 34911, 34943, 35341, 35342, 35343, 35346, 2579, 35371, 35372, 35378, 2611, 35401, 35410, 2644, 35432, 35442, 2676, 35464, 35469, 35473, 2708, 35496, 35498, 35500, 35505, 2740, 35528, 2766, 35537, 2772, 35561, 35563, 2804, 35594, 35595, 35596, 35597, 35598, 2833, 2834, 2835};
    static uint16_t const *left_tap_diff[TAP_FRAMES] = {left_tap_diff_0, left_tap_diff_1};

    static uint8_t current_frame = 0;
    //	current_frame = (current_frame + 1 > TAP_FRAMES - 1) ? 0 : current_frame + 1;
    current_frame = (current_frame + 1) & 1;  // Quicker maths cycle two frames

    render_frames(left_base, left_tap_diff[current_frame]);
}

static void render_bongocat(void) {
    // Animation frame timer
    static uint16_t anim_timer = 0;

#    ifdef WPM_ENABLE
    static uint8_t  prev_wpm  = 0;
    static uint32_t tap_timer = 0;
    // tap_timer updated by sustained WPM
    if (get_current_wpm() > prev_wpm) {
        tap_timer = timer_read32();
    }
    prev_wpm = get_current_wpm();
#    else
    // tap_timer updated by key presses in process_record_user()
    extern uint32_t tap_timer;
#    endif

    // Time gap between tap_timer updates
    uint32_t keystroke = timer_elapsed32(tap_timer);

    void draw_frame(void) {
        oled_clear();
        if (keystroke < FRAME_DURATION * 2) {
            render_cat_tap();
        } else if (keystroke < FRAME_DURATION * 8) {
            render_cat_prep();
        } else {
            render_cat_idle();
        }
    }

    if (keystroke > OLED_TIMEOUT) {
        oled_off();
    } else if (timer_elapsed(anim_timer) > FRAME_DURATION) {
        anim_timer = timer_read();
        draw_frame();
    }
}

// static void print_master_oled(void) {
//     /* wpm counter */
//     char wpm_str[4];
//     oled_set_cursor(1,5);
//     wpm_str[3] = '\0';
//     wpm_str[2] = '0' + current_wpm % 10;
//     wpm_str[1] = '0' + ( current_wpm   /= 10) % 10;
//     wpm_str[0] = '0' + current_wpm / 10;
//     oled_write(wpm_str, false);

//     oled_set_cursor(0,6);
//     oled_write("  wpm", false);
// }

void oled_task_user(void) {
    if (is_keyboard_master()) {
        // oled_render_layer_state();
        // oled_render_keylog();
        current_wpm   = get_current_wpm();
        led_usb_state = host_keyboard_led_state();
        render_luna(0, 13);
    } else {
        render_bongocat();
    }
}
#endif  // OLED_DRIVER_ENABLE

LEADER_EXTERNS();

void matrix_scan_user(void) {
    LEADER_DICTIONARY() {
        leading = false;
        leader_end();

        SEQ_TWO_KEYS(KC_W, KC_E) { SEND_STRING("mail@valmik.in"); }

        SEQ_TWO_KEYS(KC_G, KC_E) { SEND_STRING("valmik2k2@gmail.com"); }

        SEQ_ONE_KEY(KC_ESC) {
            {
                register_code(KC_LALT);
                register_code(KC_F4);
                unregister_code(KC_LALT);
                unregister_code(KC_F4);
            }
        }
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_LCTL:
        case KC_RCTL:
            if (record->event.pressed) {
                isSneaking = true;
            } else {
                isSneaking = false;
            }
            break;
        case KC_SPC:
            if (record->event.pressed) {
                isJumping  = true;
                showedJump = false;
            } else {
                isJumping = false;
            }
            break;
    }
    return true;
}
