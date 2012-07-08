#ifndef TEXENGINE_BYTEORDER_INL_INCLUDED
#define TEXENGINE_BYTEORDER_INL_INCLUDED

#ifndef TEXENGINE_BYTEORDER_H_INCLUDED
    #error "byteorder.h must be included before byteorder.inl"
#endif /* TEXENGINE_BYTEORDER_H_INCLUDED */

namespace TEXEngine {
	namespace Port {
		namespace Byteorder {

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

static inline TEXENG_ENDIAN_TYPE endian() /* returns the machine endianess code */
{
    union
    {
        uint32_t i;
        uint8_t c[4];
    } bint = {0x01020304};

    switch (bint.c[0])
    {
        case 0x01: return TEXENG_ENDIAN_BIG;
		case 0x03: return TEXENG_ENDIAN_MIDDLE;
        case 0x04: return TEXENG_ENDIAN_LITTLE;
    }

    return TEXENG_ENDIAN_UNKNOWN;
}

static inline void uint16_swap_endian(uint16_t *val)
{
    *val = (*val<<8) | (*val>>8);
}

static inline void uint32_swap_endian(uint32_t *val)
{
    *val = (*val<<24) | ((*val<<8) & 0x00ff0000) | ((*val>>8) & 0x0000ff00) | (*val>>24);
}

static inline void uint16_local_to_big_endian(uint16_t *val)
{
    if(endian() == TEXENG_ENDIAN_LITTLE)
        uint16_swap_endian(val);
}

static inline void uint32_local_to_big_endian(uint32_t *val)
{
    if(endian() == TEXENG_ENDIAN_LITTLE)
		uint32_swap_endian(val);
}

static inline void uint16_local_to_little_endian(uint16_t *val)
{
    if(endian() == TEXENG_ENDIAN_BIG)
        uint16_swap_endian(val);
}

static inline void uint32_local_to_little_endian(uint32_t *val)
{
    if(endian() == TEXENG_ENDIAN_BIG)
        uint32_swap_endian(val);
}

static inline void uint16_big_to_local_endian(uint16_t *val)
{
    if(endian() == TEXENG_ENDIAN_LITTLE)
        uint16_swap_endian(val);
}

static inline void uint32_big_to_local_endian(uint32_t *val)
{
    if(endian() == TEXENG_ENDIAN_LITTLE)
        uint32_swap_endian(val);
}

static inline void uint16_little_to_local_endian(uint16_t *val)
{
    if(endian() == TEXENG_ENDIAN_BIG)
        uint16_swap_endian(val);
}

static inline void uint32_little_to_local_endian(uint32_t *val)
{
    if(endian() == TEXENG_ENDIAN_BIG)
        uint32_swap_endian(val);
}

#ifdef __cplusplus
} /* __cplusplus */
#endif

		} /* namespace Byteorder */
	} /* namespace Port */
} /* namespace TEXEngine */

#endif /* TEXENGINE_BYTEORDER_INL_INCLUDED */