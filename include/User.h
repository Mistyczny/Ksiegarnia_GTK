#ifndef USER_H
#define USER_H

#include <iostream>
#include <gtk/gtk.h>
#include <windows.h>
#include <mysql.h>
#include <string>
#include <boost/noncopyable.hpp>
#include "Baza_danych.h"
#include "M_Dialog_pracownik.h"

class User : boost::noncopyable
{
private:
    int my_id;
    std::string name;
    std::string surname;
    bool administator;

    GtkWidget *message;
    ///
    GtkWidget* tabela;
    /// operacje na pracownikach
    ///nagłówek
    GtkWidget* L_naglowek;
    ///Imie i nazwisko
    GtkWidget* L_imie_i_nazwisko;
    GtkWidget* E_imie_i_nazwisko;
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
public:
    User();
    virtual ~User();

    void setMyId(const int);
    void setName(const std::string);
    void setSurname(const std::string);
    void setAdministator(const bool);

    const int getMyId();
    const std::string getName();
    const std::string getSurname();
    const bool getAdministator();

    void logIn(int i);
    void logOff();
};

#endif // USER_H
