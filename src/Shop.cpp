#include "Shop.h"
#include "define.h"

void Shop::run()
{
    gtk_widget_show_all(okno_programu);
    gtk_main();
}

Shop::Shop(boost::shared_ptr<User>& uzytkownik_)
{
    this->uzytkownik = uzytkownik_;
    this->build();
}

Shop::~Shop()
{
    delete okno_programu,box_glowny,about_me_frame,button_wyloguj,Label_imie,Label_nazwisko;
}
/****************************************************************************
*
* METHODS THAT BUILD APPLICATION
*
*****************************************************************************/
/****************************************************************************
*
* /functional MAIN BUILDING METHOD
*
*****************************************************************************/
void Shop::build()
{
    payment_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    payment = new Payment_application(payment_window);
    /// build window
    okno_programu = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(okno_programu), GTK_WIN_POS_CENTER);
    gtk_widget_set_size_request(okno_programu, 900, 700);
    gtk_window_set_resizable (GTK_WINDOW(okno_programu), FALSE);
    gtk_window_set_title(GTK_WINDOW(okno_programu), "AutoMoto");

    sposoby_sortowania = new GtkWidget*[8];
    zaznaczenie_elementu_listy = new GtkTreeSelection*[2];

    /// build main container
    box_glowny = gtk_table_new(10, 10, TRUE);
    gtk_container_add(GTK_CONTAINER(okno_programu), box_glowny);
    gtk_container_set_border_width(GTK_CONTAINER(box_glowny),10);

    this->build_casual_widgets();
    this->build_info_about_user(box_glowny,0,2,0,1);
    this->zbuduj_liste_ksiazek();
    this->build_orders_list();
    this->build_menu_tool_bar_for_orders();
    koszyczek = new Koszyk(okno_programu,box_glowny,payment_window);


    g_signal_connect(G_OBJECT(okno_programu), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}
/****************************************************************************
*
* /functional BUILD navigating buttons of  application
*
*****************************************************************************/
void Shop::build_casual_widgets()
{
    if(uzytkownik->getAdministator()==true)
    {
        /// przyciski dla admina
        Btn_spis_dostepnych_ksiazek = gtk_button_new();
        this->Active_button = Btn_spis_dostepnych_ksiazek;
        gtk_button_set_label(GTK_BUTTON(Btn_spis_dostepnych_ksiazek), "Available books");
        gtk_table_attach_defaults(GTK_TABLE(box_glowny), Btn_spis_dostepnych_ksiazek, 3, 4, 0, 1);
        g_signal_connect(Btn_spis_dostepnych_ksiazek, "clicked", G_CALLBACK(&Shop::change_to_spis_ksiazek), this);

        Btn_zamowienie = gtk_button_new();
        gtk_button_set_label(GTK_BUTTON(Btn_zamowienie), "Orders");
        gtk_table_attach_defaults(GTK_TABLE(box_glowny), Btn_zamowienie, 5, 6, 0, 1);
        g_signal_connect(Btn_zamowienie, "clicked", G_CALLBACK(&Shop::change_to_orders), this);

        Btn_spis_pracownikow = gtk_button_new();
        gtk_button_set_label(GTK_BUTTON(Btn_spis_pracownikow), "Employee");
        gtk_table_attach_defaults(GTK_TABLE(box_glowny), Btn_spis_pracownikow, 7, 8, 0, 1);
        g_signal_connect(Btn_spis_pracownikow, "clicked", G_CALLBACK(&Shop::log_off), this);

        /// build frame about user
    }
    else
    {
        Btn_spis_dostepnych_ksiazek = gtk_button_new();
        gtk_button_set_label(GTK_BUTTON(Btn_spis_dostepnych_ksiazek), "Available books");
        gtk_table_attach_defaults(GTK_TABLE(box_glowny), Btn_spis_dostepnych_ksiazek, 3, 5, 0, 1);
        g_signal_connect(Btn_spis_dostepnych_ksiazek, "clicked", G_CALLBACK(&Shop::log_off), this);

        Btn_zamowienie = gtk_button_new();
        gtk_button_set_label(GTK_BUTTON(Btn_zamowienie), "Orders");
        gtk_table_attach_defaults(GTK_TABLE(box_glowny), Btn_zamowienie, 6, 8, 0, 1);
        g_signal_connect(Btn_zamowienie, "clicked", G_CALLBACK(&Shop::change_to_orders), this);
    }
}
/****************************************************************************
*
* /functional BUILDS FRAME WHICH HOLDS INFO ABOUT CURRENT USER
*
*****************************************************************************/
void Shop::build_info_about_user(GtkWidget* box_,int x,int y,int xx,int yy)
{
    /// labels
    Label_imie = gtk_label_new(this->uzytkownik->getName().c_str());
    gtk_label_set_justify(GTK_LABEL(Label_imie), GTK_JUSTIFY_LEFT);
    gtk_table_attach_defaults (GTK_TABLE(box_), Label_imie, x, y, xx, yy);

    Label_nazwisko = gtk_label_new(this->uzytkownik->getSurname().c_str());
    gtk_label_set_justify(GTK_LABEL(Label_nazwisko), GTK_JUSTIFY_LEFT);
    gtk_table_attach_defaults(GTK_TABLE(box_), Label_nazwisko, x, y, xx+1, yy+1);

    /// sign out button
    button_wyloguj = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(button_wyloguj), "Sign out");
    gtk_table_attach_defaults(GTK_TABLE(box_), button_wyloguj, x, y, xx+2, yy+2);
    g_signal_connect(button_wyloguj, "clicked", G_CALLBACK(&Shop::log_off), this);

    /// frame
    about_me_frame  = gtk_frame_new("O mnie");
    gtk_frame_set_shadow_type(GTK_FRAME(about_me_frame), GTK_SHADOW_IN);
    gtk_frame_set_label_align(GTK_FRAME(about_me_frame),0.5,0.5);
    gtk_table_attach_defaults(GTK_TABLE(box_), about_me_frame, 0, 2, 0, 3);
}
/****************************************************************************
*
* /functional BUILDS TABLE OF AVAILABLE BOOKS
*
*****************************************************************************/
void Shop::zbuduj_liste_ksiazek()
{
    scrolled_box_for_info_about_books = gtk_scrolled_window_new(NULL, NULL);
    g_object_ref(G_OBJECT(scrolled_box_for_info_about_books ));

    FrameAboutBooks = gtk_frame_new("About Book");
    g_object_ref(G_OBJECT(FrameAboutBooks ));

    InfoAboutBooks = gtk_label_new("There will be some info");
    gtk_label_set_line_wrap(GTK_LABEL(InfoAboutBooks),TRUE);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(scrolled_box_for_info_about_books),InfoAboutBooks);

    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(scrolled_box_for_info_about_books), 7, 9, 3, 9);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny), FrameAboutBooks, 7, 10, 3, 9);
    gtk_label_set_text(GTK_LABEL(InfoAboutBooks),"Press book twice to show info about it");

    Btn_add_info_about_book = gtk_button_new();
    g_object_ref(G_OBJECT(Btn_add_info_about_book));

    gtk_button_set_label(GTK_BUTTON(Btn_add_info_about_book), "Dodaj informacje\n  o tej ksiazce");
    gtk_table_attach_defaults(GTK_TABLE(box_glowny), Btn_add_info_about_book, 5, 6, 9, 10);
    g_signal_connect(Btn_add_info_about_book, "clicked", G_CALLBACK(&Shop::add_info_about_book), this);
