#include <preprocess_source.h>

void processarArquivo (const std::string& arquivo){

	std::ifstream arquivo_original(arquivo);
	if (!arquivo_original.is_open()){
		std::cout << "Erro ao abrir o arquivo:" <<arquivo+"Use ./montador <nome_arquivo>" << std::endl;
		return;
	}

	std::string linha;

	while (std::getline(arquivo_original,linha)){
		size_t pos = linha.find(';');
		if(pos != std::string::npos){
			linha = linha.substr(0,pos);
		}
		for (char& c: linha){
			c = std::toupper(static_cast<unsigned char>(c));
		}
			std::cout <<linha << std::endl;
	}	

	arquivo_original.close();

}