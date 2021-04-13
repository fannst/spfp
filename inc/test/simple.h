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
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "spfp.h"

#ifndef _SPFP_SIMPLE
#define _SPFP_SIMPLE

/// Override for the write byte method.
void __spfp_write_byte (uint8_t byte, void *u);

/// Handles an overflwo of the buffer in a state machine.
void __spfp_sm_overflow_handler (spfp_sm_t *sm);

/// Gets called once a valid packet has been received.
void __spfp_sm_packet_handler (spfp_sm_t *sm);

/// Gets called at startup.
int32_t main(int32_t argc, char **argv);

#endif