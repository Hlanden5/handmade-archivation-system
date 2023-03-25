#pragma once

#include "structs.hpp"

class toUnzipData : private structs
{
    QString pathOfZip;
    QString pathOfFiles;
    std::ifstream fileZip;
    std::vector <structs> headerArray;
    struct endOfCentralDirectory eocd;
    std::string data;

    void setLFH();
    void setCFH();
    void setEOCD();

public:
    toUnzipData();
    ~toUnzipData();
    size_t findEOCD();
    void setData(QString &pathOfZip,QString &pathOfFiles);
    void findAllSignatures(std::vector<size_t> &LFH, std::vector<size_t> &CFH, size_t& EOCD);
    void getDataFromZip();
};




//struct endOfCentralDirectoryRecord
//{
//       uint   header;                // Header of end central directory record, always equals 0x06054b50
//       ushort disk_number;           // Number of this disk
//       ushort disk_number_cd;        // Number of the disk with the start of the central directory
//       ushort total_entries_disk;    // Total number of entries in the central directory on this disk
//       ushort total_entries;         // Total number of entries in the central directory
//       uint   size_central_dir;      // Size of central directory
//       uint   start_cd_offset;       // Starting disk number
//       ushort file_comment_lengtt;   // File comment length
//       //string FileComment(void);
//       //bool   LoadFromCharArray(uchar& array[]);
//       //int    ToCharArray(uchar &array[]);
//       endOfCentralDirectoryRecord(void) : header(0x06054B50){;}
//};

