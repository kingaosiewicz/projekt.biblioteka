#include <iostream>
#include <string>
#include<ctime>
#include<fstream>
#include<stdio.h>
using namespace std;
#include"sqlite/sqlite3.h"

char wybor;
char wybor2;
char wybor3;
int uzytkownik = 0;
string login;
string haslo;
string login_rejestracja[50];
string haslo_rejestracja[50];

int main()
{
	string linia;
	fstream plik;

start:
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

		bool zalogowany = false;
		switch (wybor)
		{
		case'1':
			system("cls");
			cout << "Wybrales opcje 1" << endl;
			cout << "Podaj login: " << endl;
			cin >> login;
			cout << "Podaj haslo: " << endl;
			cin >> haslo;
			system("cls");

			if (login == "admin" && haslo == "admin")
			{
				cout << "Udane zalogowanie na konto aministatora " << endl;


				while (true)
				{
					cout << "1.Dodaj ksiazke" << endl;
					cout << "2.Usun ksiazke" << endl;
					cout << "3.Pokaz uzytkownikow" << endl;
					cout << "4.Usun uzytkownikow" << endl;
					cout << "5.Wyloguj" << endl;
					cin >> wybor2;
					switch (wybor2)
					{
					case'1':
					{
						system("cls");
						cout << "Wybrales opcje 1" << endl;
						getchar();
						getchar();
						break;
					}
					case'2':
					{
						system("cls");
						cout << "Wybrales opcje 2" << endl;
						getchar();
						getchar();
						break;
					}
					case'3':
					{
						system("cls");
						cout << "Wybrales opcje 3" << endl;
						getchar();
						getchar();
						break;
					}
					case'4':
					{
						system("cls");
						cout << "Wybrales opcje 4" << endl;
						getchar();
						getchar();
						break;
					}
					case'5':
					{
						system("cls");
						cout << "Wybrales opcje 5" << endl;
						getchar();
						getchar();
						goto start;
					}
					default:
					{
						cout << "Wybierz opcje z listy!" << endl;
						system("cls");
					}

					}

				}
			}
			
			for (int i = 0; i < uzytkownik; i++)
			{
				if (login == login_rejestracja[i] && haslo == haslo_rejestracja[i])
				{
					cout << "Udane zalogowanie na konto uzytkownika " << login_rejestracja[i] << endl;


					while (true)
					{
						cout << "1.Pokaz ksiazki" << endl;
						cout << "2.Wypozycz ksiazke" << endl;
						cout << "3.Oddaj ksiazki" << endl;
						cout << "4.Przegladaj moje ksiazki" << endl;
						cout << "5.Wyloguj" << endl;
						cin >> wybor3;
						switch (wybor3)
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
							cout << "Wybrales opcje 4" << endl;
							getchar();
							getchar();
							break;

						case'5':

							system("cls");
							cout << "Wybrales opcje 5" << endl;
							getchar();
							getchar();
							goto start;

						default:
							cout << "Wybierz opcje z listy!" << endl;
							system("cls");


						}
					}
				}
			}
			if (!zalogowany)
				{
					cout << "Nieudane logowanie!" << endl;
					break;
				}
				
				

		
			

				getchar();
				getchar();
				break;
		case'2':
			system("cls");
			cout << "Wybrales opcje 2" << endl;
			cout << "Podaj login:" << endl;
			cin >> login_rejestracja[uzytkownik];
			cout << "Podaj haslo:" << endl;
			cin >> haslo_rejestracja[uzytkownik];
			uzytkownik++;
			cout << "Gratulujemy udanego zalozenia karty bibliotecznej" << endl;
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
			plik.open("regulamin.txt", ios::in);
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

