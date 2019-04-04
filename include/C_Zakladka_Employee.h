#ifndef C_ZAKLADKA_EMPLOYEE_H
#define C_ZAKLADKA_EMPLOYEE_H
#include "gtk/gtk.h"

class C_Zakladka_Employee
{
private:
    GtkWidget* okno_glowne;
    GtkWidget* box_glowny;
    GtkWidget* scr_kontener_pracownicy;
    GtkListStore* magazyn_pracownikow;
    GtkWidget* tabela_pracownikow;

    GtkListStore* magazyn_godzin_pracy;
    GtkWidget* tabela_godzin;

    GtkListStore* magazyn_godzin_pracy_pracownikow;

    GtkWidget* kalendarz;
public:
    C_Zakladka_Employee(GtkWidget* okno_glowne,GtkWidget* box);
    void build();
    static void run(GtkWidget *target, gpointer arguments);
    void remove_employee_widgets();
    virtual ~C_Zakladka_Employee();
    static void zmiana_komorki(GtkTreeView * treeview, GtkTreePath * path, GtkTreeViewColumn * col, gpointer arguments);
    static void wypelnianie_rozpiski(GtkWidget *target, gpointer arguments);
};

#endif // C_ZAKLADKA_EMPLOYEE_H
