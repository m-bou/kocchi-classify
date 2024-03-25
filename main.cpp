#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QDebug>

#include <iostream>
#include <Eigen/Dense>
#include <fstream>

int main() {
    // Create 100 random documents with a vocabulary of 1000 words
    Eigen::MatrixXi X = (Eigen::MatrixXi::Random(1000, 100).array() * 20).cwiseMax(0).cast<int>();

    // Create the class labels for all documents in the corpus
    Eigen::VectorXi y = (Eigen::VectorXi::Random(100).array() * 5).cast<int>();

    // Save the data in a file
    std::ofstream file("/tmp/data.npy", std::ios::binary);
    if (file.is_open()) {
        // Write X to file
        file.write(reinterpret_cast<const char*>(X.data()), X.size() * sizeof(int));

        // Write y to file
        file.write(reinterpret_cast<const char*>(y.data()), y.size() * sizeof(int));

        file.close();
    } else {
        std::cout << "Unable to open file for writing." << std::endl;
        return 1;
    }

    return 0;
}

//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);
//    QString test= "AAA";

//    qDebug() << test;

//    return a.exec();
//}
