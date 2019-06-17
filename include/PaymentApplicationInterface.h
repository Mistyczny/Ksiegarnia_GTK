#ifndef PAYMENTAPPLICATIONINTERFACE_H
#define PAYMENTAPPLICATIONINTERFACE_H
#include <gtk/gtk.h>
#include <iostream>
#include <map>
#include <Ksiazka.h>

class PaymentApplicationInterface
{
    private:
        GtkWidget* oknoProgramu;
        GtkWidget* boxNaWidgety;

        GtkWidget* scrBoxZakupy;
        GtkListStore* magazynNaZakupy;
        GtkWidget* tabelaPokazujacaZakupy;

        GtkWidget* btnPlatnoscKarta;
        GtkWidget* btnPlatnoscGotowka;
    public:
        PaymentApplicationInterface();
        virtual ~PaymentApplicationInterface();

        void buildTreeViewZakupow(std::map<Ksiazka*,int> zakupy);
        void buildButtons();
        void run();

        GtkWidget* getBtnPlatnoscKarta();
};

#endif // PAYMENTAPPLICATIONINTERFACE_H
