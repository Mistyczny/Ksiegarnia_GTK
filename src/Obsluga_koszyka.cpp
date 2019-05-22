#include "Obsluga_koszyka.h"

std::unique_ptr<Obsluga_koszyka> Obsluga_koszyka::Obsluga_koszyka_init(GtkWidget* okno_prog,GtkWidget* box_,std::shared_ptr<Koszyk> kosz)
{
    std::unique_ptr<Obsluga_koszyka> p(new Obsluga_koszyka(okno_prog,box_,kosz));
return p;
}


Obsluga_koszyka::Obsluga_koszyka(GtkWidget* okno_prog,GtkWidget* box_,std::shared_ptr<Koszyk> kosz) :
                                                            okno_programu(okno_prog),
                                                            box_glowny(box_),
                                                            koszyczek(kosz)
{
}

Obsluga_koszyka::~Obsluga_koszyka()
{
    g_object_unref(G_OBJECT(Btn_dodaj));
    g_object_unref(G_OBJECT(ramka_koszyka));
    g_object_unref(G_OBJECT(L_do_zaplaty));
    g_object_unref(G_OBJECT(Scr_box_zakupy));
    g_object_unref(G_OBJECT(Btn_zaplac));
    g_object_unref(G_OBJECT(Btn_usun));
}

void Obsluga_koszyka::build()
{
    ramka_koszyka = gtk_frame_new("Koszyk");
    g_object_ref(G_OBJECT(ramka_koszyka));

    L_do_zaplaty = gtk_label_new("To pay: 0");
    gtk_label_set_justify(GTK_LABEL(L_do_zaplaty), GTK_JUSTIFY_LEFT);
    g_object_ref(G_OBJECT(L_do_zaplaty));

    tabela_pokazujaca_zakupy = gtk_tree_view_new();
    magazyn_na_zakupy = gtk_list_store_new(4,G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT,G_TYPE_INT );
    gtk_tree_view_set_model(GTK_TREE_VIEW(tabela_pokazujaca_zakupy), GTK_TREE_MODEL(magazyn_na_zakupy));

    selekcja_zakupow = gtk_tree_view_get_selection(GTK_TREE_VIEW(tabela_pokazujaca_zakupy));

    Btn_zaplac = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(Btn_zaplac),"Co dalej");
    g_object_ref(G_OBJECT(Btn_zaplac));
    //g_signal_connect(G_OBJECT(Btn_zaplac),"clicked",G_CALLBACK(&Koszyk::zaplac), this);

    Btn_usun = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(Btn_usun),"Usun");
    g_object_ref(G_OBJECT(Btn_usun));
    g_signal_connect(G_OBJECT(Btn_usun),"clicked",G_CALLBACK(&Koszyk::delete_book_from_purchases), this);

    GtkTreeViewColumn *kolumna;
    GtkCellRenderer *komorka;
    for(int  i = 0; i < 4; i++ )
    {
        komorka = gtk_cell_renderer_text_new();
//        kolumna = gtk_tree_view_column_new_with_attributes( nazwy_kolumn_koszyk[ i ], komorka, "text", i, NULL );
   //     gtk_tree_view_append_column( GTK_TREE_VIEW( tabela_pokazujaca_zakupy ), kolumna );
    }
    Btn_dodaj = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(Btn_dodaj),"Dodaj do koszyka");
    g_object_ref(G_OBJECT(Btn_dodaj));
}

void Obsluga_koszyka::pokaz_widzety()
{
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(ramka_koszyka),0,2,3,8);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(L_do_zaplaty),0,2,3,4);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Scr_box_zakupy),0,2,4,9);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_zaplac),0,1,9,10);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_usun),1,2,9,10);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_dodaj), 4,5, 9, 10);

}

void Obsluga_koszyka::schowaj_widzety()
{
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(ramka_koszyka));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(L_do_zaplaty));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(Scr_box_zakupy));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(Btn_zaplac));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(Btn_usun));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(Btn_dodaj));
}

