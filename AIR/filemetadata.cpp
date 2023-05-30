#include "filemetadata.hpp"

namespace fs = std::filesystem;

quint32 dostime(int year, int month, int day, int hour, int minute, int second)
{
  if(year < 1980 || year > 2107
     || month < 1 || month > 12
     || day < 1 || day > 31
     || hour < 0 || hour > 23
     || minute < 0 || minute > 59
     || second < 0 || second > 59)
    {
      return 0;
    }

  return (((quint32) year - 1980) << 25)
      | (((quint32) month      ) << 21)
      | (((quint32) day        ) << 16)
      | (((quint32) hour       ) << 11)
      | (((quint32) minute     ) <<  5)
      | (((quint32) second     ) >>  1);  // 1 every other second
}

filesMetadata::filesMetadata()
{

}

void filesMetadata::setPath(QString path){
  this->path = path;
}

std::vector<metadata>& filesMetadata::getMetadata(){
  return metainfo;
}

void filesMetadata::getMSDOSFormatTime(time_t time, quint16 &timeResult,quint16 &dateResult){
  time_t even_time;
  struct tm *s;
  even_time = (time + 1) & ~1;
  s = localtime(&even_time);
  quint32 timeFormated = dostime(s->tm_year + 1900, s->tm_mon + 1, s->tm_mday, s->tm_hour, s->tm_min, s->tm_sec);
  timeResult = timeFormated >> 16;
  dateResult = timeFormated >> 32;
}

template <typename TP>
std::time_t filesMetadata::time_t_(TP tp){
  using namespace std::chrono;
  return system_clock::to_time_t(time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now()));
}

void filesMetadata::DisplayDirectoryTreeImp(const fs::path& pathToShow, int level,std::vector<metadata>& check)
{
  if (fs::exists(pathToShow) && fs::is_directory(pathToShow))
    {
      auto lead = std::string(level * 3, ' ');
      for (const auto& entry : fs::directory_iterator(pathToShow))
        {
          auto filename = entry.path().filename();
          time_t ftime = time_t_(fs::last_write_time(entry));
          check.resize(check.size()+1);
          check[check.size()-1].name = QString::fromUtf16(entry.path().generic_u16string().c_str());
          check[check.size()-1].is_directory = fs::is_directory(entry.status());
          check[check.size()-1].sizeofFile = (fs::is_directory(entry.status()) == 1 ? quint32(0) : static_cast<quint32>(fs::file_size(entry.path())));
          getMSDOSFormatTime(ftime,check[check.size()-1].timeOfLastEdit,check[check.size()-1].dataOfLastEdit);
          if (fs::is_directory(entry.status()))
            DisplayDirectoryTreeImp(entry, level + 1,check);
        }
    }
}



void filesMetadata::collectData(){
  fs::path pathToShow{path.toStdU32String()};
  if(fs::exists(pathToShow) && !fs::is_directory(pathToShow)){
      metainfo.resize(1);
      metainfo[0].is_directory = false;
      metainfo[0].name = QString::fromUtf16(pathToShow.generic_u16string().c_str());
      metainfo[0].sizeofFile = file_size(pathToShow);
      time_t ftime = time_t_(fs::last_write_time(pathToShow));
      getMSDOSFormatTime(ftime,metainfo[0].timeOfLastEdit,metainfo[0].dataOfLastEdit);
    }
  else
    DisplayDirectoryTreeImp(pathToShow,0,metainfo);
}

void filesMetadata::printMetadata(){
  std::cout <<  GetConsoleOutputCP() << std::endl;
  std::vector<metadata>::iterator iter = metainfo.begin();
  for(;iter!=metainfo.end();iter++){
      std::cout << "\n";
      std::cout << "filename = " << std::string(iter->name.toLocal8Bit())
                << "\nsize of file = " << iter->sizeofFile
                << "\ndirectory status(" << (iter->is_directory ? "YES)" : "NO)")
                << "\ntime of last edit(MS-DOS) " << iter->timeOfLastEdit
                << "\ndata of last edit(MS-DOS) " << iter->dataOfLastEdit
                << std::endl;
    }
}




