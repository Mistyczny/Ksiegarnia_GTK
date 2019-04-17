#include "C_Zakladka_Employee.h"
#include "Shop.h"
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
    "Kto",
    "Data",
    "Wejscie",
    "Wyjscie",
};

C_Zakladka_Employee::C_Zakladka_Employee(GtkWidget* okno_glowne_,GtkWidget* box_glowny_)
{
    this->okno_glowne = okno_glowne_;
    this->box_glowny = box_glowny_;
}
C_Zakladka_Employee::~C_Zakladka_Employee()
{

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

    std::string sentence = "SELECT pracownicy.ID_Pracownika,pracownicy.Imie,pracownicy.Nazwisko,adres_zamieszkania.Miejscowość,CONCAT(adres_zamieszkania.Ulica,' ',adres_zamieszkania.Nr_Domu) FROM (pracownicy INNER JOIN adres_zamieszkania ON pracownicy.ID_adresu_zamieszkania=adres_zamieszkania.ID_adresu_zamieszkania)";

    Baza_danych baza;
    MYSQL_RES* idZapytania = baza.wyslij_zapytanie(sentence);

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
        delete komorka;
        delete kolumna;
    }

    magazyn_godzin_pracy = gtk_list_store_new(KOLUMNY_GODZIN,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_INT);
    tabela_godzin = gtk_tree_view_new();
    gtk_tree_view_set_model(GTK_TREE_VIEW(tabela_godzin), GTK_TREE_MODEL(magazyn_godzin_pracy));
    selekcja_tabeli_pracownikow = gtk_tree_view_get_selection(GTK_TREE_VIEW(tabela_pracownikow));

    sentence = "SELECT pracownicy.Imie,dzien_pracy.Data,dzien_pracy.Godzina_wejscia,dzien_pracy.Godzina_wyjscia FROM(dzien_pracy INNER JOIN pracownicy ON dzien_pracy.ID_Pracownika=pracownicy.ID_Pracownika) WHERE dzien_pracy.Data>=CURDATE() AND dzien_pracy.Data<CURRENT_DATE+7";
    idZapytania = baza.wyslij_zapytanie(sentence);

    GtkTreeIter iter2;
    row = mysql_fetch_row(idZapytania);
    while((row = mysql_fetch_row(idZapytania)) != NULL)
    {
        gtk_list_store_append(magazyn_godzin_pracy,&iter2);
        gtk_list_store_set(magazyn_godzin_pracy, &iter2,KTO_PRACUJE,row[0],KIEDY,row[1],GODZINA_WEJSCIA,row[2],GODZINA_WYJSCIA,row[3], - 1 );
    }

    komorka = gtk_cell_renderer_text_new();
    gtk_cell_renderer_set_alignment(komorka,0.5,0.5);
    g_object_set( komorka, "editable", TRUE, NULL );
    g_signal_connect(komorka, "edited", (GCallback)C_Zakladka_Employee::zmiana_komorki, this);

    for(int  i = 0; i <= 3; i++ )
    {
        kolumna = gtk_tree_view_column_new_with_attributes( dni_pracy[i], komorka, "text", i, NULL );
        gtk_tree_view_append_column( GTK_TREE_VIEW( tabela_godzin ), kolumna );
        g_free(kolumna);
    }
    g_free(komorka);

    this->kalendarz = gtk_calendar_new();
    g_signal_connect(this->kalendarz, "day-selected",G_CALLBACK(&C_Zakladka_Employee::wypelnianie_rozpiski), this);
    magazyn_godzin_pracy_pracownikow = gtk_list_store_new(KOLUMNY_PRAC,G_TYPE_INT,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_INT);

    Btn_dodaj_pracownika = gtk_button_new_with_label("Nowy pracownik");
    g_signal_connect(G_OBJECT(Btn_dodaj_pracownika),"clicked",G_CALLBACK(&C_Zakladka_Employee::dodaj_nowego_pracownika),this);

    Btn_modyfikuj_pracownika = gtk_button_new_with_label("Modyfikuj");
    g_signal_connect(G_OBJECT(Btn_modyfikuj_pracownika),"clicked",G_CALLBACK(&C_Zakladka_Employee::modyfikuj_pracownika),this);

    Btn_usun_pracownika = gtk_button_new_with_label("Usuń pracownika");
    g_signal_connect(G_OBJECT(Btn_usun_pracownika),"clicked",G_CALLBACK(&C_Zakladka_Employee::usun_pracownika),this);
}

