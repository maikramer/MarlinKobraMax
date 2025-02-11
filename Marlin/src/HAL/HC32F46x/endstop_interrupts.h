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
 * Endstop interrupts for Libmaple HC32F46x based on STM32F1 based targets.
 *
 * On STM32F, all pins support external interrupt capability.
 * Any pin can be used for external interrupts, but there are some restrictions.
 * At most 16 different external interrupts can be used at one time.
 * Further, you can’t just pick any 16 pins to use. This is because every pin on
 * the STM32 connects to what is called an EXTI line, and only one pin per EXTI
 * line can be used for external interrupts at a time Check the Reference Manual
 * of the MCU to confirm which line is used by each pin
 */

/**
 * Endstop Interrupts
 *
 * Without endstop interrupts the endstop pins must be polled continually in
 * the temperature-ISR via endstops.update(), most of the time finding no
 * change. With this feature endstops.update() is called only when we know that
 * at least one endstop has changed state, saving valuable CPU cycles.
 *
 * This feature only works when all used endstop pins can generate an 'external
 * interrupt'.
 *
 * Test whether pins issue interrupts on your board by flashing
 * 'pin_interrupt_test.ino'. (Located in
 * Marlin/buildroot/share/pin_interrupt_test/pin_interrupt_test.ino)
 */

void setup_endstop_interrupts();
