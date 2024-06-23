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
class Compare
{
    public:
    bool operator()(node *n1, node *n2){
        return n1->frequency > n2->frequency;

    }
};

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
            auxNode->caracter = L'+';
            auxNode->frequency = left->frequency+right->frequency;
            auxNode ->left = left;
            auxNode ->right = right;
            pq.push(auxNode);
        }
        
        node* root = pq.top();    
        return root;   
    }

int height(node *root) {
  if (root == nullptr) return -1;
  return std::max(height(root->left), height(root->right)) + 1;
}

void show(node *root, int level = 0, const std::wstring &prefix = L"√ ") {
  if (!root) return;

  auto ident = std::wstring(level * 4, ' ');

  std::wcout << ident << L"N:"<<level<< prefix << root->caracter << L"--"<<root->frequency << std::endl;
  show(root->left, level + 1, L"↙ ");
  show(root->right, level + 1, L"↘ ");
}

void generate_codification(std::unordered_map<wchar_t, std::string> &dicionario, node *root, const std::string &caminho) {
  if (root->left == nullptr && root->right == nullptr) {
    dicionario[root->caracter] = caminho;
  } else {
    generate_codification(dicionario, root->left, caminho + "0");
    generate_codification(dicionario, root->right, caminho + "1");
  }
}
};