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
        std::cout << "open() falhou\n";
        return 1;
    }

    std::unordered_map<wchar_t, int> word_count;
    wchar_t ch;

    if (arq.is_open()) {
        std::wcout << L"arquivo aberto com sucesso\n";
        
        while (arq.get(ch)) {
        
            word_count[ch]++;
        }
    }

    arq.close();
    std::wcout << L"arquivo aberto com sucesso\n";


    std::wcout << word_count.size() << L" caracteres distintos encontrados\n";

    for (const auto& entry : word_count) {
        std::wcout << entry.first << L": " << entry.second << L'\n';

    }
    auto hTree = huffman_tree::create_huffman(word_count);

    return 0;
}
