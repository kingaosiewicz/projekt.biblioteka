#include <iostream>
#include <string>
#include<time.h>
#include<fstream>
#include<stdio.h>
#include<cstdlib>
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

class Book
{
private:
    string tytul;
    string autor;
    int rok_wydania;
    string gatunek;
    bool czy_wypozyczona;
    int przez_kogo_wyp;
    sqlite3* db;

    void createTable() {
        string sql = "CREATE TABLE IF NOT EXISTS BOOKS("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
            "TYTUL TEXT NOT NULL, "
            "AUTOR TEXT NOT NULL, "
            "ROK_WYDANIA INTEGER NOT NULL, "
            "GATUNEK TEXT NOT NULL, "
            "CZY_WYPOZYCZONA BOOLEAN NOT NULL DEFAULT 0, "
            "PRZEZ_KOGO_WYP INT NOT NULL DEFAULT 0);";
        char* messageError;
        int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "Blad tworzenia tablicy: " << messageError << endl;
            sqlite3_free(messageError);
        }
        else {
            cout << "Tablica utworzona pomyslnie" << endl;
        }
    }

public:
    Book(sqlite3* db_connection) : db(db_connection) {
        createTable();
    }

    void dodajKsiazke() {
        cout << "Podaj tytul: ";
        cin.ignore();
        getline(cin, tytul);
        cout << "Podaj autora: ";
        getline(cin, autor);
        cout << "Podaj rok wydania: ";
        cin >> rok_wydania;
        cout << "Podaj gatunek: ";
        cin.ignore();
        getline(cin, gatunek);


        string sql = "INSERT INTO BOOKS (TYTUL, AUTOR, ROK_WYDANIA, GATUNEK) VALUES('" + tytul + "', '" + autor + "', " + to_string(rok_wydania) + ", '" + gatunek + "'); ";
        char* messageError;
        int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "Blad: " << messageError << endl;
            sqlite3_free(messageError);
        }
        else {
            cout << "Ksiazka dodana pomyslnie!" << endl;
        }
    }

    bool ksiazkaIstnieje(const string& tytul) {
        string sql = "SELECT * FROM BOOKS WHERE TYTUL = '" + tytul + "';";
        sqlite3_stmt* stmt;
        int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            cerr << "Blad: " << sqlite3_errmsg(db) << endl;
            return false;
        }
        bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        return exists;
    }

    void usunKsiazke() {
        string tytul;
        cout << "Podaj tytul ksiazki ktora chcesz usunac: ";
        cin.ignore();
        getline(cin, tytul);

        if (!ksiazkaIstnieje(tytul)) {
            cout << "Ta ksiazka nie istnieje!" << endl;
            return;
        }

        string sql = "DELETE FROM BOOKS WHERE TYTUL = '" + tytul + "';";
        char* messageError;
        int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "Blad: " << messageError << endl;
            sqlite3_free(messageError);
        }
        else {
            cout << "Ksiazka usunieta pomyslnie!" << endl;
        }
    }

    void wypozyczKsiazke(const string& tytul_ksiazki, int id_uzytkownika) {
        tytul = tytul_ksiazki;

        if (!ksiazkaIstnieje(tytul)) {
            cout << "Ksiazka o tytule " << tytul << " nie istnieje!" << endl;
            return;
        }

        string check_sql = "SELECT CZY_WYPOZYCZONA, PRZEZ_KOGO_WYP FROM BOOKS WHERE TYTUL = '" + tytul + "';";
        sqlite3_stmt* stmt;
        int exit = sqlite3_prepare_v2(db, check_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            cerr << "Blad: " << sqlite3_errmsg(db) << endl;
            return;
        }

        czy_wypozyczona = false;
        przez_kogo_wyp = 0;

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            czy_wypozyczona = sqlite3_column_int(stmt, 0);
            przez_kogo_wyp = sqlite3_column_int(stmt, 1);
        }
        sqlite3_finalize(stmt);

        if (czy_wypozyczona)
        {
            cout << "Ksiazka jest juz wypozyczona przez innego uzytkownika " << endl;
            return;
        }

        check_sql = "SELECT ILOSC_WYP_KSIAZEK FROM USERS WHERE ID = " + to_string(id_uzytkownika) + ";";
        exit = sqlite3_prepare_v2(db, check_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            cerr << "Blad: " << sqlite3_errmsg(db) << endl;
            return;
        }

        int ilosc_wyp_ksiazek = 0;

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            ilosc_wyp_ksiazek = sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);

        if (ilosc_wyp_ksiazek >= 3) {
            cout << "Nie mozesz wypozyczyc wiecej niz 3 ksiazki." << endl;
            return;
        }

        string sql = "UPDATE BOOKS SET CZY_WYPOZYCZONA = 1, PRZEZ_KOGO_WYP = " + to_string(id_uzytkownika) + " WHERE TYTUL = '" + tytul + "';";
        char* messageError;
        exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK)
        {
            cerr << "Blad: " << messageError << endl;
            sqlite3_free(messageError);
        }
        else
        {
            sql = "UPDATE USERS SET ILOSC_WYP_KSIAZEK = ILOSC_WYP_KSIAZEK + 1 WHERE ID = " + to_string(id_uzytkownika) + ";";
            exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
            if (exit != SQLITE_OK) {
                cerr << "Blad: " << messageError << endl;
                sqlite3_free(messageError);
            }
            else {
                cout << "Ksiazka o tytule " << tytul << " zostala pomyslnie wypozyczona przez uzytkownika " << id_uzytkownika << "!" << endl;
            }
        }

    }

    void oddajKsiazke(const string& tytul_ksiazki, int id_uzytkownika) {
        tytul = tytul_ksiazki;

        string check_sql = "SELECT CZY_WYPOZYCZONA, PRZEZ_KOGO_WYP FROM BOOKS WHERE TYTUL = '" + tytul + "';";
        sqlite3_stmt* stmt;
        int exit = sqlite3_prepare_v2(db, check_sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            cerr << "Blad: " << sqlite3_errmsg(db) << endl;
            return;
        }

        int czy_wypozyczona = false;
        int przez_kogo_wyp = 0;
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            czy_wypozyczona = sqlite3_column_int(stmt, 0);
            przez_kogo_wyp = sqlite3_column_int(stmt, 1);
        }
        sqlite3_finalize(stmt);

        if (!czy_wypozyczona)
        {
            cout << "Ksiazka o tytule " << tytul << " nie jest wypozyczona." << endl;
            return;
        }

        if (przez_kogo_wyp != id_uzytkownika)
        {
            cout << "Ksiazka o tytule " << tytul << " nie zostala wypozyczona przez ciebie." << endl;
            return;
        }

        string sql = "UPDATE BOOKS SET CZY_WYPOZYCZONA = 0, PRZEZ_KOGO_WYP = 0 WHERE TYTUL = '" + tytul + "';";
        char* messageError;
        exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK)
        {
            cerr << "Blad: " << messageError << endl;
            sqlite3_free(messageError);
        }
        else
        {
            sql = "UPDATE USERS SET ILOSC_WYP_KSIAZEK = ILOSC_WYP_KSIAZEK - 1 WHERE ID = " + to_string(id_uzytkownika) + ";";
            exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
            if (exit != SQLITE_OK) {
                cerr << "Blad: " << messageError << endl;
                sqlite3_free(messageError);
            }
            else {
                cout << "Ksiazka o tytule " << tytul << " zostala pomyslnie oddana przez uzytkownika " << id_uzytkownika << "!" << endl;
            }
        }
    }

    void wypiszKsiazki() {
        string sql = "SELECT * FROM BOOKS;";
        sqlite3_stmt* stmt;

        int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            cerr << "Blad: " << sqlite3_errmsg(db) << endl;
            return;
        }

        cout << "ID | TYTUL | AUTOR | ROK_WYDANIA | GATUNEK | CZY_WYPOZYCZONA | PRZEZ_KOGO_WYP" << endl;
        cout << "---------------------------------------------------------------" << endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char* tytul = sqlite3_column_text(stmt, 1);
            const unsigned char* autor = sqlite3_column_text(stmt, 2);
            int rok_wydania = sqlite3_column_int(stmt, 3);
            const unsigned char* gatunek = sqlite3_column_text(stmt, 4);
            bool czy_wypozyczona = sqlite3_column_int(stmt, 5);
            int przez_kogo_wyp = sqlite3_column_int(stmt, 6);


            cout << id << " | " << tytul << " | " << autor << " | " << rok_wydania << " | " << gatunek << " | " << (czy_wypozyczona ? "Tak" : "Nie") << " | " << przez_kogo_wyp << endl;
        }

        sqlite3_finalize(stmt);
    }

    void mojeKsiazki(int id_uzytkownika) {
        string sql = "SELECT TYTUL, AUTOR, ROK_WYDANIA, GATUNEK FROM BOOKS WHERE PRZEZ_KOGO_WYP = " + to_string(id_uzytkownika) + ";";
        sqlite3_stmt* stmt;
        int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

        if (exit != SQLITE_OK) {
            cerr << "Blad: " << sqlite3_errmsg(db) << endl;
            return;
        }

        cout << "Ksiazki wypozyczone przez uzytkownika " << id_uzytkownika << ":" << endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string tytul = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            string autor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int rok = sqlite3_column_int(stmt, 2);
            string gatunek = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

            cout << "Tytul: " << tytul << ", Autor: " << autor << ", Rok: " << rok << ", Gatunek: " << gatunek << endl;
        }

        sqlite3_finalize(stmt);
    }

    void sortujGatunek() {
        string gatunek;
        cout << "Podaj gatunek: ";
        cin.ignore(); // Ignoruje poprzedni znak nowej linii, jeœli istnieje
        getline(cin, gatunek);

        string sql = "SELECT TYTUL, AUTOR, ROK, GATUNEK FROM BOOKS WHERE GATUNEK = '" + gatunek + "';";
        sqlite3_stmt* stmt;
        int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

        if (exit != SQLITE_OK) {
            cerr << "Blad: " << sqlite3_errmsg(db) << endl;
            return;
        }

        cout << "Ksiazki w gatunku " << gatunek << ":" << endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string tytul = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            string autor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int rok = sqlite3_column_int(stmt, 2);
            string gatunek = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

            cout << "Tytul: " << tytul << ", Autor: " << autor << ", Rok: " << rok << ", Gatunek: " << gatunek << endl;
        }

        sqlite3_finalize(stmt);
    }

    void sortujAutor() {
        string autor;
        cout << "Podaj autora: ";
        cin.ignore(); // Ignoruje poprzedni znak nowej linii, jeœli istnieje
        getline(cin, autor);

        string sql = "SELECT TYTUL, AUTOR, ROK, GATUNEK FROM BOOKS WHERE AUTOR = '" + autor + "';";
        sqlite3_stmt* stmt;
        int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

        if (exit != SQLITE_OK) {
            cerr << "Blad: " << sqlite3_errmsg(db) << endl;
            return;
        }

        cout << "Ksiazki autora " << autor << ":" << endl;

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string tytul = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            string autor = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            int rok = sqlite3_column_int(stmt, 2);
            string gatunek = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

            cout << "Tytul: " << tytul << ", Autor: " << autor << ", Rok: " << rok << ", Gatunek: " << gatunek << endl;
        }

        sqlite3_finalize(stmt);
    }



};

