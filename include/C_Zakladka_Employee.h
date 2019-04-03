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
public:
    C_Zakladka_Employee(GtkWidget* okno_glowne,GtkWidget* box);
    void build();
    static void run(GtkWidget *target, gpointer arguments);
    void remove_employee_widgets();
    virtual ~C_Zakladka_Employee();
};

#endif // C_ZAKLADKA_EMPLOYEE_H
