#include "Tokenizer.hpp"
#include "Common.hpp"
#include <fstream>
#include <string>

Tokenizer::Tokenizer(std::string p_data)
{
    _X = Common::import(p_data);
}

bool Tokenizer::import_labels(std::string path)
{
    std::ifstream inFile(path);

    if (!inFile.is_open())
    {
        PRINT_ERROR( std::format("Error opening file: {}", path) );
        return false;
    }

    std::string line;

    while (std::getline(inFile, line)) _labels.push_back(line);

    inFile.close();

    return true;

}