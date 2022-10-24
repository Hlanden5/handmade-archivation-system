#pragma once

#include "structs.hpp"
#include "filemetadata.hpp"

class data_ : private structs
{
    QString pathOfZip;
    QString pathOfFiles;
    std::ofstream fileZip;
    std::vector<structs> headerArray;
    std::vector<metadata> metainfo;
    std::string getNameOfFile(std::string path);

public:

    data_();

    ~data_();

    void setPathOfFiles(QString pathOfFiles);

    void setPathOfZip(QString pathOfZip);

    void compess(bool isDeflate = 0);

    void collectAndLoadData();

};

