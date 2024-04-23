#ifndef classify_common_hpp
#define classify_common_hpp

#include <iostream>
// #include "ldaplusplus/NumpyFormat.hpp"

#include <QString>
#include <QTextStream>
#include <QDebug>
#include <sstream>

class Common{
    public:
        static void     QStringtoStream(const QString, std::istringstream&);
        static void     printStream(std::istringstream&);
};


#endif