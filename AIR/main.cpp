#include "filemetadata.hpp"
using namespace std;
#include "toZipData.hpp"

int main()
{
 data_ test;
 QString path;
 path.push_back("D:\\Labs");
 test.setPathOfFiles(path);
 test.setPathOfZip("D:\\AIR\\");
 test.collectAndLoadData();
 std::cout << "\n\n\nSuccessful archivation!" << std::endl;
}
