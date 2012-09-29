/*	This file is part of FimbulwinterClient.

	FimbulwinterClient is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Foobar is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with FimbulwinterClient.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef _GRFCRYPT_H_
#define _GRFCRYPT_H_

#define FILELIST_TYPE_FILE           0x01 // entry is a file
#define FILELIST_TYPE_ENCRYPT_HEADER 0x04 // encryption mode 1 (header DES only)
#define FILELIST_TYPE_ENCRYPT_MIXED  0x02 // encryption mode 0 (header DES + periodic DES/shuffle)

#include <YA3DE/Helpers.h>

#include <YA3DE/Pack1.h>
typedef struct BIT64 
{ 
	unsigned char b[8]; 
} STRUCT_PACKED BIT64;
#include <YA3DE/PackEnd.h>

void DES_DecryptBlock(BIT64 *block);
void DES_Decrypt(unsigned char *data, unsigned int size);
void GRF_Decode(unsigned char *buf, unsigned int len, unsigned char entry_type, int entry_len);

#endif
