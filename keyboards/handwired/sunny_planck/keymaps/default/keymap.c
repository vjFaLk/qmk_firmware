#include "kb.h"


enum {
    ESC_ALTF4 = 0 // Single tap = ESCAPE; Triple tap = ALT+F4
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

	LAYOUT_1x2uC(
		TD(ESC_ALTF4), KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
		KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
		KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, RSFT_T(KC_ENT),
		KC_LEAD, KC_LCTL, KC_LALT, KC_LGUI, MO(1), KC_SPC, MO(2), KC_LEFT, KC_DOWN, KC_UP, KC_RGHT),

	LAYOUT_1x2uC(
		KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_DEL,
		KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE,
		KC_TRNS, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, LSFT(KC_NUHS), LSFT(KC_NUBS), KC_HOME, KC_END, KC_TRNS,
		BL_BRTG, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY),

	LAYOUT_1x2uC(
		KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_TRNS,
		KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,
		KC_TRNS, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_NUHS, KC_NUBS, KC_PGUP, KC_PGDN, KC_TRNS,
		BL_STEP, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_MNXT, KC_VOLD, KC_VOLU, KC_MPLY),

    LAYOUT_1x2uC(
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
		KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS)
};

void escape_and_altf4_tapdance(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
    TAP(KC_ESC);
  }

  else if (state->count == 3) {
    TAP_WITH_MOD(KC_LALT, KC_F4);
  }
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [ESC_ALTF4] = ACTION_TAP_DANCE_FN(escape_and_altf4_tapdance)
};

LEADER_EXTERNS();

void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();

    SEQ_ONE_KEY(KC_T) {
      // When I press KC_LEAD and then T, this sends CTRL + SHIFT + T
      SEND_STRING(SS_LCTRL(SS_LSFT("t")));
    }

    SEQ_ONE_KEY(KC_P) {
      // When I press KC_LEAD and then P, this sends CTRL + SHIFT + P
      SEND_STRING(SS_LCTRL(SS_LSFT("p")));
    }

    SEQ_ONE_KEY(KC_RGHT) {
      // When I press KC_LEAD and then right arrow, this sends CTRL + SHIFT + Right Arrow
      SEND_STRING(SS_LCTRL(SS_LSFT(SS_TAP(X_RGHT))));
    }

    SEQ_TWO_KEYS(KC_W, KC_E) {
      // When I press KC_LEAD and then right arrow, this sends CTRL + SHIFT + Right Arrow
      SEND_STRING("mail@valmik.in");
    }

    SEQ_TWO_KEYS(KC_G, KC_E) {
      // When I press KC_LEAD and then right arrow, this sends CTRL + SHIFT + Right Arrow
      SEND_STRING("valmik2k2@gmail.com");
    }
  }
}
