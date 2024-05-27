#pragma once

#include <qwidget.h>
#include <qpushbutton.h>
#include <qlineedit.h>
#include <QVBoxLayout>
#include <qgridlayout.h>
#include <qlabel.h>
#include <qvalidator.h>

class InputDataWindow : public QWidget {
	Q_OBJECT
	
public: 
	explicit InputDataWindow(QWidget* parent = nullptr);
	~InputDataWindow();

signals:
	void dataEntered(double linearA1, double linearA2, double linearA3, double linearB1, double linearB2, double linearB3,
					 double expA1, double expA2, double expA3, double expB1, double expB2, double expB3,
					 unsigned int maxT, unsigned int k, unsigned int n,
					 double PtLarge, double PtLess);

private slots:
	void inputFinishedCloseWindow();

private:
	void setupUI();
	void setupLayouts();
	void setupWidgets();
	void setupConnections();

	QPushButton* finishedInputButton; // Button to finish input data

	// Layouts
	QVBoxLayout* mainVLayout;	
	QWidget* topZone;
	QWidget* middleZone;
	QWidget* middle1Zone;
	QWidget* bottomZone;
	QGridLayout* topGridLay;
	QGridLayout* middleGridLay;
	QGridLayout* middle1GridLay;
	QGridLayout* bottomGridLay;
	// Layouts
	
	// Linear params Labels
	QLabel* mainLinearLabel;
	QLabel* linearA1label;
	QLabel* linearA2label;
	QLabel* linearA3label;
	QLabel* linearB1label;
	QLabel* linearB2label;
	QLabel* linearB3label;
	// Linear Params
	QLineEdit* inputLinearA1;
	QLineEdit* inputLinearA2;
	QLineEdit* inputLinearA3;
	QLineEdit* inputLinearB1;
	QLineEdit* inputLinearB2;
	QLineEdit* inputLinearB3;

	// Exponential params Labels
	QLabel* mainExpLabel;
	QLabel* expA1label;
	QLabel* expA2label;
	QLabel* expA3label;
	QLabel* expB1label;
	QLabel* expB2label;
	QLabel* expB3label;
	// For Exponential Params
	QLineEdit* inputExpA1;
	QLineEdit* inputExpA2;
	QLineEdit* inputExpA3;
	QLineEdit* inputExpB1;
	QLineEdit* inputExpB2;
	QLineEdit* inputExpB3;

	// t, k, n Params Labels
	QLabel* inputMaxTLabel;
	QLabel* inputKLabel;
	QLabel* inputNLabel;
	// t, k, n Params LineEdit
	QLineEdit* inputMaxT;
	QLineEdit* inputK;
	QLineEdit* inputN;

	// p(t) Params Labels
	QLabel* PtLargerLabel;
	QLabel* PtLessLabel;
	// p(t) Params LineEdits
	QLineEdit* inputPtLarger;
	QLineEdit* inputPtLess;

	// Input Validators
	QDoubleValidator* doubleValidator;
	QIntValidator* intValidator;
};