// nPDF timer routines
// Copyright (C) 2014  Legimet
//
// This file is part of nPDF.
//
// nPDF is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// nPDF is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with nPDF.  If not, see <http://www.gnu.org/licenses/>.

#include <cstdint>
#include <cstdio>
#include <libndls.h>

/*
   Classic Nspires use a timer documented on Hackspire, and CX Nspires use the ARM SP804 timer, documented
   on ARM's website.

Links:
Classic: http://hackspire.unsads.com/wiki/index.php/Memory-mapped_I/O_ports#900D0000_-_Second_timer
CX: http://www.omnimaga.org/calculator-c-language/issues-with-the-ti-nspire-cx-timer/
CX: http://infocenter.arm.com/help/topic/com.arm.doc.ddi0271d/DDI0271.pdf
*/

namespace Timer {
	volatile uint32_t* const busAccess = reinterpret_cast<volatile uint32_t*>(0x900B0018);
	volatile uint32_t* const control = reinterpret_cast<volatile uint32_t*>(0x900C0008);
	volatile uint32_t* const cxLoad = reinterpret_cast<volatile uint32_t*>(0x900C0000);
	volatile uint16_t* const classicDivider = reinterpret_cast<volatile uint16_t*>(0x900C0010);
	volatile uint32_t* value = nullptr;

	// See the nSDL timer code
	void init() {
		*busAccess &= ~(1 << 11); // Enable bus access
		if (has_colors) {
			value = reinterpret_cast<uint32_t*>(0x900C0004);
			*reinterpret_cast<volatile uint32_t*>(0x900C0080) = 0xA; // This is an Nspire-specific register to set the 32768Hz timer speed
			*control = (*control & 0b10000011) | 0b10010011; // Avoid writing bit 4 10000011
			*cxLoad = 0xFFFFFFFF;
		} else {
			value = reinterpret_cast<uint32_t*>(0x900C0000);
			*control = 0;
			*classicDivider = 1;
			*value = 0xFFFFFFFF;
		}
	}

	void start(uint32_t startVal) {
		if (has_colors) {
			*cxLoad = 33 * startVal;
		} else {
			*value = 33 * startVal;
			*control = 0;
		}
	}

	void stop() {
		start(0);
	}

	bool done() {
		return !(*value);
	}
}
