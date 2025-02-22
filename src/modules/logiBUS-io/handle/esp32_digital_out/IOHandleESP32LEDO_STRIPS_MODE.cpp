/*******************************************************************************
 * Copyright (c) 2021, 2025 Jonathan Lainer (kontakt@lainer.co.at), HR Agrartechnik GmbH
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Jonathan Lainer - Initial implementation.
 *   Franz Höpfinger
 *******************************************************************************/

#include "IOHandleESP32LEDO_STRIPS_MODE.h"

#include <cstdbool>
#include <cstdint>


// for LED
#include "led_indicator.h"
#include "led_strip_types.h"

#include "led_strip_io.h"
#include "LED_COLOURS_gcf.h"

#define WS2812_STRIPS_NUM     1

#define LED_STRIP_RMT_RES_HZ  (10 * 1000 * 1000)  // 10MHz


typedef enum {
  BLINK_INVALID,
  BLINK_RED_5HZ,      //highest Priority
  BLINK_RED_2HZ,
  BLINK_RED_1HZ,
  BLINK_RED_05HZ,
  LED_RED_ON,
  BLINK_YELLOW_5HZ,
  BLINK_YELLOW_2HZ,
  BLINK_YELLOW_1HZ,
  BLINK_YELLOW_05HZ,
  LED_YELLOW_ON,
  BLINK_ORANGE_5HZ,
  BLINK_ORANGE_2HZ,
  BLINK_ORANGE_1HZ,
  BLINK_ORANGE_05HZ,
  LED_ORANGE_ON,
  BLINK_GREEN_5HZ,
  BLINK_GREEN_2HZ,
  BLINK_GREEN_1HZ,
  BLINK_GREEN_05HZ,
  LED_GREEN_ON,
  BLINK_BLUE_5HZ,
  BLINK_BLUE_2HZ,
  BLINK_BLUE_1HZ,
  BLINK_BLUE_05HZ,
  LED_BLUE_ON,
  BLINK_WHITE_5HZ,
  BLINK_WHITE_2HZ,
  BLINK_WHITE_1HZ,
  BLINK_WHITE_05HZ,
  LED_WHITE_ON,
  BLINK_MAGENTA_5HZ,
  BLINK_MAGENTA_2HZ,
  BLINK_MAGENTA_1HZ,
  BLINK_MAGENTA_05HZ,
  LED_MAGENTA_ON,
  BLINK_CYAN_5HZ,
  BLINK_CYAN_2HZ,
  BLINK_CYAN_1HZ,
  BLINK_CYAN_05HZ,
  LED_CYAN_ON,
  BLINK_OFF, //lowest Priority
  BLINK_NUM,
} led_blink_type_t;



/**
 * off
 *
 */