void C_Zakladka_Employee::run(GtkWidget *target, gpointer arguments)
{
    Shop *temp = static_cast<Shop*>(arguments);
    if(temp->get_active_button()==temp->get_BtnDostepnychKsiazek())
    {
        temp->usun_widzety_dostepnych_ksiazek();
    }
    else if(temp->get_active_button()==temp->get_BtnZamowien())
    {

    }
    temp->set_ActiveButton(temp->get_BtnPracownikow());
    gtk_table_attach_defaults(GTK_TABLE(temp->zakladka_employee->box_glowny),GTK_WIDGET(temp->zakladka_employee->scr_kontener_pracownicy),2,5,2,7);
    gtk_table_attach_defaults(GTK_TABLE(temp->zakladka_employee->box_glowny),GTK_WIDGET(temp->zakladka_employee->tabela_godzin),7,9,2,4);
    gtk_table_attach_defaults(GTK_TABLE(temp->zakladka_employee->box_glowny),GTK_WIDGET(temp->zakladka_employee->kalendarz),7,10,5,7);
    ///BTN pracownicy
    gtk_table_attach_defaults(GTK_TABLE(temp->zakladka_employee->box_glowny),GTK_WIDGET(temp->zakladka_employee->Btn_dodaj_pracownika),2,3,7,8);
    gtk_table_attach_defaults(GTK_TABLE(temp->zakladka_employee->box_glowny),GTK_WIDGET(temp->zakladka_employee->Btn_modyfikuj_pracownika),3,4,7,8);
    gtk_table_attach_defaults(GTK_TABLE(temp->zakladka_employee->box_glowny),GTK_WIDGET(temp->zakladka_employee->Btn_usun_pracownika),4,5,7,8);

    gtk_widget_show_all(temp->zakladka_employee->okno_glowne);
}

void C_Zakladka_Employee::remove_employee_widgets()
{
    gtk_container_remove(GTK_CONTAINER(box_glowny),scr_kontener_pracownicy);
}

void C_Zakladka_Employee::zmiana_komorki(GtkTreeView * treeview, GtkTreePath * path, GtkTreeViewColumn * col, gpointer arguments)
{
    C_Zakladka_Employee* temp = static_cast<C_Zakladka_Employee*>(arguments);
}

void C_Zakladka_Employee::wypelnianie_rozpiski(GtkWidget *target, gpointer arguments)
{
    C_Zakladka_Employee* temp = static_cast<C_Zakladka_Employee*>(arguments);

    guint year;
    guint month;
    guint day;
    gtk_calendar_get_date(GTK_CALENDAR(target),&year,&month,&day);

    //Baza_danych baza;
}

void C_Zakladka_Employee::dodaj_nowego_pracownika(GtkWidget *target, gpointer arguments)
{
    C_Zakladka_Employee* temp = static_cast<C_Zakladka_Employee*>(arguments);
    M_Dialog_pracownik dialog("add");
    dialog.nowy_pracownik();
}

void C_Zakladka_Employee::modyfikuj_pracownika(GtkWidget *target, gpointer arguments)
{
    C_Zakladka_Employee* temp = static_cast<C_Zakladka_Employee*>(arguments);
    GtkTreeIter iter;
    GtkTreeModel *model;

    if(gtk_tree_selection_get_selected(temp->selekcja_tabeli_pracownikow, &model, & iter ))
    {
        int id_pracownika;
        gtk_tree_model_get(model, & iter,ID_PRAC,&id_pracownika,- 1);
        M_Dialog_pracownik dialog("modify");
        dialog.modyfikuj_pracownika(id_pracownika);
    }
}

void C_Zakladka_Employee::usun_pracownika(GtkWidget *target, gpointer arguments)
{
    C_Zakladka_Employee* temp = static_cast<C_Zakladka_Employee*>(arguments);

}
