#include "Terminarz.h"

enum godziny
{
    KTO_PRACUJE=0,
    KIEDY,
    GODZINA_WEJSCIA,
    GODZINA_WYJSCIA,
    KOLUMNY_GODZIN
};

const gchar* nazwy_kolumn_rozpiska[4] ={
    "ID Prac",
    "Data",
    "Wejscie",
    "Wyjscie",
};
std::unique_ptr<Terminarz> Terminarz::Terminarz_init(GtkWidget* box_)
{
    std::cout<<"INIT TERMINARZ"<<std::endl;
    std::unique_ptr<Terminarz> p (new Terminarz(box_));
return p;
}

Terminarz::Terminarz(GtkWidget* box_) : box_glowny(box_)
{
    try{
    scr_tabela_godzin = gtk_scrolled_window_new(NULL, NULL);
    magazyn_godzin_pracy = gtk_list_store_new(KOLUMNY_GODZIN,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_INT);
    tabela_godzin = gtk_tree_view_new();
    g_object_ref(G_OBJECT(scr_tabela_godzin));
    gtk_tree_view_set_model(GTK_TREE_VIEW(tabela_godzin), GTK_TREE_MODEL(magazyn_godzin_pracy));
    selekcja_tabeli_godzin= gtk_tree_view_get_selection(GTK_TREE_VIEW(tabela_godzin));
    gtk_container_add(GTK_CONTAINER(scr_tabela_godzin),tabela_godzin);

    Baza_danych baza;
    std::string zapytanie = "SELECT pracownicy.ID_Pracownika,Godzina_wejscia,Godzina_wyjscia,Data FROM dzien_pracy INNER JOIN pracownicy ON dzien_pracy.ID_Pracownika=pracownicy.Id_Pracownika WHERE WEEK('SYSDATE()')=WEEK(dzien_pracy.DATA)";

    MYSQL_RES* result = baza.wyslij_zapytanie(zapytanie);
    GtkTreeIter iter;

    MYSQL_ROW row = mysql_fetch_row(result);
    GtkTreeViewColumn *kolumna;
    GtkCellRenderer *komorka;

    while((row = mysql_fetch_row(result)) != NULL)
    {
        gtk_list_store_append(magazyn_godzin_pracy,&iter);
        gtk_list_store_set(magazyn_godzin_pracy, &iter,KTO_PRACUJE,row[0],KIEDY,row[3],GODZINA_WEJSCIA,row[1],GODZINA_WYJSCIA,row[2], - 1 );
    }
    for(int  i = 0; i <= 3; i++ )
    {
        komorka = gtk_cell_renderer_text_new();
        gtk_cell_renderer_set_alignment(komorka,0.5,0.5);
        kolumna = gtk_tree_view_column_new_with_attributes( nazwy_kolumn_rozpiska[i], komorka, "text", i, NULL );
        gtk_tree_view_append_column( GTK_TREE_VIEW( tabela_godzin ), kolumna );

        if(i==0)
        {
            g_object_set( komorka, "editable", TRUE, NULL );
            g_object_set_data (G_OBJECT (komorka), "column", GINT_TO_POINTER (KTO_PRACUJE));
            g_signal_connect(komorka, "edited", (GCallback)&Terminarz::zmiana_komorki, this);
        }
        if(i==2)
        {
            g_object_set( komorka, "editable", TRUE, NULL );
            g_object_set_data(G_OBJECT (komorka), "column", GINT_TO_POINTER (GODZINA_WEJSCIA));
            g_signal_connect(komorka, "edited", (GCallback)&Terminarz::zmiana_komorki, this);
        }
        if(i==3)
        {
            g_object_set( komorka, "editable", TRUE, NULL );
            g_object_set_data (G_OBJECT (komorka), "column", GINT_TO_POINTER (GODZINA_WYJSCIA));
            g_signal_connect(komorka, "edited", (GCallback)&Terminarz::zmiana_komorki, this);
        }
    }

    this->kalendarz = gtk_calendar_new();
    g_object_ref(G_OBJECT(kalendarz));
    g_signal_connect(this->kalendarz, "day-selected",G_CALLBACK(&Terminarz::wypelnianie_rozpiski), this);

    Btn_zapisz_zmiany_godziny = gtk_button_new_with_label("Zapisz zmiany");
    g_object_ref(G_OBJECT(Btn_zapisz_zmiany_godziny));
    g_signal_connect(G_OBJECT(Btn_zapisz_zmiany_godziny),"clicked",G_CALLBACK(&Terminarz::wprowadz_zmiany_w_planie_pracownika),this);

    Btn_usun_termin = gtk_button_new_with_label("Usun termin");
    g_object_ref(G_OBJECT(Btn_usun_termin));
    g_signal_connect(G_OBJECT(Btn_usun_termin),"clicked",G_CALLBACK(&Terminarz::usun_termin),this);

    Btn_dodaj_termin = gtk_button_new_with_label("Dodaj termin");
    g_object_ref(G_OBJECT(Btn_dodaj_termin));
    g_signal_connect(G_OBJECT(Btn_dodaj_termin),"clicked",G_CALLBACK(&Terminarz::dodaj_termin),this);
    }
    catch(std::bad_alloc& a)
    {
        a.what();
    }
}

