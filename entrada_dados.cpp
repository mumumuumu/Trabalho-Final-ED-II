#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <unordered_map>
#include "huffman_tree.cpp"


void menu();
int main() {
    std::locale::global(std::locale(""));
    menu();
    int op;
    std::cin >> op;
    switch (op)
    {
    case 1:
        huffman_tree::compressFile();
        // huffman_tree::showCodeTable();
        //huffman_tree::showTree();
        break;
    case 2:
        huffman_tree::descompressFile();
        break;

    default:
        break;
    }
    
    // auto word_count =  huffman_tree::calculate_frequency("output.txt");
    // for (const auto& entry : word_count) {
    //     std::wcout << entry.first << L": " << entry.second << L'\n';

    // }
    // std::wcout << L" ------------------------------------\n";
    // huffman_tree::compressFile();
    // std::wcout << L" ------------------------------------\n";

    return 0;
}
void menu() {
    std::cout << "==================================" << std::endl;
    std::cout << "    Compactação de Arquivos" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "1. Compactar arquivo" << std::endl;
    std::cout << "2. Descompactar arquivo" << std::endl;
    std::cout << "3. Sair" << std::endl;
    std::cout << "Escolha uma opção: ";
}