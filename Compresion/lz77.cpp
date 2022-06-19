#include "lz77.h"
#include "linkedlist.h"
using namespace std;
unsigned int LZ77::readFile(unsigned char *&raw, std::fstream &inp)
{
    inp.seekg(0,ios::beg);
    unsigned int file_start = inp.tellg();
    inp.seekg(0,ios::end);
    unsigned int file_end = inp.tellg();
    unsigned int file_size = file_end - file_start;
    inp.read((char*)raw,file_size);
    return file_size;
}

void LZ77::FindLongestMatch(unsigned char *s, unsigned int buf_start, unsigned short &len, unsigned short &off, Linkedlist dict[])
{

    Node* current = dict[s[buf_start]].lastNode;
    unsigned int max_offset = buf_start - cur_dict_length;
    unsigned int j = 0;
    unsigned int k = 0;
    if(current!=nullptr&& (current->index) >= max_offset){len=1; off = buf_start-(current->index); }
    while(current!=nullptr && (current->index) >= max_offset){
        j=1, k=1;
        while(k<cur_buf_length && s[(current->index)+j] == s[buf_start+k]){
            if((current->index)+j == buf_start-1){ j= 0; }
            else j++;
            k++;
        }
        if(k>len){
            len = k;
            off == buf_start-(current->index);
            if(len==cur_buf_length) break;
        }
        else
        {
         current=current->prev;
        }
    }
}

int LZ77::UpdateDictionary(unsigned char *s, unsigned int shift_start, unsigned short Length, Linkedlist dict[])
{
    for(unsigned int i = shift_start; i<(shift_start+Length+1);i++ ){
        dict[s[i]].PushBack(i);
    }
    return Length;
}

void LZ77::compactAndWriteLink(link inp, vector<unsigned char> &out)
{
    if(inp.length!=0){
        out.push_back((unsigned char)((inp.length<<4) | (inp.offset>>8)));
        out.push_back((unsigned char)(inp.offset));
    } else{
        out.push_back((unsigned char)((inp.length<<4)));
    }
    out.push_back(inp.next);
}

void LZ77::compressData(unsigned int file_size, unsigned char *data, fstream &file_out)
{
    Linkedlist dict[256];
    link myLink;
    vector<unsigned char> lz77_coded;
    lz77_coded.reserve(2*file_size);
    bool hasOddSymbol=false;
    unsigned int target_size = 0;
    file_out.seekp(0, ios_base::beg);
    cur_dict_length = 0;
    cur_buf_length = max_buf_length;
        for(unsigned int i=0; i<file_size; i++)
        {
            if((i+max_buf_length)>=file_size)
            {
                cur_buf_length = file_size-i;
            }
            myLink.length=0;myLink.offset=0;
            FindLongestMatch(data,i,myLink.length,myLink.offset, dict);
            i=i+UpdateDictionary(data, i, myLink.length, dict);
            if(i<file_size) {
                myLink.next=data[i]; }
                else { myLink.next='_'; hasOddSymbol=true; }
            compactAndWriteLink(myLink, lz77_coded);
            if(cur_dict_length<max_dict_length) {
            while((cur_dict_length < max_dict_length) && cur_dict_length < (i+1)) cur_dict_length++;
            }
       }
       if(hasOddSymbol==true) { lz77_coded.push_back((unsigned char)0x1); }
       else lz77_coded.push_back((unsigned char)0x0);
       target_size=lz77_coded.size();
       file_out.write((char*)lz77_coded.data(), target_size);
       lz77_coded.clear();
       printf("Output file size: %d bytes\n", target_size);
       printf("Compression ratio: %.3Lf:1\n", ((double)file_size/(double)target_size));
}

void LZ77::uncompressData(unsigned int file_size, unsigned char *data, fstream &file_out)
{
    if(file_size==0) { printf("Error! Input file is empty\n"); return; }
    link myLink;
    vector<unsigned char> lz77_decoded;
    lz77_decoded.reserve(4*file_size);
    unsigned int target_size = 0;
    unsigned int i=0;
    int k=0;
    file_out.seekg(0, ios::beg);
    while(i<(file_size-1))
    {
        if(i!=0) { lz77_decoded.push_back(myLink.next); }
        myLink.length = (unsigned short)(data[i] >> 4);
        if(myLink.length!=0)
        {
            myLink.offset = (unsigned short)(data[i] & 0xF);
            myLink.offset = myLink.offset << 8;
            myLink.offset = myLink.offset | (unsigned short)data[i+1];
            myLink.next = (unsigned char)data[i+2];
            if(myLink.offset>lz77_decoded.size()) {
                printf("Error! Offset is out of range!");
                lz77_decoded.clear();
                return;
            }
            if(myLink.length>myLink.offset)
            {
                k = myLink.length;
                while(k>0)
                {
                    if(k>=myLink.offset)
                    {
                        lz77_decoded.insert(lz77_decoded.end(), lz77_decoded.end()-myLink.offset, lz77_decoded.end());
                        k=k-myLink.offset;
                    }
                    else
                    {
                        lz77_decoded.insert(lz77_decoded.end(), lz77_decoded.end()-myLink.offset, lz77_decoded.end()-myLink.offset+k);
                        k=0;
                    }
                }
            }
            else lz77_decoded.insert(lz77_decoded.end(), lz77_decoded.end()-myLink.offset, lz77_decoded.end()-myLink.offset+myLink.length);
            i++;
        }
        else {
            myLink.offset = 0;
            myLink.next = (unsigned char)data[i+1];
        }
        i=i+2;
    }
    unsigned char hasOddSymbol = data[file_size-1];
    if(hasOddSymbol==0x0 && file_size>1) { lz77_decoded.push_back(myLink.next); }
    target_size = lz77_decoded.size();
    file_out.write((char*)lz77_decoded.data(), target_size);
    lz77_decoded.clear();
    printf("Output file size: %d bytes\n", target_size);
    printf("Unpacking finished!");

}


