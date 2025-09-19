#ifndef TABLEHEADER_H
#define TABLEHEADER_H

#include <inttypes.h>

typedef struct {
	uint64_t signature;
	uint32_t revision;
	uint32_t header_size;
	uint32_t crc32;
	uint32_t reserved;
 } EFI_TABLE_HEADER;

 #endif // TABLEHEADER_H
