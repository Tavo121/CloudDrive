#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include "Huffman.h"

using namespace std;

int main() {
    Huffman huf("/home/tavo/Documentos/TestImage.png");
    huf.compress();

}
