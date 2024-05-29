#include "MathModel.h"

// Определение конструктора, полей модели
Model::Model() :
	// Параметры Линейных атак
	linearA1_B1(0.0f, 0.0f),
	linearA2_B2(0.0f, 0.0f),
	linearA3_B3(0.0f, 0.0f),

	// Параметры Экспоненциальных атак
	expA1_B1(0.0f, 0.0f),
	expA2_B2(0.0f, 0.0f),
	expA3_B3(0.0f, 0.0f),

	// Остальные праметры
	maxt(0), k(0), n(0),
	PtLarge(0), PtLess(0),

	// Векторы для матрицы смежности
	adjacencyMatrix(15, std::vector<int>(15, 0)), 
	adjMatrixWithMinCoverage(),
	// Элементы генератора случ. чисел
	rd(), gen(rd()), distributionForEdgesCounts(1, 6), distrForIndx(0, 14) {}

Model::~Model() {}

// ----------------------------- Определение getTERS ----------------------------- //
unsigned int Model::getMaxT() {
	return maxt;
}
unsigned int Model::getK() {
	return k;
}
unsigned int Model::getN() {
	return n;
}

// Линейные
std::vector<double> Model::getLinearDataSetWithPt1() {
	return linearDataSetWithPt1;
}
std::vector<double> Model::getLinearDataSetWithPt2() {
	return linearDataSetWithPt2;
}
std::vector<double> Model::getLinearDataSetWithPt3() {
	return linearDataSetWithPt3;
}
// Экспоненциальные
std::vector<double> Model::getExpDataSetWithPt1() {
	return expDataSetWithPt1;
}
std::vector<double> Model::getExpDataSetWithPt2() {
	return expDataSetWithPt2;
}
std::vector<double> Model::getExpDataSetWithPt3() {
	return expDataSetWithPt3;
}
std::vector<double> Model::getPtLargeDataSetONEDevice() {
	return PtLargeDataSetOneDevice;
}
std::vector<double> Model::getPtLessDataSetONEDevice() {
	return PtLessDataSetOneDevice;
}
std::vector<double> Model::getPtLargeDataSetMULTIPLEDevices() {
	return PtLargeDataSetMultipleDevices;
}
std::vector<double> Model::getPtLessDataSetMULTIPLEDevices() {
	return PtLessDataSetMultipleDevices;
}
std::vector<double> Model::getPtkDataSetMULTIPLEDevicesWithOPPOSITION() {
	return PtkDataSetMULTIPLEDevicesWithOPPOSITION;
}
std::vector<std::vector<int>> Model::getAdjMatrix() {
	return adjacencyMatrix;
}
std::vector<std::vector<int>> Model::getAdjMatrixWithMinCoverage() {
	return adjMatrixWithMinCoverage;
}
std::unordered_set<int> Model::getMinCoveredSetXi() {
	return minCoveredSetNodesXi;
}
std::vector<std::vector<double>> Model::getPtkMultiDataSet() {
	return PtkMultiDataSet;
}



void Model::setLinearParams(double linearA1, double linearA2, double linearA3, double linearB1, double linearB2, double linearB3) {
	linearA1_B1 = { linearA1, linearB1 };
	linearA2_B2 = { linearA2, linearA2 };
	linearA3_B3 = { linearA3, linearA3 };
}

void Model::setExpParams(double expA1, double expA2, double expA3, double expB1, double expB2, double expB3) {
	expA1_B1 = { expA1, expB1 };
	expA2_B2 = { expA2, expB2 };
	expA3_B3 = { expA3, expB3 };
}

void Model::setOtherParams(unsigned int maxT, unsigned int inK, unsigned int inN, double inPtLarge, double inPtLess) {
	maxt = maxT != 0 ? maxT : 1;
	k = inK != 0 ? inK : 1;
	n = inN != 0 ? inN : 1;
	if (k > n) {
		k = n;
	}
	PtLarge = inPtLarge;
	PtLess = inPtLess;
}

