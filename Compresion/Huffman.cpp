//
// Created by tavo on 17/06/22.
//

#include <iostream>
#include "Huffman.h"

void Huffman::charList() {
    for(int i=0; i<257; i++){
        charCodes.push_back(new Node);
        charCodes[i]->frequency = 0;
        charCodes[i]->ID = i;
    }
}

void Huffman::findFrequencies() {
    int ID;
    inFile.open(fileName, ios::in);
    ID = inFile.get(); //ID of char

    while(!inFile.eof()){
        charCodes[ID]->frequency++;
        ID = inFile.get();
    }
    inFile.close();

    for(int i=0; i<257; i++){
        if(charCodes[i]->frequency > 0){ //save frequencies
            frequencies.push(charCodes[i]);
        }
    }
}

void Huffman::createTree(){
    priority_queue<Node*, vector<Node*>, Compare> tempQueue(frequencies);
    Node *left, *right;
    while(tempQueue.size() != 1){
        left = tempQueue.top();
        tempQueue.pop();
        right = tempQueue.top();
        tempQueue.pop();

        root = new Node();
        root->left = left;
        root->right = right;
        root->frequency = left->frequency + right->frequency;
        tempQueue.push(root);
    }
}

bool Huffman::isLeaf(Node* node){
    return node->left == NULL && node->right == NULL;
}

void Huffman::setCode(Node* node, string bin) {
    if(isLeaf(node)){
        node->code = bin;
        cout <<  bin << endl;
        return;
    }
    setCode(node->left, bin + '0');
    setCode(node->right, bin + '1');
}

void Huffman::createBinCodes() {
    setCode(root, "");
}

void Huffman::compress(){
    charList();
    findFrequencies();
    createTree();
    createBinCodes();
    saveCompressedFile();
}

void Huffman::decompress() {
    getTree();
    saveDecodedFile();
}

int Huffman::binToDec(string num){
    int res = 0;
    for(auto letter : num){
        res = res * 2 + letter -'0';
    }
    return res;
}

string Huffman::paddingForSize(string size) {
    if(size.length() == 3){
        return size;
    }
    string result = "";
    result.assign(3 - size.length(), '0');
    result += size;
    return result;
}

void Huffman::saveCompressedFile(){
    string data = "";
    string aux = "";
    string size = paddingForSize(to_string(frequencies.size()));
    data += size; //amount of frequencies of the tree
    priority_queue<Node*, vector<Node*>, Compare> tempQueue(frequencies);

    while(!tempQueue.empty()){
        Node* current = tempQueue.top();
        data += current->ID;

        aux.assign(257 - current->code.length(), '0'); //padding
        aux += '1';
        aux += current->code;

        data += (char) binToDec(aux.substr(0, 8));
        for(int i=0; i<15; i++){
            aux = aux.substr(8);
            data += (char) binToDec(aux.substr(0, 8));
        }
        tempQueue.pop();
    }
    aux.clear();

    inFile.open(fileName, ios::in);
    int ID;
    ID = inFile.get();
    while(!inFile.eof()){
        aux += charCodes[ID]->code;
        while(aux.length() > 8){
            data += (char) binToDec(aux.substr(0, 8));
            aux = aux.substr(8);
        }
        ID = inFile.get();
    }

    int count = 8 - aux.length();
    if(aux.length() < 8){
        aux.append(count, '0');
    }
    data += (char) binToDec(aux);
    data += (char)count;

    outFile.open("CompressedFile.dat", ios::out | ios::binary);
    outFile.write(data.c_str(), data.size());
    outFile.close();
    inFile.close();
}

string Huffman::decToBin(int num) {
    string temp = "", res = "";
    while(num > 0){
        temp += (num % 2 + '0');
        num /= 2;
    }
    res.append(8-temp.length(), '0');
    for(int i = temp.length()-1; i>=0; i--){
        res += temp[i];
    }
    return res;
}

void Huffman::buildTree(char code, string &path) {
    Node* current = root;
    for(int i=0; i<path.length(); i++){
        if(path[i] == '0'){
            if(current->left == NULL){
                current->left = new Node();
            }
            current = current->left;
        }
        else if (path[i] == '1'){
            if(current->right == NULL){
                current->right = new Node();
            }
            current = current->right;
        }
    }
    current->ID = code;
}

void Huffman::getTree(){
    inFile.open(fileName, ios::in | ios::binary);

    unsigned char size;
    inFile.read(reinterpret_cast<char*>(&size), 1);
    root = new Node();

    for(int i=0; i<size; i++){
        char Code;
        unsigned char Codec[16];
        inFile.read(&Code, 1);
        inFile.read(reinterpret_cast<char*>(Codec), 16);

        string CodecStr = "";
        for(int i=0; i<16; i++){
            CodecStr += decToBin(Codec[i]);
        }

        int j=0;
        while(CodecStr[j] == '0'){
            j++;
        }
        CodecStr = CodecStr.substr(j+1);
        buildTree(Code, CodecStr);
    }
    inFile.close();
}

void Huffman::saveDecodedFile() {
    inFile.open(fileName, ios::in | ios::binary);
    outFile.open("outFileName.pdf", ios::out);
    unsigned char size;
    inFile.read(reinterpret_cast<char*>(&size), 1);
    //Reading count at the end of the file which is number of bits appended to make final value 8-bit
    inFile.seekg(-1, ios::end);
    char count0;
    inFile.read(&count0, 1);
    //Ignoring the meta data (huffman tree) (1 + 17 * size) and reading remaining file
    inFile.seekg(1 + 17 * size, ios::beg);

    vector<unsigned char> text;
    unsigned char textseg;
    inFile.read(reinterpret_cast<char*>(&textseg), 1);
    while (!inFile.eof()) {
        text.push_back(textseg);
        inFile.read(reinterpret_cast<char*>(&textseg), 1);
    }

    Node *curr = root;
    string path;
    for (int i = 0; i < text.size() - 1; i++) {
        //Converting decimal number to its equivalent 8-bit binary code
        path = decToBin(text[i]);
        if (i == text.size() - 2) {
            path = path.substr(0, 8 - count0);
        }
        //Traversing huffman tree and appending resultant data to the file
        for (int j = 0; j < path.size(); j++) {
            if (path[j] == '0') {
                curr = curr->left;
            }
            else {
                curr = curr->right;
            }

            if (curr->left == NULL && curr->right == NULL) {
                outFile.put(curr->ID);
                curr = root;
            }
        }
    }
    inFile.close();
    outFile.close();
}

Huffman::Huffman(string FileName) {
    fileName = FileName;
}