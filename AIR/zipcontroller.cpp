#include "zipcontroller.hpp"
#include <fstream>

zipController::zipController(){}

void zipController::setZip(QString a){
  pathOfZip=a;
}

void zipController::setFiles(QString b){
  pathOfFiles=b;
}

int zipController::dataFromZip(){
  if(pathOfZip.size()==0 or pathOfFiles.size()==0){
      return 404; // Not enough data
    }
  if(pathOfFiles[pathOfFiles.size()-1]!='\\') pathOfFiles += '\\';
  unArch.setData(pathOfZip,pathOfFiles);
  unArch.collectAndLoadData();
  return 0;
};

int zipController::dataFromMulZip(){
  if(pathOfZip.size()==0 or pathOfFiles.size()==0){
      return 404; // Not enough data
    }
  if(pathOfFiles[pathOfFiles.size()-1]!='\\') pathOfFiles += '\\';
  std::vector<metadata> files;
  fs::path path{pathOfFiles.toStdU32String()};
  for (const auto& entry : fs::directory_iterator(path))
    {
      QString tmp(QString::fromUtf16(entry.path().generic_u16string().c_str()));
      if(tmp.size()>5)
        if(tmp[tmp.size()-1]=='p' and tmp[tmp.size()-2]=='i' and tmp[tmp.size()-3]=='z' and tmp[tmp.size()-4]=='.'){
            files.resize(files.size()+1);
            files[files.size()-1].name = tmp;
          }
    }
  for(auto &c:files){
      c.name.resize(c.name.size()-4);
      fs::path pathDir{c.name.toStdU32String()};
      fs::create_directory(pathDir);
      QString path{c.name};
      path +=".zip";
      QString dir{c.name};
      dir += '\\';
      unArch.setData(path,dir);
      unArch.collectAndLoadData();
    }
  return 0;
};

int zipController::dataToZip(){
  if(pathOfZip.size()==0 or pathOfFiles.size()==0){
      return 404; // Not enough data
    }
  std::fstream check{pathOfFiles.toLocal8Bit()};
  if(!check.is_open()){
      check.close();
      if(pathOfFiles[pathOfFiles.size()-1]!='\\') pathOfFiles += '\\';
    }
  archive.setPathOfFiles(pathOfFiles);
  archive.setPathOfZip(pathOfZip);
  archive.collectAndLoadData();
  return 0;
};
