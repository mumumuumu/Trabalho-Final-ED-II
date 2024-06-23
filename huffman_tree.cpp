#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <queue>
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>

namespace huffman_tree{

struct node {
  wchar_t caracter;
  int frequency;
  node *left, *right;
};

node *create_node(const wchar_t &caracter, const int &frequency ) {
  node *p = new node;
  p->caracter = caracter;
  p->frequency = frequency;
  p->left = p->right = nullptr;
  return p;
}

int height(node *root) {
  if (root == nullptr) return -1;
  return std::max(height(root->left), height(root->right)) + 1;
}

class Compare
{
    public:
    bool operator()(node *n1, node *n2){
        return n1->frequency > n2->frequency;

    }
};

std::string read_path(){
  std::string path;
  std::cout<< "Insira caminho do arquivo: ";
  std::cin >> path;
  return path;
}

std::unordered_map<wchar_t, int> calculate_frequency(const std::string path){
    std::wifstream arq(path);
    std::unordered_map<wchar_t, int> word_count;
    // if (!arq) {
    //     std::wcout << L"Arquivo não encontrado!\n";
    //     return 1;
    // }
    wchar_t ch;
    if (arq.is_open()) {
        while (arq.get(ch)) {
            word_count[ch]++;
        }
    }
    arq.close();
    return word_count;
}

node* create_huffman(std::unordered_map<wchar_t, int> frequency){
        std::priority_queue<node*,std::vector<node*>,Compare> pq;
        for (auto p : frequency) {
            auto nodeC = create_node(p.first,p.second);
            pq.push(nodeC);
        }
        while(pq.size()!=1){
            auto *left = pq.top();
            pq.pop();            
            auto *right = pq.top();
            pq.pop();
            node* auxNode = new node;
            auxNode->caracter = L'I';
            auxNode->frequency = left->frequency+right->frequency;
            auxNode ->left = left;
            auxNode ->right = right;
            pq.push(auxNode);
        }
        node* root = pq.top();    
        return root;   
    }

void generate_codification(std::unordered_map<wchar_t, std::wstring> &dicionario, node *root, const std::wstring &caminho) {
  if (root->left == nullptr && root->right == nullptr) {
    dicionario[root->caracter] = caminho;
  } else {
    generate_codification(dicionario, root->left, caminho + L"0");
    generate_codification(dicionario, root->right, caminho + L"1");
  }
}


// void showTree(node *root, int level = 0, const std::wstring &prefix = L"√ ") {
//   if (!root) return;

//   auto ident = std::wstring(level * 4, ' ');

//   std::wcout << ident << L"N:"<<level<< prefix << root->caracter << L"--"<<root->frequency << std::endl;
//   showTree(root->left, level + 1, L"↙ ");
//   showTree(root->right, level + 1, L"↘ ");
// }
void showTreeRecursive(std::wstring &dot, node *root, const std::wstring &caminho) {
    if (root == nullptr) {
        return;
    }

    if (root->left == nullptr && root->right == nullptr) {
        dot += L"\"" + std::to_wstring(root->caracter )+ L"\"";
        dot += L"[shape=record, label=\"{{";
        dot += root->caracter;
        dot += L"|";
        dot += std::to_wstring(root->frequency);
        dot += L"}|";
        dot += caminho;
        dot += L"}\"];\n";
    } else {
        //Esquerda
        dot += std::to_wstring(root->frequency);
        dot += L"->";
        if(root->left->caracter == (wchar_t)L'I'){
          dot += std::to_wstring(root->left->frequency);
        }else{  
        dot += L"\"" + std::to_wstring(root->left->caracter )+ L"\"";
        }
        dot += L";\n";

        //Direita
        dot += std::to_wstring(root->frequency);
        dot += L"->";
        if(root->right->caracter == (wchar_t)L'I'){
          dot += std::to_wstring(root->right->frequency);
        }else{  
        dot += L"\"" + std::to_wstring(root->right->caracter) +L"\"";
        }        
        dot += L"[label=1]";
        dot += L";\n";

        showTreeRecursive(dot, root->left, caminho + L"0");
        showTreeRecursive(dot, root->right, caminho + L"1");
    }
}

void showTree(node *root) {
    std::wofstream dot("/tmp/huffman.dot");
    dot << L"digraph G {\n";
    dot << L"edge [label=0];\n";
    dot << L"graph [ranksep=0];\n";
    std::wstring corpoDot;
    showTreeRecursive(corpoDot, root, L"");
    dot << corpoDot;
    dot << L"\n}\n";
    dot.close();
    system("dot /tmp/huffman.dot -Tx11");
}

void compressFile(){
    auto path = read_path();
    auto caracter_count = calculate_frequency(path);
    auto root = create_huffman(caracter_count);
    std::unordered_map<wchar_t, std::wstring> dicionario;
    generate_codification(dicionario,root,L"");
    //showTree(root);
    showTree(root);
    // for (const auto &entry : dicionario) {
    //     std::wcout << entry.first << L": " << entry.second.c_str() << std::endl;
    // }
}

void descompressFile(){
  
}




};