#ifndef classify_common_hpp
#define classify_common_hpp

// #include "ldaplusplus/NumpyFormat.hpp"

#include <QString>
#include <QTextStream>
#include <QDebug>
#include <sstream>

#include <ldaplusplus/NumpyFormat.hpp>

#ifndef NDEBUG
#define PRINT_DEBUG(X) std::cout << X << std::endl
#else
#define PRINT_DEBUG(X) do {} while(0)
#endif

#define PRINT_ERROR(X) std::cout << X << std::endl

class Common
{
    public:
        static void     QStringtoStream(const QString, std::istringstream&);
        static void     printStream(std::istringstream&);
        static Eigen::MatrixXi  import(std::string p);
        static void printMatrix(Eigen::MatrixXi mat);
};


#endif