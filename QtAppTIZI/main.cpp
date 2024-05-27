#include <qapplication.h>
#include <MainWindow.h>
#include <qfile.h>

int main(int argc, char* argv[]) {
	// setlocale(LC_ALL, "en_US");
	QApplication app(argc, argv);	// Make app
	MainWindow mainWin;				// Make Main Window
	
	QFile file(":/styles/styles.qss");
	if (file.open(QFile::ReadOnly)) {
		QString styleSheet = QLatin1String(file.readAll());
		app.setStyleSheet(styleSheet);
	}

	mainWin.show();
	return app.exec();
}