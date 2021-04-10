#include "main.h"

#include "spfp.h"

#ifdef __SPFP_MAIN

/// Override for the write byte method.
void __spfp_write_byte (uint8_t byte) {
	#ifdef __SPFP_MAIN

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

	#endif
}

/// Gets called at startup.
int32_t main(int32_t argc, char **argv) {
	const uint8_t payload[] = {'H', 'E', 'L', 'L', 'O', SPFP_ESCAPE_FLAG, SPFP_START_FLAG};

	uint16_t size = sizeof (spfp_packet_t) + sizeof (payload) / sizeof (uint8_t);

	spfp_packet_t *packet = malloc(size);
	memcpy (packet->p, payload, sizeof (payload) / sizeof (uint8_t));

	packet->len = size;
	packet->cs = spfp_calc_checksum (packet);

	spfp_write_packet (packet);

	return 0;
}

#endif
