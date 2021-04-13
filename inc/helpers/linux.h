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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <err.h>
#include <errno.h>

#include "spfp.h"

#ifndef _SPFP_HELPERS_LINUX
#define _SPFP_HELPERS_LINUX

typedef struct {
	int32_t 		fd;			/* The File Descriptor */
	spfp_sm_t		sm;			/* The SPFP State Machine */
} spfp_linux_session_t;

/// Creates a new SPFP Linux session instance.
spfp_linux_session_t *spfp_linux_session_create(const char *path, uint16_t b_size, uint32_t baud);

/// Writes an SPFP packet.
void spfp_linux_session_write(spfp_linux_session_t *session, const spfp_packet_t *packet);

/// Override for the write byte method.
void __spfp_write_byte (uint8_t byte, void *u);

#endif
