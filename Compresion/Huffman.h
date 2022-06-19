//
// Created by tavo on 17/06/22.
//

#ifndef TEST_HUFFMAN_H
#define TEST_HUFFMAN_H

#include <string>
#include <fstream>
#include <queue>

using namespace std;

class Huffman {
public:
    Huffman(string FileName);
    void compress(), decompress();
private:
    struct Node{
        char ID;
        unsigned frequency;
        string code;
        Node *left, *right;

    };
    ifstream inFile;
    ofstream outFile;
    string fileName;
    Node *root;
    class Compare{
    public:
        bool operator() (Node *left, Node *right){
            return left->frequency > right->frequency;
        }
    };
    priority_queue<Node*, vector<Node*>, Compare> frequencies;
    vector<Node*> charCodes;
    void findFrequencies(), charList(), createTree(), createBinCodes(), setCode(Node* node, string bin), saveCompressedFile();
    void saveDecodedFile(), buildTree(char code, string& path), getTree();
    string paddingForSize(string size);
    bool isLeaf(Node* node);
    int binToDec(string num);
    string decToBin(int num);

};


#endif //TEST_HUFFMAN_H
