#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;
using namespace std;

class Dane {
public:
	int ilezadan;
	int ilemaszyn;
	vector<vector<int>> zadania; //zapisane w postaci [ID][t na m1][t na m2]...[t na m]
	Dane(int n, int m) {
		ilezadan = n;
		ilemaszyn = m ; 
		zadania.resize(ilezadan, vector<int>(ilemaszyn+1, 0));
	}
};

Dane wczytajDanezpliku(const char nazwa[], int& iz, int& im)
{
	int nrmaszyny;
	ifstream myfile(nazwa);
	if (myfile.is_open())
	{
		myfile >> iz;
		myfile >> im;
	}
	Dane _dane(iz, im);
	if (myfile.is_open())
	{
		for (int i = 0;i < _dane.ilezadan;i++)
		{
			_dane.zadania[i][_dane.ilemaszyn] = i;
			for (int j = 0; j < _dane.ilemaszyn;j++)
			{
				myfile >> nrmaszyny;
				myfile >> _dane.zadania[i][j];
			}
		}
		myfile.close();
	}
	return _dane;
}
int obliczCmax(vector<int> permutacja, Dane& dane)
{
	vector<vector<int>> starty(dane.ilezadan, vector<int>(dane.ilemaszyn, 0));
	vector<vector<int>> zakonczenia(dane.ilezadan, vector<int>(dane.ilemaszyn, 0));
	starty[permutacja[0]][0] = 0;
	zakonczenia[permutacja[0]][0] = dane.zadania[permutacja[0]][0];
		for (int i = 1;i < dane.ilemaszyn;i++)
		{
			starty[permutacja[0]][i] = starty[permutacja[0]][i - 1] + dane.zadania[permutacja[0]][i - 1];
			zakonczenia[permutacja[0]][i] = starty[permutacja[0]][i] + dane.zadania[permutacja[0]][i];
		}
	for (int j = 1;j <dane.ilezadan;j++)
	{
		zakonczenia[permutacja[j]][0] = zakonczenia[permutacja[j - 1]][0] + dane.zadania[permutacja[j]][0];
		for (int i = 1;i < dane.ilemaszyn;i++)
		{
			starty[permutacja[j]][i] = max(zakonczenia[permutacja[j]][i - 1], zakonczenia[permutacja[j-1]][i]);
			zakonczenia[permutacja[j]][i] = starty[permutacja[j]][i] + dane.zadania[permutacja[j]][i];
		}
	}
	return zakonczenia[permutacja[dane.ilezadan-1]][dane.ilemaszyn - 1];
}

void generujPermutacjeRekursywnie(int k, vector<int>& zadania, Dane &dane, int& minKara, vector<int>& optymalnaPermutacja)
{
	if (k == 1)
	{
		int CmaxDlaPermutacji = obliczCmax(zadania, dane);
		if (CmaxDlaPermutacji < minKara)
		{
			minKara = CmaxDlaPermutacji;
			optymalnaPermutacja = zadania;
		}
	}
	else
	{
		generujPermutacjeRekursywnie(k - 1, zadania, dane, minKara, optymalnaPermutacja);
		for (int i = 0; i < k - 1;i++)
		{
			if (k % 2 == 0)
			{
				swap(zadania[i], zadania[k - 1]);
			}
			else
			{
				swap(zadania[0], zadania[k - 1]);
			}
			generujPermutacjeRekursywnie(k - 1, zadania, dane, minKara, optymalnaPermutacja);
		}
	}
}

vector<int> przegladZupelnyRekursywnie(Dane &dane)
{
	vector<int> permutacja;
	for (int i = 0; i < dane.ilezadan;i++)
	{
		permutacja.push_back(i);
	}
	int minKara = obliczCmax(permutacja, dane);
	vector<int> idealnapermutacja = permutacja;

	generujPermutacjeRekursywnie(permutacja.size(), permutacja, dane, minKara, idealnapermutacja);
	return idealnapermutacja;
}

void generujPermutacjeIteracyjnie(vector<int> zadania, Dane &dane, int& minKara, vector<int>& optymalnaPermutacja)
{
	int n = zadania.size();
	vector<int> c(n, 0);
	//sprawdzenie kary poczatkowej permutacji//
	int karaDlaPermutacji = obliczCmax(zadania, dane);
	if (karaDlaPermutacji < minKara)
	{
		minKara = karaDlaPermutacji;
		optymalnaPermutacja = zadania;
	}
	//										//
	int i = 0;
	while (i < n)
	{
		if (c[i] < i)
		{
			if (i % 2 == 0)
			{
				swap(zadania[0], zadania[i]);
			}
			else
			{
				swap(zadania[c[i]], zadania[i]);
			}
			//sprawdzenie kary obecnej permutacji//
			karaDlaPermutacji = obliczCmax(zadania, dane);
			if (karaDlaPermutacji < minKara)
			{
				minKara = karaDlaPermutacji;
				optymalnaPermutacja = zadania;
			}
			//									//
			c[i]++;
			i = 0;
		}
		else
		{
			c[i] = 0;
			i++;
		}
	}
}

vector<int> przegladZupelnyIteracyjnie(Dane dane)
{
	vector<int> permutacja;
	for (int i = 0; i < dane.ilezadan;i++)
	{
		permutacja.push_back(i);
	}
	int minKara = obliczCmax(permutacja, dane);
	vector<int> idealnapermutacja = permutacja;

	generujPermutacjeIteracyjnie(permutacja, dane, minKara, idealnapermutacja);
	return idealnapermutacja;
}


int main()
{
	int ilezadan;
	int ilemaszyn;
	//Wczytywanie danych
	char nazwapliku[100];
	cout << "Podaj nazwe pliku" << endl;
	cin >> nazwapliku;
	Dane dane = wczytajDanezpliku(nazwapliku, ilezadan, ilemaszyn);
	//Permutacja naturalna
	vector<int> permutacja;
	for (int i = 0; i < dane.ilezadan;i++)
	{
		permutacja.push_back(i);
	}
	cout << "Permutacja naturalna: " << obliczCmax(permutacja, dane)<<endl;
	cout << "Przeglad zupelny rekursywny (optymalne)" << obliczCmax(przegladZupelnyRekursywnie(dane), dane) << endl;
	cout << "Przeglad zupelny iteracyjny (optymalne)" << obliczCmax(przegladZupelnyIteracyjnie(dane), dane) << endl;



	
}