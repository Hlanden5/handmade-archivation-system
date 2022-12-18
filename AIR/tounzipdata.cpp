#include "crc.hpp"
#include "structs.hpp"
#include "tounzipdata.hpp"
#include <cstddef>
#include <iostream>
#include <string>

toUnzipData::toUnzipData()
{

}

toUnzipData::~toUnzipData()
{
}
size_t toUnzipData::findEOCD(std::string &signature){
    size_t tmp;
    //std::string str;
    tmp = signature.find_last_of(eocd.eocd_signature);
}

//struct endOfCentralDirectory eocd;

