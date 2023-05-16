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
public:
  zipController();

  void setZip(QString a);

  void setFiles(QString b);

  int dataFromZip(); // Int for errors

  int dataToZip();
  
  int dataFromMulZip();
};
