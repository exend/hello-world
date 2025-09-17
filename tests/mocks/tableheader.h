#ifndef TABLEHEADER_H
#define TABLEHEADER_H

#include <inttypes.h>

typedef struct {
	uint64_t Signature;
	uint32_t Revision;
	uint32_t HeaderSize;
	uint32_t CRC32;
	uint32_t Reserved;
 } EFI_TABLE_HEADER;

 #endif // TABLEHEADER_H
