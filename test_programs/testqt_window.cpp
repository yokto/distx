#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtGui/QFontDatabase>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    const auto ret = QFontDatabase::addApplicationFont("/_zwolf/fonts/DejaVuSans.ttf");
    qWarning() << "r " << ret << "f " <<  QFontDatabase::families() << "\n";

    // Create a QLabel (a simple text label)
    QLabel label("Hello, Qt!");

    // Show the label
    label.show();

    // Enter the application's event loop
    return app.exec();
}