/////////////////////////////////////////////////////////////////////////////////////////////////////////
    GtkTreeIter iter;
    tabela_pokazujaca_liste = gtk_tree_view_new();
    g_signal_connect(tabela_pokazujaca_liste, "row-activated",G_CALLBACK(&Shop::set_text_to_info_label), this);
    gtk_tree_view_columns_autosize(GTK_TREE_VIEW(tabela_pokazujaca_liste));

    magazyn_do_list = gtk_list_store_new(ILOSC_KOLUMN,G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT,G_TYPE_INT,G_TYPE_INT );
    gtk_tree_view_set_model(GTK_TREE_VIEW(tabela_pokazujaca_liste), GTK_TREE_MODEL(magazyn_do_list));
    gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(tabela_pokazujaca_liste),TRUE);

    MYSQL *connect = mysql_init(0);
    mysql_options(connect, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(connect, MYSQL_INIT_COMMAND, "SET NAMES utf8");
    /************************/
    scrolled_box_for_books = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(scrolled_box_for_books),tabela_pokazujaca_liste);
    g_object_ref(G_OBJECT(scrolled_box_for_books ));
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_box_for_books), GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(scrolled_box_for_books),3,7,3,9);
    /********************/
    zaznaczenie_elementu_listy[0] = gtk_tree_view_get_selection( GTK_TREE_VIEW( tabela_pokazujaca_liste ) );

    if((connect = mysql_real_connect(connect, "127.0.0.1","root","","sklep_ksiazek",0,NULL,0)) == NULL)
    {
        std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
        error = gtk_message_dialog_new(GTK_WINDOW(okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
        gtk_window_set_title(GTK_WINDOW(error), "Database connection error.");
        gtk_dialog_run(GTK_DIALOG(error));
        gtk_widget_destroy(error);
        return;
    }
    else
    {
        std::string sentence = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora)";
        if(mysql_query(connect,sentence.c_str()))
        {
            std::cout<<"Zle wyrazenie"<<std::endl;
            error = gtk_message_dialog_new(GTK_WINDOW(okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Errors");
            gtk_window_set_title(GTK_WINDOW(error), "Database connection error.");
            gtk_dialog_run(GTK_DIALOG(error));
            gtk_widget_destroy(error);
            return;
        }
        else
        {
            MYSQL_RES *idZapytania = mysql_store_result(connect);
            MYSQL_ROW row;
            GtkTreeIter iter;

            while((row = mysql_fetch_row(idZapytania)) != NULL)
            {
                gtk_list_store_append(magazyn_do_list,&iter);
                gtk_list_store_set(magazyn_do_list, &iter,ID_KSIAZKI,(gint) atoi(row[0]),
                NAZWA_KSIAZKI, row[1], AUTOR, row[2], CENA, (gint) atoi(row[3]), ILOSC,( gint ) atoi(row[4]), - 1 );
            }
            for(int  i = 0; i <= 4; i++ )
            {
                komorka = gtk_cell_renderer_text_new();
                gtk_cell_renderer_set_alignment(komorka,0.5,0.5);
                kolumna = gtk_tree_view_column_new_with_attributes( nazwy_kolumn[ i ], komorka, "text", i,"size",3, NULL );
                gtk_tree_view_append_column( GTK_TREE_VIEW( tabela_pokazujaca_liste ), kolumna );
                delete komorka;
                delete kolumna;
            }
        this->build_menu_tool_bar();
        }
    }
}
/****************************************************************************
*
* /functional BUILDS TOOLS TO MODIFY TABLE OF AVAILABLE BOOKS
*
*****************************************************************************/
void Shop::build_menu_tool_bar()
{
    VBox_wybieranie_sposobu_sortowania_dostepnych_ksiazek = gtk_vbox_new(FALSE,1);
    GtkWidget* opis = gtk_label_new("Sortuj wg:");
    gtk_container_add(GTK_CONTAINER(VBox_wybieranie_sposobu_sortowania_dostepnych_ksiazek), opis);

    GtkWidget *combo_box = gtk_combo_box_text_new();
    for(int i=0;i<8;i++)
    {
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_box),NULL, sposoby_sortowania_ksiazek[i]);
    }

    gtk_container_add(GTK_CONTAINER(VBox_wybieranie_sposobu_sortowania_dostepnych_ksiazek), combo_box);
    g_signal_connect(G_OBJECT(combo_box), "changed", G_CALLBACK(&Shop::sortowanie_dostepnych_ksiazek), this);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),VBox_wybieranie_sposobu_sortowania_dostepnych_ksiazek,3, 4, 2, 3);
    g_object_ref(G_OBJECT(VBox_wybieranie_sposobu_sortowania_dostepnych_ksiazek));
    g_free(combo_box);
    g_free(opis);

    TextBoxSearching = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(TextBoxSearching),"Searching");
    gtk_entry_set_width_chars(GTK_ENTRY(TextBoxSearching),1);
    gtk_table_attach_defaults (GTK_TABLE(box_glowny), TextBoxSearching, 4, 5, 2, 3);
    g_object_ref(G_OBJECT(TextBoxSearching));

    Btn_searching = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(Btn_searching),"Look for");
    g_signal_connect(Btn_searching, "clicked",G_CALLBACK(&Shop::szukanie_ksiazki), this);
    gtk_table_attach_defaults (GTK_TABLE(box_glowny), Btn_searching, 5, 7, 2, 3);
    g_object_ref(G_OBJECT(Btn_searching));

    Btn_dodaj_do_koszyka = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(Btn_dodaj_do_koszyka),"Dodaj do koszyka");
    g_signal_connect(Btn_dodaj_do_koszyka, "clicked",G_CALLBACK(&Koszyk::add_book_to_purchases), this);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny), Btn_dodaj_do_koszyka, 4, 5, 9, 10);
    g_object_ref(G_OBJECT(Btn_dodaj_do_koszyka ));
}
/****************************************************************************
*
* /functional BUILDS TOOLS TO MODIFY TABLE OF ORDERS
*
*****************************************************************************/
void Shop::build_menu_tool_bar_for_orders()
{
    /// utworzenie okna dla sortowania
    VBox_wybieranie_sposobu_sortowania_zamowien = gtk_vbox_new(FALSE,1);
    GtkWidget* opis = gtk_label_new("Sortuj wg:");
    gtk_container_add(GTK_CONTAINER(VBox_wybieranie_sposobu_sortowania_zamowien), opis);

    GtkWidget *combo_box = gtk_combo_box_text_new();

    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_box),NULL, "ID zamowienia +");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_box),NULL, "ID zamowienia -");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_box),NULL, "Imie A-Z");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_box),NULL, "Imie Z-A");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_box),NULL, "Nazwisko A-Z");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_box),NULL, "Nazwisko Z-A");

    gtk_container_add(GTK_CONTAINER(VBox_wybieranie_sposobu_sortowania_zamowien), combo_box);
    g_signal_connect(G_OBJECT(combo_box), "changed", G_CALLBACK(&Shop::sortowanie_zamowien), this);
    g_object_ref(G_OBJECT(VBox_wybieranie_sposobu_sortowania_zamowien));
