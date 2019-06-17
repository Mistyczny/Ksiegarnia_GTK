#include "Shop.h"
#include "define.h"
void Shop::run()
{
    gtk_widget_show_all(okno_programu);
    gtk_main();
}

Shop::Shop(std::shared_ptr<User>& uzytkownik_)
{
    std::cout<<"TWORZE SHOP"<<std::endl;
    this->uzytkownik = uzytkownik_;
    this->build();
    aktualna_strona = 1;
}

Shop::~Shop()
{
    std::cout<<"DESTRUKTOR SHOP"<<std::endl;

}
/****************************************************************************
*
* METHODS THAT BUILD APPLICATION
*
*****************************************************************************/
void Shop::build()
{
    try
    {
        /// build window
        okno_programu = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_position(GTK_WINDOW(okno_programu), GTK_WIN_POS_CENTER);
        gtk_window_fullscreen(GTK_WINDOW(okno_programu));
        gtk_window_set_resizable (GTK_WINDOW(okno_programu), FALSE);
        gtk_window_set_title(GTK_WINDOW(okno_programu), "Ksiegarnia_GTK");

        /// build main container
        box_glowny = gtk_table_new(10, 10, TRUE);
        gtk_container_add(GTK_CONTAINER(okno_programu), box_glowny);
        gtk_container_set_border_width(GTK_CONTAINER(box_glowny),10);

        this->build_Btn_wyboru_karty();
        this->build_info_about_user(box_glowny,0,2,0,1);
        /// creating an instance of C_Zakladka_Employee (as unique_ptr)
        zakladka_employee = C_Zakladka_Employee::C_Zakladka_Employee_init(okno_programu,box_glowny);
        zakladka_employee->build();
        /// creating an instance of C_Zakladka_zamowienia (as unique_ptr)
        zakladka_zamowienia = C_Zakladka_Zamowienia::C_Zakladka_Zamowienia_init(okno_programu,box_glowny);
        zakladka_zamowienia->build();
        /// creating an instance of C_Zakladka_Ksiegarnia - main page (as unique_ptr)
        zakladka_ksiegarnia = C_Zakladka_Ksiegarnia::C_Zakladka_Ksiegarnia_init(okno_programu,box_glowny);
        zakladka_ksiegarnia->build();
        zakladka_ksiegarnia->pokaz_widzety();
        /// creating an instance of Koszyk which holds all books and stuffs to buy (as shared_ptr)
        koszyczek = Koszyk::Koszyk_init(okno_programu,box_glowny,okno_platnosci);
        g_signal_connect(G_OBJECT(koszyczek->getInterface()->GetBtnDodaj()), "clicked", G_CALLBACK(&koszykView::TriggerNaDodaj),this);
        g_signal_connect(G_OBJECT(koszyczek->getInterface()->GetBtnZaplac()), "clicked", G_CALLBACK(&Shop::TriggerNaZaplac),this);

        okno_uzytkownika = C_Obsluga_uzytkownika::C_Obsluga_uzytkownika_init(okno_programu,box_glowny,uzytkownik);
        okno_uzytkownika->build();
        okno_uzytkownika->pokaz_widzety();
        g_signal_connect(G_OBJECT(okno_programu), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    }
    catch(std::bad_alloc& alloc)
    {
        exit(0);
    }
}
/****************************************************************************
*
* /functional BUILD navigating buttons of  application
*
*****************************************************************************/
void Shop::build_Btn_wyboru_karty()
{
    if(uzytkownik->getInformacjeOpracowniku()->getAdmin()==true)
    {
        /// przyciski dla admina
        Btn_spis_dostepnych_ksiazek = gtk_button_new();
        g_object_set_data(G_OBJECT(Btn_spis_dostepnych_ksiazek), "number", GINT_TO_POINTER (1));

        this->Active_button = Btn_spis_dostepnych_ksiazek;
        gtk_button_set_label(GTK_BUTTON(Btn_spis_dostepnych_ksiazek), "Available books");
        gtk_table_attach_defaults(GTK_TABLE(box_glowny), Btn_spis_dostepnych_ksiazek, 3, 4, 0, 1);
        g_signal_connect(Btn_spis_dostepnych_ksiazek, "clicked", G_CALLBACK(&Shop::change_page), this);

        Btn_zamowienie = gtk_button_new();
        g_object_set_data(G_OBJECT(Btn_zamowienie), "number", GINT_TO_POINTER (2));

        gtk_button_set_label(GTK_BUTTON(Btn_zamowienie), "Orders");
        gtk_table_attach_defaults(GTK_TABLE(box_glowny), Btn_zamowienie, 5, 6, 0, 1);
        g_signal_connect(Btn_zamowienie, "clicked", G_CALLBACK(&Shop::change_page), this);

        Btn_spis_pracownikow = gtk_button_new();
        g_object_set_data(G_OBJECT(Btn_spis_pracownikow), "number", GINT_TO_POINTER (3));

        gtk_button_set_label(GTK_BUTTON(Btn_spis_pracownikow), "Employee");
        gtk_table_attach_defaults(GTK_TABLE(box_glowny), Btn_spis_pracownikow, 7, 8, 0, 1);
        g_signal_connect(Btn_spis_pracownikow, "clicked", G_CALLBACK(&Shop::change_page), this);
    }
    else
    {
        Btn_spis_dostepnych_ksiazek = gtk_button_new();
        gtk_button_set_label(GTK_BUTTON(Btn_spis_dostepnych_ksiazek), "Available books");
        gtk_table_attach_defaults(GTK_TABLE(box_glowny), Btn_spis_dostepnych_ksiazek, 3, 5, 0, 1);
        g_signal_connect(Btn_spis_dostepnych_ksiazek, "clicked", G_CALLBACK(&Shop::change_page), this);

        Btn_zamowienie = gtk_button_new();
        gtk_button_set_label(GTK_BUTTON(Btn_zamowienie), "Orders");
        gtk_table_attach_defaults(GTK_TABLE(box_glowny), Btn_zamowienie, 6, 8, 0, 1);
        g_signal_connect(Btn_zamowienie, "clicked", G_CALLBACK(&Shop::change_page), this);
    }
}
/****************************************************************************
*
* /functional BUILDS FRAME WHICH HOLDS INFO ABOUT CURRENT USER
*
*****************************************************************************/
void Shop::build_info_about_user(GtkWidget* box_,int x,int y,int xx,int yy)
{
    /// sign out button
    button_wyloguj = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(button_wyloguj), "Sign out");
    gtk_table_attach_defaults(GTK_TABLE(box_), button_wyloguj, x, y, xx+2, yy+2);
    g_signal_connect(button_wyloguj, "clicked", G_CALLBACK(&Shop::log_off), this);
}
/***********************************************************************************
*
*   /// method that log off you from shop application,
*   /// and returns to log in app
************************************************************************************/
void Shop::log_off(GtkWidget *target, gpointer arguments)
{
    Shop *temp = static_cast<Shop*>(arguments);
    gtk_widget_destroy(temp->okno_programu);
    gtk_main_quit();
}
std::unique_ptr<Koszyk>& Shop::get_koszyczek()
{
    return koszyczek;
}