class User {
private:
    string imie;
    string nazwisko;
    int wiek;
    string user_haslo;
    int id_uzytkownika;
    int ilosc_wyp_ksiazek;
    sqlite3* db;

    void createTable() {
        string sql = "CREATE TABLE IF NOT EXISTS USERS("
            "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
            "IMIE TEXT NOT NULL, "
            "NAZWISKO TEXT NOT NULL, "
            "WIEK INT NOT NULL, "
            "HASLO TEXT NOT NULL, "
            "ILOSC_WYP_KSIAZEK INT NOT NULL DEFAULT 0);";
        char* messageError;
        int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "blad tworzenia tabeli: " << messageError << endl;
            sqlite3_free(messageError);
        }
        else {
            cout << "Tablica uzytkownikow utworzona pomyslnie" << endl;
        }
    }

public:
    User(sqlite3* db_connection) : db(db_connection) {
        createTable();
    }

    void dodajUzytkownika() {
        cout << "Podaj imie: ";
        cin >> imie;
        cout << "Podaj nazwisko: ";
        cin >> nazwisko;
        cout << "Podaj wiek: ";
        cin >> wiek;
        cout << "Wymysl haslo: ";
        cin >> user_haslo;

        string sql = "INSERT INTO USERS (IMIE, NAZWISKO, WIEK, HASLO, ILOSC_WYP_KSIAZEK) VALUES('" + imie + "', '" + nazwisko + "', " + to_string(wiek) + ", '" + user_haslo + "', 0);";

        char* messageError;
        int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "Blad dodawania uzytkownika: " << messageError << endl;
            sqlite3_free(messageError);
            return;
        }

        // Pobieramy automatycznie przyznane user_id
        id_uzytkownika = sqlite3_last_insert_rowid(db);

        cout << "Uzytkownik zostal dodany z ID: " << id_uzytkownika << endl;
        cout << "Od teraz mozesz logowac sie do naszego systemu." << endl;
    }

    bool uzytkownikIstnieje(const string& id_uzytkownika) {
        string sql = "SELECT * FROM USERS WHERE ID = '" + id_uzytkownika + "';";
        sqlite3_stmt* stmt;
        int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            cerr << "Error SELECT: " << sqlite3_errmsg(db) << endl;
            return false;
        }
        bool exists = (sqlite3_step(stmt) == SQLITE_ROW);
        sqlite3_finalize(stmt);
        return exists;
    }

    void usunUzytkownika() {
        string id_uzytkownika;
        cout << "Podaj id uzytkownika, ktorego chcesz usunac: ";
        cin >> id_uzytkownika;

        if (!uzytkownikIstnieje(id_uzytkownika)) {
            cout << "Ten uzytkownik nie istnieje!" << endl;
            return;
        }

        string sql = "DELETE FROM USERS WHERE ID = '" + id_uzytkownika + "';";
        char* messageError;
        int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &messageError);
        if (exit != SQLITE_OK) {
            cerr << "Blad: " << messageError << endl;
            sqlite3_free(messageError);
        }
        else {
            cout << "Uzytkownik usuniety pomyslnie!" << endl;
        }
    }

    void wyswietlUzytkownikow() {
        string sql = "SELECT * FROM USERS;";
        sqlite3_stmt* stmt;
        int exit = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
        if (exit != SQLITE_OK) {
            cerr << "Blad: " << sqlite3_errmsg(db) << endl;
            return;
        }

        cout << "Uzytkownicy:" << endl;
        cout << "------------------------------------------" << endl;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char* imie = sqlite3_column_text(stmt, 1);
            const unsigned char* nazwisko = sqlite3_column_text(stmt, 2);
            int wiek = sqlite3_column_int(stmt, 3);

            /*  string imie_str(reinterpret_cast<const char*>(imie));
              string nazwisko_str(reinterpret_cast<const char*>(nazwisko));*/

            cout << ", Imie: " << imie << ", Nazwisko: " << nazwisko << ", Wiek: " << wiek << " ID: " << id << endl;
        }
        cout << "------------------------------------------" << endl;

        sqlite3_finalize(stmt);
    }
};

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

        switch (wybor)
		{
		case'1':
			system("cls");
			cout << "Wybrales opcje 1" << endl;
			cout << "Podaj ID: " << endl;
			cin >> login;
			cout << "Podaj haslo: " << endl;
			cin >> haslo;
			system("cls");

			if (login == "0" && haslo == "admin")
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
                        void dodajKsiazke();
						getchar();
						getchar();
						break;
					}
					case'2':
					{
						system("cls");
						cout << "Wybrales opcje 2" << endl;
                        void usunKsiazke();
						getchar();
						getchar();
						break;
					}
					case'3':
					{
						system("cls");
						cout << "Wybrales opcje 3" << endl;
                        void wyswietlUzytkownikow();
						getchar();
						getchar();
						break;
					}
					case'4':
					{
						system("cls");
						cout << "Wybrales opcje 4" << endl;
                        void usunUzytkownika();
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
			
				 if (login == User::id_uzytkownika && haslo == User::user_haslo)
				{
                     
					cout << "Udane zalogowanie na konto uzytkownika " << User::imie << " " << User::nazwisko << endl; //trzeba to jakos polaczyc z ta metoda


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
                            void wypozyczKsiazke();
							getchar();
							getchar();
							break;

						case'3':

							system("cls");
							cout << "Wybrales opcje 3" << endl;
                            void oddajKsiazke(const string & tytul_ksiazki, int id_uzytkownika);
							getchar();
							getchar();
							break;

						case'4':

							system("cls");
							cout << "Wybrales opcje 4" << endl;
                            void mojeKsiazki();
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
            void dodajUzytkownika();
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

