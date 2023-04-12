#pragma once
#include <toZipData.hpp>
#include <toUnzipData.hpp>
class zipController
{
  //Main objects to working
  toZip archive;
  toUnzipData unArch;

  //Needed variables
  QString pathOfZip;
  QString pathOfFiles; // Or directory with + "\\"
  /*
   * case archive:
   * pathOfZip = pathOfZip;
   * pathOfFiles = directory/file where it comes file/files
   *
   * case unArch:
   * pathOfZip = pathOfZip
   * pathOfFiles = exporting files to pathOfFiles
  */

  bool isDeflate;

  bool checkIfDir();
public:
  zipController();

  int setData(QString PathOfZip, QString PathOfFiles);

  int dataFromZip(); // Int for errors

  int multipleUnArch(QString directory);

  int loadDataToZip();

  size_t sizeOfDir(QString directory);

  size_t countFilesOfDir(QString directory);
};
