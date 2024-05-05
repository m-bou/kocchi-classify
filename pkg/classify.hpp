#ifndef classify_classify_h
#define classify_classify_h

#include <memory>
#include <string>
#include "ldaplusplus/LDA.hpp"
#include "Common.hpp"

class Classify{
    public:
        Classify(std::string p_data);

        void import(std::string p, Eigen::MatrixXi& X);
        
        void buildLDA();
        ldaplusplus::LDA<double>& getLda();
        void train();
        void save_model(std::string path);

        void addListener();

        // void get_topics();
        // void train_posts();
        // void get_topics();
        // void train();
    
    private:
        int _n_topics;
        std::unique_ptr<ldaplusplus::LDA<double>> _lda = nullptr;
        Eigen::MatrixXi _X;

        double  _likelihood = 0;
        int     _count_likelihood = 0;
        int     _count = 0;
};

#endif