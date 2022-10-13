#pragma once

#include <QByteArray>
#include <QString>
#include <fstream>
#include "structs.hpp"

class data : private structs
{
    QString pathOfZip;
    std::vector<QString> pathOfFiles;
    std::ofstream fileZip;
    structs *headerArray;

public:

    data();

    data(std::string &path);

    data(QString &path);

    void compess(bool isDeflate=0);

    void collectData();

    void loadData();

    void writeHeader(QString header);
};

