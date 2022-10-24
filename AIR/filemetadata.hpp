#pragma once

namespace fs = std::filesystem;

struct metadata{
  quint16 timeOfLastEdit;
  quint16 dataOfLastEdit;
  quint32 sizeofFile;
  QString name;
  bool is_directory;
};

  template <typename TP>
    std::time_t time_t_(TP tp);
class filesMetadata
{
  std::vector<metadata> metainfo;
  QString path;
public:
  filesMetadata();

  void setPath(QString path);

  void collectData();

  void printMetadata();

  std::vector<metadata>& getMetadata();
};

