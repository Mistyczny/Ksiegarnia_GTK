#include "koszykView.h"
#include "Shop.h"

const gchar* nazwy_kolumn_koszyka[4] = {
    "Id ksiazki",
    "Nazwa ksiazki",
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

enum paczki
{
    ID_PACZKI=0,
    ID_KSIAZKI_W_PACZCE,
    NAZWA_KSIAZKI_W_PACZCE,
    IMIE_ODBIORCY,
    NAZWISKO_ODBIORCY,
    CENA_PACZKI,
    ILOSC_W_PACZCE,
    ODEBRANA,
    ILOSC_KOLUMN_PACZEK
};

enum koszyk_en
{
    ID_KSIAZKI_KOSZYK=0,
    NAZWA_KSIAZKI_KOSZYK,
    CENA_KOSZYK,
    ILOSC_KOSZYK,
    ILOSC_KOLUMN_KOSZYKA
};

std::unique_ptr<koszykView> koszykView::koszykView_init(GtkWidget* okno_prog,GtkWidget* box_)
{
    std::unique_ptr<koszykView> p(new koszykView(okno_prog,box_));
return p;
}


koszykView::koszykView(GtkWidget* okno_prog,GtkWidget* box_) :
                                                            okno_programu(okno_prog),
                                                            box_glowny(box_)
{
    this->build();
}

koszykView::~koszykView()
{
    g_object_unref(G_OBJECT(Btn_dodaj));
    g_object_unref(G_OBJECT(ramka_koszyka));
    g_object_unref(G_OBJECT(L_do_zaplaty));
    g_object_unref(G_OBJECT(Scr_box_zakupy));
    g_object_unref(G_OBJECT(Btn_zaplac));
    g_object_unref(G_OBJECT(Btn_usun));
}

void koszykView::build()
{
    ramka_koszyka = gtk_frame_new("Koszyk");
    g_object_ref(G_OBJECT(ramka_koszyka));

    L_do_zaplaty = gtk_label_new("Do zapłaty: 0");
    gtk_label_set_justify(GTK_LABEL(L_do_zaplaty), GTK_JUSTIFY_LEFT);
    g_object_ref(G_OBJECT(L_do_zaplaty));

    tabela_pokazujaca_zakupy = gtk_tree_view_new();
    magazyn_na_zakupy = gtk_list_store_new(4,G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT,G_TYPE_INT );
    gtk_tree_view_set_model(GTK_TREE_VIEW(tabela_pokazujaca_zakupy), GTK_TREE_MODEL(magazyn_na_zakupy));

    selekcja_zakupow = gtk_tree_view_get_selection(GTK_TREE_VIEW(tabela_pokazujaca_zakupy));

    Scr_box_zakupy = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(Scr_box_zakupy),tabela_pokazujaca_zakupy);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(Scr_box_zakupy), GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
    g_object_ref(G_OBJECT(Scr_box_zakupy));

    Btn_zaplac = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(Btn_zaplac),"Przejdz do płatności");
    g_object_ref(G_OBJECT(Btn_zaplac));

    Btn_usun = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(Btn_usun),"Usuń z koszyka");
    g_object_ref(G_OBJECT(Btn_usun));

    GtkTreeViewColumn *kolumna;
    GtkCellRenderer *komorka;
    for(int  i = 0; i < 4; i++ )
    {
        komorka = gtk_cell_renderer_text_new();
        kolumna = gtk_tree_view_column_new_with_attributes( nazwy_kolumn_koszyka[ i ], komorka, "text", i, NULL );
        gtk_tree_view_append_column( GTK_TREE_VIEW( tabela_pokazujaca_zakupy ), kolumna );
    }
    Btn_dodaj = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(Btn_dodaj),"Dodaj do koszyka");
    g_object_ref(G_OBJECT(Btn_dodaj));
}

void koszykView::pokaz_widzety()
{
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(ramka_koszyka),0,2,3,8);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(L_do_zaplaty),0,2,3,4);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Scr_box_zakupy),0,2,4,9);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_zaplac),0,1,9,10);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_usun),1,2,9,10);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),GTK_WIDGET(Btn_dodaj), 4,5, 9, 10);
}

void koszykView::schowaj_widzety()
{
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(ramka_koszyka));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(L_do_zaplaty));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(Scr_box_zakupy));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(Btn_zaplac));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(Btn_usun));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(Btn_dodaj));
}


