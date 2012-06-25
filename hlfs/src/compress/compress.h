#ifndef _HLFS_COMPRESS_H_
#define _HLFS_COMPRESS_H_
/*
 *  Copyright (C) 2012 edsionte <edsionte@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published by
 *  the Free Software Foundation.
 */
#include "minilzo.h"
/*
#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

extern lzo_voidp wkmem;
//static HEAP_ALLOC(wkmem, LZO1X_1_MEM_COMPRESS);
*/
int compress_data(const char *src, uint32_t inlen, char *dest, uint32_t outlen);
int decompress_data(const char *src, uint32_t inlen, char *dest, uint32_t outlen);

#endif
