#ifndef FORMULARZ_ZMIANY_HASLA_H
#define FORMULARZ_ZMIANY_HASLA_H
#include <memory>
#include "User.h"

class Formularz_zmiany_hasla
{
    private:
        std::shared_ptr<User> uzytkownik;

        GtkWidget* okno_formularza;
        GtkWidget* tabela;

        GtkWidget* L_naglowek;

        GtkWidget* L_stare_haslo;
        GtkWidget* L_nowe_haslo;
        GtkWidget* L_powtorz_haslo;

        GtkWidget* E_stare_haslo;
        GtkWidget* E_nowe_haslo;
        GtkWidget* E_powtorz_haslo;

        GtkWidget* L_komentarz;

    public:
        Formularz_zmiany_hasla(std::shared_ptr<User>);
        virtual ~Formularz_zmiany_hasla();
        void build();
        void run();

        bool zmiana_hasla();
        void pobierz_dane_z_formularza(std::vector<std::string>&);
        bool sprawdz_stare_haslo(const std::string& haslo);
        void wprowadz_zmiane_hasla(std::string&);
};

#endif // FORMULARZ_ZMIANY_HASLA_H
