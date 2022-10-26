#pragma once

#define UPDC32(octet,crc) (crc_32_tab[((crc)\
  ^ ((BYTE)octet)) & 0xff] ^ ((crc) >> 8))

DWORD crc32File(std::string fileName, QByteArray &data);

