#ifndef PLATFORM_H
#define PLATFORM_H

#define BIGENDIAN


#define SWAP16(x) ( (((x & 0x00FF) << 8) | ((x >> 8) & 0x00ff)) )
#define SWAP32(x) (  ((uint32_t) SWAP16(x) << 16) | ((uint32_t) SWAP16(x >> 16)) )


#endif
