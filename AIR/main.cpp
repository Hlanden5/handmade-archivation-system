#include <QApplication>
#include <QTextCodec>
#include <clocale>
#include "userinterface.hpp"

#pragma GCC optimize("O3")

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  userInterface w;
  w.show();
  return a.exec();
 //   zipController zip;
//    QString PathOfZip, PathOfFiles;
//    PathOfZip.push_back("D:\\C++\\archive.zip");
//    PathOfFiles.push_back("D:\\C++\\");
//    zip.setFiles(PathOfFiles);
//    zip.setZip(PathOfZip);
//    zip.dataFromMulZip();


 //   return 0;
}
