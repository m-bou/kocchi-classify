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