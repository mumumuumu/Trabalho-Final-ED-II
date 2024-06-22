#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <unordered_map>
#include "huffman_tree.cpp"

int main() {
    std::locale::global(std::locale(""));
   
    std::wifstream arq("output.txt");
    
    if (!arq) {
        std::wcout << L"Arquivo não encontrado!\n";
        return 1;
    }

    std::unordered_map<wchar_t, int> word_count;
    wchar_t ch;

    if (arq.is_open()) {
        while (arq.get(ch)) {
            word_count[ch]++;
        }
    }

    arq.close();
    for (const auto& entry : word_count) {
        std::wcout << entry.first << L": " << entry.second << L'\n';

    }
    auto hTree = huffman_tree::create_huffman(word_count);
    std::wcout << L" Árvore de huffman criada!\n";
    std::wcout << L" ------------------------------------\n";
    huffman_tree::show(hTree);
    return 0;
}
