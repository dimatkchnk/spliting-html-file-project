#include <iostream>
#include <string>
#include <fstream>
#include <clocale>
#include <sstream>
#include <vector>
using namespace std;
const int MAX_TAGI = 5; //Definiujemy stala ktora okresla co ile znacznikow chcemy utworzyc nowy plik
//Funkcja ktora generuje nazwe pliku, bazujac na indexie(numerze dokumentu)
string zwrocNazwe(int index)
{
    //Typ stringstream pozwala na tworzenie zlozonych ciagow znakow ze zmiennych string oraz int co pozwala na stworzenie odpowiednich nazw plikow
    stringstream nazwa;
    nazwa.str("");
    nazwa.clear();
    nazwa << "krotki" << index << ".html";
    string nz = nazwa.str();
    return nz;
}
void odczytaj_z_pliku(string nazwaPliku)
{
    //Wektor stringow przechowujaca poczatek glownego dokumentu (od <html> do <body> wraz z headem w srodku)
    vector<string> POCZATEK; //Nie wiedzac jak dluga jest sekcja HEAD ciezko poslugiwac sie tablica
    int sumaTagow = 0;
    string linia;
    //Zdefiniowane 3 zamykajace znaczniki - dzieki nim mozemy sprawdzic kiedy konczy sie dana sekcja
    string tagUL = "/ul";
    string tagP = "/p";
    string tagTABLE = "/table";
    fstream plik;
    plik.open(nazwaPliku.c_str(), ios::in);
    //Petla ktora liczy ile jest w sumie znacznikow, co pomoze w dzieleniu pliku na wybrana ilosc czesci
    while (!plik.eof())
    {
        getline(plik, linia);
        if (linia.find("<html>") != string::npos) // jesli odnaleziono
        {
            //Zapisujemy do wektora stringow calego HEAD'A w celu poprawnego tworzenia nowych plikow
            POCZATEK.push_back(linia);
            do
            {
                getline(plik, linia);
                POCZATEK.push_back(linia);
            } while (linia.find("<body>") == string::npos); // dopoki nie odnaleziono 
        }
        //Jesli w danej linijce znalazl sie znacznik zamykajacy to dodaje sie 1 do sumy znacznikow aby pozniej moc poprawnie podzielic
        if (linia.find(tagUL) != string::npos || linia.find(tagP) != string::npos || linia.find(tagTABLE) != string::npos)
        {
            sumaTagow++;
        }
    }
    plik.close();
    //Sprawdzamy czy jest sens dzielic plik, czy plik jest odpowiednio dlugi
    if (sumaTagow <= MAX_TAGI)
    {
        cout << "Plik jest zbyt krotki aby go podzielic!" << endl;
    }
    else
    {
        fstream zapis;
        //Zmienna tagi liczy w sumie wszystkie napotkane zamykajace znaczniki
        int licznik = 0, tagi = 0;
        int index = 1;
        plik.open(nazwaPliku.c_str(), ios::in);
        zapis.open(zwrocNazwe(index).c_str(), ios::out);
        while (!plik.eof())
        {
            getline(plik, linia);
            zapis << linia << endl;
            if (linia.find(tagUL) != string::npos || linia.find(tagP) != string::npos || linia.find(tagTABLE) != string::npos)
            {
                licznik++;
                tagi++;
            }
            //Sprawdzamy czy doszlismy do ostatniego zamykajacego znacznika przed </body>, jesli tak to mozemy zamknac ostatni plik i wyjsc z petli
            if (tagi == sumaTagow)
            {
                zapis << "<a href = " << zwrocNazwe(index - 1) << ">Poprzedni</a>" << endl;
                zapis << "</body>" << endl;
                zapis << "</html>" << endl;
                zapis.close();
                break;
            }
            else
            {
                if (licznik >= MAX_TAGI)
                {
                    if (index == 1)
                    {
                        zapis << "<a href = " << zwrocNazwe(index + 1) << ">Nastepny</a>" << endl;
                    }
                    else
                    {
                        zapis << "<a href = " << zwrocNazwe(index - 1) << ">Poprzedni</a>" << endl;
                        zapis << "<a href = " << zwrocNazwe(index + 1) << ">Nastepny</a>" << endl;
                    }
                    zapis << "</body>" << endl;
                    zapis << "</html>" << endl;
                    zapis.close();
                    index++;
                    zapis.open(zwrocNazwe(index).c_str(), ios::out);
                    for (int i = 0; i < POCZATEK.size(); i++)
                        zapis << POCZATEK[i] << endl;
                    licznik = 0;
                }
            }
        }
        cout << "Pomyslnie podzielono plik na mniejsze czesci!" << endl;
        plik.close();
    }
}
int main()
{
    setlocale(LC_CTYPE, "Polish");
    //Zmieniajac nazwe stringa mozna podzielic pare roznych plikow html (np. dlugi.html oraz dlugi2.html)
    string nazwaPliku = "dlugi.html";
    odczytaj_z_pliku(nazwaPliku);
    return 0;
}
