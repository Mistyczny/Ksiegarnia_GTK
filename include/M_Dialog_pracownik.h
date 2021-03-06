#ifndef M_DIALOG_PRACOWNIK_H
#define M_DIALOG_PRACOWNIK_H
#include <gtk/gtk.h>
#include <iostream>
#include <Baza_danych.h>
#include <regex>

struct dane
{
    std::string Imie_i_nazwisko;
    std::string Pesel;
    std::string Miasta_zamieszkania;
    std::string Ulica_i_numer;
    std::string Kod_pocztowy;
    std::string A_email;
    std::string Nr_tel;
    std::string Admin;
    std::string Login;
    std::string Haslo;
    int id_adresu_zamieszkania;
};

class M_Dialog_pracownik
{
private:
    Baza_danych* baza;
    GtkWidget* message;
    GtkWidget* result_message;
    ///
    GtkWidget* tabela;
    /// operacje na pracownikach
    ///nagłówek
    GtkWidget* L_naglowek;
    ///Imie i nazwisko
    GtkWidget* L_imie_i_nazwisko;
    GtkWidget* E_imie_i_nazwisko;
    ///Pesel
    GtkWidget* L_pesel;
    GtkWidget* E_pesel;
    ///Miasto zamieszkania
    GtkWidget* L_miasto_zamieszkania;
    GtkWidget* E_miasto_zamieszkania;
    ///Adres
    GtkWidget* L_adres_zamieszkania;
    GtkWidget* E_adres_zamieszkania;
    ///Kod pocztowy
    GtkWidget* L_kod_pocztowy;
    GtkWidget* E_kod_pocztowy;
    ///Email
    GtkWidget* L_email;
    GtkWidget* E_email;
    ///Numer telefonu
    GtkWidget* L_tel;
    GtkWidget* E_tel;
    ///Czy administator
    GtkWidget* L_administrator;
    GtkWidget* E_administrator;
    ///Informacja co zle
    GtkWidget* InfoAboutWrongData;
    ///dane do logowania
    GtkWidget* L_login;
    GtkWidget* L_haslo;

    GtkWidget* E_login;
    GtkWidget* E_haslo;

    GtkWidget* L_nowe_haslo;
    GtkWidget* L_powtorz_haslo;

    GtkWidget* E_nowe_haslo;
    GtkWidget* E_powtorz_haslo;


public:
    M_Dialog_pracownik(std::string);
    M_Dialog_pracownik();
    M_Dialog_pracownik(int);
    virtual ~M_Dialog_pracownik() noexcept;

    ///metody budujace
    void zbuduj_pusty_formularz();
    void zbuduj_modyfikacje_pracownika();
    void zbuduj_formularz_zmiany_hasla();


    std::string sprawdz_czy_dane_poprawne(dane);
    bool przetworz_dane();
    /// dodanie pracownika do bazy danych
    void nowy_pracownik();
    void wstaw_informacje(dane);
    void loop_zmiana_hasla();

    /// modyfikacja danych pracownika w bazie danych
    void pobierz_informacje(int index,dane&);
    void modyfikuj_pracownika(int index);
    bool sprawdz_dane(dane& data,int index);
    bool sprawdz_haslo(const std::string&,const std::string&,const std::string&);
    void zapisz_zmiany(dane data,int index);
    int sprawdzenie_istnienia_adresu(std::string miasto,std::string ulica,std::string nr_domu);
    void usun_pracownika(int index);
    void usun_dane(int index);

    void wstaw_dane_do_formularza(dane& data);

};

#endif // M_DIALOG_PRACOWNIK_H
