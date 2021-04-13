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

#include "test/linux.h"

spfp_linux_session_t *session = NULL;

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
	spfp_linux_session_write(session, packet);
}

/// Gets called at startup.
int32_t main(int32_t argc, char **argv) {
	if ((session = spfp_linux_session_create("/dev/ttyUSB0", 1024, 9600)) == NULL) {
		return -1;
	}

	__main_send_test_packet ();
	__main_send_test_packet ();

	return 0;
}