void Model::generateDataAboutProbabilityAttacks() {
	// Очищаем предыдущие значения
	linearDataSetWithPt1.clear();
	linearDataSetWithPt2.clear();
	linearDataSetWithPt3.clear();

	expDataSetWithPt1.clear();
	expDataSetWithPt2.clear();
	expDataSetWithPt3.clear();

	// Для каждого момента времени t
	for (unsigned int i = 1; i <= maxt; i++) {
		// Считаем Линейные подТипы атак
		linearDataSetWithPt1.push_back(linearA1_B1.first * static_cast<double>(i) + linearA1_B1.second);
		linearDataSetWithPt2.push_back(linearA2_B2.first * static_cast<double>(i) + linearA2_B2.second);
		linearDataSetWithPt3.push_back(linearA3_B3.first * static_cast<double>(i) + linearA3_B3.second);

		// Считаем Экспоненциальные подТипы атак
		expDataSetWithPt1.push_back(1.0 - expA1_B1.first * exp(-expA1_B1.second * static_cast<double>(i)));
		expDataSetWithPt2.push_back(1.0 - expA2_B2.first * exp(-expA2_B2.second * static_cast<double>(i)));
		expDataSetWithPt3.push_back(1.0 - expA3_B3.first * exp(-expA3_B3.second * static_cast<double>(i)));
	}
}

void Model::generateDataAboutTestingByONEDevice() {
	PtLargeDataSetOneDevice.clear();
	PtLessDataSetOneDevice.clear();

	for (unsigned int i = 1; i <= k; i++) {
		PtLargeDataSetOneDevice.push_back(PtLarge * pow((1.0 - PtLarge), static_cast<double>(i - 1)));
		PtLessDataSetOneDevice.push_back(PtLess * pow((1.0 - PtLess), static_cast<double>(i - 1)));
	}
}

