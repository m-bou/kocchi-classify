#include "Common.hpp"
#include <sstream>

void Common::QStringtoStream(const QString qString, std::istringstream& s) {
    // Convert QString to QByteArray
    QByteArray byteArray = qString.toUtf8();

    s.str("");
    s.clear(); // Clear any error flags
    s.seekg(0);
    s.seekg(0, std::ios::end);
    // Write the content of the QByteArray to the std::istringstream
    s.str(byteArray.constData()); // Set the new content
    // s << byteArray.constData();

}

void Common::printStream(std::istringstream& s){
   std::string line;

   while (std::getline(s, line)) {
        qDebug() << QString::fromStdString(line);
    }

}

Eigen::MatrixXi Common::import(std::string p)
{
    Eigen::MatrixXi X;
    std::fstream input_file(p, std::ios::in | std::ios::binary);
    
    ldaplusplus::numpy_format::NumpyInput<int> ni;
    input_file >> ni;
    X = ni;

    return X;
}

void Common::printMatrix(Eigen::MatrixXi mat)
{
    std::cout << "Matrix mat:" << std::endl;
    for (int i = 0; i < mat.rows(); ++i) {
        for (int j = 0; j < mat.cols(); ++j) {
            std::cout << mat(i, j) << " ";
        }
        std::cout << std::endl;
    }
}