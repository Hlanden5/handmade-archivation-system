#pragma once

#include "structs.hpp"

class toUnzipData
{
  QString pathOfZip;
  QString pathOfFiles;
  std::ifstream fileZip;
  std::vector <structs> headerArray;
public:
    toUnzipData();
};

