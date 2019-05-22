#ifndef OBSLUGA_KOSZYKA_H
#define OBSLUGA_KOSZYKA_H
#include <iostream>
#include <memory>
#include <gtk/gtk.h>
#include "koszyk.h"

class Obsluga_koszyka
{
    private:
        Obsluga_koszyka(GtkWidget*,GtkWidget*,std::shared_ptr<Koszyk>);

        const GtkWidget *okno_programu;
        const GtkWidget *box_glowny;
        std::shared_ptr<Koszyk> koszyczek;
        const char* nazwy_kolumn_koszyk;

        GtkWidget* ramka_koszyka;
        GtkWidget* L_do_zaplaty;
        GtkWidget* Scr_box_zakupy;
        GtkListStore* magazyn_na_zakupy;
        GtkWidget* tabela_pokazujaca_zakupy;
        GtkTreeSelection* selekcja_zakupow;

        GtkWidget *Btn_zaplac;
        GtkWidget *Btn_usun;
        GtkWidget *Btn_dodaj;
    public:
        static std::unique_ptr<Obsluga_koszyka> Obsluga_koszyka_init(GtkWidget*,GtkWidget*,std::shared_ptr<Koszyk>);
        ~Obsluga_koszyka();

        void build();
        void pokaz_widzety();
        void schowaj_widzety();
        void dodaj_do_zakupow(GtkWidget *target, gpointer arguments);
};

#endif // OBSLUGA_KOSZYKA_H
