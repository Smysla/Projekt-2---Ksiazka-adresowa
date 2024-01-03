/*
    7. Książka adresowa. Wprowadzanie i usuwanie pozycji z książki. Odczyt i zapis książki do pliku (najlepiej w formacie binarnym). Wyświetlanie danych z danej pozycji książki.
    Zofia Fidura
    313544
*/
#include <iostream>
#include <locale> //do polskich znaków w terminalu
#include <map>
#include <variant>
#include <algorithm>
#include <string>
#include <fstream> 
#include <memory>

class Wpis
{
    public:
    Wpis() : nrTel("brak") {nPoz++;}        //konstruktor domyślny
    Wpis(std::string tel) : nrTel(tel) {nPoz++;}
    virtual ~Wpis() {nPoz--;}
    virtual const void print() = 0;
    std::string getNr() {return nrTel;}     //zwraca numer telfonu
    void setNr(std::string nr) {nrTel=nr;}  //ustawia numer telefonu
    protected:
    std::string nrTel;                      //numer telefonu
    static int nPoz;                        //liczba pozycji zapisanych w książce
};
int Wpis::nPoz = 0; //początkowo pozycji jest 0

//klasa reprezentująca wpisy osobowe
class Czlowiek: public Wpis
{
    public:
    Czlowiek() : imie("brak"), Wpis(), urodziny("brak") {nCzl++;}                                               //std::map nie działał z std::get bez defaultowego konstruktora
    Czlowiek(std::string im) : imie(im), Wpis(), urodziny("brak") {nCzl++;}                                     //konstruktor używany przy tworzeniu nowych wpisów
    Czlowiek(std::string im, std::string nrTel, std::string ur) : imie(im), Wpis(nrTel), urodziny(ur) {nCzl++;}
    Czlowiek(const Czlowiek& czlek) : imie(czlek.imie), Wpis(czlek.nrTel), urodziny(czlek.urodziny) {nCzl++;}   //konstruktor kopiujący
    virtual ~Czlowiek() {nCzl--;}
    std::string getImie() {return imie;}                //zwraca imie
    std::string getUrodziny() {return urodziny;}        //zwraca datę urodzin
    void setImie(std::string im) {imie = im;}           //pozwala zmienić imię
    void setUrodziny(std::string dat) {urodziny = dat;} //pozwala zmienić datę urodzin
    const void print() override {std::cout << "Imię i nazwisko: " << imie << "\tnumer telefonu: " << nrTel << "\turodziny: " << urodziny << "\n";}   //wyświetla iformacje o wpisie
    static int getNCzl() {return nCzl;}                 //zwraca liczbę zapisanych osób
    private:
    std::string imie;
    std::string urodziny;
    static int nCzl;    //liczba zapisanych osób
};
int Czlowiek::nCzl = 0;

//klasa reprezentująca pozostałe wpisy
class Firma: public Wpis
{
    public:
    Firma() : nazwa("brak"), adres("brak"), Wpis() {nFir++;}                                        //std::map nie działał z std::get bez defaultowego konstruktora
    Firma(std::string naz) : nazwa(naz), adres("brak"), Wpis() {nFir++;}                            //konstruktor używany przy tworzeniu nowych wpisów
    Firma(std::string naz, std::string nrTel, std::string adr) : nazwa(naz), Wpis(nrTel), adres(adr) {nFir++;}
    Firma(const Firma& firma) : nazwa(firma.nazwa), Wpis(firma.nrTel), adres(firma.adres) {nFir++;} //konstruktor kopiujący
    virtual ~Firma() {nFir--;}
    std::string getNazwa() {return nazwa;}          //zraca nazwę firmy
    std::string getAdres() {return adres;}          //zwraca adres
    void setAdres(std::string adr) {adres = adr;}   //zmienia adres
    void setNazwa(std::string naz) {nazwa=naz;}     //zmienia nazwę
    const void print() override {std::cout << "Nazwa firmy: " << nazwa << "\tnumer telefonu: " << nrTel << "\tadres: " << adres << "\n";}
    static int getNFir() {return nFir;}             //zwraca liczbę zapisanych innych wpisów
    private:
    std::string nazwa;
    std::string adres;
    static int nFir;    //liczba pozostałych wpisów
};
int Firma::nFir = 0;