static const blink_step_t blink_off[] = {
    {LED_BLINK_HOLD, LED_STATE_OFF, 100},
    {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 5 Hz
 * means a T of 0.2s
 * so every half is 0.1s
 *
 */
static const blink_step_t blink_red_5HZ[] = {
  /*!< Set color to red by R:255 G:0 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 0, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 100},
  {LED_BLINK_HOLD, LED_STATE_OFF, 100},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 2 Hz
 * means a T of 0.5s
 * so every half is 0.25s
 *
 */
static const blink_step_t blink_red_2HZ[] = {
  /*!< Set color to red by R:255 G:0 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 0, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 250},
  {LED_BLINK_HOLD, LED_STATE_OFF, 250},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 1 Hz
 * means a T of 1s
 * so every half is 0.5s
 *
 */
static const blink_step_t blink_red_1HZ[] = {
  /*!< Set color to red by R:255 G:0 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 0, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 500},
  {LED_BLINK_HOLD, LED_STATE_OFF, 500},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 0.5 Hz
 * means a T of 2s
 * so every half is 1s
 *
 */
static const blink_step_t blink_red_05HZ[] = {
  /*!< Set color to red by R:255 G:0 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 0, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_HOLD, LED_STATE_OFF, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Just ON !
 *
 */
static const blink_step_t just_red_ON[] = {
  /*!< Set color to red by R:255 G:0 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 0, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Blink with 5 Hz
 * means a T of 0.2s
 * so every half is 0.1s
 *
 */
static const blink_step_t blink_yellow_5HZ[] = {
  /*!< Set color to yellow by R:255 G:255 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 255, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 100},
  {LED_BLINK_HOLD, LED_STATE_OFF, 100},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 2 Hz
 * means a T of 0.5s
 * so every half is 0.25s
 *
 */
static const blink_step_t blink_yellow_2HZ[] = {
  /*!< Set color to yellow by R:255 G:255 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 255, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 250},
  {LED_BLINK_HOLD, LED_STATE_OFF, 250},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 1 Hz
 * means a T of 1s
 * so every half is 0.5s
 *
 */
static const blink_step_t blink_yellow_1HZ[] = {
  /*!< Set color to yellow by R:255 G:255 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 255, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 500},
  {LED_BLINK_HOLD, LED_STATE_OFF, 500},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 0.5 Hz
 * means a T of 2s
 * so every half is 1s
 *
 */
static const blink_step_t blink_yellow_05HZ[] = {
  /*!< Set color to yellow by R:255 G:255 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 255, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_HOLD, LED_STATE_OFF, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Just ON !
 *
 */
static const blink_step_t just_yellow_ON[] = {
  /*!< Set color to yellow by R:255 G:255 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 255, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Blink with 5 Hz
 * means a T of 0.2s
 * so every half is 0.1s
 *
 */
static const blink_step_t blink_orange_5HZ[] = {
  /*!< Set color to orange by R:255 G:127 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 127, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 100},
  {LED_BLINK_HOLD, LED_STATE_OFF, 100},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 2 Hz
 * means a T of 0.5s
 * so every half is 0.25s
 *
 */
static const blink_step_t blink_orange_2HZ[] = {
  /*!< Set color to orange by R:255 G:127 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 127, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 250},
  {LED_BLINK_HOLD, LED_STATE_OFF, 250},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 1 Hz
 * means a T of 1s
 * so every half is 0.5s
 *
 */
static const blink_step_t blink_orange_1HZ[] = {
  /*!< Set color to orange by R:255 G:127 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 127, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 500},
  {LED_BLINK_HOLD, LED_STATE_OFF, 500},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 0.5 Hz
 * means a T of 2s
 * so every half is 1s
 *
 */
static const blink_step_t blink_orange_05HZ[] = {
  /*!< Set color to orange by R:255 G:127 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 127, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_HOLD, LED_STATE_OFF, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Just ON !
 *
 */
static const blink_step_t just_orange_ON[] = {
  /*!< Set color to orange by R:255 G:127 B:0 */
  {LED_BLINK_RGB, SET_RGB(255, 127, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Blink with 5 Hz
 * means a T of 0.2s
 * so every half is 0.1s
 *
 */
static const blink_step_t blink_green_5HZ[] = {
  /*!< Set color to green by R:0 G:255 B:0 */
  {LED_BLINK_RGB, SET_RGB(0, 255, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 100},
  {LED_BLINK_HOLD, LED_STATE_OFF, 100},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 2 Hz
 * means a T of 0.5s
 * so every half is 0.25s
 *
 */
static const blink_step_t blink_green_2HZ[] = {
  /*!< Set color to green by R:0 G:255 B:0 */
  {LED_BLINK_RGB, SET_RGB(0, 255, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 250},
  {LED_BLINK_HOLD, LED_STATE_OFF, 250},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 1 Hz
 * means a T of 1s
 * so every half is 0.5s
 *
 */
static const blink_step_t blink_green_1HZ[] = {
  /*!< Set color to green by R:0 G:255 B:0 */
  {LED_BLINK_RGB, SET_RGB(0, 255, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 500},
  {LED_BLINK_HOLD, LED_STATE_OFF, 500},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 0.5 Hz
 * means a T of 2s
 * so every half is 1s
 *
 */
static const blink_step_t blink_green_05HZ[] = {
  /*!< Set color to green by R:0 G:255 B:0 */
  {LED_BLINK_RGB, SET_RGB(0, 255, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_HOLD, LED_STATE_OFF, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Just ON !
 *
 */
static const blink_step_t just_green_ON[] = {
  /*!< Set color to green by R:0 G:255 B:0 */
  {LED_BLINK_RGB, SET_RGB(0, 255, 0), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Blink with 5 Hz
 * means a T of 0.2s
 * so every half is 0.1s
 *
 */
static const blink_step_t blink_blue_5HZ[] = {
  /*!< Set color to blue by R:0 G:0 B:255 */
  {LED_BLINK_RGB, SET_RGB(0, 0, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 100},
  {LED_BLINK_HOLD, LED_STATE_OFF, 100},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 2 Hz
 * means a T of 0.5s
 * so every half is 0.25s
 *
 */
static const blink_step_t blink_blue_2HZ[] = {
  /*!< Set color to blue by R:0 G:0 B:255 */
  {LED_BLINK_RGB, SET_RGB(0, 0, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 250},
  {LED_BLINK_HOLD, LED_STATE_OFF, 250},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 1 Hz
 * means a T of 1s
 * so every half is 0.5s
 *
 */
static const blink_step_t blink_blue_1HZ[] = {
  /*!< Set color to blue by R:0 G:0 B:255 */
  {LED_BLINK_RGB, SET_RGB(0, 0, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 500},
  {LED_BLINK_HOLD, LED_STATE_OFF, 500},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 0.5 Hz
 * means a T of 2s
 * so every half is 1s
 *
 */
static const blink_step_t blink_blue_05HZ[] = {
  /*!< Set color to blue by R:0 G:0 B:255 */
  {LED_BLINK_RGB, SET_RGB(0, 0, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_HOLD, LED_STATE_OFF, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Just ON !
 *
 */
static const blink_step_t just_blue_ON[] = {
  /*!< Set color to blue by R:0 G:0 B:255 */
  {LED_BLINK_RGB, SET_RGB(0, 0, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Blink with 5 Hz
 * means a T of 0.2s
 * so every half is 0.1s
 *
 */
static const blink_step_t blink_white_5HZ[] = {
  /*!< Set color to white by R:255 G:255 B:255 */
  {LED_BLINK_RGB, SET_RGB(255, 255, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 100},
  {LED_BLINK_HOLD, LED_STATE_OFF, 100},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 2 Hz
 * means a T of 0.5s
 * so every half is 0.25s
 *
 */
static const blink_step_t blink_white_2HZ[] = {
  /*!< Set color to white by R:255 G:255 B:255 */
  {LED_BLINK_RGB, SET_RGB(255, 255, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 250},
  {LED_BLINK_HOLD, LED_STATE_OFF, 250},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 1 Hz
 * means a T of 1s
 * so every half is 0.5s
 *
 */
static const blink_step_t blink_white_1HZ[] = {
  /*!< Set color to white by R:255 G:255 B:255 */
  {LED_BLINK_RGB, SET_RGB(255, 255, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 500},
  {LED_BLINK_HOLD, LED_STATE_OFF, 500},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 0.5 Hz
 * means a T of 2s
 * so every half is 1s
 *
 */
static const blink_step_t blink_white_05HZ[] = {
  /*!< Set color to white by R:255 G:255 B:255 */
  {LED_BLINK_RGB, SET_RGB(255, 255, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_HOLD, LED_STATE_OFF, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Just ON !
 *
 */
static const blink_step_t just_white_ON[] = {
  /*!< Set color to white by R:255 G:255 B:255 */
  {LED_BLINK_RGB, SET_RGB(255, 255, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Blink with 5 Hz
 * means a T of 0.2s
 * so every half is 0.1s
 *
 */
static const blink_step_t blink_magenta_5HZ[] = {
  /*!< Set color to magenta by R:255 G:0 B:255 */
  {LED_BLINK_RGB, SET_RGB(255, 0, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 100},
  {LED_BLINK_HOLD, LED_STATE_OFF, 100},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 2 Hz
 * means a T of 0.5s
 * so every half is 0.25s
 *
 */
static const blink_step_t blink_magenta_2HZ[] = {
  /*!< Set color to magenta by R:255 G:0 B:255 */
  {LED_BLINK_RGB, SET_RGB(255, 0, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 250},
  {LED_BLINK_HOLD, LED_STATE_OFF, 250},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 1 Hz
 * means a T of 1s
 * so every half is 0.5s
 *
 */
static const blink_step_t blink_magenta_1HZ[] = {
  /*!< Set color to magenta by R:255 G:0 B:255 */
  {LED_BLINK_RGB, SET_RGB(255, 0, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 500},
  {LED_BLINK_HOLD, LED_STATE_OFF, 500},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 0.5 Hz
 * means a T of 2s
 * so every half is 1s
 *
 */
static const blink_step_t blink_magenta_05HZ[] = {
  /*!< Set color to magenta by R:255 G:0 B:255 */
  {LED_BLINK_RGB, SET_RGB(255, 0, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_HOLD, LED_STATE_OFF, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Just ON !
 *
 */
static const blink_step_t just_magenta_ON[] = {
  /*!< Set color to magenta by R:255 G:0 B:255 */
  {LED_BLINK_RGB, SET_RGB(255, 0, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Blink with 5 Hz
 * means a T of 0.2s
 * so every half is 0.1s
 *
 */
static const blink_step_t blink_cyan_5HZ[] = {
  /*!< Set color to cyan by R:0 G:255 B:255 */
  {LED_BLINK_RGB, SET_RGB(0, 255, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 100},
  {LED_BLINK_HOLD, LED_STATE_OFF, 100},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 2 Hz
 * means a T of 0.5s
 * so every half is 0.25s
 *
 */
static const blink_step_t blink_cyan_2HZ[] = {
  /*!< Set color to cyan by R:0 G:255 B:255 */
  {LED_BLINK_RGB, SET_RGB(0, 255, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 250},
  {LED_BLINK_HOLD, LED_STATE_OFF, 250},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 1 Hz
 * means a T of 1s
 * so every half is 0.5s
 *
 */
static const blink_step_t blink_cyan_1HZ[] = {
  /*!< Set color to cyan by R:0 G:255 B:255 */
  {LED_BLINK_RGB, SET_RGB(0, 255, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 500},
  {LED_BLINK_HOLD, LED_STATE_OFF, 500},
  {LED_BLINK_LOOP, 0, 0},
};

/**
 * Blink with 0.5 Hz
 * means a T of 2s
 * so every half is 1s
 *
 */
static const blink_step_t blink_cyan_05HZ[] = {
  /*!< Set color to cyan by R:0 G:255 B:255 */
  {LED_BLINK_RGB, SET_RGB(0, 255, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_HOLD, LED_STATE_OFF, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


/**
 * Just ON !
 *
 */
static const blink_step_t just_cyan_ON[] = {
  /*!< Set color to cyan by R:0 G:255 B:255 */
  {LED_BLINK_RGB, SET_RGB(0, 255, 255), 0},
  {LED_BLINK_HOLD, LED_STATE_ON, 1000},
  {LED_BLINK_LOOP, 0, 0},
};


//Priorities:


static blink_step_t const *led_blink_lst[] = {
  [BLINK_INVALID]       = nullptr,
  [BLINK_RED_5HZ]       = blink_red_5HZ,  //highest Priority
  [BLINK_RED_2HZ]       = blink_red_2HZ,
  [BLINK_RED_1HZ]       = blink_red_1HZ,
  [BLINK_RED_05HZ]      = blink_red_05HZ,
  [LED_RED_ON]          = just_red_ON,
  [BLINK_YELLOW_5HZ]    = blink_yellow_5HZ,
  [BLINK_YELLOW_2HZ]    = blink_yellow_2HZ,
  [BLINK_YELLOW_1HZ]    = blink_yellow_1HZ,
  [BLINK_YELLOW_05HZ]   = blink_yellow_05HZ,
  [LED_YELLOW_ON]       = just_yellow_ON,
  [BLINK_ORANGE_5HZ]    = blink_orange_5HZ,
  [BLINK_ORANGE_2HZ]    = blink_orange_2HZ,
  [BLINK_ORANGE_1HZ]    = blink_orange_1HZ,
  [BLINK_ORANGE_05HZ]   = blink_orange_05HZ,
  [LED_ORANGE_ON]       = just_orange_ON,
  [BLINK_GREEN_5HZ]     = blink_green_5HZ,
  [BLINK_GREEN_2HZ]     = blink_green_2HZ,
  [BLINK_GREEN_1HZ]     = blink_green_1HZ,
  [BLINK_GREEN_05HZ]    = blink_green_05HZ,
  [LED_GREEN_ON]        = just_green_ON,
  [BLINK_BLUE_5HZ]      = blink_blue_5HZ,
  [BLINK_BLUE_2HZ]      = blink_blue_2HZ,
  [BLINK_BLUE_1HZ]      = blink_blue_1HZ,
  [BLINK_BLUE_05HZ]     = blink_blue_05HZ,
  [LED_BLUE_ON]         = just_blue_ON,
  [BLINK_WHITE_5HZ]     = blink_white_5HZ,
  [BLINK_WHITE_2HZ]     = blink_white_2HZ,
  [BLINK_WHITE_1HZ]     = blink_white_1HZ,
  [BLINK_WHITE_05HZ]    = blink_white_05HZ,
  [LED_WHITE_ON]        = just_white_ON,
  [BLINK_MAGENTA_5HZ]   = blink_magenta_5HZ,
  [BLINK_MAGENTA_2HZ]   = blink_magenta_2HZ,
  [BLINK_MAGENTA_1HZ]   = blink_magenta_1HZ,
  [BLINK_MAGENTA_05HZ]  = blink_magenta_05HZ,
  [LED_MAGENTA_ON]      = just_magenta_ON,
  [BLINK_CYAN_5HZ]      = blink_cyan_5HZ,
  [BLINK_CYAN_2HZ]      = blink_cyan_2HZ,
  [BLINK_CYAN_1HZ]      = blink_cyan_1HZ,
  [BLINK_CYAN_05HZ]     = blink_cyan_05HZ,
  [LED_CYAN_ON]         = just_cyan_ON,
  [BLINK_OFF]           = blink_off,  //lowest Priority
  [BLINK_NUM]           = nullptr,
};


led_indicator_handle_t IOHandleESP32LEDO_STRIPS_MODE::led_handles[static_cast<int32_t>(logiBUSIOEnums::LEDStripsPinNumber::LEDStripsNumberOfPins)] = {nullptr};



IOHandleESP32LEDO_STRIPS_MODE::~IOHandleESP32LEDO_STRIPS_MODE(){
  //TODO led_indicator_delete
  IOHandleESP32LEDO_STRIPS_MODE::led_handles[static_cast<int32_t>(this->mPin)] = nullptr;
}





IOHandleESP32LEDO_STRIPS_MODE::IOHandleESP32LEDO_STRIPS_MODE(
    logiBUSIODeviceController            *paDeviceCtrl,
    forte::core::io::IOMapper::Direction paDirection,
    CIEC_ANY::EDataTypeID                paDataType,
    logiBUSIOEnums::LEDStripsPinNumber    paPin,
    logiBUSIODeviceController::HandleType paHandleType,
    uint16_t                             paColour
    )
    : forte::core::io::IOHandle(static_cast<forte::core::io::IODeviceController*>(paDeviceCtrl), paDirection, paDataType)
    , mPin(paPin)
    , mHandleType(paHandleType)
    , gpio_num(GPIO_NUM_NC)
    , mColour(paColour)
  {
    switch (paPin) {
      case logiBUSIOEnums::LEDStripsPinNumber::PinS00:        gpio_num = LED_IO;        break;
  }


  if(gpio_num > GPIO_NUM_NC) {
    if(IOHandleESP32LEDO_STRIPS_MODE::led_handles[static_cast<int32_t>(this->mPin)] == nullptr) {

      led_strip_config_t strip_config = {
        .strip_gpio_num = gpio_num,              // The GPIO that connected to the LED strip's data line
        .max_leds = WS2812_STRIPS_NUM,                  // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB,       // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812,                  // LED strip model
      };

      strip_config.flags.invert_out = false;                      // whether to invert the output signal

      // LED strip backend configuration: RMT
      led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,        // different clock source can lead to different power consumption
        .resolution_hz = LED_STRIP_RMT_RES_HZ, // RMT counter clock frequency
      };

      rmt_config.flags.with_dma = false;               // DMA feature is available on ESP target like ESP32-S3

      led_indicator_strips_config_t strips_config = {
        .led_strip_cfg = strip_config,
        .led_strip_driver = LED_STRIP_RMT,
        .led_strip_rmt_cfg = rmt_config,
      };

      led_indicator_config_t config = {
        .mode = LED_STRIPS_MODE,
        .led_indicator_strips_config = &strips_config,
        .blink_lists = led_blink_lst,
        .blink_list_num = BLINK_NUM,
      };

      IOHandleESP32LEDO_STRIPS_MODE::led_handles[static_cast<int32_t>(this->mPin)] = led_indicator_create(&config);
      led_indicator_start(IOHandleESP32LEDO_STRIPS_MODE::led_handles[static_cast<int32_t>(this->mPin)], BLINK_OFF);
    }
  }
}


void IOHandleESP32LEDO_STRIPS_MODE::get(CIEC_ANY &paState) {
}

int IOHandleESP32LEDO_STRIPS_MODE::set_red(int blink_type) {
  switch(mHandleType){
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_5HZ:      blink_type = BLINK_RED_5HZ;       break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_2HZ:      blink_type = BLINK_RED_2HZ;       break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_1HZ:      blink_type = BLINK_RED_1HZ;       break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_05HZ:     blink_type = BLINK_RED_05HZ;      break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_ON:       blink_type = LED_RED_ON;          break;
    default:                                                            blink_type = BLINK_INVALID;       break;
  }
  return blink_type;
}

int IOHandleESP32LEDO_STRIPS_MODE::set_yellow(int blink_type) {
  switch(mHandleType){
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_5HZ:      blink_type = BLINK_YELLOW_5HZ;    break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_2HZ:      blink_type = BLINK_YELLOW_2HZ;    break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_1HZ:      blink_type = BLINK_YELLOW_1HZ;    break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_05HZ:     blink_type = BLINK_YELLOW_05HZ;   break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_ON:       blink_type = LED_YELLOW_ON;       break;
    default:                                                            blink_type = BLINK_INVALID;       break;
  }
  return blink_type;
}

int IOHandleESP32LEDO_STRIPS_MODE::set_orange(int blink_type) {
  switch(mHandleType){
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_5HZ:      blink_type = BLINK_ORANGE_5HZ;    break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_2HZ:      blink_type = BLINK_ORANGE_2HZ;    break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_1HZ:      blink_type = BLINK_ORANGE_1HZ;    break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_05HZ:     blink_type = BLINK_ORANGE_05HZ;   break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_ON:       blink_type = LED_ORANGE_ON;       break;
    default:                                                            blink_type = BLINK_INVALID;       break;
  }
  return blink_type;
}

int IOHandleESP32LEDO_STRIPS_MODE::set_green(int blink_type) {
  switch(mHandleType){
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_5HZ:      blink_type = BLINK_GREEN_5HZ;     break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_2HZ:      blink_type = BLINK_GREEN_2HZ;     break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_1HZ:      blink_type = BLINK_GREEN_1HZ;     break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_05HZ:     blink_type = BLINK_GREEN_05HZ;    break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_ON:       blink_type = LED_GREEN_ON;        break;
    default:                                                            blink_type = BLINK_INVALID;       break;
  }
  return blink_type;
}

int IOHandleESP32LEDO_STRIPS_MODE::set_blue(int blink_type) {
  switch(mHandleType){
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_5HZ:      blink_type = BLINK_BLUE_5HZ;      break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_2HZ:      blink_type = BLINK_BLUE_2HZ;      break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_1HZ:      blink_type = BLINK_BLUE_1HZ;      break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_05HZ:     blink_type = BLINK_BLUE_05HZ;     break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_ON:       blink_type = LED_BLUE_ON;         break;
    default:                                                            blink_type = BLINK_INVALID;       break;
  }
  return blink_type;
}

int IOHandleESP32LEDO_STRIPS_MODE::set_white(int blink_type) {
  switch(mHandleType){
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_5HZ:      blink_type = BLINK_WHITE_5HZ;     break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_2HZ:      blink_type = BLINK_WHITE_2HZ;     break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_1HZ:      blink_type = BLINK_WHITE_1HZ;     break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_05HZ:     blink_type = BLINK_WHITE_05HZ;    break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_ON:       blink_type = LED_WHITE_ON;        break;
    default:                                                            blink_type = BLINK_INVALID;       break;
  }
  return blink_type;
}

int IOHandleESP32LEDO_STRIPS_MODE::set_magenta(int blink_type) {
  switch(mHandleType){
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_5HZ:      blink_type = BLINK_MAGENTA_5HZ;   break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_2HZ:      blink_type = BLINK_MAGENTA_2HZ;   break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_1HZ:      blink_type = BLINK_MAGENTA_1HZ;   break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_05HZ:     blink_type = BLINK_MAGENTA_05HZ;  break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_ON:       blink_type = LED_MAGENTA_ON;      break;
    default:                                                            blink_type = BLINK_INVALID;       break;
  }
  return blink_type;
}

int IOHandleESP32LEDO_STRIPS_MODE::set_cyan(int blink_type) {
  switch(mHandleType){
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_5HZ:      blink_type = BLINK_CYAN_5HZ;      break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_2HZ:      blink_type = BLINK_CYAN_2HZ;      break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_1HZ:      blink_type = BLINK_CYAN_1HZ;      break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_05HZ:     blink_type = BLINK_CYAN_05HZ;     break;
    case logiBUSIODeviceController::HandleType::ESP32LED_STRIP_ON:       blink_type = LED_CYAN_ON;         break;
    default:                                                            blink_type = BLINK_INVALID;       break;
  }
  return blink_type;
}

void IOHandleESP32LEDO_STRIPS_MODE::set(const CIEC_ANY &paState) {
  int targetState = (true == static_cast<const CIEC_BOOL &>(paState)) ? 1 : 0;
  int blink_type = BLINK_INVALID;
  switch(mColour){
    case c_LED_RED:         blink_type = set_red     (blink_type);    break;
    case c_LED_YELLOW:      blink_type = set_yellow  (blink_type);    break;
    case c_LED_ORANGE:      blink_type = set_orange  (blink_type);    break;
    case c_LED_GREEN:       blink_type = set_green   (blink_type);    break;
    case c_LED_BLUE:        blink_type = set_blue    (blink_type);    break;
    case c_LED_WHITE:       blink_type = set_white   (blink_type);    break;
    case c_LED_MAGENTA:     blink_type = set_magenta (blink_type);    break;
    case c_LED_CYAN:        blink_type = set_cyan    (blink_type);    break;
    default:                blink_type = BLINK_INVALID;               break;
  }


  if(blink_type > BLINK_INVALID) {
    switch(targetState){
      case 1:
        led_indicator_start (IOHandleESP32LEDO_STRIPS_MODE::led_handles[static_cast<int32_t>(this->mPin)], blink_type);
        break;
      case 0:
        led_indicator_stop (IOHandleESP32LEDO_STRIPS_MODE::led_handles[static_cast<int32_t>(this->mPin)], blink_type);
        break;
      default:
        break;
    }
  }
}