int Shop::get_aktualna_strona()
{
    return this->aktualna_strona;
}

GtkWidget* Shop::get_BtnDostepnychKsiazek()
{
    return this->Btn_spis_dostepnych_ksiazek;
}

GtkWidget* Shop::get_BtnZamowien()
{
    return this->Btn_zamowienie;
}
GtkWidget* Shop::get_BtnPracownikow()
{
    return this->Btn_spis_pracownikow;
}

void Shop::set_ActiveButton(GtkWidget* button)
{
    this->Active_button = button;
}

void Shop::change_page(GtkWidget *target, gpointer arguments)
{
    Shop* temp = static_cast<Shop*>(arguments);
    int column = GPOINTER_TO_INT(g_object_get_data (G_OBJECT(target), "number"));

    if(column!=temp->aktualna_strona)
    {
        switch(temp->aktualna_strona)
        {
            case 1:
            {
                temp->zakladka_ksiegarnia->schowaj_widzety();
                break;
            }
            case 2:
            {
                temp->zakladka_zamowienia->schowaj_widzety();
                break;
            }
            case 3:
            {
                temp->zakladka_employee->schowaj_widzety();
                temp->koszyczek->getInterface()->pokaz_widzety();
                break;
            }
        }

        switch(column)
        {
            case 1:
            {
                temp->zakladka_ksiegarnia->pokaz_widzety();
                temp->aktualna_strona=1;
                break;
            }
            case 2:
            {
                temp->zakladka_zamowienia->pokaz_widzety();
                temp->aktualna_strona=2;
                break;
            }
            case 3:
            {
                temp->koszyczek->getInterface()->schowaj_widzety();
                temp->zakladka_employee->pokaz_widzety();
                temp->aktualna_strona=3;
                break;
            }
        }
        gtk_widget_show_all(temp->okno_programu);
    }
}

GtkTreeSelection* Shop::get_ksiegarnia_selekcja_ksiazki()
{
    return this->zakladka_ksiegarnia->get_selekcja_ksiazki();
}

GtkTreeSelection* Shop::get_zamowienia_selekcja_zamowienia()
{
    return this->zakladka_zamowienia->get_selekcja_zamowienia();
}

void Shop::TriggerNaZaplac(GtkWidget *target, gpointer arguments)
{
    Shop* temp = static_cast<Shop*>(arguments);
    Payment_application aplikacja(temp->get_koszyczek()->getController()->getZbiorKsiazek(),temp->get_koszyczek()->getController()->getAmountOfMoneyToPay());
    aplikacja.run();
}