using ksiazka_t = std::map<std::string, std::variant<Czlowiek, Firma>>;

template<typename T>    //template przeciążenia operatora dodawania do dodawania nowych wpisów do książki
void operator+(ksiazka_t& ksiazka, T& wpis){ksiazka.emplace(wpis.getImie(), wpis);}

template<>
void operator+<Firma>(ksiazka_t& ksiazka, Firma& wpis){ksiazka.emplace(wpis.getNazwa(), wpis);}

//przeciążenie operatora odejmowania do usuwania wpisów z książki
void operator-(ksiazka_t& ksiazka, std::string nazwa){ksiazka.erase(nazwa);}

//template do dodawania nowych wpisów
template <typename T>
void dodawanieWpisu(ksiazka_t& ksiazka) {std::cout << "Niepoprawny typ";}

//template specjalny dla wpisów osobowych
template<>
void dodawanieWpisu<Czlowiek>(ksiazka_t& ksiazka)
{
    std::string imie, komenda;
    std::cout << "Imie i nazwisko: \n";
    std::getline(std::cin>>std::ws, imie);
    Czlowiek czl(imie);
    ksiazka + czl;  //dodawanie nowego wpisu do książki (podstawową informacją jest imie i nazwisko)
    std::cout << "Dodatkowe informacje? y/n/anuluj\n";
    std::cin >> komenda;
    while (komenda != "y" && komenda != "n" && komenda != "anuluj") //sprawdzanie poprawności wpisanej komendy
    {
        std::cout << "Niepoprawna komenda\n";
        std::cin >> komenda;
    }
    if (komenda == "y") //ewentualne wprowadzenie danych dodatkowych
    {
        std::cout << "Numer telefonu? y/n\n";
        std::cin >> komenda;
        while (komenda != "y" && komenda != "n")    //sprawdzenie poprawności wpisanej komendy
        {
            std::cout << "Niepoprawna komenda\n";
            std::cin >> komenda;
        }
        if (komenda == "y") //wprowadzenie numeru telefonu
        {
            std::cout << "Podaj numer telefonu:\n";
            std::cin >> komenda;
            std::get<Czlowiek>(ksiazka[imie]).setNr(komenda);
        }
        std::cout << "Data urodzenia? y/n\n";
        std::cin >> komenda;
        while (komenda != "y" && komenda != "n")    //sprawdzenie poprawności wpisanej komendy
        {
            std::cout << "Niepoprawna komenda\n";
            std::cin >> komenda;
        }
        if (komenda == "y") //wprowadzenie daty urodzin
        {
            std::cout << "Podaj datę urodzin:\n";
            std::getline(std::cin>>std::ws, komenda);
            std::get<Czlowiek>(ksiazka[imie]).setUrodziny(komenda);
        }
    }
    if (komenda == "anuluj")    //anulowanie operacji
    {
        ksiazka.erase(imie);    //usuń dodaną pozycję z książki
        std::string anuluj{"Anulowano"};
        throw anuluj;
    }
}

