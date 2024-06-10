#include <iostream>
#include <string>
#include<ctime>
#include<fstream>
using namespace std;

char wybor;

int main()
{
	string linia;
	fstream plik;
	while (true)
	{
		cout <<
			R"( ___     __        __    ___     __
|   |___|  |__    |  |__|   |  _|  |
|   |   |  |  |___|  |  |   |_| |  |
|   |   |  |  |   |  |  |   | | |  |
|   |   |  |  |   |  |  |   | | |  |
|   |   |  |  |   |  |  |   | | |  |
------------------------------------)";
		cout << endl;
		cout << "             BIBLIOTEKA" << "\n";

		cout << "\n";
		cout << "Witamy w Bibliotece!" << endl;
		cout << "---------------------" << endl;
		cout << "1.Zaloguj" << endl;
		cout << "2.Utworz konto" << endl;
		cout << "3.Przegladaj jako gosc" << endl;
		cout << "4.Regulamin biblioteki" << endl;
		cout << "5.Wyjdz" << endl;
		cin >> wybor;

		switch (wybor)
		{
		case'1':
			system("cls");
			cout << "Wybrales opcje 1" << endl;
			getchar();
			getchar();
			break;
		case'2':
			system("cls");
			cout << "Wybrales opcje 2" << endl;
			getchar();
			getchar();
			break;
		case'3':
			system("cls");
			cout << "Wybrales opcje 3" << endl;
			getchar();
			getchar();
			break;
		case'4':
			system("cls");
			plik.open("C:/Users/conta/Desktop/regulamin.txt", ios::in);
			if (plik.good())
			{
				while (!plik.eof())
				{
					getline(plik, linia);
					cout << linia << endl;
				}
				plik.close();
			}
			else
			{
				cout << "Nie mozna otworzyc pliku." << endl;
			}

			getchar();
			getchar();
			break;
		case'5':
			exit(0);
			break;
		default: cout << "Wybierz opcje z listy!" << endl;
		}
		//getchar();
		system("cls");

	}

	return 0;
}