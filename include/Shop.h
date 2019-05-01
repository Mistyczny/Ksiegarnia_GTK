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

class Shop : private boost::noncopyable
{
private:
    /// Tworzenie okna
    GtkWidget* okno_programu;
    GtkWidget* payment_window;
    GtkWidget* error;
    GtkWidget* box_glowny;
    GtkWidget* scrolled_box_for_books;
    GtkWidget* scrolled_box_for_info_about_books;
    GtkWidget* Btn_spis_dostepnych_ksiazek,*Btn_zamowienie,*Btn_spis_pracownikow,*Active_button; /// przyciski wyboru strony
    GtkWidget* InfoAboutBooks;
    ///
    Koszyk* koszyczek;
    std::unique_ptr<C_Zakladka_Employee> zakladka_employee;
    std::unique_ptr<C_Zakladka_Zamowienia> zakladka_zamowienia;
    Payment_application* payment;
    boost::shared_ptr<User> uzytkownik;

    int aktualna_strona;
    /***********************************************
    ** okno 1
    ***********************************************/
    /// wszystko do obslugi okna przegladania ksiazek
    GtkWidget *Btn_add_info_about_book;
    GtkListStore *magazyn_do_list;
    GtkWidget *tabela_pokazujaca_liste;
    GtkTreeViewColumn *kolumna;
    GtkCellRenderer *komorka;
    GtkWidget *TextBoxSearching;
    GtkWidget *Btn_searching;
    GtkWidget *Btn_dodaj_do_koszyka;

    GtkTreeSelection **zaznaczenie_elementu_listy;
    /**
    *   zaznaczenie_elementu_listy[0] - lista ksiazek
    *   zaznaczenie_elementu_listy[1] - lista zamówień
    **/
    /// do sortowania
    GtkWidget *sortowanie;
    GtkWidget **sposoby_sortowania;
    GtkWidget* VBox_wybieranie_sposobu_sortowania_dostepnych_ksiazek;
    /// pasek informacji osobistych oraz koszyka klienta
    GtkWidget* about_me_frame,*koszyk;  /// ramki
    GtkWidget* Label_imie,*Label_nazwisko; /// napis: imie oraz nazwisko
    GtkWidget* button_wyloguj;  /// przycisk wylogowania sie
    GtkWidget *zaplac,*usun_z_koszyka;

public:
    /// sposoby przeszukiwania ksiazek
    static void sortowanie_dostepnych_ksiazek(GtkWidget *, gpointer);
    static void szukanie_ksiazki(GtkWidget *, gpointer);

    /// log off
    static void log_off(GtkWidget *, gpointer);

    /// changing main window
    static void change_page(GtkWidget *target, gpointer arguments);
    static void dodaj_ksiazke_do_koszyka(GtkWidget *target, gpointer arguments);
    /// managing shopping

    /// methods that builds
    void build_casual_widgets();
    void build_info_about_user(GtkWidget*,int,int,int,int);
    void build_koszyk();
    void zbuduj_liste_ksiazek();
    void build_menu_tool_bar();
    /// elo elo
    static void set_text_to_info_label(GtkTreeView*, GtkTreePath*, GtkTreeViewColumn*, gpointer);
    static void add_info_about_book(GtkWidget *target, gpointer arguments);
    Koszyk* get_koszyczek();
    GtkTreeSelection* get_selected(int i);
    GtkWidget* get_active_button();
    GtkWidget* get_BtnDostepnychKsiazek();
    GtkWidget* get_BtnZamowien();
    GtkWidget* get_BtnPracownikow();
    void set_ActiveButton(GtkWidget*);
    /// metody glowne
    Shop(boost::shared_ptr<User>&);
    virtual ~Shop();
    void build();
    void run();
    void usun_widzety_dostepnych_ksiazek();
    void dodaj_widzety_obslugi_tabel();
};

#endif // SHOP_H