//template specjalny dla wpisów nieosobowych
template<>
void dodawanieWpisu<Firma>(ksiazka_t& ksiazka)
{
    std::string nazwa, komenda;
    std::cout << "Nazwa: \n";
    std::getline(std::cin>>std::ws, nazwa);
    Firma fir(nazwa);
    ksiazka + fir;  //dodawanie nowego wpisu do książki (podstawową informacją jest nazwa)
    std::cout << "Dodatkowe informacje? y/n/anuluj\n";
    std::cin >> komenda;
    while (komenda != "y" && komenda != "n" && komenda != "anuluj") //sprawdznie poprawności wpisanej komendy
    {
        std::cout << "Niepoprawna komenda\n";
        std::cin >> komenda;
    }
    if (komenda == "y")
    {
        std::cout << "Numer telefonu? y/n\n";
        std::cin >> komenda;
        while (komenda != "y" && komenda != "n")    //sprawdznie poprawności wpisanej komendy
        {
            std::cout << "Niepoprawna komenda\n";
            std::cin >> komenda;
        }
        if (komenda == "y") //wprowadzenie numeru telefonu
        {
            std::cout << "Podaj numer telefonu:\n";
            std::cin >> komenda;
            std::get<Firma>(ksiazka[nazwa]).setNr(komenda);
        }
        std::cout << "Adres? y/n\n";
        std::cin >> komenda;
        while (komenda != "y" && komenda != "n")    //sprawdznie poprawności wpisanej komendy
        {
            std::cout << "Niepoprawna komenda\n";
            std::cin >> komenda;
        }
        if (komenda == "y") //wprowadzenie adresu
        {
            std::cout << "Podaj adres:\n";
            std::getline(std::cin>>std::ws, komenda);
            std::get<Firma>(ksiazka[nazwa]).setAdres(komenda);
        }
    }
    else if (komenda == "anuluj")   //anulowanie operacji
    {
        ksiazka.erase(nazwa);   //usu dodaną pozycję z książki
        std::string anuluj{"Anulowano"};
        throw anuluj;
    }
}

//funkcja dodająca nowe pozycje do książki
void dodaj(ksiazka_t& ksiazka)
{
    std::string komenda;
    std::cout << "osoba/firma/anuluj\n";
    std::cin >> komenda;
    while (komenda != "osoba" && komenda != "firma" && komenda != "anuluj") //sprawdzenie poprawności wpisanej komendy]
    {
        std::cout << "Niepoprawna komenda\n";
        std::cin >> komenda;
    }
    if (komenda == "anuluj")    //anulowanie operacji
    {
        std::string anuluj{"Anulowano"};
        throw anuluj;
    }
    if (komenda == "osoba") //wywołanie odpowiedniej wersji funkcji w zależności od typu wpisu
        dodawanieWpisu<Czlowiek>(ksiazka);
    else if (komenda == "firma")
        dodawanieWpisu<Firma>(ksiazka);
    std::cout << "Pozycja dodana\n";
}

//wyświetlanie wszystkich pozycji w książce
void wyswietl(ksiazka_t& ksiazka) {std::for_each(ksiazka.begin(), ksiazka.end(), [&](std::pair<const std::string, std::variant<Czlowiek, Firma>>& wpis) {std::visit([](auto& wpi){wpi.print();}, wpis.second);});} 

//wyświetlanie konkretnej pozycji z książki
void wyswietlK(ksiazka_t& ksiazka)
{
    std::string nazwa;
    std::cout << "Podaj imię i nazwisko lub nazwę firmy\n";
    std::getline(std::cin>>std::ws, nazwa);
    while (ksiazka.find(nazwa) == ksiazka.end() && nazwa != "anuluj") //sprawdzenie czy podany wpis istnieje
    {
        std::cout << "Taki wpis nie istnieje! Podaj poprawną wartość, lub wpisz \"anuluj\":\n";
        std::getline(std::cin>>std::ws, nazwa);
    }
    if (nazwa == "anuluj")
    {
        std::string anuluj{"Anulowano"};
        throw anuluj;
    }
    std::visit([](auto& wpi){wpi.print();}, ksiazka[nazwa]);
}

template<typename T>
void edytowanie(ksiazka_t& ksiazka, std::string& komenda, int& i, std::string& nazwa) {std::cout << "Zły typ!\n";}

