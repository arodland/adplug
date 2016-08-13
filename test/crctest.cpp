/*
 * Adplug - Replayer for many OPL2/OPL3 audio file formats.
 * Copyright (C) 1999 - 2005 Simon Peter, <dn.tlp@gmx.net>, et al.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * crctest.cpp - Test AdPlug CRC calculation, by Stas'M <binarymaster@mail.ru>
 */

#include <stdlib.h>
#include <stdio.h>
#include <binfile.h>

#include "../src/fprovide.h"
#include "../src/database.h"

/***** Local variables *****/

// String holding the relative path to the source directory
static char *srcdir;

/***** Local variables *****/

struct crcEntry
{
	const char *filename;
	unsigned short crc16;
	unsigned long  crc32;
};

static const crcEntry testlist[] = {
	{"2001.MKJ", 0x783C, 0x44B25BC7},
	{"ADAGIO.DFM", 0x21FF, 0x979A0582},
	{"adlibsp.s3m", 0xE800, 0xA7316E24},
	{NULL, 0, 0}
};

/***** Main program *****/

int main(int argc, char *argv[])
{
	bool retval = true;

	// Set path to source directory
	srcdir = getenv("srcdir");
	if (!srcdir) srcdir = ".";
	{
		const CFileProvider &fp = CProvider_Filesystem();
		for (int i = 0; testlist[i].filename != NULL; i++)
		{
			binistream *f = fp.open(testlist[i].filename);
			if (!f)
			{
				std::cerr << "Error opening for reading: " << testlist[i].filename << "\n";
				retval = false;
				continue;
			}

			std::cout << "Checking CRC: " << testlist[i].filename;
			f->seek(0);
			CAdPlugDatabase::CKey key(*f);
			fp.close(f);
			if (testlist[i].crc16 != key.crc16 ||
				testlist[i].crc32 != key.crc32)
			{
				std::cout << " [FAIL: " << key.crc16 << ", " << key.crc32 << "]\n";
				retval = false;
			}
			else
				std::cout << " [OK]\n";
		}
	}

	return retval ? EXIT_SUCCESS : EXIT_FAILURE;
}
