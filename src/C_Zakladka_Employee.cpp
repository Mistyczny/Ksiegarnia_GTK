#include "C_Zakladka_Employee.h"
#include "Baza_danych.h"

enum magazyn_prac
{
    ID_PRAC=0,
    IMIE_PRAC,
    NAZ_PRAC,
    MIAST_PRAC,
    ADRES_PRAC,
    KOLUMNY_PRAC
};

enum godziny
{
    KTO_PRACUJE=0,
    KIEDY,
    GODZINA_WEJSCIA,
    GODZINA_WYJSCIA,
    KOLUMNY_GODZIN
};
const char* dni_tygodnia[7] ={
    "Poniedziałek",
    "Wtorek",
    "Środa",
    "Czwartek",
    "Piątek",
    "Sobota",
    "Niedziela"
};

const gchar* nazwy_kol_magazynu[5] ={
    "ID",
    "Imie",
    "Nazwisko",
    "Miasto zamieszkania",
    "Adres zamieszkania"
};

const gchar* dni_pracy[4] ={
    "ID Prac",
    "Data",
    "Wejscie",
    "Wyjscie",
};

std::unique_ptr<C_Zakladka_Employee> C_Zakladka_Employee::C_Zakladka_Employee_init(GtkWidget* glowne_okno,GtkWidget* box)
{
    std::cout<<"INIT ZAKLADKI EMPLOYE"<<std::endl;
    std::unique_ptr<C_Zakladka_Employee> p (new C_Zakladka_Employee(glowne_okno,box));
return p;
}

C_Zakladka_Employee::C_Zakladka_Employee(GtkWidget* okno_glowne_,GtkWidget* box_glowny_)
{
    this->okno_glowne = okno_glowne_;
    this->box_glowny = box_glowny_;
}

C_Zakladka_Employee::~C_Zakladka_Employee()
{
    std::cout<<"NISZCZE C_ZAKLADKA_EMPLOYEE"<<std::endl;
    g_object_unref(G_OBJECT(scr_kontener_pracownicy));
    g_object_unref(G_OBJECT(scr_tabela_godzin));
    g_object_unref(G_OBJECT(kalendarz));
    g_object_unref(G_OBJECT(Btn_dodaj_pracownika));
    g_object_unref(G_OBJECT(Btn_modyfikuj_pracownika));
    g_object_unref(G_OBJECT(Btn_usun_pracownika));
}

