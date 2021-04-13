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

#include "helpers/linux.h"

/// Creates a new SPFP Linux session instance.
spfp_linux_session_t *spfp_linux_session_create(const char *path, uint16_t b_size, uint32_t baud) {
	struct termios tty;
	struct sigaction sa;
	int32_t fd;

	//
	// Creates the FD and sets options.
	//
	
	// Attempts to open the FD.
	if ((fd = open (path, O_RDWR | O_NOCTTY | O_SYNC)) < 0) {
		fprintf (stderr, "Failed to open SPFP session %s, error: %d:%s\r\n", path, errno, strerror (errno));
		return NULL;
	}

	if (tcgetattr (fd, &tty) != 0) {
		fprintf (stderr, "Failed to get attributes %d:%s\r\n", errno, strerror (errno));
		return NULL;
	}

	cfsetspeed (&tty, baud);

	tty.c_cflag &= ~(CSIZE | PARENB | PARODD | CSTOPB | CRTSCTS);
	tty.c_cflag |= (CLOCAL | CREAD | CS8);

	tty.c_iflag &= ~(IGNBRK | IXON | IXOFF | IXANY);
	
	tty.c_lflag = 0;
	tty.c_oflag = 0;

	tty.c_cc[VMIN] = 0;
	tty.c_cc[VTIME] = 1;

	if (tcsetattr (fd, TCSANOW, &tty) != 0) {
		fprintf (stderr, "Failed to set attributes %d:%s\r\n", errno, strerror (errno));
		return NULL;
	}

	//
	// Creates the session
	//
	
	// Allocates the required memory for session storage.
	spfp_linux_session_t *session = (spfp_linux_session_t *) malloc (sizeof (spfp_linux_session_t) + sizeof (spfp_sm_t));

	// Sets the previously created FD.
	session->fd = fd;

	// Initializes the SPFP state machine.
	spfp_sm_init(&session->sm, (uint8_t *) malloc (b_size), b_size);

	return session;
}

/// Writes an SPFP packet.
void spfp_linux_session_write(spfp_linux_session_t *session, const spfp_packet_t *packet) {
	spfp_write_packet (packet, session);
}

/// Override for the write byte method.
void __spfp_write_byte (uint8_t byte, void *u) {
	spfp_linux_session_t *session = (spfp_linux_session_t *) u;
	if (write (session->fd, &byte, 1) < 0) {
		fprintf (stderr, "Failed to write byte %d:%s\r\n", errno, strerror (errno));
	}
}