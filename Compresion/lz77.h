#ifndef LZ77_H
#define LZ77_H


#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <vector>
#include "linkedlist.h"
using namespace std;


struct link {
    unsigned short length;
    unsigned short offset;
    unsigned short next;
};


class LZ77
{
    public:
    //max_buf_length=15; //16-1;
   // max_dict_length=4095; //4096-1
    unsigned int max_buf_length = 15;
    unsigned int max_dict_length = 4095;
    unsigned int cur_dict_length = 0;
    unsigned int cur_buf_length = 0;


    unsigned int readFile(unsigned char* &raw, std::fstream &inp);
    void FindLongestMatch(unsigned char* s, unsigned int buf_start, unsigned short &len, unsigned short &off, Linkedlist dict[]);
    int UpdateDictionary(unsigned char* s, unsigned int shift_start, unsigned short Length, Linkedlist dict[]);
    void compactAndWriteLink(link inp, vector<unsigned char>&out);
    void compressData(unsigned int file_size, unsigned char* data, fstream &file_out);
    void uncompressData(unsigned int file_size, unsigned char* data, fstream &file_out);
};

#endif // LZ77_Hict
