#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>
#define INT unsigned long long int
#define DBL long double

using namespace std;

// Данный код содержит в себе несколько способов вычисления n-го числа Фибоначчи: степенной, линейный, логарифмический (с помощью матрицы, формулы Бине и пар для формулы Бине)

// Класс матриц 2х2
class Mat2x2 {
	public:
		INT _a11, _a12, _a21, _a22;
		Mat2x2() : _a11(0), _a12(0), _a21(0), _a22(0) {};
		void SetMat2x2(INT a11, INT a12, INT a21, INT a22) {
			_a11 = a11;
			_a12 = a12;
			_a21 = a21;
			_a22 = a22;
		}
};
// Умножение матриц
Mat2x2 operator* (const Mat2x2& A, const Mat2x2& B) {
	Mat2x2 prod;
	prod.SetMat2x2(A._a11 * B._a11 + A._a12 * B._a21, A._a11 * B._a21 + A._a12 * B._a22, A._a21 * B._a11 + A._a22 * B._a21, A._a21 * B._a21 + A._a22 * B._a22);
	return prod;
}
// Класс чисел { a + b * sqrt(5) : a,b - целые }
class Pair {
public:
	long long int _a, _b;
	Pair() : _a(0), _b(0) {};
	void SetPair(long long int a, long long int b) { _a = a; _b = b; }
};
// Умножение чисел { a + b * sqrt(5) : a,b - целые }
Pair operator* (const Pair& A, const Pair& B) {
	Pair prod;
	prod.SetPair(A._a * B._a + 5 * A._b * B._b, A._a * B._b + A._b * B._a);
	return prod;
}
// Шаблон возведения в степень (mult > 0, иначе код сильно усложнится)
template <typename T>
T Pow(T n, INT mult) {
	if (mult == 1) return n;
	if (mult % 2 == 0) return Pow(n * n, mult / 2);
	return n * Pow(n, mult - 1);
}
// Степенной: О(2^n), знает только значения нулевое и первое число, остальные получает через рекурсию
INT Fib_step(INT n) {
	if (n < 2) return n;
	return Fib_step(n - 1) + Fib_step(n - 2);
}
// Линейный: О(n), вычисляет числа последовательно по формуле f(n) = f(n-1) + f(n-2), зная изначально нулевое и первое число и перезаписывая меньшее
INT Fib_lin(INT n) {
	if (n == 0) return 0;
	INT a = 0, b = 1;
	for (INT i = 0; i < n - 1; i++) {
		INT c = b;
		b += a;
		a = c;
	}
	return b;
}
// Логарифмический: О(log2(n)), работает по формуле Бине, однако не очень точна в силу ошибки типа double, подходит для быстрой оценки
/*
	Работает это так: существует формула, математически точно описывающая n-ое число Фибоначчи: f(n) = (phi^n - dephi^n)/sqrt(5),
	где phi = (1+sqrt(5))/2 ~ 1.618, dephi = (1-sqrt(5))/2 ~ -0.618
*/
// Логарифмический: О(log2(n))
INT Fib_log(INT n) {
	if (n == 0) return 0;
	const long double sqph = long double(sqrt(5));
	const long double phi = (1 + sqph) / 2, dephi = (1 - sqph) / 2;
	return round((Pow(phi, n)-Pow(dephi, n))/sqph);
}
// Логарифмический точный: О(log2(n)), работает по формуле Бине, сделана модификация, благодаря чему работает в целых числах и не ошибается
// Однако из-за переполнения целого типа при n = 32 почти бесполезен на С++, хорошо работает на Python из-за отсутствия ограничения целого типа
/*
	Работает это так: множество чисел вида { a + b * sqrt(5) : a,b - целые } является полем, а значит, что можно избавиться 
	от иррациональности формулы с помощью разбиения чисел на две части - целую и иррациональную, причем целая исчезнет при
	вычитании, а иррациональная станет рациональной при делении на sqrt(5), который уже есть в формуле. 
	(a + b * sqrt(5)) * (c + d * sqrt(5)) = (a * c + 5 * b * d) + (b * c + a * d) * sqrt(5), тогда введем такую формулу умножения:
	(a,b) * (c,d) = (ab+5cd, bc+ad)
	Тогда возводим числа (1,1) и (1,-1) в нужную степень, смотрим разность иррациональных частей и делим на 2^n
	При решении окажется, что иррациональные части отличаются лишь знаком, тогда можно возводить лишь (1,1) и делить его на 2^(n-1)
*/
// Логарифмический парный: О(log2(n))
INT Fib_log_pair(INT n) {
	if (n == 0) return 0;
	if (n == 1) return 1;
	Pair a, b;
	a.SetPair(1, 1);
	//b.SetPair(1, -1);
	//return (Pow(a,n)._b - Pow(b,n)._b) / Pow(2, n);
	return Pow(a, n)._b / Pow(2, n-1);
}
// Логарифмический матричный: O(log2(n))
INT Fib_log_mat(INT n) {
	if (n == 0) return 0;
	if (n == 1) return 1;
	Mat2x2 m;
	m.SetMat2x2(1, 1, 1, 0);
	return Pow(m,n-1)._a11;
}
DBL TestTime(INT (*func)(INT), INT maxN, INT minN = 0) {
	INT t1, t2;
	t1 = clock();
	for (int i = minN; i <= maxN; i++) func(i);
	t2 = clock();
	return long double(t2 - t1) / CLOCKS_PER_SEC;
}
// Стандартный тест: проверяет время работы алгоритмов при вычислении всех чисел от 0 до maxN
void DefaultTest(INT maxN = 1e5, bool testStep = false) {
	cout << "Default test (maxN = " << maxN << ")\n";
	 // при maxN = 1e5 максимально наглядна скорость логарифмического алгоритма по сравнению с линейным
	cout << "F(93) = " << Fib_log_mat(93) << " - biggest correct number";
	cout << "\n";
	cout << "Fib_log_mat time: " << TestTime(Fib_log_mat, maxN) << " s\n";
	cout << "Fib_log time: " << TestTime(Fib_log, maxN) << " s\n";
	cout << "Fib_lin time: " << TestTime(Fib_lin, maxN) << " s\n";

	// Степенной алгоритм очень быстро наращивает время работы, поэтому не рекомендуется к тестам (уже при maxN = 50 тяжко)
	if(testStep) cout << "Fib_step time: " << TestTime(Fib_step, maxN) << " s" << "\n";
}
void LogTestTime(INT(*func)(INT), INT maxD, bool showLog = false) {
	INT t1, t2, c = 0;
	for (int i = 1; i < maxD; i++) {
		for (int j = 1; j < 10; j++) {
			t1 = clock();
			for (INT k = j * Pow(10, i); k < (j+1) * Pow(10, i); k++) func(k);
			t2 = clock();
			c += (t2 - t1);
			if(showLog) cout << j << "e" << i << ": " << DBL(c) / CLOCKS_PER_SEC << " s\n";
		}
	}
	cout << "Total time: " << DBL(c) / CLOCKS_PER_SEC << " s\n";
}
// Большой тест: смотрит изменение времени работы по логарифмической шкале до 10^maxD
void ComplexTest(INT maxD = 5) {
	cout << "Complex test (maxD = " << maxD << ")\n";;
	cout << "Complex Fib_log_mat time:\n";
	LogTestTime(Fib_log_mat, maxD, true);
	cout << "Complex Fib_log time:\n";
	LogTestTime(Fib_log, maxD, true);
	cout << "Complex Fib_lin time:\n";
	LogTestTime(Fib_lin, maxD, true);
}
// Выводит F(9), F(34) и F(50) для каждой функции
void AccuracyTest()
{	
	cout << "Accuracy test\n";
	//cout << "Fib_log_pair:\n" << "F(9) = " << Fib_log_pair(9) << "\n" << "F(34) = " << Fib_log_pair(34) << "\n" << "F(50) = " << Fib_log_pair(50) << "\n";
	cout << "Fib_log_mat:\n" << "F(9) = " << Fib_log_mat(9) << "\n" << "F(34) = " << Fib_log_mat(34) << "\n" << "F(50) = " << Fib_log_mat(50) << "\n";
	cout << "Fib_log:\n" << "F(9) = " << Fib_log(9) << "\n" << "F(34) = " << Fib_log(34) << "\n" << "F(50) = " << Fib_log(50) << "\n";
	cout << "Fib_lin:\n" << "F(9) = " << Fib_lin(9) << "\n" << "F(34) = " << Fib_lin(34) << "\n" << "F(50) = " << Fib_lin(50) << "\n";
}
// Пространство для тестов
int main() {
	/* На самом деле числа Фибоначчи после n = 93 перестают помещаться в формат INT, так что тесты скорости не очень корректны,
	   ведь работают не с самими числами, а с остатком от деления на UINTMAX_MAX*/
	//for (int i = 0; i < 40; i++) cout << i << " " << Fib_log_pair(i) << endl; // Fib_log_pair быстро переполняется
	AccuracyTest();
	DefaultTest();
	ComplexTest();
}