template<>
void edytowanie<Czlowiek>(ksiazka_t& ksiazka, std::string& komenda, int& i, std::string& nazwa)
{
    if (i == 0) //i = 0 aby pytanie wyświetliło się tylko raz na początku
    {
        std::cout << "Co chcesz zmienić? imie/telefon/urodziny\n";
        i=1;
    }
    std::cin >> komenda;
    while (komenda != "imie" && komenda != "telefon" && komenda != "urodziny" && komenda != "koniec" && komenda != "anuluj")    //sprawdzenie poprawności komendy
    {
        std::cout << "Komenda niepoprawna\n";
        std::cin >> komenda;
    }
    if (komenda == "imie")
    {
        std::cout << "Podaj nowe imię i nazwisko\n";
        std::getline(std::cin>>std::ws, komenda);
        ksiazka.emplace(komenda, Czlowiek(std::get<Czlowiek>(ksiazka[nazwa]))); 
        ksiazka.erase(nazwa);
        nazwa = komenda;
        std::get<Czlowiek>(ksiazka[nazwa]).setImie(nazwa);
    }
    else if (komenda == "telefon")
    {
        std::cout << "Podaj nowy numer telefonu:\n";
        std::cin >> komenda;
        std::get<Czlowiek>(ksiazka[nazwa]).setNr(komenda);
    }
    else if (komenda == "urodziny")
    {
        std::cout << "Podaj nową datę urodzin:\n";
        std::getline(std::cin>>std::ws, komenda);
        std::get<Czlowiek>(ksiazka[nazwa]).setUrodziny(komenda);
    }
}

template<>
void edytowanie<Firma>(ksiazka_t& ksiazka, std::string& komenda, int& i, std::string& nazwa)
{
    if (i == 0)
    {
        std::cout << "Co chcesz zmienić? nazwa/telefon/adres\n";
        i=1;
    }
    std::cin >> komenda;
    while (komenda != "nazwa" && komenda != "telefon" && komenda != "adres" && komenda != "koniec" && komenda != "anuluj")  //sprawdzenie poprawności komendy
    {
        std::cout << "Komenda niepoprawna\n";
        std::cin >> komenda;
    }
    if (komenda == "nazwa")
    {
        std::cout << "Podaj nową nazwę firmy\n";
        std::getline(std::cin>>std::ws, komenda);
        ksiazka.emplace(komenda, Firma(std::get<Firma>(ksiazka[nazwa])));            
        ksiazka.erase(nazwa);
        nazwa = komenda;
        std::get<Firma>(ksiazka[nazwa]).setNazwa(nazwa);
    }
    else if (komenda == "telefon")
    {
        std::cout << "Podaj nowy numer telefonu:\n";
        std::cin >> komenda;
        std::get<Firma>(ksiazka[nazwa]).setNr(komenda);
    }
    else if (komenda == "adres")
    {
        std::cout << "Podaj nowy adres:\n";
        std::getline(std::cin>>std::ws, komenda);
        std::get<Firma>(ksiazka[nazwa]).setAdres(komenda);
    }
}

