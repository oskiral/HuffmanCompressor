#include <cstdlib>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>

using namespace std;


// i use this struct to create the Node object
// every Node element has frequency (int), character inside (char), and two children nodes (they might be nullptr)
typedef struct Node {
        
        int frequency;
        char character;
        
        struct Node *left, *right;
        
} Node;


Node* createNewNode(int frequency, char character) {
      
      
  Node* newNode = (Node*)malloc(sizeof(Node));
  
  newNode->frequency = frequency;
  newNode->character = character;
  newNode->left = newNode->right = nullptr;
  
  
  return newNode;    
  
  
};


// i use this struct with operator() to compare priority of elemetns in priority queue
struct CompareNodes {
       bool operator()(Node* a, Node* b) {
            return a->frequency > b->frequency;   
       };     
};



// this function returns nothing and i use it to print the binary tree so i can see how does it look in console
// root : Node*
// indent: string ("")
void printTree(Node* root, string indent = "") {
    if (!root) return;

    if (root->character != '*') {
        cout << indent << "'" << root->character << "' (" << root->frequency << ")\n";
    } else {
        cout << indent << "* (" << root->frequency << ")\n";
    }

    printTree(root->left, indent + "  ");
    printTree(root->right, indent + "  ");
}


// this function returns nothing and im using it to generate codes for every character in binary tree
// root : Node*
// code : string
// codes: unordered_map <char, string>&

void buildCodes(Node* root, string code, unordered_map<char, string>& codes){
     
     if (!root) return;
     
     
     if (root->left == nullptr && root->right == nullptr) {
        codes[root->character] = code;
        return;
     };

     buildCodes(root->left, code + "0", codes);
     buildCodes(root->right, code + "1", codes);
};


// i use this function to read a file that i will be compresing
// this function returns content of a .txt file in string

string readFileToString(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        throw runtime_error("Cannot open file");
    }
    stringstream buffer;
    buffer << inFile.rdbuf();
    return buffer.str();
}

// i use this function to decompress file i compressed before
// this function returns decoded text

string decompress(const string& filename) {
    ifstream in(filename, ios::binary);
    size_t mapSize;
    in.read(reinterpret_cast<char*>(&mapSize), sizeof(mapSize));

    unordered_map<string, char> codeToChar;
    for (size_t i = 0; i < mapSize; i++) {
        char ch;
        uint8_t len;
        in.get(ch);
        in.read(reinterpret_cast<char*>(&len), sizeof(len));
        string code(len, ' ');
        in.read(&code[0], len);
        codeToChar[code] = ch;
    }

    vector<unsigned char> compressedBytes((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    string bits;
    for (unsigned char byte : compressedBytes) {
        for (int i = 7; i >= 0; i--) {
            bits += ((byte >> i) & 1) ? '1' : '0';
        }
    }

    string decodedText;
    string currentCode;
    for (char bit : bits) {
        currentCode += bit;
        if (codeToChar.count(currentCode)) {
            decodedText += codeToChar[currentCode];
            currentCode.clear();
        }
    }
    return decodedText;
}

int main(int argc, char *argv[])
{
    // initialize a vector that we gonna store nodes in 
    vector<Node*> nodes;
    
    
    // char* buffor = new char[2048];
    // const char* buffor = "Hello World";
    //std::string text = "Hello World";
    
    //cout << "frequency of " << b.letter << " is " << b.frequency << endl;

    string buffor = readFileToString("input1.txt");
    // intialize an array that we gonna store frequency of letters in
    int count[256] = {0};
    
    // count the characters
    for (int i = 0; buffor[i] != '\0'; i++) {
        
        unsigned char character = buffor[i];
        count[character]++;    
        
    };
    
    // create and add the nodes into the vector
    for (int i = 0; i < 256; i++) {
        
        // initialize new Node* object and add it to vector
        if (count[i] > 0) {
                     
           char character = (char)(i);
           int frequency = count[i];
           
           //cout << character << " : " << frequency << endl;
           
           
           Node* newNode = createNewNode(frequency, character);
           nodes.push_back(newNode);
           
        };
    }

    
    
    // initialize a priority queue that we gonna use to store pointers of nodes of a binary tree
    priority_queue<Node*, std::vector<Node*>, CompareNodes> pq;
    for (Node* node : nodes) {
       pq.push(node);  
    };
    
    // creating a binary tree
    while (pq.size() > 1) {
          Node* left = pq.top();
          pq.pop();
          
          Node* right = pq.top();
          pq.pop();
          
          Node* merged = createNewNode(right->frequency + left->frequency, '*');
          merged->left = left;
          merged->right = right;
          
          pq.push(merged);
               
    };
    
    Node* root = pq.top();
    
    // print the tree
    printTree(root);
    
    // create a unordered map that we gonna use to store character codes (character : code)
    
    unordered_map<char, std::string> codes;
    buildCodes(root, "", codes);
    
    // print characters with codes
    for (auto& pair : codes) {
        cout << "'" << pair.first << "' : " << pair.second << endl;
    }
    
    // print the encoded text
    
    
    string encodedBits;
    for (int i = 0; buffor[i] != '\0'; i++) {
        encodedBits += codes[buffor[i]]; 
        
    };
    //cout << "Encoded text: " << encodedBits << endl;
    
    vector<unsigned char> compressedBytes;
    unsigned char byte = 0;
    int bitCount = 0;
    
    for (char bit : encodedBits) {
        byte <<= 1;        
        if (bit == '1') byte |= 1; 
        bitCount++;
    
        if (bitCount == 8) {
            compressedBytes.push_back(byte);
            byte = 0;
            bitCount = 0;
        }
    }
    
    // if there is more bits fill with the zeroes
    
    if (bitCount > 0) {
        byte <<= (8 - bitCount); 
        compressedBytes.push_back(byte);
    };
    
    //for (char compressedByte : compressedBytes) {
    //    cout << compressedByte << endl;
    //};
    
    ofstream outFile("compressed.huff", std::ios::binary);
    
    size_t mapSize = codes.size();
    outFile.write(reinterpret_cast<const char*>(&mapSize), sizeof(mapSize));
    
    // saving the map that i will be using for decompressing
    
    for (const auto& pair : codes) {
        outFile.put(pair.first);
        uint8_t codeLength = pair.second.size();
        outFile.put(codeLength);
        outFile.write(pair.second.c_str(), codeLength);
    }
    
    // save the bytes
    
    for (unsigned char byte : compressedBytes) {
        outFile.put(byte);
    }

    outFile.close();
    
    //cout << "Decoded text is " << decompress("compressed.huff") << endl;
    
    ofstream decompressedFile("decompressed.txt", std::ofstream::trunc);
    
    decompressedFile << decompress("compressed.huff");
    
    decompressedFile.close();
    
    // delete* buffor;
    system("PAUSE");
    return EXIT_SUCCESS;
}
