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
#include <stdint.h>
#include <stdbool.h>

#ifndef _SPFP_H
#define _SPFP_H

#ifdef __USE_CXX
extern "C" {
#endif

#define SPFP_START_FLAG					(0x01)
#define SPFP_END_FLAG					(0x02)
#define SPFP_ESCAPE_FLAG				(0x03)

/**************************************************************
 * SPFP Datatypes
 **************************************************************/

typedef struct __attribute__ (( packed )) {
	uint16_t		len;			/* The Total Packet Length */
	uint8_t			cs;				/* 8-bit Checksum (Same as IP protocol) */
	uint8_t			p[0];			/* The Packet Payload */
} spfp_packet_t;

typedef struct {
	void			*udata;
	/* Buffer Stuff */
	uint8_t *		b;				/* The Buffer */
	uint16_t		b_capacity;		/* The Buffer Capacity */
	uint16_t		b_size;			/* Buffer Bytes Used */
	/* State Flags */
	unsigned		escape : 1;		/* Escape Next Byte */
	unsigned		in_data : 1;	/* In Data Reception */
} spfp_sm_t;

/**************************************************************
 * SPFP Protocol Itself
 **************************************************************/

/// Adds a single byte to specified checksum.
uint8_t spfp_checksum_add (uint8_t cs, uint8_t val);

/// Calculates the checksum for specified packet.
uint8_t spfp_calc_checksum (const spfp_packet_t *packet);

/// Writes an packet to the other device.
void spfp_write_packet (const spfp_packet_t *packet, void *u);

/**************************************************************
 * SPFP StateMachine
 **************************************************************/

/// Initializes an SPFP state machine.
void spfp_sm_init (spfp_sm_t *sm, uint8_t *b, uint16_t b_capacity);

/// Updates the state machine with a new byte, kinda ticks it or something.
void spfp_update (spfp_sm_t *sm, uint8_t byte);

#ifdef __USE_CXX
}
#endif

#endif
