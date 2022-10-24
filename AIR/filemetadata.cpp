#include "filemetadata.hpp"

namespace fs = std::filesystem;

filesMetadata::filesMetadata()
{

}

void filesMetadata::setPath(QString path){
  this->path = path;
}

std::vector<metadata>& filesMetadata::getMetadata(){
  return metainfo;
}

template <typename TP>
std::time_t time_t_(TP tp){
  using namespace std::chrono;
  return system_clock::to_time_t(time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now()));
}

void DisplayDirectoryTreeImp(const fs::path& pathToShow, int level,std::vector<metadata>& check)
{
  if (fs::exists(pathToShow) && fs::is_directory(pathToShow))
    {
      auto lead = std::string(level * 3, ' ');
      for (const auto& entry : fs::directory_iterator(pathToShow))
        {
          auto filename = entry.path().filename();
          //DisplayFileInfo(entry, lead, filename);
          auto ftime = time_t_(fs::last_write_time(entry)); // needs further implementation
          check.resize(check.size()+1);
          //std::vector<metadata>::iterator iter= check.end();
          check[check.size()-1].name = QString::fromStdString(entry.path().string());
          check[check.size()-1].is_directory = fs::is_directory(entry.status());
          check[check.size()-1].timeOfLastEdit = 0; // needs further implementation
          check[check.size()-1].dataOfLastEdit = 0; // needs further implementation
          check[check.size()-1].sizeofFile = (fs::is_directory(entry.status()) == 1 ? quint32(0) : static_cast<quint32>(fs::file_size(entry.path())));
          if (fs::is_directory(entry.status()))
            DisplayDirectoryTreeImp(entry, level + 1,check);
        }
    }
}



void filesMetadata::collectData(){
  namespace fs = std::filesystem;
  fs::path pathToShow{path.toStdU32String()};
  DisplayDirectoryTreeImp(pathToShow,0,metainfo);

}

void filesMetadata::printMetadata(){
  std::vector<metadata>::iterator iter = metainfo.begin();
  for(;iter!=metainfo.end();iter++){
      std::cout << "\n";
      std::cout << "filename = " << iter->name.toStdString()
                << "\nsize of file = " << iter->sizeofFile
                << "\ndirectory status(" << (iter->is_directory ? "YES)" : "NO)")
                << "\ntime of last edit " << iter->timeOfLastEdit
                << "\ndate of last edit " << iter->dataOfLastEdit
                << std::endl;
    }
}


