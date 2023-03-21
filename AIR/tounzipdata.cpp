#include "crc.hpp"
#include "structs.hpp"
#include "toUnzipData.hpp"


int findASCII(char data){

    for(int i=-128;i<256;i++)
        {
            if(char(i)==data)
                return i;
        }

    return 0; // Withour warning
}


toUnzipData::toUnzipData()
{

}


toUnzipData::~toUnzipData()
{

}


void toUnzipData::findAllSignatures(std::vector<size_t> &LFH, std::vector<size_t> &CFH, size_t& EOCD){

    getDataFromZip();//DELETE THIS
    LFH.clear();
    CFH.clear();
    EOCD = 0;
    std::vector<size_t> eocd_index;

    { // EOCD
        std::string rt("PK");
        rt += 0x05;
        rt += 0x06;
        std::regex r(rt);
        auto begin = std::sregex_iterator(data.begin(), data.end(), r);
        auto end = std::sregex_iterator();

    //    std::cout << "Found EOCD "
    //              << std::distance(begin, end) << std::endl;


        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            eocd_index.push_back(match.position());
        }
    } // END EOCD


    { // LFH
        std::string rt("PK");
        rt += 0x03;
        rt += 0x04;
        std::regex r(rt);
        auto begin = std::sregex_iterator(data.begin(), data.end(), r);
        auto end = std::sregex_iterator();

    //    std::cout << "Found LFH "
    //              << std::distance(begin, end) << std::endl;


        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            LFH.push_back(match.position());
        }
    } // END LFH


    { // CFH
        std::string rt("PK");
        rt += 0x01;
        rt += 0x02;
        std::regex r(rt);
        auto begin = std::sregex_iterator(data.begin(), data.end(), r);
        auto end = std::sregex_iterator();

    //    std::cout << "Found CFH "
    //              << std::distance(begin, end) << std::endl;


        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            CFH.push_back(match.position());
        }
    } // END CFH


    std::cout << "EOCD finded:" << std::endl;
    for (auto c:eocd_index){
        std::cout << c << std::endl;
    }
    std::cout << std::endl;

    std::cout << "LFH finded:" << std::endl;
    for (auto c:LFH){
        std::cout << c << std::endl;
    }
    std::cout << std::endl;


    std::cout << "CFH finded:" << std::endl;
    for (auto c:CFH){
        std::cout << c << std::endl;
    }
    std::cout << std::endl;
    std::cout<<eocd_index.size()<<std::endl;

    while(eocd_index.size()>1){
        size_t pos = eocd_index[0]+8;
        std::string str=data.substr(pos,2);
        reverse(str.begin(),str.end());
        size_t count_=0;
        std::stringstream stream1,stream2;
        stream2 << "0x";

        for(int i=0;i<2;i++){
        stream1 << std::hex << findASCII(str[i]);
        std::string tmp = stream1.str();
        if(tmp.size()>2)
        tmp.erase(0,6);
        stream1.str("");
        stream2 << tmp;
        }

        stream2 >> std::hex >> count_;
        std::cout << count_ << std::endl;

        size_t max = 0;
        size_t index = 0;
        for(size_t i=0;i<CFH.size();i++){
            if(CFH[i]>max and CFH[i]<pos){
            max = CFH[i];
            index = i;
            }
        }


        std::vector<size_t>::iterator startCHF, startLFH;
        startLFH=LFH.begin();
        startCHF=CFH.begin();
        for(size_t i=index;i<index-count_;i--){
            std::advance(startLFH,i);
            std::advance(startCHF,i);
            LFH.erase(startLFH);
            CFH.erase(startCHF);
        }

        eocd_index.erase(eocd_index.begin());
        //std::cout << "ERASE, max = " << std::endl;

        std::cout << "LFH:" << std::endl;
        for (auto c:LFH){
        std::cout << c << std::endl;
        }

        std::cout << std::endl;

        std::cout << "CFH:" << std::endl;
        for (auto c:CFH){
            std::cout << c << std::endl;

        }

    }



    std::cout << "EOCD finded:" << std::endl;
    for (auto c:eocd_index){
    std::cout << c << std::endl;
    }

    EOCD = eocd_index[0];
    std::cout << std::endl;

    std::cout << "LFH finded:" << std::endl;
    for (auto c:LFH){
        std::cout << c << std::endl;
    }

    std::cout << std::endl;
    std::cout << "CFH finded:" << std::endl;
    for (auto c:CFH){
        std::cout << c << std::endl;
    }

}

void toUnzipData::setEOCD(){
    eocd.countOfCFH=0;
}



void toUnzipData::setData(QString &pathOfZip,QString &pathOfFiles){
    std::cout <<std::string(pathOfZip.toLocal8Bit())<<std::endl;
    this->pathOfZip = pathOfZip;
    this->pathOfFiles = pathOfFiles;
    this->fileZip = std::ifstream(pathOfZip.toLocal8Bit(),std::ios_base::binary);
    if(!fileZip.is_open())
        throw;
}


void toUnzipData::getDataFromZip(){
    fileZip.seekg(0,std::ios::end);
    size_t len_input = fileZip.tellg();
    fileZip.seekg(0,std::ios::beg);
    data.resize(len_input);
    fileZip.read((char*)&data[0],len_input);
    // if(fileZip.good()) std::cout << data << std::endl;
    fileZip.close();
}