//edycja istniejących pozycji
void edycja(ksiazka_t& ksiazka) 
{
    if (ksiazka.empty())    //sprawdzenie czy książka nie jest pusta
    {
        std::string pustka{"Książka jest pusta!"};
        throw pustka;
    }
    int i = 0;
    std::string nazwa, komenda, nazwa_st, telefon_st, dodatek_st;
    std::cout << "Który wpis? Podaj imię i nazwisko lub nazwę\n";
    std::getline(std::cin>>std::ws, nazwa);
    while (ksiazka.find(nazwa) == ksiazka.end()) //sprawdzenie czy podany wpis istnieje
    {
        std::cout << "Taki wpis nie istnieje! Podaj poprawną wartość:\n";
        std::getline(std::cin>>std::ws, nazwa);
    }
    //zapisanie starych danych z pozycji na wypadek anulowania operacji
    nazwa_st = nazwa;
    if (ksiazka[nazwa].index() == 0)
    {
        telefon_st = std::get<Czlowiek>(ksiazka[nazwa]).getNr();
        dodatek_st = std::get<Czlowiek>(ksiazka[nazwa]).getUrodziny();
    }
    else if (ksiazka[nazwa].index() == 1)
    {
        telefon_st = std::get<Firma>(ksiazka[nazwa]).getNr();
        dodatek_st = std::get<Firma>(ksiazka[nazwa]).getAdres();
    }
    std::cout << "Aby zakończyć edycję wpisz: koniec. Aby anulować wpisz: anuluj\n";
    while (komenda != "koniec" && komenda != "anuluj")  //edycja trwa dopóki nie zostanie zakończona lub anulowana
    {
        if (ksiazka[nazwa].index() == 0)    //sprawdzenie czy wpis to Czlowiek
            edytowanie<Czlowiek>(ksiazka, komenda, i, nazwa);
        else if (ksiazka[nazwa].index() == 1)   //sprawdzenie czy wpis to Firma
            edytowanie<Firma>(ksiazka, komenda, i, nazwa);
    }
    if (komenda == "anuluj")    //anulowanie operacji, reset wprowadzonych zmian
    {
        //dodanie odpowiedniego wpisu ze starymi danymi
        if (ksiazka[nazwa].index() == 0)
        {
            if (nazwa_st == nazwa)
            {
                std::get<Czlowiek>(ksiazka[nazwa]).setNr(telefon_st);
                std::get<Czlowiek>(ksiazka[nazwa]).setUrodziny(dodatek_st);
            }
            else
            {
                Czlowiek czl(nazwa_st, telefon_st, dodatek_st);
                ksiazka + czl;
                ksiazka - nazwa;   //usunięcie nowej wersji wpisu
            }
        }
        else if (ksiazka[nazwa].index() == 1)
        {
            if (nazwa_st == nazwa)
            {
                std::get<Firma>(ksiazka[nazwa]).setNr(telefon_st);
                std::get<Firma>(ksiazka[nazwa]).setAdres(dodatek_st);
            }
            else
            {
                Firma fir(nazwa_st, telefon_st, dodatek_st);
                ksiazka + fir;
                ksiazka - nazwa;   //usunięcie nowej wersji wpisu
            }
        }
        std::string anuluj{"Anulowano"};
        throw anuluj;
    }
    std::cout << "Edycja zakończona\n";
}

//usuwanie pozycji z książki
void usun(ksiazka_t& ksiazka)
{
    std::string nazwa;
    std::cout << "Który wpis? Podaj imię i nazwisko lub nazwę firmy. Aby ununąć wszystkie pozycje wpisz \"wszystko\". Wpisz \"anuluj\" any anulować.\n";
    std::getline(std::cin>>std::ws, nazwa);
    while (ksiazka.find(nazwa) == ksiazka.end() && nazwa != "wszystko" && nazwa != "anuluj")    //czy wpis istnieje?
    {
        std::cout << "Taki wpis nie istnieje! Podaj poprawną wartość:\n";
        std::getline(std::cin>>std::ws, nazwa);
    }
    if (nazwa == "anuluj")  //anulowanie operacji
    {
        std::string anuluj{"Anulowano"};
        throw anuluj;
    }
    if (nazwa == "wszystko")
    {
        ksiazka.clear();
        std::cout << "Pozycje usunięte\n";
    }
    else
    {
        ksiazka - nazwa;   //usunięcie odpowiednie pozycji
        std::cout << "Pozycja usunięta\n";
    }
}

