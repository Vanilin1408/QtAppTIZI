#include "MainWindow.h"

// Определение конструктор класса MainWindow
MainWindow::MainWindow(QWidget* parent) : 
	QMainWindow(parent), 
	model(new Model),
	inputDataWindow(nullptr) {	// Определение конструктора
	setupUI();					// Отрисовка интерфейса
}

// Определение деструктор класса MainWindow
MainWindow::~MainWindow() { 
	delete model;
	delete inputDataWindow;
}

// Отрисовка UI
void MainWindow::setupUI() {
	resize(1050, 650);
	setupLayouts();
	setupWidgets();
	setupConnections();
	setupRadioButtons();
	onComboBoxChanged(); // Настраиваем начальную активность зон с выбором параметров
}

void MainWindow::setupLayouts() {
	// Центральный виджет
    centralWidget = new QWidget(this);
	centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCentralWidget(centralWidget);

    // Главный вертикальный макет
    mainVerticalLayout = new QVBoxLayout(centralWidget);
	mainVerticalLayout->setSpacing(10);
	
	// Главный Контейнер для содержимого (информационный экран)
	mainContentContainer = new QWidget(centralWidget);
	mainContentContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	mainVerticalLayout->addWidget(mainContentContainer);
	
	// Горизонтальный слой для главного контейнера (слева "экран", справа зона управления)
	topHGridLayout = new QGridLayout(mainContentContainer);
	topHGridLayout->setSpacing(5);

    // Зона размещения основной информации mainTabView
	mainTabView = new QWidget(mainContentContainer);
	
	mainTabView->setObjectName("mainTabView");
	mainTabView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	mainTabView->setMinimumSize(800, 500);

	// Cлой для визуализации информации в mainTab
	mainTabLayout = new QVBoxLayout(mainTabView);

	// Контейнер для радио кнопок
	RBGroupsContainer = new QWidget(mainContentContainer);
	RBGroupsContainer->setObjectName("RBGroupContainer1");
	RBGroupsContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	RBGroupsContainer->setFixedWidth(200);
	RBGroupsContainer->setMinimumHeight(500);

	// Сетка для RadioButtons Boxes
	radioButtonsGridLayout = new QGridLayout(RBGroupsContainer);

	// Размещение mainTabView в верхнем горизонтальном слое, который в главном вертикальном слое
	topHGridLayout->addWidget(mainTabView, 0, 0, Qt::AlignCenter);
	topHGridLayout->addWidget(RBGroupsContainer, 0, 1, Qt::AlignCenter);

	// Фиксированный контейнер для нижних кнопок
	fixedWidthContainer = new QWidget(centralWidget);
	fixedWidthContainer->setMaximumWidth(800);

    // Слой Сетки нижних кнопок
    bottomGrid = new QGridLayout(fixedWidthContainer);
    bottomGrid->setHorizontalSpacing(10);
	bottomGrid->setContentsMargins(0, 10, 0, 10);
	
	mainVerticalLayout->addWidget(mainContentContainer, 0, { Qt::AlignHCenter, Qt::AlignTop });
	mainVerticalLayout->addStretch();
	// Добавляем контейнер с сеткой с кнопками внизу
	mainVerticalLayout->addWidget(fixedWidthContainer, 0, { Qt::AlignHCenter, Qt::AlignBottom });
}

