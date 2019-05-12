#ifndef C_ZAKLADKA_KSIEGARNIA_H
#define C_ZAKLADKA_KSIEGARNIA_H
#include <memory>
#include <iostream>
#include <Baza_danych.h>
#include <boost/noncopyable.hpp>
#include <gtk/gtk.h>

class C_Zakladka_Ksiegarnia : boost::noncopyable
{
private:
    C_Zakladka_Ksiegarnia(GtkWidget*,GtkWidget*);

    GtkWidget* glowne_okno;
    GtkWidget* box_glowny;
    /// Table
    GtkWidget* Scr_box_na_ksiazki;
    GtkListStore *magazyn_na_ksiazki;
    GtkWidget *tabela_pokazujaca_ksiazki;
    GtkTreeSelection *selekcja_ksiazki;
    /// Info about books
    GtkWidget* Scr_box_informacje_o_ksiazce;
    GtkWidget* L_informacje_o_ksiazce;
    GtkWidget *Btn_dodaj_informacje_o_ksiazce;
    /// modifications of the table contents
    GtkWidget* VBox_sortowanie_ksiazek;
    GtkWidget* E_szukaj;
    GtkWidget* Btn_szukaj;
    /// message to user
    GtkWidget* message;
public:
    static std::unique_ptr<C_Zakladka_Ksiegarnia> C_Zakladka_Ksiegarnia_init(GtkWidget* glowne_okno,GtkWidget* box);
    virtual ~C_Zakladka_Ksiegarnia();
    void build();
    void zaladuj_ksiazki();
    static void pokaz_informacje_o_ksiazce(GtkTreeView * treeview, GtkTreePath * path, GtkTreeViewColumn * col, gpointer arguments);
    static void dodaj_informacje_o_ksiazce(GtkWidget* target,gpointer arguments);
    static void sortuj_ksiazki(GtkWidget *, gpointer);
    static void szukaj_ksiazki(GtkWidget *, gpointer);

    void pokaz_widzety();
    void schowaj_widzety();
    GtkTreeSelection* get_selekcja_ksiazki();
};

#endif // C_ZAKLADKA_KSIEGARNIA_H
