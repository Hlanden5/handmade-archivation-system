#pragma once

#include "structs.hpp"

class data_ : private structs
{
    QString pathOfZip;
    std::vector<QString> pathOfFiles;
    std::ofstream fileZip;
    std::vector<structs> headerArray;

public:

    data_();

    ~data_();

    void setPathOfFiles(std::vector<QString> pathOfFiles);

    void setPathOfZip(QString pathOfZip);

    void compess(bool isDeflate = 0);

    void collectAndLoadData();

    void writeHeader(QString headerfilepath);
};

