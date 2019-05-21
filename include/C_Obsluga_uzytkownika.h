#ifndef C_OBSLUGA_UZYTKOWNIKA_H
#define C_OBSLUGA_UZYTKOWNIKA_H
#include <iostream>
#include <gtk/gtk.h>
#include "User.h"
#include <memory>
#include "Formularz_zmiany_hasla.h"

class C_Obsluga_uzytkownika
{
    private:
        C_Obsluga_uzytkownika(GtkWidget*,GtkWidget*,std::shared_ptr<User>);
        const GtkWidget* okno_glowne;
        const GtkWidget* box_glowny;
        std::shared_ptr<User> uzytkownik;

        GtkWidget* ramka_uzytkownika;
        GtkWidget* L_Nazwa_uzytkownika;
        GtkWidget* Btn_wprowadz_zmiany;

        GtkCssProvider *provider;
    protected:

    public:
        static std::unique_ptr<C_Obsluga_uzytkownika> C_Obsluga_uzytkownika_init(GtkWidget*,GtkWidget*,std::shared_ptr<User>);
        virtual ~C_Obsluga_uzytkownika();
        void build();
        void pokaz_widzety();
        void schowaj_widzety();
        static void zmiana_hasla(GtkWidget*,gpointer);
        void wyloguj();
};

#endif // C_OBSLUGA_UZYTKOWNIKA_H
