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

public:

    data();

    data(std::string &path);

    data(QString &path);

    void compess(bool isDeflate=0);

    void loadData();

    void writeHeader(QString header);
};

