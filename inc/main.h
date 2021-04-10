#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifndef _SPFP_MAIN
#define _SPFP_MAIN

#ifdef __SPFP_MAIN

/// Override for the write byte method.
void __spfp_write_byte (uint8_t byte);

/// Gets called at startup.
int32_t main(int32_t argc, char **argv);

#endif

#endif