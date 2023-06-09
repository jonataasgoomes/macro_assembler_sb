#ifndef LIGADOR_H
#define LIGADOR_H
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>



using namespace std;
namespace fs = std::filesystem;



std::vector<int> ligador(const std::vector<std::string>& nomesArquivos);

#endif  // LIGADOR_H