Terminarz::~Terminarz()
{
    std::cout<<"NISZCZE TERMINARZ"<<std::endl;
    g_object_unref(G_OBJECT(scr_tabela_godzin));
    g_object_unref(G_OBJECT(kalendarz));
    g_object_unref(G_OBJECT(Btn_zapisz_zmiany_godziny));
    g_object_unref(G_OBJECT(Btn_usun_termin));
    g_object_unref(G_OBJECT(Btn_dodaj_termin));
}

void Terminarz::build()
{
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(scr_tabela_godzin),7,9,2,4);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_zapisz_zmiany_godziny),7,8,4,5);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_usun_termin),8,9,4,5);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_dodaj_termin),9,10,4,5);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(kalendarz),7,10,5,7);
}

void Terminarz::schowaj_widzety()
{
    gtk_container_remove(GTK_CONTAINER(box_glowny),scr_tabela_godzin);
    gtk_container_remove(GTK_CONTAINER(box_glowny),Btn_zapisz_zmiany_godziny);
    gtk_container_remove(GTK_CONTAINER(box_glowny),Btn_dodaj_termin);
    gtk_container_remove(GTK_CONTAINER(box_glowny),Btn_usun_termin);
    gtk_container_remove(GTK_CONTAINER(box_glowny),kalendarz);
}

void Terminarz::wprowadz_zmiany_w_planie_pracownika(GtkWidget *target, gpointer arguments)
{
    Terminarz* temp = static_cast<Terminarz*>(arguments);

    GtkTreeModel * model = gtk_tree_view_get_model(GTK_TREE_VIEW(temp->tabela_godzin));
    GtkTreeIter iter;
    Baza_danych baza;
    gboolean valid = gtk_tree_model_get_iter_first (model,&iter);
    gchar *id_prac,*data,*godz_wej,*godz_wyj;

    while(valid)
    {
        gtk_tree_model_get( model,&iter,KTO_PRACUJE,&id_prac,KIEDY,&data,GODZINA_WEJSCIA,&godz_wej,GODZINA_WYJSCIA,&godz_wyj,-1 );
        std::string zapytanie = "UPDATE dzien_pracy SET ID_Pracownika='"+static_cast<std::string>(id_prac)
        +"',Godzina_wejscia='"+static_cast<std::string>(godz_wej)+"',Godzina_wyjscia='"+static_cast<std::string>(godz_wyj)
        +"' WHERE Data='"+static_cast<std::string>(data)+"' AND ID_Pracownika='"+static_cast<std::string>(id_prac)+"'";
        baza.wyslij_zapytanie(zapytanie);
        valid = gtk_tree_model_iter_next (model,&iter);
    }
g_free(id_prac);
g_free(data);
g_free(godz_wej);
g_free(godz_wyj);
}

void Terminarz::zmiana_komorki(GtkCellRendererText *cell,gchar* path_,gchar *new_text,gpointer arguments)
{
    Terminarz* temp = static_cast<Terminarz*>(arguments);
    GtkTreeModel *model = (GtkTreeModel *)(temp->magazyn_godzin_pracy);
    GtkTreePath *path = gtk_tree_path_new_from_string (path_);
    GtkTreeIter iter;
    gint column = GPOINTER_TO_INT(g_object_get_data (G_OBJECT (cell), "column"));
    gtk_tree_model_get_iter (model, &iter, path);

    switch (column)
    {
        case KTO_PRACUJE:
        {
            gtk_list_store_set(GTK_LIST_STORE(model), &iter,column,new_text, -1);
            break;
        }
        case GODZINA_WEJSCIA:
        {
            gtk_list_store_set(GTK_LIST_STORE(model), &iter,column,new_text, -1);
            break;
        }
        case GODZINA_WYJSCIA:
        {
            gtk_list_store_set(GTK_LIST_STORE(model), &iter,column,new_text, -1);
            break;
        }
    }
    gtk_tree_path_free (path);
}

