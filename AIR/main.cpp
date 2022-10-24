
#include "filemetadata.hpp"
using namespace std;
#include "toZipData.hpp"

int main()
{
 data_ test;
 std::vector<QString> path;
 path.push_back("D:\\Labs");
 filesMetadata x;
 x.setPath(path[0]);
 x.collectData();
 x.printMetadata();
// path.push_back("D:\\Labs\\input.txt");
// path.push_back("D:\\Labs\\output.txt");
// path.push_back("D:\\Labs\\Lab2_encode\\main.cpp");
// path.push_back("D:\\Labs\\Lab2_decode\\main.cpp");
// test.setPathOfFiles(path);
// test.setPathOfZip("D:\\Labs\\");
// test.collectAndLoadData();
}