void MainWindow::setupWidgets() {
	// Cоздание кнопок
	startInputDataButton = new QPushButton("Ввод параметров", centralWidget);
	startInputDataButton->setObjectName("startInputDataButton");
	mainTablesShowButton = new QPushButton("Таблица атак pA(t)", centralWidget);
	showChartsButton = new QPushButton("Построить график", centralWidget);
	clearMainTabButton = new QPushButton("Очистить окно", centralWidget);

	// ComboBox Setup
	comboBoxWithChartsTypes = new QComboBox(centralWidget);

	// Items of ComboBox
	comboBoxWithChartsTypes->addItem("График изменения вероятностей отказов для заданных типов атак при p(ai) = 1.0");
	comboBoxWithChartsTypes->addItem("График вероятности P(p(t)) обнаружения уязвимостей при тестировании отдельным устройством");
	comboBoxWithChartsTypes->addItem("График вероятности P(p(t,k)) обнаружения уязвимостей при коллаборационной стратегии");
	comboBoxWithChartsTypes->addItem("График вероятности обнар. уязв. при коллаб. стратегии в условиях противодействия тестировнию от t");
	comboBoxWithChartsTypes->addItem("График вероятности обнар. уязв. при коллаб. стратегии в условиях противодействия тестировнию от k");
	comboBoxWithChartsTypes->addItem("Генерация матрицы смежности размерностью 15 x 15");
	comboBoxWithChartsTypes->addItem("Генерация матрицы смежности минимального покрытия");

	// Вызов слота при изменении индекса ComboBox
	connect(comboBoxWithChartsTypes, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onComboBoxChanged);

	// Добавляем кнопок в нижнюю сетку
	bottomGrid->addWidget(startInputDataButton, 0, 0, Qt::AlignCenter);
	bottomGrid->addWidget(mainTablesShowButton, 0, 1, Qt::AlignCenter);
	bottomGrid->addWidget(showChartsButton, 0, 2, Qt::AlignCenter);
	bottomGrid->addWidget(clearMainTabButton, 0, 3, Qt::AlignCenter);
	bottomGrid->addWidget(comboBoxWithChartsTypes, 1, 0, 1, 4, Qt::AlignCenter);

}

void MainWindow::setupRadioButtons() {
	// Главный Label
	RB_MainLabel = new QLabel("Параметры\nдля графиков");
	RB_MainLabel->setFixedSize(180, 50);
	radioButtonsGridLayout->addWidget(RB_MainLabel, 0, 0, Qt::AlignHCenter);

	// -------------------- Главные типы атак: Линейная, Экспоненциальная -------------------- //
	// Группа кнопок с типами атакам
	RB_AttackTypeGROUP = new QButtonGroup(RBGroupsContainer);			// Логическая
	RB_AttackTypeBOX = new QGroupBox("Тип атаки", RBGroupsContainer);	// Визуальная
	RB_AttackTypeBOX->setFixedSize(180, 100);
	VBoxForRBAttackType = new QVBoxLayout();							// Размещение по вертикали

	// RadioButtons by Attacks
	radioButtonLINEARAttack = new QRadioButton("Линейная", RBGroupsContainer);
	radioButtonEXPAttack = new QRadioButton("Экспоненциал.", RBGroupsContainer);

	// Добавление кнопок в логическую группу
	RB_AttackTypeGROUP->addButton(radioButtonLINEARAttack, 1);
	RB_AttackTypeGROUP->addButton(radioButtonEXPAttack, 2);

	// Добавление кнопок в визуальную группу
	VBoxForRBAttackType->addWidget(radioButtonLINEARAttack);
	VBoxForRBAttackType->addWidget(radioButtonEXPAttack);

	// Добавление слоёв
	RB_AttackTypeBOX->setLayout(VBoxForRBAttackType);
	radioButtonsGridLayout->addWidget(RB_AttackTypeBOX, 1, 0, Qt::AlignCenter);

	// -------------------- ПодТипы атак 1, 2, 3, согласно введенным значениям -------------------- //
	// Вторая группа радиокнопок
	RB_AttackSUBTypeGROUP = new QButtonGroup(RBGroupsContainer);
	RB_AttackSUBTypeBOX = new QGroupBox("Подтип атаки", RBGroupsContainer);
	// RB_AttackSUBTypeBOX->setFixedSize(180, 150);
	RB_AttackSUBTypeBOX->setFixedWidth(180);
	VBoxForRBAttackSUBType = new QVBoxLayout();

	// Сами кнопки
	firstSubTypeAttackRB = new QRadioButton("1", RBGroupsContainer);
	secondSubTypeAttackRB = new QRadioButton("2", RBGroupsContainer);
	thirdSubTypeAttackRB = new QRadioButton("3", RBGroupsContainer);

	// Добавляем кнопки в логическую группу
	RB_AttackSUBTypeGROUP->addButton(firstSubTypeAttackRB, 1);
	RB_AttackSUBTypeGROUP->addButton(secondSubTypeAttackRB, 2);
	RB_AttackSUBTypeGROUP->addButton(thirdSubTypeAttackRB, 3);

	// Добавляем кнопки в визуальную группу
	VBoxForRBAttackSUBType->addWidget(firstSubTypeAttackRB);
	VBoxForRBAttackSUBType->addWidget(secondSubTypeAttackRB);
	VBoxForRBAttackSUBType->addWidget(thirdSubTypeAttackRB);

	// Добавление слоёв
	RB_AttackSUBTypeBOX->setLayout(VBoxForRBAttackSUBType);
	radioButtonsGridLayout->addWidget(RB_AttackSUBTypeBOX, 2, 0, Qt::AlignCenter);

	// -------------------- Выбор значения Pt согласно введенным значениям -------------------- //
	RB_PtValueGROUP = new QButtonGroup(RBGroupsContainer);
	RB_PtValueBOX = new QGroupBox("Вероятность p(t)", RBGroupsContainer);
	RB_PtValueBOX->setFixedSize(180, 100);
	VBoxForRB_PtValue = new QVBoxLayout();

	PtLargeRB = new QRadioButton("p(t) > 0.5", RBGroupsContainer);
	PtLessRB = new QRadioButton("p(t) < 0.5", RBGroupsContainer);

	RB_PtValueGROUP->addButton(PtLargeRB, 1);
	RB_PtValueGROUP->addButton(PtLessRB, 2);

	VBoxForRB_PtValue->addWidget(PtLargeRB);
	VBoxForRB_PtValue->addWidget(PtLessRB);

	RB_PtValueBOX->setLayout(VBoxForRB_PtValue);
	radioButtonsGridLayout->addWidget(RB_PtValueBOX, 3, 0, Qt::AlignCenter);

	// Графический эффект прозрачность для ComboBoxes
	opacityHalfBox1 = new QGraphicsOpacityEffect(RBGroupsContainer);
	opacityHalfBox1->setOpacity(1);
	RB_AttackTypeBOX->setGraphicsEffect(opacityHalfBox1);

	opacityHalfBox2 = new QGraphicsOpacityEffect(RBGroupsContainer);
	opacityHalfBox2->setOpacity(1);
	RB_AttackSUBTypeBOX->setGraphicsEffect(opacityHalfBox2);

	opacityHalfBox3 = new QGraphicsOpacityEffect(RBGroupsContainer);
	opacityHalfBox3->setOpacity(1);
	RB_PtValueBOX->setGraphicsEffect(opacityHalfBox3);
}

