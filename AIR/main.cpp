using namespace std;
#include "toZipData.hpp"

int main()
{
 data_ test;
 std::vector<QString> path;
 path.push_back("D:\\Labs\\input.txt");
 test.setPathOfFiles(path);
 test.setPathOfZip("D:\\Labs\\1.zip");
 test.collectAndLoadData();
}