/*********************************************************************************************************
*
* \brief fill tabela_godzin with information obtained from database
* \brief get pressed date on calendar
* \return void
*
**********************************************************************************************************/
void Terminarz::wypelnianie_rozpiski(GtkWidget *target, gpointer arguments)
{
    Terminarz* temp = static_cast<Terminarz*>(arguments);

    guint year;
    guint month;
    guint day;
    gtk_calendar_get_date(GTK_CALENDAR(target),&year,&month,&day);
    std::string data = std::to_string(GPOINTER_TO_INT(year))+"-"+std::to_string(GPOINTER_TO_INT(month))+"-"+std::to_string(GPOINTER_TO_INT(day));

    Baza_danych baza;
    std::string zapytanie = "SELECT ID_Pracownika,Godzina_wejscia,Godzina_wyjscia,Data FROM dzien_pracy WHERE WEEK('"+data+"')=WEEK(dzien_pracy.DATA) ORDER BY Data";

    MYSQL_RES* result = baza.wyslij_zapytanie(zapytanie);
    MYSQL_ROW row;
    gtk_list_store_clear(temp->magazyn_godzin_pracy);
    GtkTreeIter iter;

    while((row = mysql_fetch_row(result)) != NULL)
    {
        gtk_list_store_append(temp->magazyn_godzin_pracy,&iter);
        gtk_list_store_set(temp->magazyn_godzin_pracy, &iter,KTO_PRACUJE,row[0],KIEDY,row[3],GODZINA_WEJSCIA,row[1],GODZINA_WYJSCIA,row[2], - 1 );
    }
}

void Terminarz::usun_termin(GtkWidget *target, gpointer arguments)
{
    Terminarz* temp = static_cast<Terminarz*>(arguments);
    GtkTreeModel * model;
    GtkTreeIter iter;

    if( gtk_tree_selection_get_selected(temp->selekcja_tabeli_godzin, &model, &iter ))
    {
        gchar* id_pracownika;
        gchar* data;
        gchar* from;
        gchar* to;
        gtk_tree_model_get(model, & iter,KTO_PRACUJE,&id_pracownika,KIEDY,&data,GODZINA_WEJSCIA,&from,GODZINA_WYJSCIA,&to,- 1 );

        Baza_danych baza;
        std::string zapytanie = "DELETE FROM dzien_pracy WHERE ID_Pracownika='"+static_cast<std::string>(id_pracownika)
        +"' AND Data='"+static_cast<std::string>(data)+"' AND Godzina_wejscia='"+static_cast<std::string>(from)
        +"' AND Godzina_wyjscia='"+static_cast<std::string>(to)+"'";

        if(baza.modyfikuj_rekord(zapytanie))
        {
            gtk_list_store_remove( temp->magazyn_godzin_pracy,&iter );
        }
        g_free(id_pracownika);
        g_free(data);
        g_free(from);
        g_free(to);
    }
}

void Terminarz::dodaj_termin(GtkWidget *target, gpointer arguments)
{
    Terminarz* temp = static_cast<Terminarz*>(arguments);
    temp->message = gtk_dialog_new_with_buttons("Message",NULL,GTK_DIALOG_DESTROY_WITH_PARENT,"_ACCEPT", GTK_BUTTONS_OK, "_CANCEL",GTK_BUTTONS_CANCEL,NULL);
    GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(temp->message));

    GtkWidget* table = gtk_table_new(8, 2, TRUE);
    gtk_table_set_row_spacings(GTK_TABLE(table), 2);
    gtk_table_set_col_spacings(GTK_TABLE(table), 2);
    gtk_container_add(GTK_CONTAINER(content_area),table);

    GtkWidget* l_identyfikacji = gtk_label_new("Imie i nazwisko lub ID pracownika");
    gtk_table_attach_defaults(GTK_TABLE(table), l_identyfikacji, 0, 1, 0, 1);

    GtkWidget* e_identyfikacji = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(table), e_identyfikacji, 1, 2, 0, 1);
