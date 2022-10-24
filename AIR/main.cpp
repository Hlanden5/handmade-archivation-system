#include "filemetadata.hpp"
using namespace std;
#include "toZipData.hpp"

int main()
{
 data_ test;
 QString path;
 path.push_back("D:\\Labs\\");
 test.setPathOfFiles(path);
 test.setPathOfZip("D:\\Labs\\");
 test.collectAndLoadData();
}
