#pragma once
#include "structs.hpp"
namespace fs = std::filesystem;


class filesMetadata
{
  std::vector<metadata> metainfo;
  QString path;

  template <typename TP>
  std::time_t time_t_(TP tp);

  void getMSDOSFormatTime(time_t time, quint16 &timeResult, quint16 &dateResult);

  void DisplayDirectoryTreeImp(const fs::path& pathToShow, int level,std::vector<metadata>& check);
public:
  filesMetadata();

  void setPath(QString path);

  std::vector<metadata> &getMetadata();

  void collectData();

  void printMetadata();

  static time_t dos2unixtime(quint16 &timeResult, quint16 &dateResult);
};

