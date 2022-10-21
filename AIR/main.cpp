using namespace std;
#include "toZipData.hpp"

int main()
{
 data_ test;
 std::vector<QString> path;
 path.push_back("D:\\Labs\\main.cpp");
 test.setPathOfFiles(path);
 test.setPathOfZip("D:\\Labs\\");
 test.collectAndLoadData();
}
