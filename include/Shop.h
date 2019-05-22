#ifndef SHOP_H
#define SHOP_H

#include <iostream>
#include <gtk/gtk.h>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <fstream>
#include "User.h"
#include "LoginWindow.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "koszyk.h"
#include "Payment_application.h"
#include "C_Zakladka_Employee.h"
#include "C_Zakladka_Zamowienia.h"
#include "C_Zakladka_Ksiegarnia.h"
#include "C_Obsluga_uzytkownika.h"
#include "Obsluga_koszyka.h"

class Shop : private boost::noncopyable
{
private:
    /// Tworzenie okna
    GtkWidget* okno_programu;
    GtkWidget* okno_platnosci;
    GtkWidget* error;
    GtkWidget* box_glowny;
    GtkWidget* Btn_spis_dostepnych_ksiazek,*Btn_zamowienie,*Btn_spis_pracownikow,*Active_button; /// przyciski wyboru strony
    ///
    std::shared_ptr<Koszyk> koszyczek;
    std::unique_ptr<C_Zakladka_Employee> zakladka_employee;
    std::unique_ptr<C_Zakladka_Zamowienia> zakladka_zamowienia;
    std::unique_ptr<C_Zakladka_Ksiegarnia> zakladka_ksiegarnia;
    std::unique_ptr<C_Obsluga_uzytkownika> okno_uzytkownika;
    Payment_application* payment;
    std::shared_ptr<User> uzytkownik;

    int aktualna_strona;
    /// pasek informacji osobistych oraz koszyka klienta
    GtkWidget* about_me_frame,*koszyk;  /// ramki
    GtkWidget* Label_imie,*Label_nazwisko; /// napis: imie oraz nazwisko
    GtkWidget* button_wyloguj;  /// przycisk wylogowania sie
    GtkWidget *zaplac,*usun_z_koszyka;

public:
    GtkTreeSelection* get_ksiegarnia_selekcja_ksiazki();
    GtkTreeSelection* get_zamowienia_selekcja_zamowienia();
    /// log off
    static void log_off(GtkWidget *, gpointer);
    int get_aktualna_strona();

    /// changing main window
    static void change_page(GtkWidget *target, gpointer arguments);
    static void dodaj_ksiazke_do_koszyka(GtkWidget *target, gpointer arguments);
    /// managing shopping

    /// methods that builds
    void build_Btn_wyboru_karty();
    void build_info_about_user(GtkWidget*,int,int,int,int);
    /// elo elo
    static void set_text_to_info_label(GtkTreeView*, GtkTreePath*, GtkTreeViewColumn*, gpointer);
    static void add_info_about_book(GtkWidget *target, gpointer arguments);
    std::shared_ptr<Koszyk> get_koszyczek();
    GtkTreeSelection* get_selected(int i);
    GtkWidget* get_active_button();
    GtkWidget* get_BtnDostepnychKsiazek();
    GtkWidget* get_BtnZamowien();
    GtkWidget* get_BtnPracownikow();
    void set_ActiveButton(GtkWidget*);
    /// metody glowne
    Shop(std::shared_ptr<User>&);
    virtual ~Shop();
    void build();
    void run();
};

#endif // SHOP_H
