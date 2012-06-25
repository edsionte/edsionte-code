/*
 *  Copyright (C) 2012 edsionte <edsionte@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published by
 *  the Free Software Foundation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "minilzo.h"


#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

static HEAP_ALLOC(wkmem, LZO1X_1_MEM_COMPRESS);
//extern lzo_voidp wkmem;
/*
 * compress_data: compress the data from @src into @dest, 
 * and return the the size of compressed data to @outlen.
 * @src: the src buffer.
 * @inlen: the size of src buffer.
 * @dest: the dest buffer.
 * @outlen: the size of compressed data.
 */
int compress_data(const char *src, uint32_t inlen, char *dest, uint32_t outlen)
{
	int ret = 0;
	
	if (src == NULL || inlen <= 0 || dest == NULL) {
		printf("error param in compress_data().\n");
		return -1;
	}

	//init the lzo lib
	if (lzo_init() != LZO_E_OK) {
		printf("lzo_init error.\n");
		return -1;
	}

	//compress from @srcbuffer to @destbuffer
	if ((ret = lzo1x_1_compress(src, inlen, dest, &outlen, wkmem)) != LZO_E_OK) {
		printf("compress error:%d.\n", ret);
		return -1;
	} else {
		printf("compress %lu bytes into %lu bytes.\n", (unsigned long)inlen, (unsigned long)outlen);
	}

	return ret;
}

/*
 * decompress_data: decompress the data from @src into @dest, 
 * and return the the size of decompressed data to @outlen.
 * @src: the src buffer.
 * @inlen: the size of src buffer.
 * @dest: the dest buffer.
 * @outlen: the size of decompressed data.
 */
int decompress_data(const char *src, uint32_t inlen, char *dest, uint32_t outlen)
{
	int ret = 0;
	
	if (src == NULL || inlen <= 0 || dest == NULL) {
		printf("error param in compress_data().\n");
		return -1;
	}

	//init the lzo lib
	if (lzo_init() != LZO_E_OK) {
		printf("lzo_init error.\n");
		return -1;
	}

	//compress from @srcbuffer to @destbuffer
	if ((ret = lzo1x_decompress(src, inlen, dest, &outlen, NULL)) != LZO_E_OK) {
		printf("decompress error:%d.\n", ret);
		return -1;
	} else {
		printf("decompress %lu bytes into %lu bytes.\n", (unsigned long)inlen, (unsigned long)outlen);
	}

	return ret;
}