void C_Zakladka_Employee::build()
{
    this->scr_kontener_pracownicy = gtk_scrolled_window_new(NULL, NULL);
    g_object_ref(G_OBJECT(scr_kontener_pracownicy));

    tabela_pracownikow = gtk_tree_view_new();
    gtk_tree_view_columns_autosize(GTK_TREE_VIEW(tabela_pracownikow));
    magazyn_pracownikow = gtk_list_store_new(KOLUMNY_PRAC,G_TYPE_INT,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_INT);
    gtk_tree_view_set_model(GTK_TREE_VIEW(tabela_pracownikow), GTK_TREE_MODEL(magazyn_pracownikow));
    gtk_container_add(GTK_CONTAINER(scr_kontener_pracownicy),tabela_pracownikow);
    g_signal_connect(tabela_pracownikow, "row-activated", (GCallback)C_Zakladka_Employee::wybrany_pracownik, this);

    std::string sentence = "SELECT pracownicy.ID_Pracownika,pracownicy.Imie,pracownicy.Nazwisko,adres_zamieszkania.Miejscowość,CONCAT(adres_zamieszkania.Ulica,' ',adres_zamieszkania.Nr_Domu) FROM (pracownicy INNER JOIN adres_zamieszkania ON pracownicy.ID_adresu_zamieszkania=adres_zamieszkania.ID_adresu_zamieszkania)";

    Baza_danych baza;
    MYSQL_RES* idZapytania = baza.wyslij_zapytanie(sentence);
    this->terminy = Terminarz::Terminarz_init(box_glowny);

    MYSQL_ROW row;
    GtkTreeIter iter;
    GtkTreeViewColumn *kolumna;
    GtkCellRenderer *komorka;

    while((row = mysql_fetch_row(idZapytania)) != NULL)
    {
        gtk_list_store_append(magazyn_pracownikow,&iter);
        gtk_list_store_set(magazyn_pracownikow, &iter,ID_PRAC,(gint) atoi(row[0]),IMIE_PRAC,row[1],NAZ_PRAC,row[2],
        MIAST_PRAC,row[3],ADRES_PRAC,row[4], - 1 );
    }
    for(int  i = 0; i <= 4; i++ )
    {
        komorka = gtk_cell_renderer_text_new();
        gtk_cell_renderer_set_alignment(komorka,0.5,0.5);
        kolumna = gtk_tree_view_column_new_with_attributes( nazwy_kol_magazynu[i], komorka, "text", i, NULL );
        gtk_tree_view_append_column( GTK_TREE_VIEW( tabela_pracownikow ), kolumna );
    }

    scr_tabela_godzin = gtk_scrolled_window_new(NULL, NULL);
    magazyn_godzin_pracy = gtk_list_store_new(KOLUMNY_GODZIN,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_INT);
    tabela_godzin = gtk_tree_view_new();
    g_object_ref(G_OBJECT(scr_tabela_godzin));
    gtk_tree_view_set_model(GTK_TREE_VIEW(tabela_godzin), GTK_TREE_MODEL(magazyn_godzin_pracy));
    selekcja_tabeli_pracownikow = gtk_tree_view_get_selection(GTK_TREE_VIEW(tabela_pracownikow));
    gtk_container_add(GTK_CONTAINER(scr_tabela_godzin),tabela_godzin);

    selekcja_tabeli_godzin= gtk_tree_view_get_selection(GTK_TREE_VIEW(tabela_godzin));
    gtk_tree_selection_set_mode (selekcja_tabeli_godzin,GTK_SELECTION_SINGLE);


    sentence = "SELECT pracownicy.ID_Pracownika,Godzina_wejscia,Godzina_wyjscia,Data FROM dzien_pracy INNER JOIN pracownicy ON dzien_pracy.ID_Pracownika=pracownicy.Id_Pracownika WHERE WEEK('SYSDATE()')=WEEK(dzien_pracy.DATA)";
    idZapytania = baza.wyslij_zapytanie(sentence);

    GtkTreeIter iter2;
    row = mysql_fetch_row(idZapytania);
    while((row = mysql_fetch_row(idZapytania)) != NULL)
    {
        gtk_list_store_append(magazyn_godzin_pracy,&iter2);
        gtk_list_store_set(magazyn_godzin_pracy, &iter2,KTO_PRACUJE,row[0],KIEDY,row[3],GODZINA_WEJSCIA,row[1],GODZINA_WYJSCIA,row[2], - 1 );
    }

    for(int  i = 0; i <= 3; i++ )
    {
        komorka = gtk_cell_renderer_text_new();
        gtk_cell_renderer_set_alignment(komorka,0.5,0.5);
        kolumna = gtk_tree_view_column_new_with_attributes( dni_pracy[i], komorka, "text", i, NULL );
        gtk_tree_view_append_column( GTK_TREE_VIEW( tabela_godzin ), kolumna );

        if(i==0)
        {
            g_object_set( komorka, "editable", TRUE, NULL );
            g_object_set_data (G_OBJECT (komorka), "column", GINT_TO_POINTER (KTO_PRACUJE));
            g_signal_connect(komorka, "edited", (GCallback)&C_Zakladka_Employee::zmiana_komorki, this);
        }
        if(i==2)
        {
            g_object_set( komorka, "editable", TRUE, NULL );
            g_object_set_data (G_OBJECT (komorka), "column", GINT_TO_POINTER (GODZINA_WEJSCIA));
            g_signal_connect(komorka, "edited", (GCallback)&C_Zakladka_Employee::zmiana_komorki, this);
        }
        if(i==3)
        {
            g_object_set( komorka, "editable", TRUE, NULL );
            g_object_set_data (G_OBJECT (komorka), "column", GINT_TO_POINTER (GODZINA_WYJSCIA));
            g_signal_connect(komorka, "edited", (GCallback)&C_Zakladka_Employee::zmiana_komorki, this);
        }
    }

    this->kalendarz = gtk_calendar_new();

    g_object_ref(G_OBJECT(kalendarz));
    g_signal_connect(this->kalendarz, "day-selected",G_CALLBACK(&C_Zakladka_Employee::wypelnianie_rozpiski), this);
    magazyn_godzin_pracy_pracownikow = gtk_list_store_new(KOLUMNY_PRAC,G_TYPE_INT,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_INT);

    Btn_dodaj_pracownika = gtk_button_new_with_label("Nowy pracownik");
    g_object_ref(G_OBJECT(Btn_dodaj_pracownika));
    g_signal_connect(G_OBJECT(Btn_dodaj_pracownika),"clicked",G_CALLBACK(&C_Zakladka_Employee::dodaj_nowego_pracownika),this);

    Btn_modyfikuj_pracownika = gtk_button_new_with_label("Modyfikuj");
    gtk_widget_set_sensitive(Btn_modyfikuj_pracownika, FALSE);
    g_object_ref(G_OBJECT(Btn_modyfikuj_pracownika));
    g_signal_connect(G_OBJECT(Btn_modyfikuj_pracownika),"clicked",G_CALLBACK(&C_Zakladka_Employee::modyfikuj_pracownika),this);

    Btn_usun_pracownika = gtk_button_new_with_label("Usuń pracownika");
    gtk_widget_set_sensitive(Btn_usun_pracownika, FALSE);
    g_object_ref(G_OBJECT(Btn_usun_pracownika));
    g_signal_connect(G_OBJECT(Btn_usun_pracownika),"clicked",G_CALLBACK(&C_Zakladka_Employee::usun_pracownika),this);

    Btn_zapisz_zmiany_godziny = gtk_button_new_with_label("Zapisz zmiany");
    g_object_ref(G_OBJECT(Btn_zapisz_zmiany_godziny));
    g_signal_connect(G_OBJECT(Btn_zapisz_zmiany_godziny),"clicked",G_CALLBACK(&C_Zakladka_Employee::wprowadz_zmiany_w_planie_pracownika),this);
}
/*********************************************************************************************************
*
* \brief function called after pressing button "Employee" in main menu
* \brief shows all widgets from class C_Zakladka_Employee and removes others overlaps widgets
* \argument GtkWidget *target - pointer to button
* \argument gpointer argument - there its pointer to object for which we call a function
* \return void
*
**********************************************************************************************************/
void C_Zakladka_Employee::pokaz_widzety()
{
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(scr_kontener_pracownicy),2,5,2,7);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_dodaj_pracownika),2,3,7,8);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_modyfikuj_pracownika),3,4,7,8);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_usun_pracownika),4,5,7,8);
    this->terminy->build();
}
/*********************************************************************************************************
*
* \brief removes all C_Zakladka_Employee class widgets from main box
* \return void
*
**********************************************************************************************************/
void C_Zakladka_Employee::schowaj_widzety()
{
    gtk_container_remove(GTK_CONTAINER(box_glowny),scr_kontener_pracownicy);
    gtk_container_remove(GTK_CONTAINER(box_glowny),Btn_dodaj_pracownika);
    gtk_container_remove(GTK_CONTAINER(box_glowny),Btn_modyfikuj_pracownika);
    gtk_container_remove(GTK_CONTAINER(box_glowny),Btn_usun_pracownika);
    this->terminy->schowaj_widzety();

}
/*********************************************************************************************************
*
* \brief change cell in gtk_tree_view(tablica_godzin)
* \brief obtain a column number GPOINTER_TO_INT (g_object_get_data (G_OBJECT (cell), "column"));
* \argument - GtkCellRendererText *cell - pressed cell
* \argument - gchar* path_ - path to pressed cell holded by gchar[]
* \argument - gchar *new_text - text to save in cell
* \argument - gpointer arguments - pointer to magazyn_pracownikow
* \return void
*
**********************************************************************************************************/
void C_Zakladka_Employee::zmiana_komorki(GtkCellRendererText *cell,gchar* path_,gchar *new_text,gpointer arguments)
{
    C_Zakladka_Employee* temp = static_cast<C_Zakladka_Employee*>(arguments);
    GtkTreeModel *model = (GtkTreeModel *)(temp->magazyn_godzin_pracy);
    GtkTreePath *path = gtk_tree_path_new_from_string (path_);
    GtkTreeIter iter;
    gint column = GPOINTER_TO_INT(g_object_get_data (G_OBJECT (cell), "column"));
    gtk_tree_model_get_iter (model, &iter, path);

    switch (column)
    {
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
void C_Zakladka_Employee::wypelnianie_rozpiski(GtkWidget *target, gpointer arguments)
{
    C_Zakladka_Employee* temp = static_cast<C_Zakladka_Employee*>(arguments);

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
/*********************************************************************************************************
*
* \brief function called after pressing button with text-"Dodaj pracowinika"
* \brief creates instance of class M_dialog_pracownik that is responsible for adding new employee to database
* \brief constuctor called with string "add" tells it that we want instance to add employee
* \argument GtkWidget *target - pointer to button
* \argument gpointer argument - there its pointer to object for which we call a function
* \brief after all actions reload the gtk_tree with employeers
* \return void
*
**********************************************************************************************************/
void C_Zakladka_Employee::dodaj_nowego_pracownika(GtkWidget *target, gpointer arguments)
{
    C_Zakladka_Employee* temp = static_cast<C_Zakladka_Employee*>(arguments);
    M_Dialog_pracownik dialog;
    dialog.zbuduj_pusty_formularz();
    dialog.nowy_pracownik();
    odswiez_tabele_pracownikow(temp);
}
/*********************************************************************************************************
*
* \brief function called after pressing button with text-"Modyfikuj"
* \brief creates instance of class M_dialog_pracownik that is responsible for modifying employee info
* \brief constuctor called with string "add" tells it that we want instance to modify employee
* \argument GtkWidget *target - pointer to button
* \argument gpointer argument - there its pointer to object for which we call a function
* \brief function does nothing if there is no employee selected on employee gtk_tree
* \param int id_pracownika - holds an employee id by which he is noted in database
* \brief after all actions reload the gtk_tree with employeers
* \return void
*
**********************************************************************************************************/
void C_Zakladka_Employee::modyfikuj_pracownika(GtkWidget *target, gpointer arguments)
{
    C_Zakladka_Employee* temp = static_cast<C_Zakladka_Employee*>(arguments);
    GtkTreeIter iter;
    GtkTreeModel *model;

    if(gtk_tree_selection_get_selected(temp->selekcja_tabeli_pracownikow, &model, & iter ))
    {
        int id_pracownika=0;
        gtk_tree_model_get(model, & iter,ID_PRAC,&id_pracownika,- 1);
        M_Dialog_pracownik dialog;
        dialog.zbuduj_modyfikacje_pracownika();
        dialog.modyfikuj_pracownika(id_pracownika);
        odswiez_tabele_pracownikow(temp);
    }
}
/*********************************************************************************************************
*
* \brief function called after pressing button with text-"Usuń pracownika"
* \brief creates instance of class M_dialog_pracownik that is responsible for deleting employee from database
* \argument GtkWidget *target - pointer to button
* \argument gpointer argument - there its pointer to object for which we call a function
* \brief function does nothing if there is no employee selected on employee gtk_tree
* \param int id_pracownika - holds an employee id by which he is noted in database
* \brief after all actions reload the gtk_tree with employeers
* \return void
*
**********************************************************************************************************/
void C_Zakladka_Employee::usun_pracownika(GtkWidget *target, gpointer arguments)
{
    C_Zakladka_Employee* temp = static_cast<C_Zakladka_Employee*>(arguments);
    GtkTreeIter iter;
    GtkTreeModel *model;

    if(gtk_tree_selection_get_selected(temp->selekcja_tabeli_pracownikow, &model, & iter ))
    {
        int id_pracownika;
        gtk_tree_model_get(model, & iter,ID_PRAC,&id_pracownika,- 1);
        M_Dialog_pracownik dialog;
        dialog.usun_pracownika(id_pracownika);
        odswiez_tabele_pracownikow(temp);
    }
}
/*********************************************************************************************************
*
* \brief function reload info to tabela_pracownikow from database
* \return void
*
**********************************************************************************************************/
void C_Zakladka_Employee::odswiez_tabele_pracownikow(C_Zakladka_Employee* target)
{
    gtk_list_store_clear(target->magazyn_pracownikow);
    std::string sentence = "SELECT pracownicy.ID_Pracownika,pracownicy.Imie,pracownicy.Nazwisko,adres_zamieszkania.Miejscowość,CONCAT(adres_zamieszkania.Ulica,' ',adres_zamieszkania.Nr_Domu) FROM (pracownicy INNER JOIN adres_zamieszkania ON pracownicy.ID_adresu_zamieszkania=adres_zamieszkania.ID_adresu_zamieszkania)";

    Baza_danych baza;
    MYSQL_RES* result = baza.wyslij_zapytanie(sentence);
    MYSQL_ROW row;
    GtkTreeIter iter;
    while((row = mysql_fetch_row(result)) != NULL)
    {
        gtk_list_store_append(target->magazyn_pracownikow,&iter);
        gtk_list_store_set(target->magazyn_pracownikow, &iter,ID_PRAC,(gint) atoi(row[0]),IMIE_PRAC,row[1],NAZ_PRAC,row[2],
        MIAST_PRAC,row[3],ADRES_PRAC,row[4], - 1 );
    }
    mysql_free_result(result);
}
/*********************************************************************************************************
*
* \brief enables modify and delete employee buttons after pressing a employee row on gtk_tree
* \return void
*
**********************************************************************************************************/
void C_Zakladka_Employee::wybrany_pracownik(GtkTreeView* tree_, GtkTreePath* path_, GtkTreeViewColumn* col_, gpointer arguments)
{
    C_Zakladka_Employee* temp = static_cast<C_Zakladka_Employee*>(arguments);
    gtk_widget_set_sensitive(temp->Btn_modyfikuj_pracownika, TRUE);
    gtk_widget_set_sensitive(temp->Btn_usun_pracownika, TRUE);
}
/*********************************************************************************************************
*
* \argument GtkWidget* target - button with label "Zapisz zmiany", triggered on click
* \argument gpointer arguments - pointer to class C_Zakladka_Employee
* \brief function saves to database all informations from gtk_treeview
* \brief param - valid is true till it finds iter to next tree row
* \return void
*
**********************************************************************************************************/
void C_Zakladka_Employee::wprowadz_zmiany_w_planie_pracownika(GtkWidget *target, gpointer arguments)
{
    C_Zakladka_Employee* temp = static_cast<C_Zakladka_Employee*>(arguments);

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
        valid = gtk_tree_model_iter_next (model,&iter);
    g_free(id_prac);
    g_free(data);
    g_free(godz_wej);
    g_free(godz_wyj);
    }
}