void MainWindow::setupConnections() {
	// Слот для открытия окна ввода
	connect(startInputDataButton, &QPushButton::clicked, this, &MainWindow::openInputDataWindow);
	connect(mainTablesShowButton, &QPushButton::clicked, this, &MainWindow::onMainTableShowButton);
	connect(showChartsButton, &QPushButton::clicked, this, &MainWindow::onShowChartsButton);
	connect(clearMainTabButton, &QPushButton::clicked, this, &MainWindow::clearMainTabWidget);
}

// --------------------------- Buttons slots --------------------------- //
void MainWindow::openInputDataWindow() {
	if (!inputDataWindow) {
		inputDataWindow = new InputDataWindow;
		// Соединение для кнопки ввода данных "OK"
		connect(inputDataWindow, &InputDataWindow::dataEntered, this, &MainWindow::transportDataToModel);
	}
	
	inputDataWindow->show();
}

void MainWindow::onMainTableShowButton() {
	int currModeId = comboBoxWithChartsTypes->currentIndex();
	
	// Если режим генерации матриц смежности, то выводим матрицу
	if (currModeId == 5 || currModeId == 6) {
		showAdjMatrixTable();
	}
	// Иначе выводим таблицу с информацией об атаках
	else {
		generateAndShowTableWithAttacksInfo();
	}
}

void MainWindow::onShowChartsButton() {
	int currModeId = comboBoxWithChartsTypes->currentIndex();

	// Если режим генерации матриц смежности, то генерируем выводим матрицу
	if (currModeId == 5 || currModeId == 6) {
		model->generateAdjacencyMatrix();
		showAdjMatrixTable();
	}
	// Иначе генерируем и выводим графики
	else {
		generateAndShowCharts();
	}
}

void MainWindow::clearMainTabWidget() {
	if (tableWithMainAttacksInfo) {
		tableWithMainAttacksInfo->hide();
	}
	if (generalChartView) {
		generalChartView->hide();
	}
	if (tableForAdjMatrix) {
		tableForAdjMatrix->hide();
	}
}

