#ifndef TEXENGINE_BYTEORDER_H_INCLUDED
#define TEXENGINE_BYTEORDER_H_INCLUDED

#include <stdint.h>

/*
	NOTES:
	Only little and big endians are supported. However this should
	cover up almost all modern architectures.
*/

namespace TEXEngine {
	namespace Port {
		namespace Byteorder {

enum TEXENG_ENDIAN_TYPE
{
	TEXENG_ENDIAN_UNKNOWN,		/* unknown endian */
	TEXENG_ENDIAN_LITTLE,		/* little endian */
	TEXENG_ENDIAN_BIG,			/* big endian */
	TEXENG_ENDIAN_MIDDLE		/* middle endian */
};

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */

/* Endian detection. */
static inline TEXENG_ENDIAN_TYPE endian();

/* Functions to swap between little and big endian for 16/32 bit uints. */
static inline void uint16_swap_endian(uint16_t *val);
static inline void uint32_swap_endian(uint32_t *val);

/*
	NOTES:
	val assumes a value generated in LOCAL ENDIAN.

	These functions are to be used when we read/write a uint16_t or a uint32_t from/to
	a stream and need to impose a specific endianness.
	Ex. Cases in which a file format specifies that a value inside the file is stored in big 
		endian format.

	If you are not sure how you can use this then DON'T use it cause things can break easily.
*/
static inline void uint16_local_to_big_endian(uint16_t *val);
static inline void uint32_local_to_big_endian(uint32_t *val);
static inline void uint16_local_to_little_endian(uint16_t *val);
static inline void uint32_local_to_little_endian(uint32_t *val);

static inline void uint16_big_to_local_endian(uint16_t *val);
static inline void uint32_big_to_local_endian(uint32_t *val);
static inline void uint16_little_to_local_endian(uint16_t *val);
static inline void uint32_little_to_local_endian(uint32_t *val);

#ifdef __cplusplus
	} /* extern */
#endif /* __cplusplus */

		} /* namespace Byteorder */
	} /* namespace Port */
} /* namespace TEXEngine */

#include "byteorder.inl"

#endif /* TEXENGINE_BYTEORDER_H_INCLUDED */