#include "zipcontroller.hpp"
#include <QApplication>
#include <QTextCodec>
#include <clocale>

#pragma GCC optimize("O3")

int main(int argc, char *argv[])
{/*
      QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
  QApplication a(argc, argv);
  userInterface w;
  w.show();*/
  //return a.exec();
    zipController zip;
    QString PathOfZip, PathOfFiles;
    PathOfZip.push_back("C:\\");
    PathOfFiles.push_back("C:\\test\\");
    zip.setFiles(PathOfFiles);
    zip.setZip(PathOfZip);
    zip.dataToZip();

    return 0;
}