void MainWindow::onComboBoxChanged() {
	int currComboBoxId = comboBoxWithChartsTypes->currentIndex();

	// Если режимы работы с матрицей - меняем команды кнопок на режим работы с матрицей
	if (currComboBoxId == 5 || currComboBoxId == 6) {
		showChartsButton->setText("Генерирация матрицы");
		mainTablesShowButton->setText("Вывод матрицы");
	}
	// Иначе кнопки в режиме таблицы и графиков
	else {
		showChartsButton->setText("Построить график");
		mainTablesShowButton->setText("Таблица атак pA(t)");
	}

	// Обработка эффекта прозрачности и активности блоков выбора для разных графиков
	// 0 - 4 -> режимы работы с графиками, активна правая зона с выбором атак и т.д.
	// 5, 6 -> режимы работы матрицей, правая зона неактивна
	switch (currComboBoxId) {
	case 0:
		RB_AttackTypeBOX->setEnabled(true);
		RB_AttackSUBTypeBOX->setEnabled(true);
		RB_PtValueBOX->setEnabled(false);

		opacityHalfBox1->setOpacity(1);
		opacityHalfBox2->setOpacity(1);
		opacityHalfBox3->setOpacity(0.5);
		break;
	case 1:
		RB_AttackTypeBOX->setEnabled(false);
		RB_AttackSUBTypeBOX->setEnabled(false);
		RB_PtValueBOX->setEnabled(true);

		opacityHalfBox1->setOpacity(0.5);
		opacityHalfBox2->setOpacity(0.5);
		opacityHalfBox3->setOpacity(1);
		break;
	case 2:
		RB_AttackTypeBOX->setEnabled(false);
		RB_AttackSUBTypeBOX->setEnabled(false);
		RB_PtValueBOX->setEnabled(true);

		opacityHalfBox1->setOpacity(0.5);
		opacityHalfBox2->setOpacity(0.5);
		opacityHalfBox3->setOpacity(1);
		break;
	case 3:
		RB_AttackTypeBOX->setEnabled(true);
		RB_AttackSUBTypeBOX->setEnabled(true);
		RB_PtValueBOX->setEnabled(true);

		opacityHalfBox1->setOpacity(1);
		opacityHalfBox2->setOpacity(1);
		opacityHalfBox3->setOpacity(1);
		break;
	case 4:
		RB_AttackTypeBOX->setEnabled(true);
		RB_AttackSUBTypeBOX->setEnabled(true);
		RB_PtValueBOX->setEnabled(true);

		opacityHalfBox1->setOpacity(1);
		opacityHalfBox2->setOpacity(1);
		opacityHalfBox3->setOpacity(1);
		break;
	case 5:
		RB_AttackTypeBOX->setEnabled(false);
		RB_AttackSUBTypeBOX->setEnabled(false);
		RB_PtValueBOX->setEnabled(false);

		opacityHalfBox1->setOpacity(0.5);
		opacityHalfBox2->setOpacity(0.5);
		opacityHalfBox3->setOpacity(0.5);
		break;

	case 6:
		RB_AttackTypeBOX->setEnabled(false);
		RB_AttackSUBTypeBOX->setEnabled(false);
		RB_PtValueBOX->setEnabled(false);

		opacityHalfBox1->setOpacity(0.5);
		opacityHalfBox2->setOpacity(0.5);
		opacityHalfBox3->setOpacity(0.5);
		break;
	default:
		break;
	}
}
// --------------------------- Buttons slots --------------------------- //

