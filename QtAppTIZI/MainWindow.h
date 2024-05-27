#pragma once

#include <qpushbutton.h>
#include <qmainwindow.h>
#include <qgridlayout.h>
#include <qvboxlayout>
#include <qdebug.h>
#include <qtablewidget.h>
#include <QHeaderView>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <QtCharts/qchart.h>
#include <QtCharts/qlineseries.h>
#include <QtCharts/qchartview.h>
#include <QtCharts/qvalueaxis.h>
#include <QGraphicsOpacityEffect>

#include "MathModel.h"
#include "InputDataWindow.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	// constructor\destructor declaration
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void openInputDataWindow();
	void transportDataToModel(double linearA1, double linearA2, double linearA3, double linearB1, double linearB2, double linearB3,
							  double expA1, double expA2, double expA3, double expB1, double expB2, double expB3,
							  unsigned int maxT, unsigned int k, unsigned int n,
							  double PtLarge, double PtLess);
	void onMainTableShowButton();
	void onShowChartsButton();
	void onComboBoxChanged();
	void clearMainTabWidget();

private:
	// MathModel for calculations
	Model* model;

	// Graphics effect for RadioButtons Boxes to change opacity
	QGraphicsOpacityEffect* opacityHalfBox1;
	QGraphicsOpacityEffect* opacityHalfBox2;
	QGraphicsOpacityEffect* opacityHalfBox3;

	// Widgets
	QWidget* centralWidget;
	QWidget* mainTabView;

	// Containers
	QWidget* mainContentContainer;
	QWidget* RBGroupsContainer;
	QWidget* fixedWidthContainer;

	// Layouts
	QVBoxLayout* mainVerticalLayout;
	QGridLayout* topHGridLayout;
	QGridLayout* radioButtonsGridLayout;
	QVBoxLayout* mainTabLayout;
	QGridLayout* bottomGrid;

	// Elements for Main Window
	QPushButton* startInputDataButton;
	QPushButton* mainTablesShowButton;
	QPushButton* showChartsButton;
	QPushButton* clearMainTabButton;
	QComboBox* comboBoxWithChartsTypes;

	// -------------- RadioButtons elements -------------- //
	// Main Label for Radiobuttons Zone
	QLabel* RB_MainLabel;
	// RadioButtons groups
	QButtonGroup* RB_AttackTypeGROUP;
	QButtonGroup* RB_AttackSUBTypeGROUP;
	QButtonGroup* RB_PtValueGROUP;

	// RadioButtons groupBoxes
	QGroupBox* RB_AttackTypeBOX;
	QGroupBox* RB_AttackSUBTypeBOX;
	QGroupBox* RB_PtValueBOX;

	// Vertical Layouts
	QVBoxLayout* VBoxForRBAttackType;
	QVBoxLayout* VBoxForRBAttackSUBType;
	QVBoxLayout* VBoxForRB_PtValue;

	// RadioButtons
	QRadioButton* radioButtonLINEARAttack;
	QRadioButton* radioButtonEXPAttack;

	QRadioButton* firstSubTypeAttackRB;
	QRadioButton* secondSubTypeAttackRB;
	QRadioButton* thirdSubTypeAttackRB;

	QRadioButton* PtLargeRB;
	QRadioButton* PtLessRB;
	// -------------- RadioButtons elements -------------- //

	// Tables
	QTableWidget* tableWithMainAttacksInfo; // Виджет таблицы для вероятностей атак от времени
	QTableWidget* tableForAdjMatrix;

	// Charts Elements
	QLineSeries* dataSeriesForChart;
	QChartView* generalChartView;
	QChart* generalChart;
	QValueAxis* axisX;
	QValueAxis* axisY;
	unsigned int maxValX;
	double maxValY;


	// Windows
	InputDataWindow* inputDataWindow;

	void setupUI();				// Метод первичной настройки UI
	void setupLayouts();		// Метод настройки слоёв окна
	void setupWidgets();		// Метод создания и настройки виджетов
	void setupRadioButtons();
	void setupConnections();	// Метод настройки соединений (сигналы, слоты)

	// Метод построения таблицы с информацией об атаках
	void generateAndShowTableWithAttacksInfo();

	// Генерация графиков в зависимости от выбранного режима
	void generateAndShowCharts();

	// Methods for adding data to series for diff charts
	// Метод для генерации данных для графиков по вероятностям отказа
	void fillSeriesByAttackInfo(); 

	// Генерация данных для разных p(t) и одиночных\коллаборационных тестирований
	void fillSeriesByPTsOneDevice();
	void fillSeriesByPTsMultipleDevice();

	// В условиях противодействия тестированию
	void fillSeriesByPTsMULTIPLEDeviceWithOPPOSITION_byTK();
	
	// Метод для вывода матрицы смежности, а также матрицы с минимальными покрытиями
	void showAdjMatrixTable();

	// void DEBUG_adjMatrix();
};