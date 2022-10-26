#pragma once

#define UPDC32(octet,crc) (crc_32_tab[((crc)\
  ^ ((BYTE)octet)) & 0xff] ^ ((crc) >> 8))

void crc32File(const char* filename, QByteArray &data, quint32 &result);