void MainWindow::transportDataToModel(double linearA1, double linearA2, double linearA3, double linearB1, double linearB2, double linearB3,
									  double expA1, double expA2, double expA3, double expB1, double expB2, double expB3,
									  unsigned int maxT, unsigned int k, unsigned int n, double PtLarge, double PtLess) {

	model->setLinearParams(linearA1, linearA2, linearA3, linearB1, linearB2, linearB3);
	model->setExpParams(expA1, expA2, expA3, expB1, expB2, expB3);
	model->setOtherParams(maxT, k, n, PtLarge, PtLess);

	// Генерируем datasets внутри модели согласно введенным данным
	model->generateDataAboutProbabilityAttacks();
	model->generateDataAboutTestingByONEDevice();
	model->generateDataAboutTestingByMULTIPLEDevice();

	// ------------------------ Debugging Translate Data ------------------------ // 
	qDebug() << "Res data: ";
	qDebug() << "linA1: " << linearA1;
	qDebug() << "linA2: " << linearA2;
	qDebug() << "linA3: " << linearA3;
	qDebug() << "linB1: " << linearB1;
	qDebug() << "linB2: " << linearB2;
	qDebug() << "linB3: " << linearB3;

	qDebug() << "expA1: " << expA1;
	qDebug() << "expA2: " << expA2;
	qDebug() << "expA3: " << expA3;
	qDebug() << "expB1: " << expB1;
	qDebug() << "expB2: " << expB2;
	qDebug() << "expB3: " << expB3;

	qDebug() << "maxT: " << maxT;
	qDebug() << "k: " << k;
	qDebug() << "n: " << n;

	qDebug() << "PtLarge: " << PtLarge;
	qDebug() << "PtLess: " << PtLess;
	// ------------------------ Debugging Translate Data ------------------------ // 
}

void MainWindow::generateAndShowTableWithAttacksInfo() {
	if (generalChartView) {
		generalChartView->hide();
	}
	if (tableForAdjMatrix) {
		tableForAdjMatrix->hide();
	}

	// max t
	unsigned int tempMaxT = model->getMaxT();

	// Линейные выборки
	std::vector<double> tempLinearDataSetPt1 = model->getLinearDataSetWithPt1();
	std::vector<double> tempLinearDataSetPt2 = model->getLinearDataSetWithPt2();
	std::vector<double> tempLinearDataSetPt3 = model->getLinearDataSetWithPt3();

	// Экспопенциальные выборки
	std::vector<double> tempExpDataSetPt1 = model->getExpDataSetWithPt1();
	std::vector<double> tempExpDataSetPt2 = model->getExpDataSetWithPt2();
	std::vector<double> tempExpDataSetPt3 = model->getExpDataSetWithPt3();

	if (!tableWithMainAttacksInfo) {
		tableWithMainAttacksInfo = new QTableWidget(mainTabView);
		tableWithMainAttacksInfo->setObjectName("tableWithMainAttackInfo");
		mainTabLayout->addWidget(tableWithMainAttacksInfo, 0, Qt::AlignCenter);

		// Настройка геометрии
		tableWithMainAttacksInfo->setColumnCount(7);
		tableWithMainAttacksInfo->setFixedSize(750, 480);
		tableWithMainAttacksInfo->verticalHeader()->hide();

		tableWithMainAttacksInfo->setColumnWidth(0, 40);
		for (int i = 1; i < 7; i++) {
			tableWithMainAttacksInfo->setColumnWidth(i, 115);
		}

		// Настройка заголовков
		QStringList headers;
		headers << "t" << "Лин. 1" << "Лин. 2" << "Лин. 3" << "Эксп. 1" << "Эксп. 2" << "Эксп. 3";
		tableWithMainAttacksInfo->setHorizontalHeaderLabels(headers);
	}
	else {
		tableWithMainAttacksInfo->clearContents(); // если была уже создана - очистить
	}

	// Установка кол-ва строк
	tableWithMainAttacksInfo->setRowCount(tempMaxT);

	for (unsigned int i = 0; i < tempMaxT; i++) {
		tableWithMainAttacksInfo->setItem(i, 0, new QTableWidgetItem(QString::number(i + 1)));

		tableWithMainAttacksInfo->setItem(i, 1, new QTableWidgetItem(QString::number(tempLinearDataSetPt1[i])));
		tableWithMainAttacksInfo->setItem(i, 2, new QTableWidgetItem(QString::number(tempLinearDataSetPt2[i])));
		tableWithMainAttacksInfo->setItem(i, 3, new QTableWidgetItem(QString::number(tempLinearDataSetPt3[i])));

		tableWithMainAttacksInfo->setItem(i, 4, new QTableWidgetItem(QString::number(tempExpDataSetPt1[i])));
		tableWithMainAttacksInfo->setItem(i, 5, new QTableWidgetItem(QString::number(tempExpDataSetPt2[i])));
		tableWithMainAttacksInfo->setItem(i, 6, new QTableWidgetItem(QString::number(tempExpDataSetPt3[i])));
	}

	tableWithMainAttacksInfo->show();
}