g_free(combo_box);
}
/***********************************************************************************
*
*   /// method that log off you from shop application,
*   /// and returns to log in app
************************************************************************************/
void Shop::log_off(GtkWidget *target, gpointer arguments)
{
    Shop *temp = static_cast<Shop*>(arguments);
    LoginWindow okno_logowania(temp->uzytkownik);
    temp->uzytkownik->logOff();
    gtk_widget_destroy(temp->okno_programu);

    okno_logowania.build();
    okno_logowania.run();
}

void Shop::build_order()
{
    if(uzytkownik->getAdministator()==true)
    {

    }
    else
    {

    }
}
/***********************************************************************************
*
*   /functions that gonna change between pages
*
************************************************************************************/

void Shop::change_to_orders(GtkWidget *target, gpointer arguments)
{
    Shop* temp = static_cast<Shop*>(arguments);
    if(temp->Active_button == target)
    {

    }
    else if(temp->Active_button == temp->Btn_spis_dostepnych_ksiazek)
    {
        gtk_container_remove(GTK_CONTAINER(temp->box_glowny),temp->Btn_add_info_about_book);
        gtk_container_remove(GTK_CONTAINER(temp->box_glowny),temp->FrameAboutBooks);
        gtk_container_remove(GTK_CONTAINER(temp->box_glowny),temp->scrolled_box_for_info_about_books);
        gtk_container_remove(GTK_CONTAINER(temp->box_glowny),temp->scrolled_box_for_books);

        gtk_table_attach_defaults(GTK_TABLE(temp->box_glowny),temp->scrolled_box_for_orders,3,7,3,9);
        gtk_table_attach_defaults(GTK_TABLE(temp->box_glowny),temp->VBox_wybieranie_sposobu_sortowania_zamowien,3, 4, 2, 3);
        gtk_table_attach_defaults(GTK_TABLE(temp->box_glowny),temp->SCR_BOX_informacje_o_zamowieniu, 7, 9, 3, 9);
        gtk_widget_show_all(temp->okno_programu);
        temp->Active_button = target;
    }
}

