#ifndef classify_common_hpp
#define classify_common_hpp

#include <iostream>
// #include "ldaplusplus/NumpyFormat.hpp"

#include <QString>
#include <QTextStream>
#include <QDebug>
#include <sstream>

#include <ldaplusplus/NumpyFormat.hpp>

class Common
{
    public:
        static void     QStringtoStream(const QString, std::istringstream&);
        static void     printStream(std::istringstream&);
        static Eigen::MatrixXi  import(std::string p);
        static void printMatrix(Eigen::MatrixXi mat);
};


#endif