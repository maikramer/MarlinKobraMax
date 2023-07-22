/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 * Copyright (c) 2017 Victor Perez
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Fast I/O interfaces for HC32F46x
 * These use GPIO functions instead of Direct Port Manipulation, as on AVR.
 */
#include <wiring_digital.h>
#include <gpio/gpio.h>

#define _READ(IO) (GPIO_GetBit(IO) ? HIGH : LOW)
#define _WRITE(IO, V) (((V) > 0) ? GPIO_SetBits(IO) : GPIO_ResetBits(IO))
#define _TOGGLE(IO) (GPIO_Toggle(IO))


#define _GET_MODE(IO)           getPinMode(IO)
#define _SET_MODE(IO, M)        pinMode(IO, M)
#define _SET_OUTPUT(IO)         pinMode(IO, OUTPUT)                               //!< Output Push Pull Mode & GPIO_NOPULL
#define _SET_OUTPUT_OD(IO)      pinMode(IO, OUTPUT_OPEN_DRAIN)

#define WRITE(IO, V)             _WRITE(IO,V)
#define READ(IO)                _READ(IO)
#define TOGGLE(IO)              _TOGGLE(IO)

#define OUT_WRITE(IO, V)         do{ _SET_OUTPUT(IO); WRITE(IO,V); }while(0)
#define OUT_WRITE_OD(IO, V)      do{ _SET_OUTPUT_OD(IO); WRITE(IO,V); }while(0)

#define SET_INPUT(IO)           _SET_MODE(IO, INPUT)                              //!< Input Floating Mode
#define SET_INPUT_PULLUP(IO)    _SET_MODE(IO, INPUT_PULLUP)                       //!< Input with Pull-up activation
#define SET_INPUT_PULLDOWN(IO)  _SET_MODE(IO, INPUT_PULLDOWN)                     //!< Input with Pull-down activation
#define SET_OUTPUT(IO)          _SET_OUTPUT(IO)
#define SET_PWM(IO)             _SET_MODE(IO, PWM)

#define IS_INPUT(IO) (_GET_MODE(IO) == INPUT || _GET_MODE(IO) == INPUT_ANALOG || _GET_MODE(IO) == INPUT_PULLUP || \
_GET_MODE(IO) == INPUT_PULLDOWN || _GET_MODE(IO) == INPUT_FLOATING)

#define IS_OUTPUT(IO)   (_GET_MODE(IO) == OUTPUT || _GET_MODE(IO) == OUTPUT_OPEN_DRAIN)

#define PWM_PIN(IO) (true)

#define extDigitalRead(IO) digitalRead(IO)
#define extDigitalWrite(IO, V) digitalWrite(IO, V)
