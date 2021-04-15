/*

Copyright 2021 Luke A.C.A. Rieff

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*/

#include "spfp.h"

/**************************************************************
 * SPFP Protocol Itself
 **************************************************************/

/// Adds a single byte to specified checksum.
uint8_t spfp_checksum_add (uint8_t cs, uint8_t val) {
	if (((uint16_t) cs) + ((uint16_t) val) > 0xFF) {
		++cs;
	}

	cs += val;

	return cs;
}

/// Calculates the checksum for specified packet.
uint8_t spfp_calc_checksum (const spfp_packet_t *packet) {
	const uint8_t *p = ((uint8_t *) packet);
	uint8_t cs = 0x0000;

	// Loops over the total packet, and adds each byte to the 
	//  final checksum.
	for (uint8_t i = 0; i < packet->len; ++i) {
		cs = spfp_checksum_add (cs, p[i]);
	}

	return ~cs;
}

/// The dummy write method, which can be overwritten by implementation.
extern void __spfp_write_byte (uint8_t byte, void *u);

/// Writes an packet to the other device.
void spfp_write_packet (const spfp_packet_t *packet, void *u) {
	const uint8_t *p = (uint8_t *) packet;

	// Writes the start flag.
	__spfp_write_byte (SPFP_START_FLAG, u);

	// Loops over the total packet and writes each byte
	//  to the other device, and adds required escapes.
	for (uint16_t i = 0; i < packet->len; ++i) {
		uint8_t byte = p[i];

		// Checks if the current byte is a control flag, if so we first want
		//  to send an escape byte to indicate that it's not an actual control flag.
		if (byte >= SPFP_START_FLAG && byte <= SPFP_ESCAPE_FLAG) {
			__spfp_write_byte (SPFP_ESCAPE_FLAG, u);
		}

		// Writes the actual byte.
		__spfp_write_byte (byte, u);
	}

	// Writes the end flag.
	__spfp_write_byte (SPFP_END_FLAG, u);
}

/**************************************************************
 * SPFP StateMachine
 **************************************************************/

/// Handles an overflwo of the buffer in a state machine.
extern void __spfp_sm_overflow_handler (spfp_sm_t *sm);

/// Gets called once a valid packet has been received.
extern void __spfp_sm_packet_handler (spfp_sm_t *sm);

/// Initializes an SPFP state machine.
void spfp_sm_init (spfp_sm_t *sm, uint8_t *b, uint16_t b_capacity) {
	sm->b = b;
	sm->b_capacity = b_capacity;
	sm->b_size = 0;

	sm->escape = 0;
	sm->in_data = 0;
}

/// Updates the state machine with a new byte, kinda ticks it or something.
void spfp_update (spfp_sm_t *sm, uint8_t byte) {
	if (!sm->escape) {
		switch (byte) {
			// Start flag received.
			case SPFP_START_FLAG:
				sm->in_data = true;
				return;
			// Escape flag received.
			case SPFP_ESCAPE_FLAG:
				sm->escape = true;
				return;
			// End flag received.
			case SPFP_END_FLAG:
				// Calles the SPFP packet handler.
				__spfp_sm_packet_handler (sm);

				// Clears the buffer, and goes back to waiting state.
				sm->in_data = false;
				sm->b_size = 0;

				return;
			// Just data received, break.
			default:
				break;
		}
	} else {
		// Since we've received the escaped byte, set escape false to
		//  get back to normal reception.
	    sm->escape = false;
	}

	//
	// If we've reached this point, it is data.
	//
	
	// Makes sure we're not going to overflow the buffer
	//  if we are overflowing it, just return.
	if ((sm->b_size + 1) > sm->b_capacity) {
		__spfp_sm_overflow_handler (sm);
		return;
	}

	sm->b[sm->b_size++] = byte;
}
