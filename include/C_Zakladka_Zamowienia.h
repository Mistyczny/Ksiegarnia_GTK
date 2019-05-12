#ifndef C_ZAKLADKA_ZAMOWIENIA_H
#define C_ZAKLADKA_ZAMOWIENIA_H
#include <iostream>
#include <memory>
#include <gtk/gtk.h>
#include <Baza_danych.h>
#include <boost/noncopyable.hpp>

class C_Zakladka_Zamowienia : boost::noncopyable
{
private:
    C_Zakladka_Zamowienia(GtkWidget* glowne_okno,GtkWidget* box);

    GtkWidget* box_glowny;
    GtkWidget* okno_glowne;

    GtkWidget* VBox_wybieranie_sposobu_sortowania_zamowien;
    GtkWidget* Scr_box_zamowienia;

    GtkListStore* magazyn_do_zamowien;
    GtkWidget* tabela_pokazujaca_zamowienia;
    GtkTreeSelection *selekcja_zamowienia;

    GtkWidget* E_szukaj;
    GtkWidget* Btn_szukaj;

    GtkWidget* message;

public:
    static std::unique_ptr<C_Zakladka_Zamowienia> C_Zakladka_Zamowienia_init(GtkWidget* glowne_okno,GtkWidget* box);
    virtual ~C_Zakladka_Zamowienia();

    ///metody
    void build();
    static void sortowanie_zamowien(GtkWidget*,gpointer);
    static void pobierz_informacje_o_zamowieniu(GtkTreeView * treeview, GtkTreePath * path, GtkTreeViewColumn * col, gpointer arguments);
    void zaladuj_zamowienia();
    static void szukaj_zamowienia(GtkWidget *, gpointer);
    void pokaz_widzety();
    void schowaj_widzety();
    GtkTreeSelection* get_selekcja_zamowienia();
};

#endif // C_ZAKLADKA_ZAMOWIENIA_H