void Model::generateMultiDataSetAboutPtTesting(int typeOfAttack, int subTypeOfAttack, int typeOfPt, int typeOfChart) {
	// Ничего не выбрано
	if (subTypeOfAttack == -1 || typeOfAttack == -1 || typeOfPt == -1) {
		return;
	}

	PtkMultiDataSet.clear();
	unsigned int countSeries = n >= k ? (n - k) : 0;
	std::vector<double> tempVectorWithAttacksInfoByT;

	// Выбираем выборки данных для атак от (t) согласно типу и подтипу атаки
	if (typeOfAttack == 1) {
		switch (subTypeOfAttack) {
		case 1:
			tempVectorWithAttacksInfoByT = linearDataSetWithPt1;
			break;
		case 2:
			tempVectorWithAttacksInfoByT = linearDataSetWithPt2;
			break;
		case 3:
			tempVectorWithAttacksInfoByT = linearDataSetWithPt3;
			break;
		default:
			break;
		}
	}
	else {
		switch (subTypeOfAttack) {
		case 1:
			tempVectorWithAttacksInfoByT = expDataSetWithPt1;
			break;
		case 2:
			tempVectorWithAttacksInfoByT = expDataSetWithPt2;
			break;
		case 3:
			tempVectorWithAttacksInfoByT = expDataSetWithPt3;
			break;
		default:
			break;
		}
	}

	// Генерируем множество выборок в зависимости от режима работы
	switch (typeOfChart) {
	// Режим - 2, генерируем данные для коллаборационной стратегии без противодействия в количестве n - k + 1 выборок
	case 2:	
		for (unsigned int i = 0; i <= countSeries; i++) {
			PtkMultiDataSet.push_back(std::vector<double>(0));

			// Для каждого значения k генерируем выборку P(p(t), k, n)
			for (unsigned int ik = 1; ik <= k; ik++) {
				// В зависимости от типа p(t)
				if (typeOfPt == 1) {	// P(p(t),k,n) =Ckn * p(t)^k * (1 - p(t))^(n - k)
					PtkMultiDataSet[i].push_back((static_cast<double>(factorial(k + i)) / (factorial(ik) * factorial((k + i) - ik))) * 
												 pow(PtLarge, static_cast<double>(ik)) * pow(1.0 - PtLarge, static_cast<double>((k + i) - ik)));
				}
				else {
					PtkMultiDataSet[i].push_back((static_cast<double>(factorial(k + i)) / (factorial(ik) * factorial((k + i) - ik))) * 
												 pow(PtLess, static_cast<double>(ik)) * pow(1.0 - PtLess, static_cast<double>((k + i) - ik)));
				}
			}
		}
		break;

	// Режим - 3, генерируем данные для коллаборационной стратегии с противодействием в количестве t выборок, для каждого типа атаки на промежутке t
	case 3:
		for (unsigned int ik = 1; ik <= k; ik++) {
			PtkMultiDataSet.push_back(std::vector<double>(0));

			// Для каждого значения k генерируем выборку P(p(t), k, n)
			for (unsigned int j = 0; j < maxt; j++) {
				// В зависимости от типа p(t)
				if (typeOfPt == 1) {
					PtkMultiDataSet[ik - 1].push_back((1.0 - tempVectorWithAttacksInfoByT[j]) *
												 (static_cast<double>(factorial(n)) / (factorial(ik) * factorial(n - ik))) *
												 pow(PtLarge, static_cast<double>(ik)) * pow(1.0 - PtLarge, static_cast<double>(n - ik)));
				}
				else {
					PtkMultiDataSet[ik - 1].push_back((1.0 - tempVectorWithAttacksInfoByT[j]) *
												 (static_cast<double>(factorial(n)) / (factorial(ik) * factorial(n - ik))) *
												 pow(PtLess, static_cast<double>(ik)) * pow(1.0 - PtLess, static_cast<double>(n - ik)));
				}
			}
		}
		break;

	// Режим 4, коллаборационная стратегия, построения графика от k, с учетом выборок от k до n, и противодейтсвия
	// Если режим - 4, то генерируем данные для коллаборационной стратегии с учетом противодействия в количестве n - k + 1 выборок
	case 4:	
		// Заполняем выборки данных
		for (unsigned int i = 0; i <= countSeries; i++) {
			PtkMultiDataSet.push_back(std::vector<double>(0));

			// Для каждого значения k генерируем выборку P(p(t), k, n)
			for (unsigned int ik = 1; ik <= k; ik++) {
				// В зависимости от типа p(t)
				if (typeOfPt == 1) {
					PtkMultiDataSet[i].push_back((1.0 - tempVectorWithAttacksInfoByT[(size_t)maxt - 1]) *
												 (static_cast<double>(factorial(k + i)) / (factorial(ik) * factorial((k + i) - ik))) *
												 pow(PtLarge, static_cast<double>(ik)) * pow(1.0 - PtLarge, static_cast<double>((k + i) - ik)));
				}
				else {
					PtkMultiDataSet[i].push_back((1.0 - tempVectorWithAttacksInfoByT[(size_t)maxt - 1]) *
												 (static_cast<double>(factorial(k + i)) / (factorial(ik) * factorial((k + i) - ik))) *
												 pow(PtLess, static_cast<double>(ik)) * pow(1.0 - PtLess, static_cast<double>((k + i) - ik)));
				}
			}
		}
		break;

	default:
		break;
	} 
}

