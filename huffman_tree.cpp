#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <queue>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <string>
#include <filesystem>

namespace huffman_tree{

//Estrutura do nó da árvore
struct node {
  wchar_t caracter;
  int frequency;
  node *left, *right;
};
//Função para criar o nó utilizando caracter e frequeência a struct
node *create_node(const wchar_t &caracter, const int &frequency ) {
  node *p = new node;
  p->caracter = caracter;
  p->frequency = frequency;
  p->left = p->right = nullptr;
  return p;
}

//Classe de comporação para a fila de proridade
class Compare
{
    public:
    bool operator()(node *n1, node *n2){
        return n1->frequency > n2->frequency;

    }
};

//Função que lê e retorna string do caminho de arquivo
std::string read_path(){
  std::string path;
  std::wcout<< L"Insira caminho do arquivo: ";
  std::cin >> path;
  return path;
}

//Função que calcula a frequência e retorna o unordered_map com caracter/frequência para a construção da árvore
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

//Função que constrói a árvore de huffman
node* create_huffman(std::unordered_map<wchar_t, int> frequency){
        std::priority_queue<node*,std::vector<node*>,Compare> pq;
        for (auto p : frequency) {
            auto nodeC = create_node(p.first,p.second);
            pq.push(nodeC);
        }
        while(pq.size()!=1){                        // left -> b|2
            auto *left = pq.top();  // a|3 / b|2  -> I|5
                                                    //right -> a|3
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

// Gera o código varrendo árvore de huffman e concatenando 0 e 1 no unorderep_map de argumento
void generate_codification(std::unordered_map<wchar_t, std::wstring> &dicionario, node *root, const std::wstring &caminho) {
  if (root->left == nullptr && root->right == nullptr) {
    dicionario[root->caracter] = caminho;
  } else {
    generate_codification(dicionario, root->left, caminho + L"0");
    generate_codification(dicionario, root->right, caminho + L"1");
  }
}

// FUNÇÕES DE VISUALIZAÇÃO
std::wstring escapeCharacter(wchar_t ch) {
    switch (ch) {
        case L'\n':
            return L"endl";
        case L'\t':
            return L"\\t";
        case L'\r':
            return L"\\r";
        case L' ':
            return L"espaço";
        case L'"':
            return L"aspas";
        default:
            return std::wstring(1, ch);
    }
}
void showTreeRecursive(std::wstring &dot, node *root, const std::wstring &caminho) {
    if (root == nullptr) {
        return;
    }

    auto aspas = L"\"";
    std::wstring nodeId = std::to_wstring(root->frequency) + caminho;

    // Concatena a string do nó folha 
    if (root->left == nullptr && root->right == nullptr) {
        dot += aspas + nodeId + aspas;
        dot += L"[shape=record, label=\"{{";
        dot += escapeCharacter(root->caracter);
        dot += L"|";
        dot += std::to_wstring(root->frequency);
        dot += L"}|";
        dot += caminho;
        dot += L"}\"];\n";
    } else {
        //Concatena a string do nó intermediário
        dot += aspas + nodeId + aspas;
        dot += L"[label=";
        dot += std::to_wstring(root->frequency);
        dot += L"];\n";
        // Concatena a string da conexão do intermediário -> esquerda
        std::wstring leftId = std::to_wstring(root->left->frequency) + caminho + L"0";
        dot += aspas + nodeId + aspas;
        dot += L"->";
        dot += aspas + leftId + aspas;
        dot += L";\n";
        // Concatena a string da conexão do intermediário -> direito
        std::wstring rightId = std::to_wstring(root->right->frequency) + caminho + L"1";
        dot += aspas + nodeId + aspas;
        dot += L"->";
        dot += aspas + rightId + aspas;
        dot += L"[label=1];\n";
        //Continua recursivamente
        showTreeRecursive(dot, root->left, caminho + L"0");
        showTreeRecursive(dot, root->right, caminho + L"1");
    }
}

// Função que constrói o arquivo dot e mostra na tela
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
    std::vector<std::pair<wchar_t, int>> sortedFrequencies(frequencies.begin(), frequencies.end());

    std::sort(sortedFrequencies.begin(), sortedFrequencies.end(), [](const std::pair<wchar_t, int> &a, const std::pair<wchar_t, int> &b) {
        return a.second > b.second;
    });

    std::wcout << std::left << std::setw(10) << L"Caractere" << std::setw(15) << L"Frequência" << std::setw(20) << L"Código de Huffman" << std::endl;
    std::wcout << std::left << std::setw(10) << L"---------" << std::setw(15) << L"----------" << std::setw(20) << L"----------------" << std::endl;

    for (const auto &pair : sortedFrequencies) {
        std::wcout << std::left << std::setw(10) << escapeCharacter(pair.first)
                   << std::setw(15) << pair.second
                   << std::setw(20) << dicionario.at(pair.first)
                   << std::endl;
    }
}

void showFrequenciesTable(node* root, std::string path, std::unordered_map<wchar_t,int> caracter_count) {

    std::unordered_map<wchar_t, std::wstring> dicionario;
    generate_codification(dicionario, root, L"");
    showFrequencies(caracter_count, dicionario);
}

int getFileFize(std::filesystem::path filePath){
  return std::filesystem::file_size(filePath);
}

// 00101010  10101011 10001101  011
void compress(std::wstring encodedString){ //implementar para que árvore seja guardada com o compactado
  FILE *arquivo = fopen("compactado.bin","wb");
  int i =0,j=7;
  unsigned char mascara,byte = 0; // 01100000
  //char = 1byte - 8bits

  if(arquivo){
    while (encodedString[i]!= L'\0')
    {
        mascara = 1; //0000100
        if(encodedString[i] == L'1'){
          mascara = mascara << j; //0000100
          byte = byte | mascara;  //0010000
        }                        //00101010
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

void compressFile(node* root, std::string path){
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

    std::wcout << L"Tamanho arquivo original: " << getFileFize(path) << std::endl;
    std::wcout << L"Tamanho arquivo compactado: " << getFileFize("compactado.bin") << std::endl;
    std::wcout << L"Diferença em bits: " << getFileFize(path) - getFileFize("compactado.bin") << std::endl;
    std::wcout << L"Percentual de compactação: " << (double)getFileFize("compactado.bin")/getFileFize(path)*100<< L"%" << std::endl;

}

unsigned int isBitOne(unsigned char byte, int i){
  unsigned char mascara = (1<<i);
  return byte & mascara;
  ///00001001
  ///00000010
}

void descompressFile(node *root) { //implementar para que árvore seja guardada com o compactado
    FILE *arquivo = fopen("compactado.bin", "rb");
    std::wofstream descompactado("descompactado.txt");
  
    unsigned char byte;
    node *aux = root;

    if (arquivo && descompactado) {
        while (fread(&byte, sizeof(unsigned char), 1, arquivo)) {
            for (int i = 7; i >= 0; i--) {
                if (isBitOne(byte, i)) {
                    aux = aux->right;
                } else {
                    aux = aux->left;
                }
                if (aux->left == nullptr && aux->right == nullptr) {
                    descompactado << aux->caracter;
                    aux = root;
                }
            }
        }
        fclose(arquivo);
        descompactado.close();
    std::wcout << L"=========== Arquivo descomprimido! ===========\n";

    } else {
        std::wcout << L"Erro ao abrir arquivos para descompressão!\n";
    }
}
};