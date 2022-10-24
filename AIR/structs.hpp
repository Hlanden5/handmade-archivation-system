#pragma once


struct metadata{
  quint16 timeOfLastEdit;
  quint16 dataOfLastEdit;
  quint32 sizeofFile;
  QString name;
  bool is_directory;
};

struct CentralFileHeader{
  const quint32 cfh_signature = 0x02014b50;
  quint16 versionDone;
  quint16 neededVersion;
  quint16 flag;
  quint16 methodOfCompress;
  quint16 timeOfLastEdit;
  quint16 dataOfLastEdit;
  quint32 CRC_32_uncompress;
  quint32 compressSize;
  quint32 nonCompressSize;
  quint16 sizeofNameFile;
  quint16 additionalSizeof;
  quint16 sizeofComment;
  quint16 numberOfDrive;
  quint16 internalAttributes;
  quint32 externalAttributes;
  quint32 offset;
  std::string nameOfFile;
  std::string comment;
};

/*
struct dataDescriptor{
  qint64 CRC_32;
  qint64 compressSize;
  qint64 unCompressSize;
};
*/

struct localFileHeader{
  const quint32 lfh_signature = 0x04034b50;
  quint16 neededVersion;
  quint16 flag;
  quint16 methodOfCompress;
  quint16 timeOfLastEdit;
  quint16 dataOfLastEdit;
  quint32 CRC_32_uncompress;
  quint32 compressSize;
  quint32 nonCompressSize;
  quint16 sizeofNameFile;
  quint16 additionalSizeof;
  std::string nameOfFile;
};

struct endOfCentralDirectory{
  const qint32 eocd_signature = 0x06054b50;
  qint16 numberOfDrive;
  qint16 numberOfDriveCFH;
  qint16 countOfCFH_onThisDrive;
  qint16 countOfCFH;
  quint32 sizeofCFH; // bytes
  quint32 offsetCFH_ofStartArchive;
  qint16 sizeofComment;
  std::string comment;
};

class structs
{
public:
  struct CentralFileHeader *cfh;
  //struct dataDescriptor *dd;
  struct localFileHeader *lfh;
  struct endOfCentralDirectory *eocd;

  void writeCFH(std::ofstream &file);

  //void writeDD(std::ofstream &file);

  void writeLFH(std::ofstream &file);

  void writeEOCD(std::ofstream &file);

  void writeLE(std::ofstream &file,quint16 data);

  void writeLE(std::ofstream &file,quint32 data);

  structs();

  ~structs();
};