void Model::generateAdjacencyMatrix() {
	// Обнуляем значения матрицы
	for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
		for (size_t j = 0; j < adjacencyMatrix[i].size(); j++) {
			adjacencyMatrix[i][j] = 0;
		}
	}

	std::vector<int> countEdgesForXi(15, 0);
	std::unordered_set<int> unUsedIndexesSet;

	// Генерация случайных связей
	for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
		// Выбираем случайное кол-во ребер из [1, 6]
		int currRowCountsEdges = distributionForEdgesCounts(gen);

		// Очищаем множество индексов и заполняем индексами вектора
		unUsedIndexesSet.clear();
		for (int i = 0; i < 15; i++) {
			unUsedIndexesSet.insert(i);
		}

		// Генерируем ребра количество раз, равное currRowCountsEdges
		for (int j = 1; j <= currRowCountsEdges; j++) {
			// Указатель на начало множества
			auto it = unUsedIndexesSet.begin();
			// Перемещаем итератор на число distrForIndx(gen) шагов, ограниченное значением unUsedIndexesSet.size()
			std::advance(it, distrForIndx(gen) % unUsedIndexesSet.size());

			// Разыменовывем итератор
			int randIndx = *it;

			// Добавляем сгенерированную связь, удаляя при этом индекс из множества
			adjacencyMatrix[i][randIndx] = 1;
			unUsedIndexesSet.erase(randIndx);
		}
	}
	// Сразу генерируем множество минимальных покрытий, если возможно
	generateAdjMatrixWithMinCoverage();
}

void Model::generateAdjMatrixWithMinCoverage() {
	minCoveredSetNodesXi.clear();				// Обновляем множество с минимальным покрытием (включающее наиболее оптимальные узлы Xi)
	std::unordered_set<int> coveredNodes;		// Непокрытые узлы Yi
	std::unordered_set<int> maxCoveredSubSet;	// Покрытые узлы Yi выбранным подмножеством


	// Пока количество покрытых узлов Yj не равно общему кол-ву узлов Yj
	while (coveredNodes.size() != adjacencyMatrix[0].size()) {
		// Очищаем подмножество покрытых Yj, в которое будут помещаться максимальные покрытия за прогон
		maxCoveredSubSet.clear();
		int currMaxIndxForXi = -1;	// Индекс Xi узла с максимальным покрытием оставшихся узлов (макс. подмножеством)

		// Для каждого "координатора" Xi, строки в матрице 
		for (size_t i = 0; i < adjacencyMatrix.size(); i++) {
			// Множество для покрытий для данного Xi
			std::unordered_set<int> coverSetForCurrRow;

			// Проверка наличия связей с узлами Yj
			for (size_t j = 0; j < adjacencyMatrix[i].size(); j++) {
				// Если есть связь с узлом Yj и данный узел еще не находится в множестве покрытых узлов, то добавляем его в подмножество
				if (adjacencyMatrix[i][j] > 0 && coveredNodes.find((int)j) == coveredNodes.end()) {
					coverSetForCurrRow.insert((int)j);
				}
			} 

			// Если за данную строку покрыли больше непокрытых узлов Yj чем ранее, то обновляем макс. подмножество и его индекс
			if (maxCoveredSubSet.size() < coverSetForCurrRow.size()) {
				maxCoveredSubSet = coverSetForCurrRow;
				currMaxIndxForXi = (int)i;
			}
		}

		// Если макс. подмножество пустое
		if (currMaxIndxForXi == -1) {
			// Завершаем цикл, покрытие составить не удалось
			break;
		}
		else {
			// Добавляем индекс Xi узла с макс. подмножеством
			minCoveredSetNodesXi.insert(currMaxIndxForXi);
			// Обновляем покрытые узлы
			coveredNodes.insert(maxCoveredSubSet.begin(), maxCoveredSubSet.end());
		}
	}

	adjMatrixWithMinCoverage.clear();
	// Заполняем матрицу смежности минимальных покрытий
	for (const auto indx : minCoveredSetNodesXi) {
		adjMatrixWithMinCoverage.push_back(adjacencyMatrix[indx]);
	}
}

unsigned long long Model::factorial(int n) {
	if (n == 0 || n == 1) {
		return 1;
	}
	
	unsigned long long result = 1;
	for (int i = 2; i <= n; i++) {
		result *= i;
	}
	return result;	
}