//zapis książki do pliku
void zapis(ksiazka_t& ksiazka)
{
    int i = 0;
    ksiazka_t::iterator it;
    std::string nazwa_pliku;
    std::unique_ptr<ksiazka_t::iterator[]> tab = std::make_unique<ksiazka_t::iterator[]>(Firma::getNFir()); //tablica przechowująca iteratory książki odpowiadające wpisom Firma
    std::cout << "Podaj nazwę pliku lub wpisz \"anuluj\" aby anulować\n";
    std::cin >> nazwa_pliku;
    if (nazwa_pliku == "anuluj")    //anulowanie operacji
    {
        std::string anuluj{"Anulowano"};
        throw anuluj;
    }
    std::ofstream ksiazka_plik(nazwa_pliku + ".XML");   //plik jest zapisywany w formacie XML
    ksiazka_plik << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<?mso-application progid=\"Excel.Sheet\"?>\n<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\" xmlns:x=\"urn:schemas-microsoft-com:office:excel\" xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\" xmlns:html=\"https://www.w3.org/TR/html401/\">\n<Worksheet ss:Name=\"Książka adresowa\">\n<Table>\n<Column ss:Index=\"1\" ss:AutoFitWidth=\"0\" ss:Width=\"110\"/>\n";   //pierwsze linijki określające parametry arkusza
    ksiazka_plik << "<Row>\n<Cell><Data ss:Type=\"String\">Osoby</Data></Cell>\n</Row>\n<Row>\n<Cell><Data ss:Type=\"String\">Imię i nazwisko</Data></Cell>\n<Cell><Data ss:Type=\"String\">Numer telefonu</Data></Cell>\n<Cell><Data ss:Type=\"String\">Urodziny</Data></Cell>\n</Row>\n"; //pierwsze dwa wiersze arkusza informujące, że poniżej są dane osobowe oraz jakie informacje znajdują się w danej kolumnnie
    for (it = ksiazka.begin(); it != ksiazka.end(); it++)   //iterowanie przez wszystkie pozycje książki
    {
        if ((*it).second.index() == 0)  //najpierw zapisywane są osoby
            ksiazka_plik << "<Row>\n<Cell><Data ss:Type=\"String\">" << std::get<Czlowiek>((*it).second).getImie() << "</Data></Cell>\n<Cell><Data ss:Type=\"String\">" << std::get<Czlowiek>((*it).second).getNr() << "</Data></Cell>\n<Cell><Data ss:Type=\"String\">" << std::get<Czlowiek>((*it).second).getUrodziny() << "</Data></Cell>\n</Row>\n";
        else    //iteratory do wpisów typu Firma są zapisywane w tablicy tab
        {
            tab[i] = it;
            i++;
        }
    }
    ksiazka_plik << "<Row>\n<Cell><Data ss:Type=\"String\">Firmy</Data></Cell>\n</Row>\n<Row>\n<Cell><Data ss:Type=\"String\">Nazwa firmy</Data></Cell>\n<Cell><Data ss:Type=\"String\">Numer telefonu</Data></Cell>\n<Cell><Data ss:Type=\"String\">Adres</Data></Cell>\n</Row>\n";    //dwa wiersze arkusza informujące, że poniżej są dane firmowe oraz jakie informacje znajdują się w danej kolumnnie
    for (i = 0; i < Firma::getNFir(); i++)  //wpisanie danych firmowych do arkusza
        ksiazka_plik << "<Row>\n<Cell><Data ss:Type=\"String\">" << std::get<Firma>((*tab[i]).second).getNazwa() << "</Data></Cell>\n<Cell><Data ss:Type=\"String\">" << std::get<Firma>((*tab[i]).second).getNr() << "</Data></Cell>\n<Cell><Data ss:Type=\"String\">" << std::get<Firma>((*tab[i]).second).getAdres() << "</Data></Cell>\n</Row>\n";
    ksiazka_plik << "</Table>\n</Worksheet>\n</Workbook>";  //zakończenie tabeli
    std::cout << "Dane zapisane\n";
    ksiazka_plik.close();   //zamknięcie pliku
}