void Obsluga_koszyka::dodaj_do_zakupow(GtkWidget *target, gpointer arguments)
{
    Shop *temp = static_cast<Shop*>(arguments);

    GtkTreeModel *model;
    GtkTreeIter iter;

    switch(temp->get_aktualna_strona())
    {
        case 1:
        {
            if(gtk_tree_selection_get_selected(temp->get_ksiegarnia_selekcja_ksiazki(),&model,&iter))
            {
                int id_ksiazki;
                int cena;
                gchar* nazwa;
                gtk_tree_model_get(model, & iter,ID_KSIAZKI,&id_ksiazki, NAZWA_KSIAZKI,&nazwa,CENA,&cena,- 1 );
                gtk_label_set_text(GTK_LABEL(temp->get_koszyczek()->L_do_zaplaty),temp->get_koszyczek()->add_money(GPOINTER_TO_INT(cena)).c_str());

                std::string name = static_cast<std::string>(nazwa);
                Ksiazka *ks = new Ksiazka(GPOINTER_TO_INT(id_ksiazki),name,"autor",GPOINTER_TO_INT(cena),"miekka");

                if(temp->get_koszyczek()->check_purchases(ks)==false)
                {
                    temp->get_koszyczek()->zbior[ks] = 1;
                    gtk_list_store_append(temp->get_koszyczek()->magazyn_na_zakupy,&iter);
                    gtk_list_store_set(temp->get_koszyczek()->magazyn_na_zakupy, &iter,ID_KSIAZKI_KOSZYK,id_ksiazki,
                    NAZWA_KSIAZKI_KOSZYK, nazwa,CENA_KOSZYK, cena, ILOSC_KOSZYK,1, - 1 );
                }
                else
                {
                    gtk_list_store_clear(temp->get_koszyczek()->magazyn_na_zakupy);
                    for(const auto &pair : temp->get_koszyczek()->zbior)
                    {
                        std::string nameee = pair.first->get_nazwaKsiazki();
                        gtk_list_store_append(temp->get_koszyczek()->magazyn_na_zakupy,&iter);
                        gtk_list_store_set(temp->get_koszyczek()->magazyn_na_zakupy, &iter,ID_KSIAZKI_KOSZYK,pair.first->get_idKsiazki(),
                        NAZWA_KSIAZKI_KOSZYK, nameee.c_str(),CENA_KOSZYK, pair.first->get_cenaKsiazki(), ILOSC_KOSZYK,pair.second, - 1 );
                    }
                }
                g_free(nazwa);
            }
            break;
        }
        case 2:
        {
            if(gtk_tree_selection_get_selected(temp->get_zamowienia_selekcja_zamowienia(), &model, & iter))
            {
                int id_paczki;
                gtk_tree_model_get(model,&iter,ID_PACZKI,&id_paczki,-1);
                Baza_danych baza;
                std::string zapytanie ="SELECT książka.ID_Ksiązki,książka.Nazwa_książki,książka.cena*zamowienia.ilosc,zamowienia.ilosc FROM((zamowienia INNER JOIN książka ON zamowienia.ID_ksiązki=książka.ID_ksiązki) INNER JOIN klienci ON zamowienia.ID_Klienta=klienci.ID_Klienta) WHERE ID_Zamówienia ='"+std::to_string(GPOINTER_TO_INT(id_paczki))+"';";
                MYSQL_RES* result = baza.wyslij_pytanie(zapytanie);
                MYSQL_ROW row;
                while((row = mysql_fetch_row(result)) != NULL)
                {
                    gtk_label_set_text(GTK_LABEL(temp->get_koszyczek()->L_do_zaplaty),temp->get_koszyczek()->add_money((gint) atoi(row[2])).c_str());

                    int id_ksiazki = atoi(row[0]);
                    std::string nazwa_ksiazki = row[1];
                    int ilosc = atoi(row[3]);
                    int cena = atoi(row[2])/ilosc;

                    Ksiazka *ks = new Ksiazka(id_ksiazki,nazwa_ksiazki,"autor",cena,"miekka");

                    if(temp->get_koszyczek()->check_purchases(ks,ilosc)==false)
                    {
                        temp->get_koszyczek()->zbior[ks] = ilosc;
                        gtk_list_store_append(temp->get_koszyczek()->magazyn_na_zakupy,&iter);
                        gtk_list_store_set(temp->get_koszyczek()->magazyn_na_zakupy, &iter,ID_KSIAZKI_KOSZYK,id_ksiazki,
                        NAZWA_KSIAZKI_KOSZYK,nazwa_ksiazki.c_str(),CENA_KOSZYK, cena, ILOSC_KOSZYK,ilosc, - 1 );
                    }
                    else
                    {
                        gtk_list_store_clear(temp->get_koszyczek()->magazyn_na_zakupy);
                        for(const auto &pair : temp->get_koszyczek()->zbior)
                        {
                            std::string nameee = pair.first->get_nazwaKsiazki();
                            gtk_list_store_append(temp->get_koszyczek()->magazyn_na_zakupy,&iter);
                            gtk_list_store_set(temp->get_koszyczek()->magazyn_na_zakupy, &iter,ID_KSIAZKI_KOSZYK,pair.first->get_idKsiazki(),
                            NAZWA_KSIAZKI_KOSZYK, nameee.c_str(),CENA_KOSZYK, pair.first->get_cenaKsiazki(), ILOSC_KOSZYK,pair.second, - 1 );
                        }
                    }
                }
                break;
            }
        }
        default:
        {
            std::cout<<"JAKIŚ BŁĄD"<<std::endl;
        }
    }
}
