#include <qapplication.h>
#include <MainWindow.h>
#include <qfile.h>

int main(int argc, char* argv[]) {
	QApplication app(argc, argv);	// Make app
	MainWindow mainWin;				// Make Main Window
	
	// Загрузка стилей QSS
	QFile file(":/styles/styles.qss");
	if (file.open(QFile::ReadOnly)) {
		QString styleSheet = QLatin1String(file.readAll());
		app.setStyleSheet(styleSheet);
	}

	// Показать окно
	mainWin.show();
	return app.exec();
}