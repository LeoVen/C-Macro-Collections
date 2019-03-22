/**
 * @file macro_containers.h
 *
 * @author Leonardo Vencovsky (https://github.com/LeoVen)
 * @date 11/02/2019
 *
 * Macro based generic containers.
 *
 */

#ifndef MACRO_CONTAINERS
#define MACRO_CONTAINERS

#include <stdlib.h>
#include <stdbool.h>

#define CONCATH_(C, P) C##_GENERATE_HEADER##_##P
#define CONCATC_(C, P) C##_GENERATE_SOURCE##_##P

#define CONCATH(C, P) CONCATH_(C, P)
#define CONCATC(C, P) CONCATC_(C, P)

#define CONTAINER_GENERATE(C, P, PFX, SNAME, FMOD, K, V)    \
	CONTAINER_GENERATE_HEADER(C, P, PFX, SNAME, FMOD, K, V) \
	CONTAINER_GENERATE_SOURCE(C, P, PFX, SNAME, FMOD, K, V)

#define CONTAINER_GENERATE_HEADER(C, P, PFX, SNAME, FMOD, K, V) \
	CONCATH(C, P)                                               \
	(PFX, SNAME, FMOD, K, V)

#define CONTAINER_GENERATE_SOURCE(C, P, PFX, SNAME, FMOD, K, V) \
	CONCATC(C, P)                                               \
	(PFX, SNAME, FMOD, K, V)

#include "list.h"
#include "stack.h"
#include "queue.h"
#include "deque.h"
#include "foreach.h"

#endif /* MACRO_CONTAINERS */