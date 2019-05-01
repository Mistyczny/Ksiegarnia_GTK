#include "C_Zakladka_Zamowienia.h"
#include "Informacje_o_Zamowieniu.h"

const gchar* kolumny_paczki[8] = {
    "Id zamowienia",
    "Imie",
    "Nazwisko",
    "Cena",
    "Przewidywany odbior",
    "Odebrana"
};

enum paczka
{
    ID_PACZKI=0,
    IMIE_ODBIORCY,
    NAZWISKO_ODBIORCY,
    CENA_PACZKI,
    DATA,
    ODEBRANA,
    ILOSC_KOLUMN_PACZEK
};

std::unique_ptr<C_Zakladka_Zamowienia> C_Zakladka_Zamowienia::C_Zakladka_Zamowienia_init(GtkWidget* glowne_okno,GtkWidget* box)
{
    std::cout<<"INIT C_Zakladka_zamowienia"<<std::endl;
    std::unique_ptr<C_Zakladka_Zamowienia> p (new C_Zakladka_Zamowienia(glowne_okno,box));
return p;
}

C_Zakladka_Zamowienia::C_Zakladka_Zamowienia(GtkWidget* glowne_okno_,GtkWidget* box)
{
    this->okno_glowne = glowne_okno_;
    this->box_glowny = box;
}

C_Zakladka_Zamowienia::~C_Zakladka_Zamowienia()
{
    std::cout<<"ZABIJAM C_ZAKLADKA_ZAMOWIENIA"<<std::endl;
    g_object_unref(G_OBJECT(Scr_box_zamowienia));
    g_object_unref(G_OBJECT(VBox_wybieranie_sposobu_sortowania_zamowien));
}

void C_Zakladka_Zamowienia::build()
{
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
    g_signal_connect(G_OBJECT(combo_box), "changed", G_CALLBACK(&C_Zakladka_Zamowienia::sortowanie_zamowien), this);
    g_object_ref(G_OBJECT(VBox_wybieranie_sposobu_sortowania_zamowien));


    Scr_box_zamowienia = gtk_scrolled_window_new(NULL, NULL);
    tabela_pokazujaca_zamowienia = gtk_tree_view_new();
    g_signal_connect(tabela_pokazujaca_zamowienia, "row-activated",G_CALLBACK(&C_Zakladka_Zamowienia::pobierz_informacje_o_zamowieniu), this);
    gtk_tree_view_columns_autosize(GTK_TREE_VIEW(tabela_pokazujaca_zamowienia));
    g_object_ref(G_OBJECT(Scr_box_zamowienia));

    magazyn_do_zamowien = gtk_list_store_new(ILOSC_KOLUMN_PACZEK,G_TYPE_INT,G_TYPE_STRING, G_TYPE_STRING,G_TYPE_INT,G_TYPE_STRING,G_TYPE_BOOLEAN, G_TYPE_INT);
    gtk_tree_view_set_model(GTK_TREE_VIEW(tabela_pokazujaca_zamowienia), GTK_TREE_MODEL(magazyn_do_zamowien));
    gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(tabela_pokazujaca_zamowienia),TRUE);

    gtk_container_add(GTK_CONTAINER(Scr_box_zamowienia),tabela_pokazujaca_zamowienia);

    GtkTreeViewColumn *kolumna;
    GtkCellRenderer *komorka;
    for(int  i = 0; i <= 5; i++ )
    {
        komorka = gtk_cell_renderer_text_new();
        gtk_cell_renderer_set_alignment(komorka,0.5,0.5);
        kolumna = gtk_tree_view_column_new_with_attributes( kolumny_paczki[i], komorka, "text", i, NULL );
        gtk_tree_view_append_column( GTK_TREE_VIEW( tabela_pokazujaca_zamowienia ), kolumna );
    }
    this->zaladuj_zamowienia();
}

void C_Zakladka_Zamowienia::pokaz_widzety()
{
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Scr_box_zamowienia),3,7,3,9);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(VBox_wybieranie_sposobu_sortowania_zamowien),3, 4, 2, 3);

}

void C_Zakladka_Zamowienia::schowaj_widzety()
{
    gtk_container_remove(GTK_CONTAINER(box_glowny),Scr_box_zamowienia);
    gtk_container_remove(GTK_CONTAINER(box_glowny),VBox_wybieranie_sposobu_sortowania_zamowien);
}

