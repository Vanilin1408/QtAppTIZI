#include "InputDataWindow.h"

InputDataWindow::InputDataWindow(QWidget* parent) : 
	QWidget(parent) {
	setupUI();
}

InputDataWindow::~InputDataWindow() {
	this->close(); // why not??
}

void InputDataWindow::setupUI() {
	setObjectName("inputDataWindow");
	setWindowTitle("Ввод параметров");
	setFixedSize(450, 650);
	QLocale::setDefault(QLocale::C);

	setupLayouts();
	setupWidgets();
	setupConnections();
}

void InputDataWindow::setupLayouts() {
	// Main Vertical Layout for struct main zones
	mainVLayout = new QVBoxLayout(this);

	// Zones
	topZone = new QWidget(this);
	topZone->setObjectName("inputWindowTopZone");
	topZone->setFixedHeight(200);

	middleZone = new QWidget(this);
	middleZone->setObjectName("inputWindowMiddleZone");
	middleZone->setFixedHeight(200);

	middle1Zone = new QWidget(this);
	middle1Zone->setObjectName("inputWindowMiddle1Zone");
	middle1Zone->setFixedHeight(80);

	bottomZone = new QWidget(this);
	bottomZone->setFixedHeight(80);
	bottomZone->setObjectName("inputWindowBottomZone");

	// Layouts
	topGridLay = new QGridLayout(topZone);
	middleGridLay = new QGridLayout(middleZone);
	middle1GridLay = new QGridLayout(middle1Zone);
	bottomGridLay = new QGridLayout(bottomZone);

	mainVLayout->addWidget(topZone);
	mainVLayout->addWidget(middleZone);
	mainVLayout->addWidget(middle1Zone);
	mainVLayout->addWidget(bottomZone);
}

