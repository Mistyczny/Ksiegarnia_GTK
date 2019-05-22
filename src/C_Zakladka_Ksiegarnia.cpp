#include "C_Zakladka_Ksiegarnia.h"

const gchar* kolumny_ksiegarnia[5] = {
    "Id ksiazki",
    "Nazwa ksiazki",
    "Autor",
    "Cena",
    "Ilosc"
};

enum ksiazki
{
    ID_KSIAZKI=0,
    NAZWA_KSIAZKI,
    AUTOR,
    CENA,
    ILOSC,
    ILOSC_KOLUMN
};

const char* sortowanie_ksiazek[8] = {
    "ID ksiazki +",
    "ID ksiazki -",
    "Ksiazki A-Z",
    "Ksiazki Z-A",
    "Autor A-Z",
    "Autor Z-A",
    "Cena rosnąc",
    "Cena malejąc"
};

std::unique_ptr<C_Zakladka_Ksiegarnia> C_Zakladka_Ksiegarnia::C_Zakladka_Ksiegarnia_init(GtkWidget* glowne_okno,GtkWidget* box)
{
    std::cout<<"INIT C_Zakladka_Ksiegarnia"<<std::endl;
    std::unique_ptr<C_Zakladka_Ksiegarnia> p (new C_Zakladka_Ksiegarnia(glowne_okno,box));
return p;
}

C_Zakladka_Ksiegarnia::C_Zakladka_Ksiegarnia(GtkWidget* okno,GtkWidget* box) :  glowne_okno(okno),
                                                                                box_glowny(box)
{
    std::cout<<"TWORZE KSIEGARNIE"<<std::endl;
}

C_Zakladka_Ksiegarnia::~C_Zakladka_Ksiegarnia()
{
    g_object_unref(G_OBJECT(Scr_box_informacje_o_ksiazce));
    g_object_unref(G_OBJECT(Btn_dodaj_informacje_o_ksiazce));
    g_object_unref(G_OBJECT(Scr_box_na_ksiazki));
    g_object_unref(G_OBJECT(VBox_sortowanie_ksiazek));
    std::cout<<"NISZCZE C_ZAKLADKA_KSIEGARNIA"<<std::endl;
}

void C_Zakladka_Ksiegarnia::build()
{

    Scr_box_informacje_o_ksiazce = gtk_scrolled_window_new(NULL, NULL);
    g_object_ref(G_OBJECT(Scr_box_informacje_o_ksiazce));

    L_informacje_o_ksiazce = gtk_label_new("Press book twice to show info about it");
    gtk_label_set_line_wrap(GTK_LABEL(L_informacje_o_ksiazce),TRUE);
    gtk_scrolled_window_add_with_viewport(GTK_SCROLLED_WINDOW(Scr_box_informacje_o_ksiazce),L_informacje_o_ksiazce);

    Btn_dodaj_informacje_o_ksiazce = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(Btn_dodaj_informacje_o_ksiazce), "Dodaj informacje\n  o tej ksiazce");
    g_object_ref(G_OBJECT(Btn_dodaj_informacje_o_ksiazce));
    g_signal_connect(Btn_dodaj_informacje_o_ksiazce, "clicked", G_CALLBACK(&C_Zakladka_Ksiegarnia::dodaj_informacje_o_ksiazce), this);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    tabela_pokazujaca_ksiazki = gtk_tree_view_new();
    g_signal_connect(tabela_pokazujaca_ksiazki, "row-activated",G_CALLBACK(&C_Zakladka_Ksiegarnia::pokaz_informacje_o_ksiazce), this);
    gtk_tree_view_columns_autosize(GTK_TREE_VIEW(tabela_pokazujaca_ksiazki));

    magazyn_na_ksiazki = gtk_list_store_new(ILOSC_KOLUMN,G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_INT,G_TYPE_INT,G_TYPE_INT );
    gtk_tree_view_set_model(GTK_TREE_VIEW(tabela_pokazujaca_ksiazki), GTK_TREE_MODEL(magazyn_na_ksiazki));
    gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(tabela_pokazujaca_ksiazki),TRUE);

    Scr_box_na_ksiazki = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(Scr_box_na_ksiazki),tabela_pokazujaca_ksiazki);
    g_object_ref(G_OBJECT(Scr_box_na_ksiazki ));
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(Scr_box_na_ksiazki), GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);

    selekcja_ksiazki = gtk_tree_view_get_selection(GTK_TREE_VIEW(tabela_pokazujaca_ksiazki));

    GtkTreeViewColumn *kolumna;
    GtkCellRenderer *komorka;
    for(int  i = 0; i <= 4; i++ )
    {
        komorka = gtk_cell_renderer_text_new();
        gtk_cell_renderer_set_alignment(komorka,0.5,0.5);
        kolumna = gtk_tree_view_column_new_with_attributes( kolumny_ksiegarnia[i], komorka, "text", i, NULL );
        gtk_tree_view_append_column( GTK_TREE_VIEW( tabela_pokazujaca_ksiazki ), kolumna );
    }
    this->zaladuj_ksiazki();

    VBox_sortowanie_ksiazek = gtk_vbox_new(FALSE,1);
    GtkWidget* opis = gtk_label_new("Sortuj wg:");
    gtk_container_add(GTK_CONTAINER(VBox_sortowanie_ksiazek), opis);

    GtkWidget *combo_box = gtk_combo_box_text_new();
    for(int i=0;i<8;i++)
    {
        gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo_box),NULL, sortowanie_ksiazek[i]);
    }

    gtk_container_add(GTK_CONTAINER(VBox_sortowanie_ksiazek), combo_box);
    g_signal_connect(G_OBJECT(combo_box), "changed", G_CALLBACK(&C_Zakladka_Ksiegarnia::sortuj_ksiazki), this);
    g_object_ref(G_OBJECT(VBox_sortowanie_ksiazek));

    Btn_szukaj = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(Btn_szukaj),"Look for");
    g_signal_connect(Btn_szukaj, "clicked",G_CALLBACK(&C_Zakladka_Ksiegarnia::szukaj_ksiazki), this);
    g_object_ref(G_OBJECT(Btn_szukaj));

    E_szukaj = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(E_szukaj),"Searching");
    gtk_entry_set_width_chars(GTK_ENTRY(E_szukaj),1);
    g_object_ref(G_OBJECT(E_szukaj));
}

