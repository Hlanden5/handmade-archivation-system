#include "filemetadata.hpp"
#include "toZipData.hpp"

int main()
{
 clock_t t0 = clock();

 data_ test;
 QString path;
 path.push_back("D:\\Labs");
 test.setPathOfFiles(path);
 test.setPathOfZip("D:\\AIR\\");
 test.collectAndLoadData();
 std::cout << "\n\n\nSuccessful archivation!" << std::endl;

 clock_t t1 = clock();
 std::cout << "time: " << (double)(t1 - t0) / CLOCKS_PER_SEC << std::endl;
}
