// Copyright 2022 Ani (@Ani)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once



/* key matrix size */
#define MATRIX_ROWS 5
#define MATRIX_COLS 6

#define MATRIX_ROW_PINS { GP8, GP9, GP10, GP11, GP12 }
#define MATRIX_COL_PINS { GP2, GP3, GP4, GP5, GP6, GP7 } 






#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_LED GP13
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U
/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
//#define NO_DEBUG

/* disable print */
//#define NO_PRINT

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT
