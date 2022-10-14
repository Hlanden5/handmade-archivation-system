#pragma once

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

