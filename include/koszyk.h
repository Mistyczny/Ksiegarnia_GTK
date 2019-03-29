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

class Koszyk
{
private:
    GtkWidget *window;
    GtkWidget *pay_app_window;
    GtkWidget *box;
    GtkWidget *error;
    GtkWidget *frame;
    std::map<Ksiazka*,int> zbior;

    GtkWidget *title;
    GtkWidget *scrolled_box_shopping;
    GtkListStore *storage_of_shopping;
    GtkWidget *TableShowingShopping;
    GtkTreeViewColumn *shopping_column;
    GtkTreeSelection *select;
    GtkCellRenderer *shopping_cell;
    GtkWidget *Btn_zaplac,*Btn_usun;

public:
    Koszyk(GtkWidget*,GtkWidget*,GtkWidget*);

    static void add_book_to_purchases(GtkWidget *target, gpointer arguments);
    static void delete_book_from_purchases(GtkWidget *target,gpointer arguments);
    static void purchase(GtkWidget *target,gpointer arguments);
    bool check_purchases(Ksiazka*);
    int do_zaplaty;
    std::string add_money(int i);

    ~Koszyk();
};

#endif // KOSZYK_H
