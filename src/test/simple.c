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

#include "test/simple.h"

uint8_t sm_buffer[1024];
spfp_sm_t sm;

/// Override for the write byte method.
void __spfp_write_byte (uint8_t byte, void *u) {
	static bool escape = false;
	const char *msg = NULL;

	// Some huge branching crap LOL.
	if (escape) {
		if (byte == SPFP_START_FLAG) {
			msg = "ESC STA";
		} else if (byte == SPFP_END_FLAG) {
			msg = "ESC END";
		} else if (byte == SPFP_ESCAPE_FLAG) {
			msg = "ESC ESC (LOL)";
		}

		escape = false;
	} else if (byte == SPFP_ESCAPE_FLAG) {
		msg = "ESC";
		escape = true;
	} else if (byte == SPFP_START_FLAG) {
		msg = "STA";
	} else if (byte == SPFP_END_FLAG) {
		msg = "END";
	} else {
		msg = "DAT";
	}

	// Prints the hex value of the byte to the serial console, and
	//  adds the byte type after.
	printf ("%02x: %s\r\n", byte, msg);

	// Updates the state machine.
	spfp_update (&sm, byte);
}

/// Handles an overflwo of the buffer in a state machine.
void __spfp_sm_overflow_handler (spfp_sm_t *sm) {
	printf ("Statemachine %lu overflowed!\r\n", (uintptr_t) sm);
}

/// Gets called once a valid packet has been received.
void __spfp_sm_packet_handler (spfp_sm_t *sm) {
	spfp_packet_t *packet = (spfp_packet_t *) sm->b;
	printf ("received packet!, payload: %s\r\n", packet->p);
}

void __main_send_test_packet () {
	// Creates the payload, and calculates the packet size.
	const uint8_t payload[] = {'H', 'E', 'L', 'L', 'O', '\0'};
	uint16_t size = sizeof (spfp_packet_t) + sizeof (payload) / sizeof (uint8_t);

	// Allocates the required memory for the packet.
	spfp_packet_t *packet = malloc(size);
	memcpy (packet->p, payload, sizeof (payload) / sizeof (uint8_t));

	// Sets the packet lenght, and calculates the checksum to assure
	//  there won't be any data loss.
	packet->len = size;
	packet->cs = spfp_calc_checksum (packet);

	// Writes the packet.
	spfp_write_packet (packet, NULL);
}

/// Gets called at startup.
int32_t main(int32_t argc, char **argv) {
	spfp_sm_init (&sm, sm_buffer, sizeof (sm_buffer) / sizeof (uint8_t));

	__main_send_test_packet ();
	__main_send_test_packet ();

	return 0;
}