void MainWindow::generateAndShowCharts() {
	if (tableWithMainAttacksInfo) {
		tableWithMainAttacksInfo->hide();
	}
	if (tableForAdjMatrix) {
		tableForAdjMatrix->hide();
	}

	// Обновляем макс. значения осей
	maxValX = 0;
	maxValY = 0.0;
	
	if (!generalChart) {
		generalChart = new QChart();

		// Оси графика
		axisX = new QValueAxis(generalChart);
		axisY = new QValueAxis(generalChart);

		// Вид (виджет) для графика
		generalChartView = new QChartView(generalChart, mainTabView);
		generalChartView->setRenderHint(QPainter::Antialiasing); // Сглаживание

		mainTabLayout->addWidget(generalChartView);

		// DataSeries for chart
		dataSeriesForChart = new QLineSeries(mainTabView);
		dataSeriesForChart->setPointsVisible(true);
		// dataSeriesForChart->setPointLabelsVisible(true);
		dataSeriesForChart->setBrush(QBrush(Qt::yellow));
		generalChart->addSeries(dataSeriesForChart);
		

		generalChart->addAxis(axisX, Qt::AlignBottom);
		generalChart->addAxis(axisY, Qt::AlignLeft);

		dataSeriesForChart->attachAxis(axisX);
		dataSeriesForChart->attachAxis(axisY);
	}
	else {
		dataSeriesForChart->clear();
		axisX->setTitleText("");
		axisY->setTitleText("");
	}

	// Заполнение Series методами согласно indexes in ComboBox
	int currIndexComboBox = comboBoxWithChartsTypes->currentIndex();
	qDebug() << "ComboBox" << currIndexComboBox;
	// Выбор метода заполнения данных
	switch (currIndexComboBox) {
	case 0:
		dataSeriesForChart->append(0, 0);
		axisX->setTitleText("t");
		axisY->setTitleText("pA(t)");
		fillSeriesByAttackInfo();
		break;
	case 1:
		axisX->setTitleText("k");
		axisY->setTitleText("P(p(t))");
		fillSeriesByPTsOneDevice();
		break;
	case 2:
		axisX->setTitleText("k");
		axisY->setTitleText("P(p(t),k,n)");
		fillSeriesByPTsMultipleDevice();
		break;
	case 3:
		axisX->setTitleText("t");
		axisY->setTitleText("P(pA(t), p(t, k))");
		fillSeriesByPTsMULTIPLEDeviceWithOPPOSITION_byTK();
		break;
	case 4:
		axisX->setTitleText("k");
		axisY->setTitleText("P(pA(t), p(t, k))");
		fillSeriesByPTsMULTIPLEDeviceWithOPPOSITION_byTK();
		break;
	default:
		break;
	}

	axisX->setRange(1, maxValX);
	// axisY->setRange(0, maxValY);
	axisY->setRange(0, 1.0);
	generalChartView->show();
}

// ---------------------- Filling Series for Charts ---------------------- //
void MainWindow::fillSeriesByAttackInfo() {
	int checkedIdAttackType = RB_AttackTypeGROUP->checkedId();
	int checkedIdAttackSUBType = RB_AttackSUBTypeGROUP->checkedId();

	// Если ничего не выбрано из типа и ПОДтипа атак не строим
	if (checkedIdAttackType == -1 || checkedIdAttackSUBType == -1) {
		generalChart->setTitle("Для построения графика необходимо выбрать типы и подтипы атак");
		return;
	}

	unsigned int tempMaxT = model->getMaxT();
	std::vector<double> tempVectorY;
	
	// Заполняем временный вектор выборкой согласно RadioButtons
	if (checkedIdAttackType == 1) {
		generalChart->setTitle("График изменения вероятностей отказа для линейного типа атаки");
		switch (checkedIdAttackSUBType) {
		case 1:
			tempVectorY = model->getLinearDataSetWithPt1();
			break;
		case 2:
			tempVectorY = model->getLinearDataSetWithPt2();
			break;
		case 3:
			tempVectorY = model->getLinearDataSetWithPt3();
			break;
		default:
			break;
		}
	}
	else {
		generalChart->setTitle("График изменения вероятностей отказа для экспоненциального типа атаки");
		switch (checkedIdAttackSUBType) {
		case 1:
			tempVectorY = model->getExpDataSetWithPt1();
			break;
		case 2:
			tempVectorY = model->getExpDataSetWithPt2();
			break;
		case 3:
			tempVectorY = model->getExpDataSetWithPt3();
			break;
		default:
			break;
		}
	}

	maxValX = tempMaxT;
	// Заполняем серию
	for (size_t i = 1; i <= tempMaxT; i++) {
		dataSeriesForChart->append(i, tempVectorY[i - 1]);
		maxValY = std::max(maxValY, tempVectorY[i - 1]);
	}
}

