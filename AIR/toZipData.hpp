#pragma once

#include "structs.hpp"

class data : private structs
{
    QString pathOfZip;
    std::vector<QString> pathOfFiles;
    std::ofstream fileZip;
    structs *headerArray;

public:

    data();

    ~data();

    void setPathOfFiles(std::vector<QString> pathOfFiles);

    void setPathOfZip(QString pathOfZip);

    void compess(bool isDeflate = 0);

    void collectAndLoadData();

    void writeHeader(QString header);

};

