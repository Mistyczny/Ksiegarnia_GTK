#ifndef C_OBSLUGA_UZYTKOWNIKA_H
#define C_OBSLUGA_UZYTKOWNIKA_H
#include <iostream>
#include <gtk/gtk.h>
#include "User.h"
#include <memory>

class C_Obsluga_uzytkownika
{
    private:
        C_Obsluga_uzytkownika(GtkWidget*,GtkWidget*,std::shared_ptr<User>);
        const GtkWidget* okno_glowne;
        const GtkWidget* box_glowny;
        std::shared_ptr<User> uzytkownik;

        GtkWidget* F_o_uzytkowniku;
        GtkWidget* L_Nazwa_uzytkownika;

        GtkCssProvider *provider;
    protected:

    public:
        static std::unique_ptr<C_Obsluga_uzytkownika> C_Obsluga_uzytkownika_init(GtkWidget*,GtkWidget*,std::shared_ptr<User>);
        virtual ~C_Obsluga_uzytkownika();
        void build();
        void pokaz_widzety();
        void schowaj_widzety();
};

#endif // C_OBSLUGA_UZYTKOWNIKA_H