void Shop::change_to_spis_ksiazek(GtkWidget *target, gpointer arguments)
{
    Shop* temp = static_cast<Shop*>(arguments);

    if(temp->Active_button == target)
    {
    }
    else if(temp->Active_button == temp->Btn_zamowienie)
    {
        gtk_container_remove(GTK_CONTAINER(temp->box_glowny),temp->scrolled_box_for_orders);
        gtk_container_remove(GTK_CONTAINER(temp->box_glowny),temp->VBox_wybieranie_sposobu_sortowania_zamowien);
        gtk_container_remove(GTK_CONTAINER(temp->box_glowny),temp->SCR_BOX_informacje_o_zamowieniu);

        gtk_table_attach_defaults(GTK_TABLE(temp->box_glowny),temp->Btn_add_info_about_book,5, 6, 9, 10);
        gtk_table_attach_defaults(GTK_TABLE(temp->box_glowny),temp->FrameAboutBooks,7, 10, 3, 9);
        gtk_table_attach_defaults(GTK_TABLE(temp->box_glowny),temp->scrolled_box_for_books,3,7,3,9);
        gtk_table_attach_defaults(GTK_TABLE(temp->box_glowny),temp->scrolled_box_for_info_about_books,7, 9, 3, 9);
        temp->Active_button = target;
    }
}
/***********************************************************************************
*
* Obsluga okien wyszukiwania ksiazek
*
************************************************************************************/
void Shop::sortowanie_zamowien(GtkWidget* target,gpointer arguments)
{
    Shop* temp = static_cast<Shop*>(arguments);
    GtkWidget* object = GTK_WIDGET(target);
    std::string text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(target));
    std::string question_to_database;

    if(text == "ID zamowienia -")
    {
        question_to_database = "SELECT * FROM paczki ORDER BY id_paczki DESC; ";
    }
    if(text == "ID zamowienia +")
    {
        question_to_database = "SELECT * FROM paczki ORDER BY id_paczki; ";
    }
    if(text == "ID zamowienia -")
    {
        question_to_database = "SELECT * FROM paczki ORDER BY i DESC; ";
    }
    if(text == "ID zamowienia -")
    {
        question_to_database = "SELECT * FROM paczki ORDER BY id_paczki DESC; ";
    }

    MYSQL *connect = mysql_init(0);

    if((connect = mysql_real_connect(connect, "127.0.0.1","root","","ksiegarnia",0,NULL,0)) == NULL)
    {
        std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
        temp->error = gtk_message_dialog_new(GTK_WINDOW(temp->okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
        gtk_window_set_title(GTK_WINDOW(temp->error), "Database connection error.");
        gtk_dialog_run(GTK_DIALOG(temp->error));
        gtk_widget_destroy(temp->error);
        return;
    }
    else
    {
        gtk_list_store_clear(temp->magazyn_do_zamowien);
        mysql_query(connect,question_to_database.c_str());

        MYSQL_RES *idZapytania = mysql_store_result(connect);
        MYSQL_ROW row;
        GtkTreeIter iter;

        while((row = mysql_fetch_row(idZapytania)) != NULL)
            {
                std::string zmienna = row[3];
                gtk_list_store_append(temp->magazyn_do_zamowien,&iter);
                gtk_list_store_set(temp->magazyn_do_zamowien, &iter,ID_PACZKI,(gint) atoi(row[0]),
                IMIE_ODBIORCY, row[1], NAZWISKO_ODBIORCY,row[2], CENA_PACZKI,zmienna.c_str(), ODEBRANA,(gboolean) atoi(row[4]), - 1 );
            }
        mysql_free_result(idZapytania);
    }
    g_free(object);
    mysql_close(connect);
}
/***********************************************************************************
*
* Obsluga okien wyszukiwania ksiazek
*
************************************************************************************/
void Shop::sortowanie_dostepnych_ksiazek(GtkWidget *widget, gpointer arguments)
{
    Shop* temp = static_cast<Shop*>(arguments);
    GtkWidget* object = GTK_WIDGET(widget);
    MYSQL *connect = mysql_init(0);
    mysql_options(connect, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(connect, MYSQL_INIT_COMMAND, "SET NAMES utf8");

    if((connect = mysql_real_connect(connect, "127.0.0.1","root","","sklep_ksiazek",0,NULL,0)) == NULL)
    {
        std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
        temp->error = gtk_message_dialog_new(GTK_WINDOW(temp->okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
        gtk_window_set_title(GTK_WINDOW(temp->error), "Database connection error.");
        gtk_dialog_run(GTK_DIALOG(temp->error));
        gtk_widget_destroy(temp->error);
        return;
    }
    else
    {
        std::string text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(widget));
        std::string sentence;
        if(text == "ID ksiazki +")
        {
            sentence = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora) ORDER BY książka.ID_ksiązki";
        }
        else if(text == "ID ksiazki -")
        {
            sentence = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora) ORDER BY książka.ID_ksiązki DESC; ";
        }
        else if(text == "Ksiazki A-Z")
        {
            sentence = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora) ORDER BY książka.Nazwa_książki; ";
        }
        else if(text == "Ksiazki Z-A")
        {
            sentence = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora) ORDER BY książka.Nazwa_książki DESC; ";
        }
        else if(text == "Autor A-Z")
        {
            sentence = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora) ORDER BY autorzy.Imie; ";
        }
        else if(text == "Autor Z-A")
        {
            sentence = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora) ORDER BY autorzy.Imie DESC; ";
        }
        else if(text == "Cena malejac")
        {
            sentence = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora) ORDER BY książka.cena; ";
        }
        else if(text == "Cena rosnac")
        {
            sentence = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora) ORDER BY książka.cena DESC; ";
        }

        gtk_list_store_clear(temp->magazyn_do_list);
        mysql_query(connect,sentence.c_str());

        MYSQL_RES *idZapytania = mysql_store_result(connect);
        MYSQL_ROW row;
        GtkTreeIter iter;

        while((row = mysql_fetch_row(idZapytania)) != NULL)
        {
                gtk_list_store_append(temp->magazyn_do_list,&iter);
                gtk_list_store_set(temp->magazyn_do_list, &iter,ID_KSIAZKI,(gint) atoi(row[0]),
                NAZWA_KSIAZKI, row[1], AUTOR, row[2], CENA, (gint) atoi(row[3]), ILOSC,( gint ) atoi(row[4]), - 1 );
        }
        mysql_free_result(idZapytania);
    }
