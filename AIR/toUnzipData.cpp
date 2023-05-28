#include "crc.hpp"
#include "filemetadata.hpp"
#include "toUnzipData.hpp"

int findASCII(char data){

  for(int i=-128;i<256;i++)
    {
      if(char(i)==data)
        return i;
    }

  return 0; // Without warning
}

quint16 pack16REV(std::vector<char> vec){
  std::string str(vec.begin(), vec.end());
  reverse(str.begin(),str.end());
  quint16 result = 0;
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

  stream2 >> std::hex >> result;
  return result;
}

quint32 pack32REV(std::vector<char> vec){
  std::string str(vec.begin(), vec.end());
  reverse(str.begin(),str.end());
  quint32 result = 0;
  std::stringstream stream1,stream2;
  stream2 << "0x";

  for(int i=0;i<4;i++){
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

  {
    std::vector<std::string> nameOfFiles;
    for(auto &c : headerArray){
        nameOfFiles.push_back(std::string(c.cfh->nameOfFile.c_str()));
      }

    for (auto& str : nameOfFiles) {
        std::replace(str.begin(), str.end(), char(0x3f), '-');
      }

    std::sort(nameOfFiles.begin(),nameOfFiles.end(),[](std::string& x,std::string& y){
      return x>y;
    });

    std::cout << "complete sort" << std::endl;
    std::string tmp;
    for(auto iter = nameOfFiles.begin();iter!=nameOfFiles.end();iter++){
        tmp = *iter;
        tmp.resize(tmp.size()-1);
        QString pathCodec{QString::fromLocal8Bit(CP866toWin1251(std::string(pathOfFiles.toLocal8Bit())+*iter))};
        if(tmp+"\\" == *iter or tmp+"//" == *iter){
            std::filesystem::path path(pathCodec.toStdU32String());
            std::filesystem::create_directory(path);
          }
        else{
            std::ofstream file(pathCodec.toLocal8Bit(),std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
            file.close();
          }
      }
  }

  for (auto& struct_ : headerArray) {
      struct_.cfh->nameOfFile.begin();
      std::replace(struct_.cfh->nameOfFile.begin(), struct_.cfh->nameOfFile.end(), char(0x3f), '-');
      std::replace(struct_.lfh->nameOfFile.begin(), struct_.lfh->nameOfFile.end(), char(0x3f), '-');
    }
  size_t indexOfData;
  auto iter = headerArray.begin();
  for(size_t i = 0;iter!=headerArray.end();i++,iter++){
      indexOfData = (indicesLFH[i]+30+iter->lfh->nameOfFile.size()+iter->lfh->extraField.size());
      QString pathCodec{QString::fromLocal8Bit(CP866toWin1251(std::string(pathOfFiles.toLocal8Bit())+iter->lfh->nameOfFile))};
      //std::cout << CP866toWin1251(std::string(pathOfFiles.toLocal8Bit())+iter->lfh->nameOfFile) << std::endl;
      std::ofstream file(pathCodec.toLocal8Bit(),std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
      std::string tmp;
      tmp.resize(static_cast<size_t>(iter->lfh->nonCompressSize));
      for(size_t j=0; j<iter->lfh->nonCompressSize;j++){
          tmp[j] = data[indexOfData+j];
        }
      file << tmp;
      file.close();
    }


  std::cout << "complete unpack" << std::endl;
  debug("D:\\OPD\\debugOUT.txt",eocd);
  std::cout << "complete debug file" << std::endl;
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
      iter->lfh->neededVersion = pack16REV(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->flag = pack16REV(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->methodOfCompress = pack16REV(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->dataOfLastEdit = pack16REV(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->timeOfLastEdit = pack16REV(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      parts.push_back(data[index+2]);
      parts.push_back(data[index+3]);
      index+=4;
      iter->lfh->CRC_32_uncompress = pack32REV(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      parts.push_back(data[index+2]);
      parts.push_back(data[index+3]);
      index+=4;
      iter->lfh->compressSize = pack32REV(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      parts.push_back(data[index+2]);
      parts.push_back(data[index+3]);
      index+=4;
      iter->lfh->nonCompressSize = pack32REV(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->sizeofNameFile = pack16REV(parts);
      parts.clear();

      parts.push_back(data[index]);
      parts.push_back(data[index+1]);
      index+=2;
      iter->lfh->additionalSizeof = pack16REV(parts);
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
      index += 4;

      iter->cfh->versionDone = pack16REV(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->neededVersion = pack16REV(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->flag = pack16REV(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->methodOfCompress = pack16REV(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->dataOfLastEdit = pack16REV(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->timeOfLastEdit = pack16REV(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->CRC_32_uncompress = pack32REV(std::vector<char>({data[index], data[index+1], data[index+2], data[index+3]}));
      index += 4;

      iter->cfh->compressSize = pack32REV(std::vector<char>({data[index], data[index+1], data[index+2], data[index+3]}));
      index += 4;

      iter->cfh->nonCompressSize = pack32REV(std::vector<char>({data[index], data[index+1], data[index+2], data[index+3]}));
      index += 4;

      iter->cfh->sizeofNameFile = pack16REV(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->additionalSizeof = pack16REV(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->sizeofComment = pack16REV(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->numberOfDrive = pack16REV(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->internalAttributes = pack16REV(std::vector<char>({data[index], data[index+1]}));
      index += 2;

      iter->cfh->externalAttributes = pack32REV(std::vector<char>({data[index], data[index+1], data[index+2], data[index+3]}));
      index += 4;

      iter->cfh->offset = pack32REV(std::vector<char>({data[index], data[index+1], data[index+2], data[index+3]}));
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
          iter->cfh->comment.resize(iter->cfh->sizeofComment);
          for (size_t i = 0; i < iter->cfh->sizeofComment; i++) {
              iter->cfh->comment[i] = data[index];
              index++;
            }
        }
    }
}

void toUnzipData::setEOCD(size_t index){
  index += 4;

  eocd.numberOfDrive = pack16REV(std::vector<char>({data[index], data[index+1]}));
  index += 2;

  eocd.numberOfDriveCFH = pack16REV(std::vector<char>({data[index], data[index+1]}));
  index += 2;


  eocd.countOfCFH_onThisDrive = pack16REV(std::vector<char>({data[index], data[index+1]}));
  index += 2;

  eocd.countOfCFH = pack16REV(std::vector<char>({data[index], data[index+1]}));
  index += 2;

  eocd.sizeofCFH = pack16REV(std::vector<char>({data[index], data[index+1]}));
  index += 2;

  eocd.offsetCFH_ofStartArchive = pack32REV(std::vector<char>({data[index], data[index+1], data[index+2], data[index+3]}));
  index += 4;

  eocd.sizeofComment = pack16REV(std::vector<char>({data[index], data[index+1]}));
  index += 2;

  headerArray.resize(eocd.countOfCFH);
}

void toUnzipData::findAllSignatures(std::vector<size_t> &LFH, std::vector<size_t> &CFH, size_t& EOCD){
  getDataFromZip();//DELETE THIS
  LFH.clear();
  CFH.clear();
  EOCD = 0;
  std::vector<size_t> eocd_index;
  clock_t R1 = clock();


  // define lambda for regex search
  { // EOCD
      std::string rt("PK");
      rt += 0x05;
      rt += 0x06;
      std::regex r(rt, std::regex::optimize | std::regex_constants::optimize | std::regex_constants::nosubs); // use optimize and nosubs
      auto begin = std::sregex_iterator(data.begin(), data.end(), r);
      auto end = std::sregex_iterator();
      std::smatch match;

      for (std::sregex_iterator i = begin; i != end; ++i) {
          match = *i;
          eocd_index.push_back(match.position());
        }
    } // END EOCD

    { // LFH
      std::string rt("PK");
      rt += 0x03;
      rt += 0x04;
      std::regex r(rt, std::regex::optimize | std::regex_constants::optimize | std::regex_constants::nosubs); // use optimize and nosubs
      auto begin = std::sregex_iterator(data.begin(), data.end(), r);
      auto end = std::sregex_iterator();

      std::smatch match;

      for (std::sregex_iterator i = begin; i != end; ++i) {
          match = *i;
          LFH.push_back(match.position());
        }
    } // END LFH

    { // CFH
      std::string rt("PK");
      rt += 0x01;
      rt += 0x02;
      std::regex r(rt, std::regex::optimize | std::regex_constants::optimize | std::regex_constants::nosubs);
      auto begin = std::sregex_iterator(data.begin(), data.end(), r);
      auto end = std::sregex_iterator();

      std::smatch match;
      for (std::sregex_iterator i = begin; i != end; ++i) {
          match = *i;
          CFH.push_back(match.position());
        }
    } // END CFH



  std::cout << std::setprecision(5) << "Time regex ALL0 : " << double((double)(clock()-R1)/1000) << std::endl;

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
      if(count_!=0){
          std::vector<size_t> iterators;
          auto iterCFH = CFH.end();
          iterCFH--;
          size_t tmp = count_;
          for(;tmp!=0;iterCFH--){
              if(*iterCFH<eocd_index[0]){
                  tmp--;
                  iterators.push_back(*iterCFH);
                }
            }

          std::vector<size_t> iterators2;
          auto iterLFH = LFH.end();
          iterLFH--;
          tmp = count_;
          for(;tmp!=0;iterLFH--){
              if(*iterLFH<iterators[0]){
                  tmp--;
                  iterators2.push_back(*iterLFH);
                }
            }

          for(auto &c:iterators)
            CFH.erase(std::remove(CFH.begin(), CFH.end(), c), CFH.end());
          for(auto &c:iterators2)
            LFH.erase(std::remove(LFH.begin(), LFH.end(), c), LFH.end());
        }
      eocd_index.erase(eocd_index.begin());
    }
  EOCD = eocd_index[0];
}

void toUnzipData::setData(QString &pathOfZip,QString &pathOfFiles){
  this->pathOfZip = pathOfZip;
  this->pathOfFiles = pathOfFiles;
//  std::cout <<std::string(pathOfZip.toLocal8Bit())<<std::endl;
//  std::cout <<std::string(pathOfFiles.toLocal8Bit())<<std::endl;
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
  fileZip.close();
}

void toUnzipData::debug(std::string path,struct endOfCentralDirectory eocd){
  using std::endl;
  std::ofstream debugger(path, std::ofstream::trunc);
  for(size_t i=0;i<headerArray.size();i++){
      //std::cout << "LFH with i: " << i << endl;
      debugger << endl
               << "LFH with i: " << i << endl
               << "Needed version: " << headerArray[i].lfh->neededVersion << endl
               << "Compression method: " << headerArray[i].lfh->methodOfCompress << endl
               << "Last modification time: " << headerArray[i].lfh->timeOfLastEdit << endl
               << "Last modification date: " << headerArray[i].lfh->dataOfLastEdit << endl
               << "CRC-32: " << headerArray[i].lfh->CRC_32_uncompress << endl
               << "Compressed size: " << headerArray[i].lfh->compressSize << endl
               << "Uncompressed size: " << headerArray[i].lfh->nonCompressSize << endl
               << "Name length: " << headerArray[i].lfh->sizeofNameFile << endl
               << "Extra field length: " << headerArray[i].lfh->additionalSizeof << endl
               << "Name: " << headerArray[i].lfh->nameOfFile << endl
               << "Extra field: " << headerArray[i].lfh->extraField << endl;

      //std::cout << "CFH with i: " << i << endl;
      debugger << endl
               << "Central File Header with i: " << i << endl
               << "Version made by: " << headerArray[i].cfh->versionDone << endl
               << "Version needed to extract: " << headerArray[i].cfh->neededVersion << endl
               << "Compression method: " << headerArray[i].cfh->methodOfCompress << endl
               << "Last modification time: " << headerArray[i].cfh->timeOfLastEdit << endl
               << "Last modification date: " << headerArray[i].cfh->dataOfLastEdit << endl
               << "CRC-32: " << headerArray[i].cfh->CRC_32_uncompress << endl
               << "Compressed size: " << headerArray[i].cfh->compressSize << endl
               << "Uncompressed size: " << headerArray[i].cfh->nonCompressSize << endl
               << "Name length: " << headerArray[i].cfh->sizeofNameFile << endl
               << "Extra field length: " << headerArray[i].cfh->additionalSizeof << endl
               << "Comment length: " << headerArray[i].cfh->sizeofComment << endl
               << "Number of this disk: " << headerArray[i].cfh->numberOfDrive << endl
               << "Internal file attributes: " << headerArray[i].cfh->internalAttributes << endl
               << "External file attributes: " << headerArray[i].cfh->externalAttributes << endl
               << "Offset of local header: " << headerArray[i].cfh->offset << endl
               << "Name: " << headerArray[i].cfh->nameOfFile << endl
               << "Extra field: " << headerArray[i].cfh->extraField << endl;
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