void InputDataWindow::setupWidgets() {
	// --------------------- Установки для валидаторов ввода --------------------- //
	doubleValidator = new QDoubleValidator(this);
	doubleValidator->setNotation(QDoubleValidator::StandardNotation);
	doubleValidator->setDecimals(4); 
	doubleValidator->setRange(0, 1.0); 
	doubleValidator->setLocale(QLocale::C);

	intValidator = new QIntValidator(this);
	intValidator->setRange(1, 1000);


	// --------------------- Установки для линейныых Виджетов --------------------- //
	// Labels для указания Линейных параметров
	mainLinearLabel = new QLabel("Параметры линейных атак", topZone);
	mainLinearLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	linearA1label = new QLabel("Лин. a1:", topZone);
	linearA2label = new QLabel("Лин. a2:", topZone);
	linearA3label = new QLabel("Лин. a3:", topZone);
	linearB1label = new QLabel("Лин. b1:", topZone);
	linearB2label = new QLabel("Лин. b2:", topZone);
	linearB3label = new QLabel("Лин. b3:", topZone);

	// Поля ввода для Линейных параметров
	inputLinearA1 = new QLineEdit("0.04", topZone);
	inputLinearA2 = new QLineEdit("0.035", topZone);
	inputLinearA3 = new QLineEdit("0.03", topZone);
	inputLinearB1 = new QLineEdit("0.02", topZone);
	inputLinearB2 = new QLineEdit("0.015", topZone);
	inputLinearB3 = new QLineEdit("0.02", topZone);

	// Установка валидаторов для полей
	inputLinearA1->setValidator(doubleValidator);
	inputLinearA2->setValidator(doubleValidator);
	inputLinearA3->setValidator(doubleValidator);
	inputLinearB1->setValidator(doubleValidator);
	inputLinearB2->setValidator(doubleValidator);
	inputLinearB3->setValidator(doubleValidator);
	
	// Размещение объектов в сетку Верхнего поля, линейные параметры
	topGridLay->addWidget(mainLinearLabel, 0, 0, 1, 4, Qt::AlignHCenter);
	topGridLay->addWidget(linearA1label, 1, 0); // label
	topGridLay->addWidget(inputLinearA1, 1, 1);
	topGridLay->addWidget(linearA2label, 2, 0); // label
	topGridLay->addWidget(inputLinearA2, 2, 1);
	topGridLay->addWidget(linearA3label, 3, 0); // label
	topGridLay->addWidget(inputLinearA3, 3, 1);

	topGridLay->addWidget(linearB1label, 1, 2); // label
	topGridLay->addWidget(inputLinearB1, 1, 3);
	topGridLay->addWidget(linearB2label, 2, 2); // label
	topGridLay->addWidget(inputLinearB2, 2, 3);
	topGridLay->addWidget(linearB3label, 3, 2);	// label
	topGridLay->addWidget(inputLinearB3, 3, 3);


	// --------------------- Установки для Экспоненциальных Виджетов --------------------- //
	// Labels для указания Экспоненциальных параметров
	mainExpLabel = new QLabel("Параметры экспоненциальных атак", middleZone);
	mainExpLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	expA1label = new QLabel("Эксп. a1:", middleZone);
	expA2label = new QLabel("Эксп. a2:", middleZone);
	expA3label = new QLabel("Эксп. a3:", middleZone);
	expB1label = new QLabel("Эксп. b1:", middleZone);
	expB2label = new QLabel("Эксп. b2:", middleZone);
	expB3label = new QLabel("Эксп. b3:", middleZone);

	// Поля ввода для Экспоненциальных параметров
	inputExpA1 = new QLineEdit("0.7", topZone);
	inputExpA2 = new QLineEdit("0.8", topZone);
	inputExpA3 = new QLineEdit("0.5", topZone);
	inputExpB1 = new QLineEdit("0.05", topZone);
	inputExpB2 = new QLineEdit("0.045", topZone);
	inputExpB3 = new QLineEdit("0.07", topZone);

	// Установка валидаторов для полей
	inputExpA1->setValidator(doubleValidator);
	inputExpA2->setValidator(doubleValidator);
	inputExpA3->setValidator(doubleValidator);
	inputExpB1->setValidator(doubleValidator);
	inputExpB2->setValidator(doubleValidator);
	inputExpB3->setValidator(doubleValidator);

	// Размещение объектов в сетку Среднего поля, Экспоненциальные параметры
	middleGridLay->addWidget(mainExpLabel, 0, 0, 1, 4, Qt::AlignHCenter);
	middleGridLay->addWidget(expA1label, 1, 0); // label
	middleGridLay->addWidget(inputExpA1, 1, 1);
	middleGridLay->addWidget(expA2label, 2, 0); // label
	middleGridLay->addWidget(inputExpA2, 2, 1);
	middleGridLay->addWidget(expA3label, 3, 0); // label
	middleGridLay->addWidget(inputExpA3, 3, 1);

	middleGridLay->addWidget(expB1label, 1, 2); // label
	middleGridLay->addWidget(inputExpB1, 1, 3);
	middleGridLay->addWidget(expB2label, 2, 2); // label
	middleGridLay->addWidget(inputExpB2, 2, 3);
	middleGridLay->addWidget(expB3label, 3, 2); // label
	middleGridLay->addWidget(inputExpB3, 3, 3);
	

	// --------------------- Установки для t, k, n Виджетов --------------------- //
	// Labels других параметров
	inputMaxTLabel = new QLabel("t:", middle1Zone);
	inputKLabel = new QLabel("k:", middle1Zone);
	inputNLabel = new QLabel("n:", middle1Zone);

	// Поля ввода остальных параметров
	inputMaxT = new QLineEdit("17", middle1Zone);
	inputK = new QLineEdit("5", middle1Zone);
	inputN = new QLineEdit("8", middle1Zone);

	// Установка валидаторов для остальных полей
	inputMaxT->setValidator(intValidator);
	inputK->setValidator(intValidator);
	inputN->setValidator(intValidator);

	// Размещение labels и элементов в нижней зоне
	// t - параметр
	middle1GridLay->addWidget(inputMaxTLabel, 0, 0); // label
	middle1GridLay->addWidget(inputMaxT, 0, 1);
	// k - параметр
	middle1GridLay->addWidget(inputKLabel, 0, 2);
	middle1GridLay->addWidget(inputK, 0, 3);
	// n - параметр
	middle1GridLay->addWidget(inputNLabel, 0, 4);
	middle1GridLay->addWidget(inputN, 0, 5);


	// --------------------- Установки для p(t) > 0.5, p(t) < 0.5 Виджетов --------------------- //
	// Labels для p(t) параметров
	PtLargerLabel = new QLabel("p(t) > 0.5:", bottomZone);
	PtLessLabel = new QLabel("p(t) < 0.5:", bottomZone);
	
	// Поля ввода p(t) params
	inputPtLarger = new QLineEdit("0.8", bottomZone);
	inputPtLess = new QLineEdit("0.45", bottomZone);

	// Размещение элементов p(t) параметров
	bottomGridLay->addWidget(PtLargerLabel, 0, 0);
	bottomGridLay->addWidget(inputPtLarger, 0, 1);
	bottomGridLay->addWidget(PtLessLabel, 0, 2);
	bottomGridLay->addWidget(inputPtLess, 0, 3);
	
	// Валидация полей
	inputPtLarger->setValidator(doubleValidator);
	inputPtLess->setValidator(doubleValidator);

	// --------------------- Установки для кнопки --------------------- //
	finishedInputButton = new QPushButton("Применить", this);
	finishedInputButton->setObjectName("finishedInputButton");
	mainVLayout->addWidget(finishedInputButton);
}

void InputDataWindow::setupConnections() {
	connect(finishedInputButton, &QPushButton::clicked, this, &InputDataWindow::inputFinishedCloseWindow);
}

void InputDataWindow::inputFinishedCloseWindow() {
	// transport data to --->>>>> model
	// Переводим данные с полей в нужные форматы
	double linearA1 = inputLinearA1->text().toDouble();
	double linearA2 = inputLinearA2->text().toDouble();
	double linearA3 = inputLinearA3->text().toDouble();
	double linearB1 = inputLinearB1->text().toDouble();
	double linearB2 = inputLinearB2->text().toDouble();
	double linearB3 = inputLinearB3->text().toDouble();
	double expA1 = inputExpA1->text().toDouble();
	double expA2 = inputExpA2->text().toDouble();
	double expA3 = inputExpA3->text().toDouble();
	double expB1 = inputExpB1->text().toDouble();
	double expB2 = inputExpB2->text().toDouble();
	double expB3 = inputExpB3->text().toDouble();
	unsigned int maxT = inputMaxT->text().toUInt();
	unsigned int k = inputK->text().toUInt();
	unsigned int n = inputN->text().toUInt();
	double PtLarge = inputPtLarger->text().toDouble();
	double PtLess = inputPtLess->text().toDouble();

	emit dataEntered(linearA1, linearA2, linearA3, linearB1, linearB2, linearB3,
					 expA1, expA2, expA3, expB1, expB2, expB3, maxT, k, n, PtLarge, PtLess);

	// this->close();
}