void MainWindow::fillSeriesByPTsOneDevice() {
	int checkedIdPT = RB_PtValueGROUP->checkedId();

	if (checkedIdPT == -1) {
		generalChart->setTitle("Для построения графика необходимо выбрать параметр p(t)");
		return;
	}
	
	int tempK = model->getK();
	std::vector<double> tempDataSet;
	
	if (checkedIdPT == 1) {
		generalChart->setTitle("График вероятности P(p(t)) обнаружения уязвимостей при тестировании отдельным устройством p(t) > 0.5");
		tempDataSet = model->getPtLargeDataSetONEDevice();
	}
	else {
		generalChart->setTitle("График вероятности P(p(t)) обнаружения уязвимостей при тестировании отдельным устройством p(t) less 0.5");
		tempDataSet = model->getPtLessDataSetONEDevice();
	}

	maxValX = tempK;
	for (size_t i = 1; i <= tempDataSet.size(); i++) {
		dataSeriesForChart->append(i, tempDataSet[i - 1]);
		maxValY = std::max(maxValY, tempDataSet[i - 1]);
	}
}

void MainWindow::fillSeriesByPTsMultipleDevice() {
	int checkedIdPT = RB_PtValueGROUP->checkedId();

	if (checkedIdPT == -1) {
		generalChart->setTitle("Для построения графика необходимо выбрать параметр p(t)");
		return;
	}

	int tempK = model->getK();
	std::vector<double> tempDataSet;

	if (checkedIdPT == 1) { 
		generalChart->setTitle("График вероятности P(p(t,k)) обнаружения уязвимостей при коллаборационной стратегии p(t) > 0.5");
		tempDataSet = model->getPtLargeDataSetMULTIPLEDevices();
	}
	else {
		generalChart->setTitle("График вероятности P(p(t,k)) обнаружения уязвимостей при коллаборационной стратегии p(t) less 0.5");
		tempDataSet = model->getPtLessDataSetMULTIPLEDevices();
	}

	// Переписываем данные в Серию для графика, обновляя макс. значения осей
	maxValX = tempK;
	for (size_t i = 0; i < tempDataSet.size(); i++) {
		dataSeriesForChart->append(i + 1, tempDataSet[i]);
		maxValY = std::max(maxValY, tempDataSet[i]);
	}
}

void MainWindow::fillSeriesByPTsMULTIPLEDeviceWithOPPOSITION_byTK() {
	int attackTypeId = RB_AttackTypeGROUP->checkedId();
	int attackSubTypeId = RB_AttackSUBTypeGROUP->checkedId();
	int PtId = RB_PtValueGROUP->checkedId();
	int chartId = comboBoxWithChartsTypes->currentIndex();

	// Ничего не выбрано
	if (attackTypeId == -1 || attackSubTypeId == -1 || PtId == -1) {
		generalChart->setTitle("Для построения графика следует выбрать типы и подтипы атак, а также значение p(t)");
		return;
	}

	// Генерируем данные в модели
	model->generateDataAboutTestingByMULTIPLYDeviceWithOppositionByT(attackTypeId, attackSubTypeId, PtId, chartId);
	// Получаем данные из модели
	std::vector<double> tempDataSet = model->getPtkDataSetMULTIPLEDevicesWithOPPOSITION();
	
	// Если тип графика от t
	if (chartId == 3) {
		if (attackTypeId == 1) {
			generalChart->setTitle(QString("График обнар. уязв. от t при коллаб. стр. в условиях противодействия тестированию для линейного типа, подтип %1, k = %2, n = %3").
									arg(attackSubTypeId).arg(model->getK()).arg(model->getN()));
		}
		else {
			generalChart->setTitle(QString("График обнар. уязв. от t при коллаб. стр. в условиях противодействия тестированию для экспоненц. типа, подтип %1, k = %2, n = %3").
									arg(attackSubTypeId).arg(model->getK()).arg(model->getN()));
		}
	} 
	// Если тип графика от k
	else if (chartId == 4) {
		if (attackTypeId == 1) {
			generalChart->setTitle(QString("График обнар. уязв. от k при коллаб. стр. в условиях противодействия тестированию для линейного типа, подтип %1, t = %2, n = %3").
									arg(attackSubTypeId).arg(model->getMaxT()).arg(model->getN()));
		}
		else {
			generalChart->setTitle(QString("График обнар. уязв. от k при коллаб. стр. в условиях противодействия тестированию для экспоненц. типа, подтип %1, t = %2, n = %3").
									arg(attackSubTypeId).arg(model->getMaxT()).arg(model->getN()));
		}
	}

	maxValX = (unsigned int)tempDataSet.size();
	for (size_t i = 0; i < tempDataSet.size(); i++) {
		dataSeriesForChart->append(i + 1, tempDataSet[i]);
		maxValY = std::max(tempDataSet[i], maxValY);
	}
}
// ---------------------- Filling Series for Charts ---------------------- //

