#include <iostream>
#include <stdio.h>
#include <time.h>
#define INT unsigned long long int
using namespace std;

// Вавилонский алгоритм поиска квадратного корня
INT VavilSqrt(INT n) {
	//cout << "0\n";
	if (n == 1) return 1;
	long double x = n/2, prvx = 0;
	while (x != prvx) {
		prvx = x;
		x = (prvx + n / prvx) / 2;
	}
	return x;
}
// Разбиения на суммы квадратов: O(n), возвращает пары (x, y) такие, что x^2 + y^2 = n, x >= y >= 0;
int Pairs(INT n) {
	if (n % 4 == 3) return 0; // точно нет разбиений
	// число x максимально при y = 0, когда n - полный квадрат
	// число х минимально при x = y, когда n - удвоенный полный квадрат
	// между минимальным и максимальным х: увеличиваем х, если сумма квадратов меньше n, иначе уменьшаем y, притом ни одно разбиение не будет пропущено
	INT lim = VavilSqrt(n), x = VavilSqrt(n/2), y = x; 
	while (x <= lim && y > 0) {
		INT p = x * x + y * y;
		if (p == n) cout << x << " " << y << endl;
		if (p < n) x++;
		else y--;
	}
	if(lim * lim == n) cout << lim << " " << 0 << endl;
	return 1;
}

int main() {
	INT a = 41*41, b = 25*25, c=74*74;
	INT t1, t2;
	t1 = clock();
	Pairs(a*b*c);
	t2 = clock();
	cout << "Total time: " << long double(t2 - t1) / 1000 << " s" << endl;
}
