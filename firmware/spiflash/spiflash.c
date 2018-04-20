/*
 * Copyright 2010 - 2012 Michael Ossmann
 *
 * This file is part of HackRF.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#include <libopencm3/lpc43xx/gpio.h>
#include <libopencm3/lpc43xx/scu.h>

#include "hackrf_core.h"
#include "w25q80bv.h"


int main(void)
{
	uint8_t i;

	uint8_t buf[512];

	pin_setup();

	enable_1v8_power();

	cpu_clock_init();

	for (i = 0; i < 255; i++) {
		buf[i] = i;
	}



	/* program test data to SPI flash */
	//for (i = 0; i < 515; i++)
	//	buf[i] = (i * 3) & 0xFF;


	w25q80bv_setup();
	w25q80bv_chip_erase();



	w25q80bv_program(790, 255, &buf[0]);

	blink(200000);






	return 0;
}
