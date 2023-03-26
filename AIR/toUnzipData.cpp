#include "crc.hpp"
#include "qtypes.h"
#include "structs.hpp"
#include "toUnzipData.hpp"

int findASCII(char data){

  for(int i=-128;i<256;i++)
    {
      if(char(i)==data)
        return i;
    }

  return 0; // Withour warning
}

quint16 pack16(std::vector<char> vec){
  std::string str(vec.begin(), vec.end());
  reverse(str.begin(),str.end());
  static quint16 result = 0;
  static std::stringstream stream1,stream2;
  stream2 << "0x";

  for(int i=0;i<2;i++){
      stream1 << std::hex << findASCII(str[i]);
      std::string tmp = stream1.str();
      if(tmp.size()>2)
        tmp.erase(0,6);
      stream1.str("");
      stream2 << tmp;
    }
  stream2 >> std::hex >> result;
  return result;
}

quint32 pack32(std::vector<char> vec){
  std::string str(vec.begin(), vec.end());
  reverse(str.begin(),str.end());
  static quint16 result = 0;
  static std::stringstream stream1,stream2;
  stream2 << "0x";

  for(int i=0;i<4;i++){
      stream1 << std::hex << findASCII(str[i]);
      std::string tmp = stream1.str();
      if(tmp.size()>2) // maybe problem here, needed tests
        tmp.erase(0,6);
      stream1.str("");
      stream2 << tmp;
    }
  stream2 >> std::hex >> result;
  return result;
}

toUnzipData::toUnzipData()
{

}


toUnzipData::~toUnzipData()
{

}

void toUnzipData::collectAndLoadData(){
  std::vector<size_t> indicesCFH,indicesLFH;
  size_t indexEOCD;
  findAllSignatures(indicesLFH, indicesCFH, indexEOCD);
  std::cout << "Signatures finded" << std::endl;
  setEOCD(indexEOCD);
  std::cout << "setEOCD complete" << std::endl;
  setLFH(indicesLFH);
  std::cout << "setLFH complete" << std::endl;
  setCFH(indicesCFH);
  std::cout << "setCFH complete" << std::endl;
  debug("D:\\OPD\\debugOUT.txt",eocd);
}