void C_Zakladka_Zamowienia::sortowanie_zamowien(GtkWidget* target,gpointer arguments)
{
    C_Zakladka_Zamowienia* temp = static_cast<C_Zakladka_Zamowienia*>(arguments);
    std::string text = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(target));
    std::string question_to_database;

    if(text == "ID zamowienia -")
    {
        question_to_database = "SELECT DISTINCT zamowienia.ID_Zamówienia,klienci.Imie,klienci.Nazwisko,SUM(książka.cena*zamowienia.ilosc) OVER(PARTITION BY zamowienia.ID_Zamówienia),zamowienia.Czas_Dostawy,zamowienia.Odebrana FROM((zamowienia INNER JOIN książka ON zamowienia.ID_ksiązki=książka.ID_ksiązki) INNER JOIN klienci ON zamowienia.ID_Klienta=klienci.ID_Klienta) ORDER BY zamowienia.ID_Zamówienia;";
    }
    else if(text == "ID zamowienia +")
    {
        question_to_database = "SELECT DISTINCT zamowienia.ID_Zamówienia,klienci.Imie,klienci.Nazwisko,SUM(książka.cena*zamowienia.ilosc) OVER(PARTITION BY zamowienia.ID_Zamówienia),zamowienia.Czas_Dostawy,zamowienia.Odebrana FROM((zamowienia INNER JOIN książka ON zamowienia.ID_ksiązki=książka.ID_ksiązki) INNER JOIN klienci ON zamowienia.ID_Klienta=klienci.ID_Klienta) ORDER BY zamowienia.ID_Zamówienia DESC;";
    }
    else if(text == "Imie A-Z")
    {
        question_to_database = "SELECT DISTINCT zamowienia.ID_Zamówienia,klienci.Imie,klienci.Nazwisko,SUM(książka.cena*zamowienia.ilosc) OVER(PARTITION BY zamowienia.ID_Zamówienia),zamowienia.Czas_Dostawy,zamowienia.Odebrana FROM((zamowienia INNER JOIN książka ON zamowienia.ID_ksiązki=książka.ID_ksiązki) INNER JOIN klienci ON zamowienia.ID_Klienta=klienci.ID_Klienta) ORDER BY klienci.Imie;";
    }
    else if(text == "Imie Z-A")
    {
        question_to_database = "SELECT DISTINCT zamowienia.ID_Zamówienia,klienci.Imie,klienci.Nazwisko,SUM(książka.cena*zamowienia.ilosc) OVER(PARTITION BY zamowienia.ID_Zamówienia),zamowienia.Czas_Dostawy,zamowienia.Odebrana FROM((zamowienia INNER JOIN książka ON zamowienia.ID_ksiązki=książka.ID_ksiązki) INNER JOIN klienci ON zamowienia.ID_Klienta=klienci.ID_Klienta) ORDER BY zamowienia.ID_Zamówienia DESC;";
    }
    else if(text == "Nazwisko A-Z")
    {
        question_to_database = "SELECT DISTINCT zamowienia.ID_Zamówienia,klienci.Imie,klienci.Nazwisko,SUM(książka.cena*zamowienia.ilosc) OVER(PARTITION BY zamowienia.ID_Zamówienia),zamowienia.Czas_Dostawy,zamowienia.Odebrana FROM((zamowienia INNER JOIN książka ON zamowienia.ID_ksiązki=książka.ID_ksiązki) INNER JOIN klienci ON zamowienia.ID_Klienta=klienci.ID_Klienta) ORDER BY klienci.Nazwisko;";
    }
    else if(text == "Nazwisko Z-A")
    {
        question_to_database = "SELECT DISTINCT zamowienia.ID_Zamówienia,klienci.Imie,klienci.Nazwisko,SUM(książka.cena*zamowienia.ilosc) OVER(PARTITION BY zamowienia.ID_Zamówienia),zamowienia.Czas_Dostawy,zamowienia.Odebrana FROM((zamowienia INNER JOIN książka ON zamowienia.ID_ksiązki=książka.ID_ksiązki) INNER JOIN klienci ON zamowienia.ID_Klienta=klienci.ID_Klienta) ORDER BY klienci.Nazwisko DESC;";
    }
    else return;

    gtk_list_store_clear(temp->magazyn_do_zamowien);
    Baza_danych baza;
    MYSQL_RES *result = baza.wyslij_pytanie(question_to_database);
    MYSQL_ROW row;
    GtkTreeIter iter;

    while((row = mysql_fetch_row(result)) != NULL)
    {
        char* zmienna = row[4];
        gtk_list_store_append(temp->magazyn_do_zamowien,&iter);
        gtk_list_store_set(temp->magazyn_do_zamowien, &iter,ID_PACZKI,(gint) atoi(row[0]),IMIE_ODBIORCY,row[1],NAZWISKO_ODBIORCY,row[2],
        CENA_PACZKI,(gint) atoi(row[3]), DATA,zmienna,ODEBRANA,(gboolean) atoi(row[5]), - 1 );
    }
    mysql_free_result(result);
}

