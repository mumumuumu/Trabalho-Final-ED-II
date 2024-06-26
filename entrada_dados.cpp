#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <unordered_map>
#include "huffman_tree.cpp"
std::unordered_map<wchar_t, int> caracter_count;
std::string path;
huffman_tree::node* root;
void menu();
int main() {
    std::locale::global(std::locale(""));
    int op = 0;

    while (op != 5) {
        menu();
        std::cin >> op;
        switch (op) {
        case 1:
            path = huffman_tree::read_path();
            caracter_count = huffman_tree::calculate_frequency(path);
            root = huffman_tree::create_huffman(caracter_count);
            huffman_tree::compressFile(root, path);
            break;
        case 2: 
            huffman_tree::showTree(root);
            break;
        
        case 3:
            huffman_tree::showFrequenciesTable(root, path, caracter_count);
            break;
        case 4: 
            huffman_tree::descompressFile(root);
            std::cout << "Descomprimindo..." << std::endl;

            break;
        
        case 5:
            std::cout << "Saindo..." << std::endl;
            break;
        default:
            std::cout << "Opção inválida! Por favor, escolha novamente." << std::endl;
            break;
        }
        }
    

    return 0;
}


void menu() {
    std::cout << "==================================" << std::endl;
    std::cout << "    Compactação de Arquivos" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "1. Compactar arquivo" << std::endl;
    std::cout << "2. Mostrar árvore de Huffman" << std::endl;
    std::cout << "3. Mostrar tabela de frequências e códigos de Huffman" << std::endl;
    std::cout << "4. Descompactar arquivo" << std::endl;
    std::cout << "5. Sair" << std::endl;
    std::cout << "Escolha uma opção: ";
}