//wczytanie danych z pliku
void wczytaj(ksiazka_t& ksiazka)
{
    int i = 0, j = 0;
    bool firma = false;
    bool cell_begin, row_begin;
    size_t pos_beg = 0, pos_end, len;
    std::string bufor, nazwa_pliku, separator1 = "\">", separator2 = "</Data>";
    std::string tmp[3];
    std::cout << "Podaj nazwę pliku lub wpisz \"anuluj\" aby anulować\n";
    std::cin >> nazwa_pliku;
    if (nazwa_pliku == "anuluj")    //anulowanie operacji
    {
        std::string anuluj{"Anulowano"};
        throw anuluj;
    }
    std::ifstream ksiazka_plik(nazwa_pliku + ".XML");   //otwarcie pliku w formacie XML
    while (std::getline (ksiazka_plik, bufor))
    {
        if (i > 12) //pierwsze 12 linijek nie przechowuje danych istotnych dla książki
        {
            if (bufor == "<Cell><Data ss:Type=\"String\">Firmy</Data></Cell>")  //czy typ danych zmienił się na firmę? Najpier są wpisywane do książki osoby
            {
                i = 6;  //i=6 ze względu na ilość nieistotnych dla książki linijek
                firma = true;
                row_begin = false;
            }
            if (bufor == "</Row>")  //koniec wiersza w arkuszu?
            {
                if (row_begin)      //jeżeli jest to wiersz z informacjami (jakieś dane zostały już pobrane z akrusza), stwórz odpowiedni wpis w książce
                {
                    if (!firma)
                        ksiazka.emplace(tmp[0], Czlowiek(tmp[0], tmp[1], tmp[2]));
                    else
                        ksiazka.emplace(tmp[0], Firma(tmp[0], tmp[1], tmp[2]));
                }
                row_begin = false;  //koniec wiersza
                j = 0;
            }
            if (row_begin)          //jeżeli wiersz się zaczął
            {
                pos_beg = bufor.find(separator1) + 2;   //sprawdzenie położenia informacji
                pos_end = bufor.find(separator2);
                len = pos_end - pos_beg;                //długość informacji
                tmp[j] = bufor.substr(pos_beg, len);    //zapisanie danych do tymczasowej tabeli
                j++;
            }
            if (bufor == "<Row>")   //czy początek wiersza?
                row_begin = true;
        }
        i++;    //kolejna linijka pliku
    }
    std::cout << "Dane wczytane\n";
    ksiazka_plik.close();   //zamknięcie pliku
}

//obsługa podstawowych komend wpisywanych przez użytkownika
void obslugaKomend(bool& koniec, std::string komenda, ksiazka_t& ksiazka)
{
    if (komenda=="zamk")
        koniec = true;
    else if (komenda=="dodaj")
        dodaj(ksiazka);
    else if (komenda == "wp")   //wyświetl pozycje
        wyswietl(ksiazka);
    else if (komenda == "edycja")
        edycja(ksiazka);
    else if (komenda == "usun")
        usun(ksiazka);
    else if (komenda == "wys")  //wyświetl konkretną pozycję
        wyswietlK(ksiazka);
    else if (komenda == "zapisz")
        zapis(ksiazka);
    else if (komenda == "wczytaj")
        wczytaj(ksiazka);
    else
    {
        std::string zla_komenda{"Zła komenda!"};
        throw zla_komenda;
    }
}

int main()
{
    setlocale(LC_CTYPE, "Polish"); //polskie znaki
    ksiazka_t ksiazka;      //stworzenie std::map książka
    int i = 0;
    bool koniec = false;    //zamknięcie programu
    std::string komenda;    //wpisywana przez użytkownika komenda
    while (koniec == false)
    {
        if (i == 0)
        {
            std::cout << "Książka telefoniczna\nMożliwe działania:\nwp - wyświetl pozycje\nwys - wyswietl konkretną pozycję\ndodaj - dodaj nową pozycję\nedycja - edycja istniejącej pozycji\nusun - usunięcie pozycji\nzapisz - zapisanie książki do pliku\nwczytaj - wczytanie książki z pliku\nzamk - zakończ program\n";
            i = 1;
        }
        std::cin >> komenda;
        try
        {
            obslugaKomend(koniec, komenda, ksiazka);
        }
        catch(const std::string& e)
        {
            std::cerr << e << '\n';
        }
        
    }
    return 0;
}