/////////////////////////////////////////////////////////////////////////////////////
    GtkWidget* l_godziny = gtk_label_new("Godzina start:");
    gtk_table_attach_defaults(GTK_TABLE(table), l_godziny, 0, 1, 1, 2);

    GtkWidget* Spn_Btn_godzin = gtk_spin_button_new_with_range(0,23,1);
    gtk_table_attach_defaults(GTK_TABLE(table), Spn_Btn_godzin, 1, 2, 1, 2);

    GtkWidget* l_minuta = gtk_label_new("Minuta start:");
    gtk_table_attach_defaults(GTK_TABLE(table), l_minuta, 0, 1, 2, 3);

    GtkWidget* Spn_Btn_minut = gtk_spin_button_new_with_range(0,59,1);
    gtk_table_attach_defaults(GTK_TABLE(table), Spn_Btn_minut, 1, 2, 2, 3);
/////////////////////////////////////////////////////////////////////////////////////
    GtkWidget* l_godziny_koniec = gtk_label_new("Godzina koniec:");
    gtk_table_attach_defaults(GTK_TABLE(table), l_godziny_koniec, 0, 1, 3, 4);

    GtkWidget* Spn_Btn_godzin_koniec = gtk_spin_button_new_with_range(0,23,1);
    gtk_table_attach_defaults(GTK_TABLE(table), Spn_Btn_godzin_koniec, 1, 2, 3, 4);

    GtkWidget* l_minuta_koniec = gtk_label_new("Minuta koniec:");
    gtk_table_attach_defaults(GTK_TABLE(table), l_minuta_koniec, 0, 1, 4, 5);

    GtkWidget* Spn_Btn_minut_koniec = gtk_spin_button_new_with_range(0,59,1);
    gtk_table_attach_defaults(GTK_TABLE(table), Spn_Btn_minut_koniec, 1, 2, 4, 5);
////////////////////////////////////////////////////////////////////////////////////
    GtkWidget* kal = gtk_calendar_new();
    gtk_table_attach_defaults(GTK_TABLE(table), kal, 0, 2, 5, 7);

    GtkWidget* informator = gtk_label_new("");
    gtk_table_attach_defaults(GTK_TABLE(table),informator, 0, 2, 7, 8);

    gtk_widget_show_all(temp->message);
    while(1)
    {
        switch(gtk_dialog_run(GTK_DIALOG(temp->message)))
        {
            case 1:
                {
                    std::string identyfikator = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(e_identyfikacji)));
                    Baza_danych baza;
                    std::string zapytanie  = "SELECT ID_Pracownika FROM pracownicy WHERE ID_Pracownika = '"+identyfikator+"' OR CONCAT(Imie,' ',Nazwisko)='"+identyfikator+"'";
                    MYSQL_RES* result = baza.wyslij_zapytanie(zapytanie);
                    MYSQL_ROW row = mysql_fetch_row(result);
                    mysql_free_result(result);
                    if(row!=0)
                    {
                        guint year,month,day;
                        gtk_calendar_get_date(GTK_CALENDAR(kal),&year,&month,&day);

                        std::string godzina_startu = std::to_string(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Spn_Btn_godzin)))+':'+std::to_string(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Spn_Btn_minut)));
                        std::string godzina_konca = std::to_string(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Spn_Btn_godzin_koniec)))+':'+std::to_string(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(Spn_Btn_minut_koniec)));
                        std::string data = std::to_string(GPOINTER_TO_INT(year))+"-"+std::to_string(GPOINTER_TO_INT(month))+"-"+std::to_string(GPOINTER_TO_INT(day));
                        zapytanie = "INSERT INTO dzien_pracy (ID_Pracownika,Data,Godzina_wejscia,Godzina_wyjscia) VALUES ('"+static_cast<std::string>(row[0])
                        +"','"+data+"','"+godzina_startu+"','"+godzina_konca+"')";

                        baza.modyfikuj_rekord(zapytanie);
                        wypelnianie_rozpiski(temp->kalendarz,temp);
                        break;
                    }
                    else
                    {
                        gtk_label_set_text(GTK_LABEL(informator),"Niepoprawny identyfikator pracownika");
                        continue;
                    }
                }
            case 3:
                {
                    break;

                }
            case GTK_RESPONSE_DELETE_EVENT:
                {
                    break;
                }
        }
        break;
    }
    gtk_widget_destroy(temp->message);
}