void MainWindow::showAdjMatrixTable() {
	if (generalChartView) {
		generalChartView->hide();
	}
	if (tableWithMainAttacksInfo) {
		tableWithMainAttacksInfo->hide();
	}

	// Создаем таблицу если еще не создана
	if (!tableForAdjMatrix) {
		tableForAdjMatrix = new QTableWidget(mainTabView);
		tableForAdjMatrix->setObjectName("tableForAdjMatrix");
		mainTabLayout->addWidget(tableForAdjMatrix, 0, Qt::AlignCenter);
		
		// Геометрия таблицы
		tableForAdjMatrix->setFixedSize(630, 480);
		tableForAdjMatrix->setRowCount(15);
		tableForAdjMatrix->setColumnCount(15);

		for (int i = 0; i < 15; i++) {
			tableForAdjMatrix->setColumnWidth(static_cast<int>(i), 22);
		}

		// Заголовки таблицы
		// Горизонтальные постоянны, т.к. не меняются в зависимости от выводимой матрицы
		QStringList horizontalHeadersLabels;
		for (int col = 0; col < tableForAdjMatrix->columnCount(); ++col) {
			horizontalHeadersLabels << QString("Y%1").arg(col + 1);
		}
		tableForAdjMatrix->setHorizontalHeaderLabels(horizontalHeadersLabels);
	}
	else {
		tableForAdjMatrix->clearContents(); // Удаляем контент
	}

	// Проверяем режим работы
	int currModeId = comboBoxWithChartsTypes->currentIndex();
	std::vector<std::vector<int>> tempMatrix; 

	// Выбираме данные а также Обновляем заголовки для строк в зависимости от режима работы
	QStringList verticalHeadersLabels;
	if (currModeId == 5) {
		// Если режим генерации матрицы смежности
		tempMatrix = model->getAdjMatrix();

		for (int row = 0; row < tableForAdjMatrix->rowCount(); ++row) {
			verticalHeadersLabels << QString("X%1").arg(row + 1);
		}
	}
	else if (currModeId == 6) {
		// Иначе, если режим генерации минимальных покрытий по матрице смежности
		tempMatrix = model->getAdjMatrixWithMinCoverage();

		// Обозначаем Xi минимального покрытия
		std::unordered_set<int> tempSet = model->getMinCoveredSetXi();
		for (const auto indx : tempSet) {
			verticalHeadersLabels << QString("X%1").arg(indx + 1);
		}
		// Заполняем оставшиеся поля, не относящиеся к мин. покрытию пропусками
		for (size_t row = tempSet.size() - 1; row < tableForAdjMatrix->rowCount(); ++row) {
			verticalHeadersLabels << QString("-");
		}
	}
	tableForAdjMatrix->setVerticalHeaderLabels(verticalHeadersLabels);

	for (size_t i = 0; i < tempMatrix.size(); i++) {
		for (size_t j = 0; j < tempMatrix[i].size(); j++) {
			tableForAdjMatrix->setItem(static_cast<int>(i), static_cast<int>(j), new QTableWidgetItem(QString::number(tempMatrix[i][j])));
		}
	}
	tableForAdjMatrix->show();
}
