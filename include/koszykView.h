#ifndef KOSZYKVIEW_H
#define KOSZYKVIEW_H
#include <iostream>
#include <memory>
#include <gtk/gtk.h>
#include "Ksiazka.h"
#include <map>

class koszykView
{
    private:
        koszykView(GtkWidget*,GtkWidget*);
        const GtkWidget *okno_programu;
        const GtkWidget *box_glowny;

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
        static std::unique_ptr<koszykView> koszykView_init(GtkWidget*,GtkWidget*);
        ~koszykView();

        void build();
        void pokaz_widzety();
        void schowaj_widzety();
        void dodaj_do_zakupow(GtkWidget *target, gpointer arguments);

        static void TriggerNaDodaj(GtkWidget *target, gpointer arguments);
        static void TriggerNaUsun(GtkWidget *target, gpointer arguments);
        void UaktualnijCene(int cena);
        void DodajKsiazkeDoListy(std::map<Ksiazka*,int>& zbior);
        GtkWidget* GetBtnDodaj();
        GtkWidget* getBtnUsun();
        GtkTreeSelection* getSelekcjaZakupow();
        GtkListStore* getMagazynNaZakupy();
        GtkWidget* GetBtnZaplac();
};

#endif // KOSZYKVIEW_H
