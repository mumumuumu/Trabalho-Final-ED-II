#include <fstream>
#include <unordered_map>
#include <vector>
#include <cstdlib>
#include <queue>
#include <iostream>
#include <locale>
#include <codecvt>

namespace huffman_tree{

struct node {
  wchar_t value;
  int frequency;
  node *left, *right;
};

node *create_node(const wchar_t &value, const int &frequency ) {
  node *p = new node;
  p->value = value;
  p->frequency = frequency;
  p->left = p->right = nullptr;
  return p;
}
class Compare
{
    public:
    bool operator()(node *n1, node *n2){
        return n1->frequency < n2->frequency;

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
            auxNode->value = '\0';
            auxNode->frequency = left->frequency+right->frequency;
            auxNode ->left = left;
            auxNode ->right = right;
            pq.push(auxNode);
        }
        
        node* root = pq.top();    
        return root;   
    }

};