void koszykView::TriggerNaDodaj(GtkWidget *target, gpointer arguments)
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

                std::string name = static_cast<std::string>(nazwa);
                Ksiazka *ks = new Ksiazka(GPOINTER_TO_INT(id_ksiazki),name,"autor",GPOINTER_TO_INT(cena),"miekka");
                temp->get_koszyczek()->getController()->dodajKsiazkiDoZbioru(ks);
                temp->get_koszyczek()->getInterface()->DodajKsiazkeDoListy(temp->get_koszyczek()->getController()->getZbiorKsiazek());
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
                while((row=mysql_fetch_row(result))!=NULL)
                {
                    int id_ksiazki = atoi(row[0]);
                    std::string nazwa_ksiazki = row[1];
                    int ilosc = atoi(row[3]);
                    int cena = atoi(row[2])/ilosc;
                    Ksiazka *ks = new Ksiazka(id_ksiazki,nazwa_ksiazki,"autor",cena,"miekka");
                    temp->get_koszyczek()->getController()->dodajKsiazkiDoZbioru(ks,ilosc);
                    temp->get_koszyczek()->getInterface()->DodajKsiazkeDoListy(temp->get_koszyczek()->getController()->getZbiorKsiazek());
                }
            }
        break;
        }
    }
    temp->get_koszyczek()->getInterface()->UaktualnijCene(temp->get_koszyczek()->getController()->getAmountOfMoneyToPay());
}


GtkTreeSelection* koszykView::getSelekcjaZakupow()
{
    return this->selekcja_zakupow;
}

GtkListStore* koszykView::getMagazynNaZakupy()
{
    return this->magazyn_na_zakupy;
}

GtkWidget* koszykView::getBtnUsun()
{
    return this->Btn_usun;
}
void koszykView::TriggerNaUsun(GtkWidget *target, gpointer arguments)
{
    Koszyk* temp = static_cast<Koszyk*>(arguments);
    GtkTreeIter iter;
    GtkTreeModel *model;

    if(gtk_tree_selection_get_selected( temp->getInterface()->getSelekcjaZakupow(), &model, & iter ))
    {
        gint* ilosc;
        gint* cena;
        int IdKsiazki;
        gtk_tree_model_get(model, & iter,ID_KSIAZKI_KOSZYK,&IdKsiazki,CENA_KOSZYK,&cena,ILOSC_KOSZYK,&ilosc,- 1 );

        if(GPOINTER_TO_INT(ilosc)-1!=0)
        {
            gtk_list_store_set(temp->getInterface()->getMagazynNaZakupy(),&iter,ILOSC_KOSZYK,GPOINTER_TO_INT(ilosc)-1,-1);
            temp->getController()->usunKsiazkeZeZbioru(IdKsiazki);
        }
        else
        {
            gtk_list_store_remove(temp->getInterface()->getMagazynNaZakupy(),&iter);
            temp->getController()->usunKsiazkeZeZbioru(IdKsiazki);
        }
    }
    else
    {
        temp->error = gtk_message_dialog_new(NULL,GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "SELECT SOMETHING FIRST FROM PURCHASES LIST.");
        gtk_window_set_title(GTK_WINDOW(temp->error), "ERROR");
        gtk_dialog_run(GTK_DIALOG(temp->error));
        gtk_widget_destroy(temp->error);
        return;
    }
    temp->getInterface()->UaktualnijCene(temp->getController()->getAmountOfMoneyToPay());
}


void koszykView::UaktualnijCene(int cena)
{
    std::string opis = "Do zapłaty: " + std::to_string(cena);
    gtk_label_set_text(GTK_LABEL(L_do_zaplaty),opis.c_str());
}

void koszykView::DodajKsiazkeDoListy(std::map<Ksiazka*,int>& zbior)
{
    GtkTreeModel *model;
    GtkTreeIter iter;
    gtk_list_store_clear(magazyn_na_zakupy);

    for(const auto& pair: zbior)
    {
        gtk_list_store_append(magazyn_na_zakupy,&iter);
        gtk_list_store_set(magazyn_na_zakupy, &iter,ID_KSIAZKI_KOSZYK,pair.first->get_idKsiazki(),
        NAZWA_KSIAZKI_KOSZYK, (pair.first->get_nazwaKsiazki()).c_str(),CENA_KOSZYK, pair.first->get_cenaKsiazki(), ILOSC_KOSZYK,pair.second, - 1 );
    }
}

GtkWidget* koszykView::GetBtnDodaj()
{
    return this->Btn_dodaj;
}

GtkWidget* koszykView::GetBtnZaplac()
{
    return this->Btn_zaplac;
}
