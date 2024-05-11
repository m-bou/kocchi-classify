#ifndef classify_tokenizer_h
#define classify_tokenizer_h

#include <memory>
#include <string>
#include "ldaplusplus/LDA.hpp"

class Tokenizer{
    public:
        Tokenizer(std::string p_data);
        void import();
        bool import_labels(std::string path);

    private:
        std::vector<std::string> _labels;
        Eigen::MatrixXi _X;
};

#endif