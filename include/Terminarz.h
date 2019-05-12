#ifndef TERMINARZ_H
#define TERMINARZ_H
#include <iostream>
#include "Baza_danych.h"
#include <gtk/gtk.h>
#include <regex>
#include <initializer_list>
#include <boost/noncopyable.hpp>

class Terminarz : public boost::noncopyable
{
private:
    Terminarz(GtkWidget*);

    GtkWidget* box_glowny;

    GtkWidget* scr_tabela_godzin;
    GtkListStore* magazyn_godzin_pracy;
    GtkWidget* tabela_godzin;

    GtkTreeSelection* selekcja_tabeli_godzin;
    GtkWidget* Btn_zapisz_zmiany_godziny;
    GtkWidget* Btn_usun_termin;
    GtkWidget* Btn_dodaj_termin;



    GtkWidget* kalendarz;
    GtkWidget* message;
public:
    static std::unique_ptr<Terminarz> Terminarz_init(GtkWidget* box_);
    virtual ~Terminarz();

    void build();
    void schowaj_widzety();
    void wprowadz_zmiany_w_planie_pracownika(GtkWidget *target, gpointer arguments);
    static void zmiana_komorki(GtkCellRendererText *renderer,gchar* path,gchar *new_text,gpointer arguments);
    static void wypelnianie_rozpiski(GtkWidget *target, gpointer arguments);
    static void usun_termin(GtkWidget *target, gpointer arguments);
    static void dodaj_termin(GtkWidget *target, gpointer arguments);
};

#endif // TERMINARZ_H
