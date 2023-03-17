#include "tounzipdata.hpp"
#include "userinterface.hpp"
#include "toZipData.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  userInterface w;
  w.show();

  std::ios::sync_with_stdio(false);
  std::cin.tie(0);
  std::cout.tie(0);
  clock_t t0 = clock();
  //setlocale(0,"");
  SetConsoleOutputCP(1251);
//  toZip test;
//  QString path;
//  path.push_back("D:\\Labs\\");
//  test.setPathOfFiles(path);
//  test.setPathOfZip("D:\\AIR\\");
//  test.collectAndLoadData();
  toUnzipData test;
  QString x = QString::fromLocal8Bit("D:\\AIR\\Labs.zip");
  QString y = QString::fromLocal8Bit("D:\\AIR");
  test.setData(x,y);
  //test.findEOCD();
  std::cout << (std::cout.tellp()==-1 ? "" :"\n\n\n") << "Successful archivation!" << std::endl;
  clock_t t1 = clock();
  std::cout << std::setprecision(10) <<"time: " << double((double)(t1 - t0) / CLOCKS_PER_SEC) << std::endl;
  return 0;
  return a.exec();
}
