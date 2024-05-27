#pragma once

#include <utility>
#include <vector>
#include <cmath>
#include <random>
#include <unordered_set>

// Входные параметры: 
// 1. Типы атак по условию: 
//		Линейная: pA(t) = at + b;
//		Экспоненцианальная: pA(t) = 1-aexp(-bt); 
// 2. t - параметр в диапазоне [1; 17] (момент времени действия атаки);
// 3. k - параметр в диапазоне [1; 15] (количество тестов \ число тестировщиков);
// 4. n - параметр в диапазоне [1;  8] (количество участников \ мощность коллабораций (колличество участников в коллаборации));

class Model {
private:
	// Параметры a, b для линейного типа атак
	std::pair<double, double> linearA1_B1;
	std::pair<double, double> linearA2_B2;
	std::pair<double, double> linearA3_B3;
	// Параметры a, b для экспоненциального типа атак
	std::pair<double, double> expA1_B1;
	std::pair<double, double> expA2_B2;
	std::pair<double, double> expA3_B3;

	unsigned int maxt;		// Макс. значение интервала времени
	unsigned int k;			// Количество тестов\тестировщиков
	unsigned int n;			// Кол-во участников\кол-во участников в коллаборации

	double PtLarge;			// Параметр p(t) > 0.5
	double PtLess;			// Параметр p(t) < 0.5

	// Векторы для атак Линейного типа в каждой строке значения для подтипа атаки (Н, С, В)
	std::vector<double> linearDataSetWithPt1;	
	std::vector<double> linearDataSetWithPt2;
	std::vector<double> linearDataSetWithPt3;

	// Векторы для атак Экспоненциального типа
	std::vector<double> expDataSetWithPt1;
	std::vector<double> expDataSetWithPt2;
	std::vector<double> expDataSetWithPt3;

	// Векторы для вероятности P(p(t)) обнаружения уязвимостей
	std::vector<double> PtLargeDataSetOneDevice;
	std::vector<double> PtLessDataSetOneDevice;

	// Векторы для вероятности P(p(t), k, n) обнаружения уязвимостей
	std::vector<double> PtLargeDataSetMultipleDevices;
	std::vector<double> PtLessDataSetMultipleDevices;

	// Вектор для вероятности P(p(t), k) при коллаборационной стратегии в условии противодействия тестированию
	std::vector<double> PtkDataSetMULTIPLEDevicesWithOPPOSITION;

	// Матрица смежности
	std::vector<std::vector<int>> adjacencyMatrix;
	// Матрица смежности с мин покрытием
	std::vector<std::vector<int>> adjMatrixWithMinCoverage;
	// Множество минимальных покрытий
	std::unordered_set<int> minCoveredSetNodesXi;

	// Генератор случайных чисел
	std::random_device rd;
	std::mt19937 gen;
	std::uniform_int_distribution<int> distributionForEdgesCounts;
	std::uniform_int_distribution<int> distrForIndx;

public:
	Model();
	~Model();

	// --------------------- getTERS --------------------- //
	// Получить значение макс. t
	unsigned int getMaxT();
	unsigned int getK();
	unsigned int getN();

	std::vector<double> getLinearDataSetWithPt1();
	std::vector<double> getLinearDataSetWithPt2();
	std::vector<double> getLinearDataSetWithPt3();

	std::vector<double> getExpDataSetWithPt1();
	std::vector<double> getExpDataSetWithPt2();
	std::vector<double> getExpDataSetWithPt3();

	std::vector<double> getPtLargeDataSetONEDevice();
	std::vector<double> getPtLessDataSetONEDevice();

	std::vector<double> getPtLargeDataSetMULTIPLEDevices();
	std::vector<double> getPtLessDataSetMULTIPLEDevices();
	std::vector<double> getPtkDataSetMULTIPLEDevicesWithOPPOSITION();

	std::vector<std::vector<int>> getAdjMatrix();
	std::vector<std::vector<int>> getAdjMatrixWithMinCoverage();
	std::unordered_set<int> getMinCoveredSetXi();

	// Метод установки линейных параметров
	void setLinearParams(double linearA1, double linearA2, double linearA3, double linearB1, double linearB2, double linearB3);
	// Метод установки экспоненциальных параметров
	void setExpParams(double expA1, double expA2, double expA3, double expB1, double expB2, double expB3);
	// Метод установки остальных параметров: maxt, k, n, ptLarge, ptLess
	void setOtherParams(unsigned int maxT, unsigned int k, unsigned int n, double PtLarge, double PtLess);

	// Метод для генерации вектора для трёх значений A, B определенного типа атаки
	// Каждая выборка определенного подтипа атаки (низкая, средняя, высокая интенсивность)
	void generateDataAboutProbabilityAttacks();

	// Метод генерации вероятностей обнаружения уязвимости при тестировании 1 устройстовм P(p(t))
	void generateDataAboutTestingByONEDevice();

	// Метод генерации вероятностей обнаружения уязвимости при тестировании множеством устройстовм P(p(t), k, n)
	void generateDataAboutTestingByMULTIPLEDevice();

	// Метод генерации вероятности P(p(t,k)) обнаружения уязвимостей при коллаборационной стратегии в условиях противодействия тестированию относительно времени t
	void generateDataAboutTestingByMULTIPLYDeviceWithOppositionByT(int typeOfAttack, int subTypeOfAttack, int typeOfPt, int typeOfChart);

	// Метод для генерации матрицы смежности
	void generateAdjacencyMatrix();

	// Метод для генерации матрицы смежности минимального покрытия
	void generateAdjMatrixWithMinCoverage();

	// don't know what doing this method ))))   ()_()
	unsigned long long factorial(int n);
};