void C_Zakladka_Ksiegarnia::zaladuj_ksiazki()
{
    Baza_danych baza;
    std::string sentence = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora)";
    MYSQL_RES* result = baza.wyslij_pytanie(sentence);
    MYSQL_ROW row;
    GtkTreeIter iter;

    while((row = mysql_fetch_row(result)) != NULL)
    {
        gtk_list_store_append(magazyn_na_ksiazki,&iter);
        gtk_list_store_set(magazyn_na_ksiazki, &iter,ID_KSIAZKI,(gint) atoi(row[0]),
        NAZWA_KSIAZKI, row[1], AUTOR, row[2], CENA, (gint) atoi(row[3]), ILOSC,( gint ) atoi(row[4]), - 1 );
    }
}

void C_Zakladka_Ksiegarnia::pokaz_informacje_o_ksiazce(GtkTreeView * treeview, GtkTreePath * path, GtkTreeViewColumn * col, gpointer arguments)
{
    C_Zakladka_Ksiegarnia* temp = static_cast<C_Zakladka_Ksiegarnia*>(arguments);
    GtkTreeModel * model =  gtk_tree_view_get_model( treeview );
    GtkTreeIter iter;

    if( gtk_tree_model_get_iter( model, & iter, path ) )
    {
        int id_ksiazki;
        gtk_tree_model_get( model, & iter, ID_KSIAZKI,&id_ksiazki, - 1 );
        Baza_danych baza;
        std::string sentence = "SELECT opis FROM książka WHERE książka.ID_Ksiązki ='"+std::to_string(id_ksiazki)+"';";
        MYSQL_RES* result = baza.wyslij_pytanie(sentence);
        MYSQL_ROW row;

        if((row = mysql_fetch_row(result))!=NULL)
        {
            gtk_label_set_text(GTK_LABEL(temp->L_informacje_o_ksiazce),row[0]);
        }
        else
        {
            gtk_label_set_text(GTK_LABEL(temp->L_informacje_o_ksiazce),"Brak informacji o tej książce");
        }
    mysql_free_result(result);
    }
}

void C_Zakladka_Ksiegarnia::dodaj_informacje_o_ksiazce(GtkWidget* target,gpointer arguments)
{
    C_Zakladka_Ksiegarnia* temp = static_cast<C_Zakladka_Ksiegarnia*>(arguments);
    GtkTreeIter iter;
    GtkTreeModel *model;

    if( gtk_tree_selection_get_selected( temp->selekcja_ksiazki, &model, & iter ))
    {
        int id_wybranej_ksiazki;
        gtk_tree_model_get(model, & iter,ID_KSIAZKI,&id_wybranej_ksiazki,- 1 );

        temp->message = gtk_dialog_new_with_buttons("TITLE",GTK_WINDOW(temp->glowne_okno),
        GTK_DIALOG_DESTROY_WITH_PARENT,("_OK"),GTK_RESPONSE_ACCEPT,("_CANCEL"),GTK_RESPONSE_CANCEL,NULL);
        gtk_window_set_title(GTK_WINDOW(temp->message), ("INFO about book nr"+std::to_string(id_wybranej_ksiazki)).c_str());

        GtkWidget* area = gtk_dialog_get_content_area(GTK_DIALOG(temp->message));
        gtk_container_set_border_width (GTK_CONTAINER(area),5);

        GtkWidget *entry = gtk_entry_new();
        gtk_widget_set_size_request(entry,200,100);
        gtk_entry_set_text (GTK_ENTRY(entry), gtk_label_get_text(GTK_LABEL(temp->L_informacje_o_ksiazce)));

        gtk_container_add(GTK_CONTAINER(area),entry);
        gtk_widget_show_all(temp->message);

        switch(gtk_dialog_run(GTK_DIALOG(temp->message)))
        {
            case GTK_RESPONSE_ACCEPT:
            {
                gtk_label_set_text(GTK_LABEL(temp->L_informacje_o_ksiazce),gtk_entry_get_text(GTK_ENTRY(entry)));
                Baza_danych baza;
                std::string sentence="UPDATE książka SET opis='"+static_cast<std::string>(gtk_label_get_text(GTK_LABEL(temp->L_informacje_o_ksiazce)))
                +"' WHERE książka.ID_ksiązki='"+std::to_string(id_wybranej_ksiazki)+"'";
                baza.wyslij_pytanie(sentence);
                break;
            }
            case GTK_RESPONSE_CANCEL:
            {
                break;
            }
            default: break;
        }
        gtk_widget_destroy(temp->message);
    }
}