g_free(object);
mysql_close(connect);
}

void Shop::szukanie_ksiazki(GtkWidget* target,gpointer arguments)
{
    Shop* temp = static_cast<Shop*>(arguments);

    MYSQL *connect = mysql_init(0);
    mysql_options(connect, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(connect, MYSQL_INIT_COMMAND, "SET NAMES utf8");

    if((connect = mysql_real_connect(connect, "127.0.0.1","root","","sklep_ksiazek",0,NULL,0)) == NULL)
    {
        std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
        temp->error = gtk_message_dialog_new(GTK_WINDOW(temp->okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
        gtk_window_set_title(GTK_WINDOW(temp->error), "Database connection error.");
        gtk_dialog_run(GTK_DIALOG(temp->error));
        gtk_widget_destroy(temp->error);
        return;
    }
    else
    {
        const std::string question_about = (std::string)gtk_entry_get_text(GTK_ENTRY(temp->TextBoxSearching));

        if(temp->Active_button==temp->Btn_spis_dostepnych_ksiazek)
        {
            gtk_list_store_clear(temp->magazyn_do_list);
            std::string question_to_database = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora) WHERE książka.Nazwa_książki LIKE '%"+question_about+"%';";

            if(mysql_query(connect,question_to_database.c_str()))
            {
                std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
                temp->error = gtk_message_dialog_new(GTK_WINDOW(temp->okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
                gtk_window_set_title(GTK_WINDOW(temp->error), "Database connection error.");
                gtk_dialog_run(GTK_DIALOG(temp->error));
                gtk_widget_destroy(temp->error);
                return;
            }
            else
            {
                MYSQL_RES *idZapytania = mysql_store_result(connect);
                MYSQL_ROW row;
                GtkTreeIter iter;
                while((row = mysql_fetch_row(idZapytania)) != NULL)
                {
                        gtk_list_store_append(temp->magazyn_do_list,&iter);
                        gtk_list_store_set(temp->magazyn_do_list, &iter,ID_KSIAZKI,(gint) atoi(row[0]),
                        NAZWA_KSIAZKI, row[1], AUTOR, row[2], CENA, (gint) atoi(row[3]), ILOSC,( gint ) atoi(row[4]), - 1 );
                }
                mysql_free_result(idZapytania);
            }
        }
        if(temp->Active_button == temp->Btn_zamowienie)
        {
            gtk_list_store_clear(temp->magazyn_do_zamowien);
            std::string question_to_database = "SELECT zamowienia.ID_Zamówienia,książka.ID_ksiązki, książka.Nazwa_książki,klienci.Imie,klienci.Nazwisko,książka.cena,zamowienia.ilosc,zamowienia.Odebrana FROM((zamowienia INNER JOIN książka ON zamowienia.ID_ksiązki=książka.ID_ksiązki) INNER JOIN klienci ON zamowienia.ID_Klienta=klienci.ID_Klienta) WHERE klienci.Imie LIKE '%"+question_about+"%';";

            if(mysql_query(connect,question_to_database.c_str()))
            {
                std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
                temp->error = gtk_message_dialog_new(GTK_WINDOW(temp->okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
                gtk_window_set_title(GTK_WINDOW(temp->error), "Database connection error.");
                gtk_dialog_run(GTK_DIALOG(temp->error));
                gtk_widget_destroy(temp->error);
                return;
            }
            else
            {
                MYSQL_RES *idZapytania = mysql_store_result(connect);
                MYSQL_ROW row;
                GtkTreeIter iter;
                while((row = mysql_fetch_row(idZapytania)) != NULL)
                {
                        gtk_list_store_append(temp->magazyn_do_zamowien,&iter);
                        gtk_list_store_set(temp->magazyn_do_zamowien, &iter,ID_PACZKI,(gint) atoi(row[0]),ID_KSIAZKI_W_PACZCE,(gint) atoi(row[1]),NAZWA_KSIAZKI_W_PACZCE,row[2],
                        IMIE_ODBIORCY, row[3], NAZWISKO_ODBIORCY,row[4], CENA_PACZKI,(gint) atoi(row[5]),ILOSC_W_PACZCE,(gint) atoi(row[6]), ODEBRANA,(gboolean) atoi(row[7]), - 1 );
                }
                mysql_free_result(idZapytania);
            }
        }
    }
mysql_close(connect);
}

Koszyk* Shop::get_koszyczek()
{
    return koszyczek;
}

GtkTreeSelection* Shop::get_selected(int i)
{
    return this->zaznaczenie_elementu_listy[i];
}

void Shop::set_text_to_info_label(GtkTreeView * treeview, GtkTreePath * path, GtkTreeViewColumn * col, gpointer arguments)
{
    Shop* temp = static_cast<Shop*>(arguments);
    GtkTreeModel * model;
    GtkTreeIter iter;
    model = gtk_tree_view_get_model( treeview );

    if( gtk_tree_model_get_iter( model, & iter, path ) )
    {
        int holds_pressed_book_id;
        gtk_tree_model_get( model, & iter, ID_KSIAZKI,&holds_pressed_book_id, - 1 );

        MYSQL *connect = mysql_init(0);
        mysql_options(connect, MYSQL_SET_CHARSET_NAME, "utf8");
        mysql_options(connect, MYSQL_INIT_COMMAND, "SET NAMES utf8");

        if((connect = mysql_real_connect(connect, "127.0.0.1","root","","ksiegarnia",0,NULL,0)) == NULL)
        {
            std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
            temp->error = gtk_message_dialog_new(GTK_WINDOW(temp->okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
            gtk_window_set_title(GTK_WINDOW(temp->error), "Database connection error.");
            gtk_dialog_run(GTK_DIALOG(temp->error));
            gtk_widget_destroy(temp->error);
            return;
        }
        else
        {
        std::string sentence ="SELECT opis FROM opisy_ksiazek WHERE id_ksiazki ='"+std::to_string(holds_pressed_book_id)+"';";

            if(mysql_query(connect,sentence.c_str()))
            {
                std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
                temp->error = gtk_message_dialog_new(GTK_WINDOW(temp->okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
                gtk_window_set_title(GTK_WINDOW(temp->error), "Database connection error.");
                gtk_dialog_run(GTK_DIALOG(temp->error));
                gtk_widget_destroy(temp->error);
                return;
            }
            else
            {
                MYSQL_RES* idZapytania = mysql_store_result(connect);
                MYSQL_ROW row;
                if((row = mysql_fetch_row(idZapytania))!=NULL)
                {
                    std::string prepare_sentence = row[0];
                    gtk_label_set_text(GTK_LABEL(temp->InfoAboutBooks),prepare_sentence.c_str());
                }
                else
                {
                    gtk_label_set_text(GTK_LABEL(temp->InfoAboutBooks),"Brak informacji o tej książce");
                }
                mysql_free_result(idZapytania);
            }
        }
    mysql_close(connect);
    }
g_free(model);
}

void Shop::add_info_about_book(GtkWidget* target,gpointer arguments)
{
    Shop* temp = static_cast<Shop*>(arguments);
    GtkTreeIter iter;
    GtkTreeModel *model;

    if( gtk_tree_selection_get_selected( temp->zaznaczenie_elementu_listy[0], &model, & iter ))
    {
        int id_wybranej_ksiazki;
        gtk_tree_model_get(model, & iter,ID_KSIAZKI,&id_wybranej_ksiazki,- 1 );

        temp->error = gtk_dialog_new_with_buttons("TITLE",GTK_WINDOW(temp->okno_programu),
        GTK_DIALOG_DESTROY_WITH_PARENT,("_OK"),GTK_RESPONSE_ACCEPT,("_CANCEL"),GTK_RESPONSE_CANCEL,NULL);

        GtkWidget * area = gtk_dialog_get_content_area (GTK_DIALOG (temp->error));
        gtk_container_set_border_width (GTK_CONTAINER(area), 5);

        GtkWidget *entry = gtk_entry_new();
        gtk_widget_set_size_request(entry,200,100);
        gtk_entry_set_text (GTK_ENTRY(entry), gtk_label_get_text(GTK_LABEL(temp->InfoAboutBooks)));

        gtk_container_add(GTK_CONTAINER(area),entry);
        gtk_widget_show_all(temp->error);

        gtk_window_set_title(GTK_WINDOW(temp->error), ("INFO about book nr"+std::to_string(id_wybranej_ksiazki)).c_str());
        int result = gtk_dialog_run(GTK_DIALOG(temp->error));

        switch(result)
        {
            case GTK_RESPONSE_ACCEPT:
            {
                gtk_label_set_text(GTK_LABEL(temp->InfoAboutBooks),gtk_entry_get_text(GTK_ENTRY(entry)));
                break;
            }

            case GTK_RESPONSE_CANCEL:
                {
                    break;
                }
            default: break;
        }
        gtk_widget_destroy(temp->error);
        g_free(entry);
        g_free(area);
    }
g_free(model);
delete temp;
}

void Shop::build_orders_list()
{
    L_informacje_o_zamowieniu = gtk_label_new("Informacje o zamówieniu");
    gtk_label_set_line_wrap(GTK_LABEL(L_informacje_o_zamowieniu),TRUE);

    SCR_BOX_informacje_o_zamowieniu = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(SCR_BOX_informacje_o_zamowieniu),L_informacje_o_zamowieniu);
    g_object_ref(G_OBJECT(SCR_BOX_informacje_o_zamowieniu));

    scrolled_box_for_orders = gtk_scrolled_window_new(NULL, NULL);
    GtkTreeIter iter;
    tabela_pokazujaca_zamowienia = gtk_tree_view_new();
    g_signal_connect(tabela_pokazujaca_zamowienia, "row-activated",G_CALLBACK(&Shop::pobierz_informacje_o_zamowieniu), this);
    gtk_tree_view_columns_autosize(GTK_TREE_VIEW(tabela_pokazujaca_zamowienia));

    magazyn_do_zamowien = gtk_list_store_new(ILOSC_KOLUMN_PACZEK,G_TYPE_INT,G_TYPE_INT,G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_INT,G_TYPE_INT,G_TYPE_BOOLEAN, G_TYPE_INT);
    gtk_tree_view_set_model(GTK_TREE_VIEW(tabela_pokazujaca_zamowienia), GTK_TREE_MODEL(magazyn_do_zamowien));
    gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(tabela_pokazujaca_zamowienia),TRUE);

    gtk_container_add(GTK_CONTAINER(scrolled_box_for_orders),tabela_pokazujaca_zamowienia);
    g_object_ref(G_OBJECT(scrolled_box_for_orders));

    MYSQL *connect = mysql_init(0);
    mysql_options(connect, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(connect, MYSQL_INIT_COMMAND, "SET NAMES utf8");

    if((connect = mysql_real_connect(connect, "127.0.0.1","root","","sklep_ksiazek",0,NULL,0)) == NULL)
    {
        std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
        error = gtk_message_dialog_new(GTK_WINDOW(okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
        gtk_window_set_title(GTK_WINDOW(error), "Database connection error.");
        gtk_dialog_run(GTK_DIALOG(error));
        gtk_widget_destroy(error);
        return;
    }
    else
    {
        std::string sentence = "SELECT zamowienia.ID_Zamówienia,książka.ID_ksiązki, książka.Nazwa_książki,klienci.Imie,klienci.Nazwisko,książka.cena,zamowienia.ilosc,zamowienia.Odebrana FROM((zamowienia INNER JOIN książka ON zamowienia.ID_ksiązki=książka.ID_ksiązki) INNER JOIN klienci ON zamowienia.ID_Klienta=klienci.ID_Klienta)";
        if(mysql_query(connect,sentence.c_str()))
        {
            std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
            error = gtk_message_dialog_new(GTK_WINDOW(okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
            gtk_window_set_title(GTK_WINDOW(error), "Database connection error.");
            gtk_dialog_run(GTK_DIALOG(error));
            gtk_widget_destroy(error);
            return;
        }
        else
        {
            MYSQL_RES *idZapytania = mysql_store_result(connect);
            MYSQL_ROW row;
            GtkTreeIter iter;

            zaznaczenie_elementu_listy[1] = gtk_tree_view_get_selection(GTK_TREE_VIEW(tabela_pokazujaca_zamowienia));

            while((row = mysql_fetch_row(idZapytania)) != NULL)
            {
                gtk_list_store_append(magazyn_do_zamowien,&iter);
                gtk_list_store_set(magazyn_do_zamowien, &iter,ID_PACZKI,(gint) atoi(row[0]),ID_KSIAZKI_W_PACZCE,(gint) atoi(row[1]),NAZWA_KSIAZKI_W_PACZCE,row[2],
                IMIE_ODBIORCY, row[3], NAZWISKO_ODBIORCY,row[4], CENA_PACZKI,(gint) atoi(row[5]),ILOSC_W_PACZCE,(gint) atoi(row[6]), ODEBRANA,(gboolean) atoi(row[7]), - 1 );
            }
            for(int  i = 0; i <= 7; i++ )
            {
                komorka = gtk_cell_renderer_text_new();
                gtk_cell_renderer_set_alignment(komorka,0.5,0.5);
                kolumna = gtk_tree_view_column_new_with_attributes( nazwy_kolumn_paczek[i], komorka, "text", i, NULL );
                gtk_tree_view_append_column( GTK_TREE_VIEW( tabela_pokazujaca_zamowienia ), kolumna );
                delete komorka;
                delete kolumna;
            }
        }
    }
}

void Shop::pobierz_informacje_o_zamowieniu(GtkTreeView * treeview, GtkTreePath * path, GtkTreeViewColumn * col, gpointer arguments)
{
    Shop* temp = static_cast<Shop*>(arguments);
    GtkTreeModel * model;
    GtkTreeIter iter;
    model = gtk_tree_view_get_model( treeview );

    if(gtk_tree_model_get_iter( model, & iter, path ))
    {
        int trzyma_id_paczki;
        gtk_tree_model_get( model, & iter, ID_KSIAZKI,&trzyma_id_paczki, - 1 );

        MYSQL *connect = mysql_init(0);
        mysql_options(connect, MYSQL_SET_CHARSET_NAME, "utf8");
        mysql_options(connect, MYSQL_INIT_COMMAND, "SET NAMES utf8");

        if((connect = mysql_real_connect(connect, "127.0.0.1","root","","ksiegarnia",0,NULL,0)) == NULL)
        {
            std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
            temp->error = gtk_message_dialog_new(GTK_WINDOW(temp->okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
            gtk_window_set_title(GTK_WINDOW(temp->error), "Database connection error.");
            gtk_dialog_run(GTK_DIALOG(temp->error));
            gtk_widget_destroy(temp->error);
            return;
        }
        else
        {
        std::string sentence ="SELECT * FROM paczki WHERE id_paczki = '"+std::to_string(trzyma_id_paczki)+"';";

            if(mysql_query(connect,sentence.c_str()))
            {
                std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
                temp->error = gtk_message_dialog_new(GTK_WINDOW(temp->okno_programu),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
                gtk_window_set_title(GTK_WINDOW(temp->error), "Database connection error.");
                gtk_dialog_run(GTK_DIALOG(temp->error));
                gtk_widget_destroy(temp->error);
                return;
            }
            else
            {
                MYSQL_RES* idZapytania = mysql_store_result(connect);
                MYSQL_ROW row;
                if((row = mysql_fetch_row(idZapytania))!=NULL)
                {
                    std::string prepare_sentence = row[0];
                    gtk_label_set_text(GTK_LABEL(temp->L_informacje_o_zamowieniu),prepare_sentence.c_str());
                }
                else
                {
                    gtk_label_set_text(GTK_LABEL(temp->L_informacje_o_zamowieniu),"Brak informacji o tej książce");
                }
                mysql_free_result(idZapytania);
            }
        }
    mysql_close(connect);
    }
g_free(model);
}

GtkWidget* Shop::get_active_button()
{
    return this->Active_button;
}

GtkWidget* Shop::get_BtnDostepnychKsiazek()
{
    return this->Btn_spis_dostepnych_ksiazek;
}

GtkWidget* Shop::get_BtnZamowien()
{
    return this->Btn_zamowienie;
}