void toUnzipData::setLFH(std::vector<size_t>& indicesLFH){
  auto iter = headerArray.begin();
  std::vector<char> parts;
  for(size_t i = 0, index;iter!=headerArray.end();iter++,i++){
      index = indicesLFH[i];

      index+=4;// to data
      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->neededVersion = pack16(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->flag = pack16(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->methodOfCompress = pack16(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->timeOfLastEdit = pack16(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->dataOfLastEdit = pack16(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      parts.push_back(data[index+2]);
      parts.push_back(data[index+3]);
      index+=4;
      iter->lfh->CRC_32_uncompress = pack32(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      parts.push_back(data[index+2]);
      parts.push_back(data[index+3]);
      index+=4;
      iter->lfh->compressSize = pack32(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      parts.push_back(data[index+2]);
      parts.push_back(data[index+3]);
      index+=4;
      iter->lfh->nonCompressSize = pack32(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->sizeofNameFile = pack16(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->additionalSizeof = pack16(parts);
      parts.clear();

      //namefile here
      std::string tmp;
      tmp.resize(iter->lfh->sizeofNameFile);

      if (iter->lfh->sizeofNameFile > 0) {
          iter->lfh->nameOfFile.resize(iter->lfh->sizeofNameFile);
          for (size_t i = 0; i < iter->lfh->sizeofNameFile; i++) {
              iter->lfh->nameOfFile[i] = data[index];
              index++;
            }
        }

      // Store extra field if it exists
      if (iter->lfh->additionalSizeof > 0) {
          iter->lfh->extraField.resize(iter->lfh->additionalSizeof);
          for (size_t i = 0; i < iter->lfh->additionalSizeof; i++) {
              iter->lfh->extraField[i] = data[index];
              index++;
            }
        }

    }
}

void toUnzipData::setCFH(std::vector<size_t>& indicesCFH){
  auto iter = headerArray.begin();
  size_t i = 0;
  for(size_t index;iter!=headerArray.end();iter++,i++){
      index = indicesCFH[i];
      iter->cfh->versionDone = pack16(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->neededVersion = pack16(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->flag = pack16(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->methodOfCompress = pack16(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->timeOfLastEdit = pack16(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->dataOfLastEdit = pack16(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->CRC_32_uncompress = pack32(std::vector<char>({data[index], data[index+1], data[index+2], data[index+3]}));
      index += 4;

      iter->cfh->compressSize = pack32(std::vector<char>({data[index], data[index+1], data[index+2], data[index+3]}));
      index += 4;

      iter->cfh->nonCompressSize = pack32(std::vector<char>({data[index], data[index+1], data[index+2], data[index+3]}));
      index += 4;

      iter->cfh->sizeofNameFile = pack16(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->additionalSizeof = pack16(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->sizeofComment = pack16(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->numberOfDrive = pack16(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->internalAttributes = pack16(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->externalAttributes = pack32(std::vector<char>({data[index], data[index+1], data[index+2], data[index+3]}));
      index += 4;

      iter->cfh->offset = pack32(std::vector<char>({data[index], data[index+1], data[index+2], data[index+3]}));
      index += 4;

      // Store file name if it exists
      if (iter->cfh->sizeofNameFile > 0) {
          iter->cfh->nameOfFile.resize(iter->cfh->sizeofNameFile);
          for (size_t i = 0; i < iter->cfh->sizeofNameFile; i++) {
              iter->cfh->nameOfFile[i] = data[index];
              index++;
            }
        }

      // Store extra field if it exists
      if (iter->cfh->additionalSizeof > 0) {
          iter->cfh->extraField.resize(iter->cfh->additionalSizeof);
          for (size_t i = 0; i < iter->cfh->additionalSizeof; i++) {
              iter->cfh->extraField[i] = data[index];
              index++;
            }
        }

      // Store comment if it exists
      if (iter->cfh->sizeofComment > 0) {
          iter->cfh->comment.resize(eocd.sizeofComment);
          for (size_t i = 0; i < eocd.sizeofComment; i++) {
              iter->cfh->comment[i] = data[index];
              index++;
            }
        }
    }
}

void toUnzipData::setEOCD(size_t index){

  eocd.numberOfDrive = pack16(std::vector<char>({data[index], data[index+1]}));
  index += 2;

  eocd.numberOfDriveCFH = pack16(std::vector<char>({data[index], data[index+1]}));
  index += 2;

  eocd.countOfCFH_onThisDrive = pack16(std::vector<char>({data[index], data[index+1]}));
  index += 2;

  eocd.countOfCFH = pack16(std::vector<char>({data[index], data[index+1]}));
  index += 2;

  eocd.sizeofCFH = pack16(std::vector<char>({data[index], data[index+1]}));
  index += 2;

  eocd.offsetCFH_ofStartArchive = pack32(std::vector<char>({data[index], data[index+1], data[index+2], data[index+3]}));
  index += 4;

  eocd.sizeofComment = pack16(std::vector<char>({data[index], data[index+1]}));
  index += 2;

  // Store EOCD comment if it exists
  std::cout << eocd.sizeofComment << std::endl;
  if (eocd.sizeofComment > 0) {
      eocd.comment.resize(eocd.sizeofComment);
      for (size_t i = 0; i < eocd.sizeofComment; i++) {
          eocd.comment[i] = data[index];
          index++;
        }
    }
  headerArray.resize(eocd.countOfCFH);
}

void toUnzipData::findAllSignatures(std::vector<size_t> &LFH, std::vector<size_t> &CFH, size_t& EOCD){

  getDataFromZip();//DELETE THIS
  LFH.clear();
  CFH.clear();
  EOCD = 0;
  std::vector<size_t> eocd_index;

  { // EOCD
    std::string rt("PK");
    rt += 0x05;
    rt += 0x06;
    std::regex r(rt);
    auto begin = std::sregex_iterator(data.begin(), data.end(), r);
    auto end = std::sregex_iterator();

    //    std::cout << "Found EOCD "
    //              << std::distance(begin, end) << std::endl;


    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        eocd_index.push_back(match.position());
      }
  } // END EOCD


  { // LFH
    std::string rt("PK");
    rt += 0x03;
    rt += 0x04;
    std::regex r(rt);
    auto begin = std::sregex_iterator(data.begin(), data.end(), r);
    auto end = std::sregex_iterator();

    //    std::cout << "Found LFH "
    //              << std::distance(begin, end) << std::endl;


    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        LFH.push_back(match.position());
      }
  } // END LFH


  { // CFH
    std::string rt("PK");
    rt += 0x01;
    rt += 0x02;
    std::regex r(rt);
    auto begin = std::sregex_iterator(data.begin(), data.end(), r);
    auto end = std::sregex_iterator();

    //    std::cout << "Found CFH "
    //              << std::distance(begin, end) << std::endl;


    for (std::sregex_iterator i = begin; i != end; ++i) {
        std::smatch match = *i;
        CFH.push_back(match.position());
      }
  } // END CFH


  //  std::cout << "EOCD finded:" << std::endl;
  //  for (auto c:eocd_index){
  //      std::cout << c << std::endl;
  //    }
  //  std::cout << std::endl;

  //  std::cout << "LFH finded:" << std::endl;
  //  for (auto c:LFH){
  //      std::cout << c << std::endl;
  //    }
  //  std::cout << std::endl;


  //  std::cout << "CFH finded:" << std::endl;
  //  for (auto c:CFH){
  //      std::cout << c << std::endl;
  //    }
  //  std::cout << std::endl;
  //  std::cout<<eocd_index.size()<<std::endl;

  while(eocd_index.size()>1){
      size_t pos = eocd_index[0]+8;
      std::string str=data.substr(pos,2);
      reverse(str.begin(),str.end());
      size_t count_=0;
      std::stringstream stream1,stream2;
      stream2 << "0x";

      for(int i=0;i<2;i++){
          stream1 << std::hex << findASCII(str[i]);
          std::string tmp = stream1.str();
          if(tmp.size()>2)
            tmp.erase(0,6);
          stream1.str("");
          stream2 << tmp;
        }

      stream2 >> std::hex >> count_;
      //std::cout << count_ << std::endl;

      size_t max = 0;
      size_t index = 0;
      for(size_t i=0;i<CFH.size();i++){
          if(CFH[i]>max and CFH[i]<pos){
              max = CFH[i];
              index = i;
            }
        }


      std::vector<size_t>::iterator startCHF, startLFH;
      startLFH=LFH.begin();
      startCHF=CFH.begin();
      for(size_t i=index;i<index-count_;i--){
          std::advance(startLFH,i);
          std::advance(startCHF,i);
          LFH.erase(startLFH);
          CFH.erase(startCHF);
        }

      eocd_index.erase(eocd_index.begin());
    }
  //std::cout << "ERASE, max = " << std::endl;

  //      std::cout << "LFH:" << std::endl;
  //      for (auto c:LFH){
  //          std::cout << c << std::endl;
  //        }

  //      std::cout << std::endl;

  //      std::cout << "CFH:" << std::endl;
  //      for (auto c:CFH){
  //          std::cout << c << std::endl;

  //        }

  //    }



  //  std::cout << "EOCD finded:" << std::endl;
  //  for (auto c:eocd_index){
  //      std::cout << c << std::endl;
  //    }

  EOCD = eocd_index[0];
  //  std::cout << std::endl;

  //  std::cout << "LFH finded:" << std::endl;
  //  for (auto c:LFH){
  //      std::cout << c << std::endl;
  //    }

  //  std::cout << std::endl;
  //  std::cout << "CFH finded:" << std::endl;
  //  for (auto c:CFH){
  //      std::cout << c << std::endl;
  //    }

}

void toUnzipData::setData(QString &pathOfZip,QString &pathOfFiles){
  std::cout <<std::string(pathOfZip.toLocal8Bit())<<std::endl;
  this->pathOfZip = pathOfZip;
  this->pathOfFiles = pathOfFiles;
  this->fileZip = std::ifstream(pathOfZip.toLocal8Bit(),std::ios_base::binary);
  if(!fileZip.is_open())
    throw;
}


void toUnzipData::getDataFromZip(){
  fileZip.seekg(0,std::ios::end);
  size_t len_input = fileZip.tellg();
  fileZip.seekg(0,std::ios::beg);
  data.resize(len_input);
  fileZip.read((char*)&data[0],len_input);
  // if(fileZip.good()) std::cout << data << std::endl;
  fileZip.close();
}

void toUnzipData::debug(std::string path,struct endOfCentralDirectory eocd){
  using std::endl;
  std::ofstream debugger(path, std::ofstream::trunc);
  size_t index = 0;
  for(size_t i=0;i<headerArray.size();i++){
      debugger << endl
               << "LFH with index: " << index << endl
               << "Needed version: " << headerArray[index].lfh->neededVersion << endl
               << "Compression method: " << headerArray[index].lfh->methodOfCompress << endl
               << "Last modification time: " << headerArray[index].lfh->timeOfLastEdit << endl
               << "Last modification date: " << headerArray[index].lfh->dataOfLastEdit << endl
               << "CRC-32: " << headerArray[index].lfh->CRC_32_uncompress << endl
               << "Compressed size: " << headerArray[index].lfh->compressSize << endl
               << "Uncompressed size: " << headerArray[index].lfh->nonCompressSize << endl
               << "Name length: " << headerArray[index].lfh->sizeofNameFile << endl
               << "Extra field length: " << headerArray[index].lfh->additionalSizeof << endl
               << "Name: " << headerArray[index].lfh->nameOfFile << endl
               << "Extra field: " << headerArray[index].lfh->extraField << endl;

      debugger << endl
               << "Central File Header with index: " << index << endl
               << "Version made by: " << headerArray[index].cfh->versionDone << endl
               << "Version needed to extract: " << headerArray[index].cfh->neededVersion << endl
               << "Compression method: " << headerArray[index].cfh->methodOfCompress << endl
               << "Last modification time: " << headerArray[index].cfh->timeOfLastEdit << endl
               << "Last modification date: " << headerArray[index].cfh->dataOfLastEdit << endl
               << "CRC-32: " << headerArray[index].cfh->CRC_32_uncompress << endl
               << "Compressed size: " << headerArray[index].cfh->compressSize << endl
               << "Uncompressed size: " << headerArray[index].cfh->nonCompressSize << endl
               << "Name length: " << headerArray[index].cfh->sizeofNameFile << endl
               << "Extra field length: " << headerArray[index].cfh->additionalSizeof << endl
               << "Comment length: " << headerArray[index].cfh->sizeofComment << endl
               << "Number of this disk: " << headerArray[index].cfh->numberOfDrive << endl
               << "Internal file attributes: " << headerArray[index].cfh->internalAttributes << endl
               << "External file attributes: " << headerArray[index].cfh->externalAttributes << endl
               << "Offset of local header: " << headerArray[index].cfh->offset << endl
               << "Name: " << headerArray[index].cfh->nameOfFile << endl
               << "Extra field: " << headerArray[index].cfh->extraField << endl
               << "Comment: " << headerArray[index].cfh->comment << endl;
    }
  debugger << endl
           << "End of Central Directory Record" << endl
           << "Number of this disk: " << eocd.numberOfDrive << endl
           << "Number of the disk with the start of the central directory: " << eocd.numberOfDriveCFH << endl
           << "Total number of entries in the central directory on this disk: " << eocd.countOfCFH_onThisDrive << endl
           << "Total number of entries in the central directory: " << eocd.countOfCFH << endl
           << "Size of the central directory: " << eocd.sizeofCFH << " bytes" << endl
           << "Offset of start of central directory: " << eocd.offsetCFH_ofStartArchive << endl
           << "Comment length: " << eocd.sizeofComment << endl
           << "Comment: " << eocd.comment << endl;
  debugger.close();
}
