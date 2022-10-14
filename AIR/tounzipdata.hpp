#pragma once

#include <QByteArray>
#include <QString>
#include <fstream>
#include "structs.hpp"

class toUnzipData
{
  QString pathOfZip;
  std::vector<QString> pathOfFiles;
  std::ofstream fileZip;
  structs *headerArray;
public:
    toUnzipData();
};