void C_Zakladka_Ksiegarnia::pokaz_widzety()
{
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Scr_box_informacje_o_ksiazce), 7, 10, 3, 9);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_dodaj_informacje_o_ksiazce), 5, 6, 9, 10);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Scr_box_na_ksiazki),3,7,3,9);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(VBox_sortowanie_ksiazek),3, 4, 2,3);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_szukaj), 5, 7, 2, 3);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(E_szukaj), 4, 5, 2, 3);
}

void C_Zakladka_Ksiegarnia::schowaj_widzety()
{
    gtk_container_remove(GTK_CONTAINER(box_glowny),Btn_dodaj_informacje_o_ksiazce);
    gtk_container_remove(GTK_CONTAINER(box_glowny),Scr_box_informacje_o_ksiazce);
    gtk_container_remove(GTK_CONTAINER(box_glowny),Scr_box_na_ksiazki);
    gtk_container_remove(GTK_CONTAINER(box_glowny),VBox_sortowanie_ksiazek);
    gtk_container_remove(GTK_CONTAINER(box_glowny),Btn_szukaj);
    gtk_container_remove(GTK_CONTAINER(box_glowny),E_szukaj);
}

void C_Zakladka_Ksiegarnia::sortuj_ksiazki(GtkWidget* target, gpointer arguments)
{
    C_Zakladka_Ksiegarnia* temp = static_cast<C_Zakladka_Ksiegarnia*>(arguments);
    Baza_danych baza;
    std::string text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(target));
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
    else if(text == "Cena rosnąc")
    {
        sentence = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora) ORDER BY książka.cena; ";
    }
    else if(text == "Cena malejąc")
    {
        sentence = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora) ORDER BY książka.cena DESC; ";
    }

    gtk_list_store_clear(temp->magazyn_na_ksiazki);
    MYSQL_RES* result = baza.wyslij_pytanie(sentence);
    MYSQL_ROW row;
    GtkTreeIter iter;

    while((row = mysql_fetch_row(result)) != NULL)
    {
        gtk_list_store_append(temp->magazyn_na_ksiazki,&iter);
        gtk_list_store_set(temp->magazyn_na_ksiazki, &iter,ID_KSIAZKI,(gint) atoi(row[0]),
        NAZWA_KSIAZKI, row[1], AUTOR, row[2], CENA, (gint) atoi(row[3]), ILOSC,( gint ) atoi(row[4]), - 1 );
    }
mysql_free_result(result);
}


void C_Zakladka_Ksiegarnia::szukaj_ksiazki(GtkWidget* target,gpointer arguments)
{
    C_Zakladka_Ksiegarnia* temp = static_cast<C_Zakladka_Ksiegarnia*>(arguments);

    Baza_danych baza;
    std::string zapytanie = "SELECT książka.ID_ksiązki, książka.Nazwa_książki,autorzy.Imie,książka.cena,Dostępność FROM((baza_książek INNER JOIN książka ON baza_książek.ID_ksiązki=książka.ID_ksiązki) INNER JOIN autorzy ON baza_książek.ID_Autora=autorzy.ID_Autora) WHERE książka.Nazwa_książki LIKE '%"+(std::string)gtk_entry_get_text(GTK_ENTRY(temp->E_szukaj))+"%';";
    MYSQL_RES *result = baza.wyslij_pytanie(zapytanie);
    MYSQL_ROW row;
    GtkTreeIter iter;
    gtk_list_store_clear(temp->magazyn_na_ksiazki);

    while((row = mysql_fetch_row(result)) != NULL)
    {
        gtk_list_store_append(temp->magazyn_na_ksiazki,&iter);
        gtk_list_store_set(temp->magazyn_na_ksiazki, &iter,ID_KSIAZKI,(gint) atoi(row[0]),
        NAZWA_KSIAZKI, row[1], AUTOR, row[2], CENA, (gint) atoi(row[3]), ILOSC,( gint ) atoi(row[4]), - 1 );
    }
    mysql_free_result(result);
}

GtkTreeSelection* C_Zakladka_Ksiegarnia::get_selekcja_ksiazki()
{
    return this->selekcja_ksiazki;
}
