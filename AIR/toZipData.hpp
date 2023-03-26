#pragma once

#include "structs.hpp"
#include "filemetadata.hpp"

class toZip : private structs
{
    QString pathOfZip;
    QString pathOfFiles;
    std::ofstream fileZip;
    std::vector<structs> headerArray;
    std::vector<metadata> metainfo;

    std::string getNameOfFile(std::string path);
    bool isUtf_8(std::string toCheck);
    void debug(std::string path, struct endOfCentralDirectory eocd);
public:

    toZip();

    ~toZip();

    void setPathOfFiles(QString pathOfFiles);

    void setPathOfZip(QString pathOfZip);

    void compess(bool isDeflate = 0);

    void collectAndLoadData();

    std::string delMainPath(std::string path);   
};

