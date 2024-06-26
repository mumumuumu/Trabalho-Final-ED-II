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
    std::unordered_map<wchar_t, int> caracter_count;
    if (!arq) {
        std::wcout << L"================ Arquivo não encontrado! ==================\n";
        return caracter_count;
    }
    wchar_t ch;
    if (arq.is_open()) {
        while (arq.get(ch)) {
            caracter_count[ch]++;
        }
    }
    arq.close();
    return caracter_count;
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

// FUNÇÕES DE VISUALIZAÇÃO
void showTreeRecursive(std::wstring &dot, node *root, const std::wstring &caminho) {
    if (root == nullptr) {
        return;
    }

    auto aspas = L"\"";
    std::wstring nodeId = std::to_wstring(root->frequency) + caminho;

    if (root->left == nullptr && root->right == nullptr) {
        dot += aspas + nodeId + aspas;
        dot += L"[shape=record, label=\"{{";
        dot += root->caracter;
        dot += L"|";
        dot += std::to_wstring(root->frequency);
        dot += L"}|";
        dot += caminho;
        dot += L"}\"];\n";
    } else {
        dot += aspas + nodeId + aspas;
        dot += L"[label=";
        dot += std::to_wstring(root->frequency);
        dot += L"];\n";
        //Conexão com o nó da esquerda
        std::wstring leftId = std::to_wstring(root->left->frequency) + caminho + L"0";
        dot += aspas + nodeId + aspas;
        dot += L"->";
        dot += aspas + leftId + aspas;
        dot += L";\n";

        std::wstring rightId = std::to_wstring(root->right->frequency) + caminho + L"1";
        dot += aspas + nodeId + aspas;
        dot += L"->";
        dot += aspas + rightId + aspas;
        dot += L"[label=1];\n";

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
void showFrequencies(const std::unordered_map<wchar_t, int> &frequencies, const std::unordered_map<wchar_t, std::wstring> &dicionario) {
    std::wcout << L"Tabela de Frequências e Códigos de Huffman:\n";
    for (const auto &pair : frequencies) {
        std::wcout << pair.first << L" / " << pair.second << L" / " << dicionario.at(pair.first) << L'\n';
    }
}

void showFrequenciesTable() {
    auto path = read_path();
    auto caracter_count = calculate_frequency(path);
    auto root = create_huffman(caracter_count);
    std::unordered_map<wchar_t, std::wstring> dicionario;
    generate_codification(dicionario, root, L"");
    showFrequencies(caracter_count, dicionario);
}

void compress(std::wstring encodedString){
  FILE *arquivo = fopen("compactado.bin","wb");
  int i =0,j=7;
  unsigned char mascara,byte = 0;

  if(arquivo){
    while (encodedString[i]!= L'\0')
    {
        mascara = 1;
        if(encodedString[i] == L'1'){
          mascara = mascara << j;
          byte = byte | mascara;
        }
        j--;
        if(j<0){
          fwrite(&byte,sizeof(unsigned char),1,arquivo);
          byte = 0;
          j = 7;
        }
        i++;
    }
    if(j!=7){
      fwrite(&byte,sizeof(unsigned char),1,arquivo);
    }
    fclose(arquivo);  
    std::wcout << L"=========== Arquivo comprimido! ===========\n";

  }else{
    std::wcout << L"Erro fazer arquivo compactado!\n";
  }

}

void compressFile(){
    auto path = read_path();
    auto caracter_count = calculate_frequency(path);
    auto root = create_huffman(caracter_count);
    std::unordered_map<wchar_t, std::wstring> dicionario;
    generate_codification(dicionario, root, L"");

    std::wifstream inFile(path, std::ios::binary);
    std::wstring buffer;
    wchar_t ch;
    while (inFile.get(ch)) {
        buffer += dicionario[ch];
    }
    inFile.close();
    compress(buffer);

}

void descompressFile() {
}
};