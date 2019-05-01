#ifndef C_ZAKLADKA_EMPLOYEE_H
#define C_ZAKLADKA_EMPLOYEE_H

#include "gtk/gtk.h"
#include "User.h"
#include "M_Dialog_pracownik.h"
#include "Terminarz.h"
#include <memory>

class C_Zakladka_Employee
{
private:
    GtkWidget* okno_glowne;
    GtkWidget* box_glowny;
    GtkWidget* scr_kontener_pracownicy;
    GtkWidget* scr_tabela_godzin;
    GtkListStore* magazyn_pracownikow;
    GtkWidget* tabela_pracownikow;
    GtkTreeSelection* selekcja_tabeli_pracownikow;

    GtkListStore* magazyn_godzin_pracy;
    GtkWidget* tabela_godzin;

    GtkListStore* magazyn_godzin_pracy_pracownikow;
    GtkTreeSelection* selekcja_tabeli_godzin;
    GtkWidget *Btn_zapisz_zmiany_godziny;

    GtkWidget* kalendarz;

    ///przyciski
    GtkWidget* Btn_dodaj_pracownika;
    GtkWidget* Btn_modyfikuj_pracownika;
    GtkWidget* Btn_usun_pracownika;

    std::unique_ptr<Terminarz> terminy;
public:
    static std::unique_ptr<C_Zakladka_Employee> C_Zakladka_Employee_init(GtkWidget*,GtkWidget*);
    C_Zakladka_Employee(GtkWidget* okno_glowne,GtkWidget* box);
    void build();
    static void odswiez_tabele_pracownikow(C_Zakladka_Employee*);
    static void run(GtkWidget *target, gpointer arguments);
    void schowaj_widzety();
    virtual ~C_Zakladka_Employee();
    static void zmiana_komorki(GtkCellRendererText *renderer,gchar* path,gchar *new_text,gpointer arguments);
    static void wypelnianie_rozpiski(GtkWidget *target, gpointer arguments);
    static void wybrany_pracownik(GtkTreeView*, GtkTreePath*, GtkTreeViewColumn*, gpointer);

    static void dodaj_nowego_pracownika(GtkWidget *target, gpointer arguments);
    static void modyfikuj_pracownika(GtkWidget* target,gpointer arguments);
    static void usun_pracownika(GtkWidget* target,gpointer arguments);
    void wprowadz_zmiany_w_planie_pracownika(GtkWidget *target, gpointer arguments);
    void pokaz_widzety();
};

#endif // C_ZAKLADKA_EMPLOYEE_H
