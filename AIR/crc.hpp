#pragma once

#include <cstring>
#include <QByteArray>
#include <windows.h>

WORD crc16(char *data_p, WORD length);

WORD updcrc(WORD icrc, BYTE *icp, size_t icnt);

#define UPDC32(octet,crc) (crc_32_tab[((crc)\
  ^ ((BYTE)octet)) & 0xff] ^ ((crc) >> 8))

DWORD updateCRC32(unsigned char ch, DWORD crc);
DWORD crc32File(std::string fileName , size_t &count, QByteArray &data);

