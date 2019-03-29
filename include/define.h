
enum ksiazki
{
    ID_KSIAZKI=0,
    NAZWA_KSIAZKI,
    AUTOR,
    CENA,
    ILOSC,
    ILOSC_KOLUMN
};

enum paczki
{
    ID_PACZKI=0,
    ID_KSIAZKI_W_PACZCE,
    NAZWA_KSIAZKI_W_PACZCE,
    IMIE_ODBIORCY,
    NAZWISKO_ODBIORCY,
    CENA_PACZKI,
    ILOSC_W_PACZCE,
    ODEBRANA,
    ILOSC_KOLUMN_PACZEK
};

enum koszyk_en
{
    ID_KSIAZKI_KOSZYK=0,
    NAZWA_KSIAZKI_KOSZYK,
    CENA_KOSZYK,
    ILOSC_KOSZYK,
    ILOSC_KOLUMN_KOSZYKA
};

const gchar* nazwy_kolumn[5] = {
    "Id ksiazki",
    "Nazwa ksiazki",
    "Autor",
    "Cena",
    "Ilosc"
};

const gchar* nazwy_kolumn_paczek[8] = {
    "Id zamowienia",
    "ID Ksiazki",
    "Nazwa Książki",
    "Imie",
    "Nazwisko",
    "Cena",
    "ilosc",
    "Odebrana"
};

const char* sposoby_sortowania_ksiazek[8] = {
    "ID ksiazki +",
    "ID ksiazki -",
    "Ksiazki A-Z",
    "Ksiazki Z-A",
    "Autor A-Z",
    "Autor Z-A",
    "Cena malejac",
    "Cena rosnac"
};