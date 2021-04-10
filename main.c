#include "main.h"

#include "spfp.h"

#ifdef __SPFP_MAIN

int32_t main(int32_t argc, char **argv) {
	const uint8_t payload[] = {'H', 'E', 'L', 'L', 'O', 0x13};

	uint16_t size = sizeof (spfp_packet_t) + sizeof (payload) / sizeof (uint8_t);

	spfp_packet_t *packet = malloc(size);
	memcpy (packet->p, payload, sizeof (payload) / sizeof (uint8_t));

	packet->len = size;
	packet->cs = spfp_calc_checksum (packet);

	spfp_write_packet (packet);

	return 0;
}

#endif
