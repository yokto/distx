#include <QCoreApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Use Qt logging mechanism (QDebug) for console output
    qDebug() << "Hello, Qt! This is a simple Qt console application.";

    // Other Qt functionalities can be used here as needed

    // Return the application's exit code
    return a.exec();
}

