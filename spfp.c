#include "spfp.h"

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
	const uint8_t *p = (uint8_t *) packet;
	uint8_t cs = 0x00;

	// Loops over the total packet, and adds each byte to the 
	//  final checksum.
	for (uint16_t i = 0; i < packet->len; ++i) {
		cs = spfp_checksum_add (cs, p[i]);
	}

	return ~cs;
}

/// The dummy write method, which can be overwritten by implementation.
void __attribute__ (( weak )) __spfp_write_byte (uint8_t byte) {
	#ifdef __SPFP_MAIN
	printf ("%02x\r\n", byte);
	#endif
}

/// Writes an packet to the other device.
void spfp_write_packet (const spfp_packet_t *packet) {
	const uint8_t *p = (uint8_t *) packet;

	// Writes the start flag.
	__spfp_write_byte (SPFP_START_FLAG);

	// Loops over the total packet and writes each byte
	//  to the other device, and adds required escapes.
	for (uint16_t i = 0; i < packet->len; ++i) {
		uint8_t byte = p[i];

		// Checks if the current byte is a control flag, if so we first want
		//  to send an escape byte to indicate that it's not an actual control flag.
		if (byte == SPFP_ESCAPE_FLAG || byte == SPFP_START_FLAG || byte == SPFP_END_FLAG) {
			__spfp_write_byte (SPFP_ESCAPE_FLAG);
		}

		// Writes the actual byte.
		__spfp_write_byte (byte);
	}

	// Writes the end flag.
	__spfp_write_byte (SPFP_END_FLAG);
}
