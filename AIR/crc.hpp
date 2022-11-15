#pragma once

#define UPDC32(octet,crc) (crc_32_tab[((crc)\
  ^ ((BYTE)octet)) & 0xff] ^ ((crc) >> 8))

void crc32File(std::string filename, QByteArray &data, quint32 &result);

void crc32STR(std::string filename, quint32 &result);

std::string Win1251toCP866(std::string str);
