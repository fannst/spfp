#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef _SPFP_H
#define _SPFP_H

#define SPFP_START_FLAG					(0x40)
#define SPFP_END_FLAG					(0x41)
#define SPFP_ESCAPE_FLAG				(0x42)

/**************************************************************
 * SPFP Datatypes
 **************************************************************/

typedef struct __attribute__ (( packed )) {
	uint16_t		len;			/* The Total Packet Length */
	uint8_t			cs;				/* 16-bit Checksum (Same as IP protocol) */
	uint8_t			p[0];			/* The Packet Payload */
} spfp_packet_t;

typedef struct {
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

/// The dummy write method, which can be overwritten by implementation.
void __attribute__ (( weak )) __spfp_write_byte (uint8_t byte);

/// Writes an packet to the other device.
void spfp_write_packet (const spfp_packet_t *packet);

/**************************************************************
 * SPFP StateMachine
 **************************************************************/

/// Updates the state machine with a new byte, kinda ticks it or something.
void spfp_update (spfp_sm_t *sm, uint8_t byte);

#endif
