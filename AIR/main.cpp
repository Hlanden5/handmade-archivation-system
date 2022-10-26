#include "filemetadata.hpp"
#include "toZipData.hpp"


int main()
{
 std::ios::sync_with_stdio(false);
 std::cin.tie(0);
 std::cout.tie(0);
 clock_t t0 = clock();

 data_ test;
 QString path;
 path.push_back("D:\\Gif for OBS\\");
 test.setPathOfFiles(path);
 test.setPathOfZip("D:\\AIR\\");
 test.collectAndLoadData();
 std::cout << (std::cout.tellp()==-1 ? "" :"\n\n\n") << "Successful archivation!" << std::endl;
 clock_t t1 = clock();
 std::cout << "time: " << (double)(t1 - t0) / CLOCKS_PER_SEC << std::endl;
}
