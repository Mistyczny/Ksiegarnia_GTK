#ifndef KOSZYK_H
#define KOSZYK_H
#include <iostream>
#include <vector>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include "Ksiazka.h"
#include <map>
#include <thread>
#include "Payment_application.h"
#include <memory>

class Koszyk
{
private:
    std::map<Ksiazka*,int> zbior;
    std::unique_ptr<Payment_application> aplikacja_platnosci;

    Koszyk(GtkWidget*,GtkWidget*,GtkWidget*);
    GtkWidget *okno_programu;
    GtkWidget *okno_aplikacji_platnosci;
    GtkWidget *box_glowny;

    GtkWidget* ramka_koszyka;
    GtkWidget* L_do_zaplaty;
    GtkWidget* Scr_box_zakupy;
    GtkListStore* magazyn_na_zakupy;
    GtkWidget* tabela_pokazujaca_zakupy;
    GtkTreeSelection* selekcja_zakupow;

    GtkWidget *Btn_zaplac;
    GtkWidget *Btn_usun;
    GtkWidget *Btn_dodaj;

    GtkWidget *message;
public:
    static std::shared_ptr<Koszyk> Koszyk_init(GtkWidget*,GtkWidget*,GtkWidget*);
    ~Koszyk();

    static void dodaj_do_zakupow(GtkWidget *target, gpointer arguments);
    static void delete_book_from_purchases(GtkWidget *target,gpointer arguments);
    static void zaplac(GtkWidget *target,gpointer arguments);
    bool check_purchases(Ksiazka*,int ilosc=1);
    int do_zaplaty;
    std::string add_money(int i);
    std::string remove_money(int i);

    void pokaz_widzety();
    void schowaj_widzety();
    GtkWidget* get_Btn_dodaj();
};

#endif // KOSZYK_H