void C_Zakladka_Zamowienia::pobierz_informacje_o_zamowieniu(GtkTreeView * treeview, GtkTreePath * path, GtkTreeViewColumn * col, gpointer arguments)
{
    C_Zakladka_Zamowienia* temp = static_cast<C_Zakladka_Zamowienia*>(arguments);
    GtkTreeModel* model = gtk_tree_view_get_model(treeview);
    GtkTreeIter iter;

    if(gtk_tree_model_get_iter( model, & iter, path ))
    {
        int trzyma_id_paczki;
        gtk_tree_model_get( model, & iter, ID_PACZKI,&trzyma_id_paczki, - 1 );
        Baza_danych baza;

        std::string sentence ="SELECT klienci.Imie,klienci.Nazwisko,książka.Nazwa_książki,książka.cena,zamowienia.ilosc FROM((zamowienia INNER JOIN książka ON zamowienia.ID_ksiązki=książka.ID_ksiązki) INNER JOIN klienci ON zamowienia.ID_Klienta=klienci.ID_Klienta) WHERE ID_Zamówienia ='"
        +std::to_string(trzyma_id_paczki)+"';";

        MYSQL_RES* result = baza.wyslij_pytanie(sentence);
        MYSQL_ROW row;

        Str_informacje_o_zamowieniu informacje;

        if((row = mysql_fetch_row(result))!=NULL)
        {
            informacje.imie_i_nazwisko = row[0];
            informacje.imie_i_nazwisko +=' ';
            informacje.imie_i_nazwisko +=row[1];
            do{
                informacje.zamowione_ksiazki.push_back(row[2]);
                informacje.ilosc_ksiazki.push_back(row[4]);
                informacje.ceny_ksiazek.push_back(row[3]);
            }
            while((row = mysql_fetch_row(result)) != NULL);
            informacje.prepare();

            temp->message = gtk_message_dialog_new(GTK_WINDOW(temp->okno_glowne),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, informacje.gotowe_zdanie.c_str());
            gtk_window_set_title(GTK_WINDOW(temp->message), "INFORMACJA O ZAMOWIENIU");
            gtk_dialog_run(GTK_DIALOG(temp->message));
            gtk_widget_destroy(temp->message);
        }
    mysql_free_result(result);
    }
}

void C_Zakladka_Zamowienia::zaladuj_zamowienia()
{
    Baza_danych baza;
    std::string sentence = "SELECT DISTINCT zamowienia.ID_Zamówienia,klienci.Imie,klienci.Nazwisko,SUM(książka.cena*zamowienia.ilosc) OVER(PARTITION BY zamowienia.ID_Zamówienia),zamowienia.Czas_Dostawy,zamowienia.Odebrana FROM((zamowienia INNER JOIN książka ON zamowienia.ID_ksiązki=książka.ID_ksiązki) INNER JOIN klienci ON zamowienia.ID_Klienta=klienci.ID_Klienta)";
    MYSQL_RES *result = baza.wyslij_pytanie(sentence);
    MYSQL_ROW row;
    GtkTreeIter iter;

    while((row = mysql_fetch_row(result)) != NULL)
    {
        char* zmienna = row[4];
        gtk_list_store_append(magazyn_do_zamowien,&iter);
        gtk_list_store_set(magazyn_do_zamowien, &iter,ID_PACZKI,(gint) atoi(row[0]),IMIE_ODBIORCY,row[1],NAZWISKO_ODBIORCY,row[2],
        CENA_PACZKI,(gint) atoi(row[3]), DATA,zmienna,ODEBRANA,(gboolean) atoi(row[5]), - 1 );
    }
}
