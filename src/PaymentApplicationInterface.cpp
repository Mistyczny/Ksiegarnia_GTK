#include "PaymentApplicationInterface.h"

enum kolumny
{
    NAZWA_KSIAZKI=0,
    CENA_KSIAZKI,
    ILOSC_KSIAZEK,
    ILOSC_KOLUMN
};

PaymentApplicationInterface::PaymentApplicationInterface()
{
    this->oknoProgramu = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(oknoProgramu), "Płatność");
    this->boxNaWidgety = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(oknoProgramu), boxNaWidgety);

    g_signal_connect(G_OBJECT(oknoProgramu), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

PaymentApplicationInterface::~PaymentApplicationInterface()
{
    std::cout<<"ZABIJAM INTERFACE APLIKACJI PLATNOSCI"<<std::endl;
}

const gchar* nazwy_kolumn_aaa[3] = {
    "Nazwa ksiazki",
    "Cena",
    "Ilosc"
};

void PaymentApplicationInterface::buildTreeViewZakupow(std::map<Ksiazka*,int> zakupy)
{
    scrBoxZakupy = gtk_scrolled_window_new(NULL, NULL);
    tabelaPokazujacaZakupy = gtk_tree_view_new();
    magazynNaZakupy = gtk_list_store_new(ILOSC_KOLUMN,G_TYPE_STRING,G_TYPE_INT,G_TYPE_INT,G_TYPE_INT);
    gtk_tree_view_set_model(GTK_TREE_VIEW(tabelaPokazujacaZakupy), GTK_TREE_MODEL(magazynNaZakupy));

    GtkTreeViewColumn *kolumna;
    GtkCellRenderer *komorka;
    for(int  i = 0; i < 3; i++ )
    {
        komorka = gtk_cell_renderer_text_new();
        kolumna = gtk_tree_view_column_new_with_attributes( nazwy_kolumn_aaa[ i ], komorka, "text", i, NULL );
        gtk_tree_view_append_column( GTK_TREE_VIEW( tabelaPokazujacaZakupy ), kolumna );
    }
    GtkTreeIter iter;
    for(const auto &pair : zakupy)
    {
        gtk_list_store_append(magazynNaZakupy,&iter);
        gtk_list_store_set(magazynNaZakupy, &iter,NAZWA_KSIAZKI,(pair.first->get_nazwaKsiazki()).c_str(),CENA_KSIAZKI,pair.first->get_cenaKsiazki(),
                           ILOSC_KSIAZEK,pair.second, - 1 );
    }
    gtk_grid_attach(GTK_GRID(boxNaWidgety),tabelaPokazujacaZakupy,0,0,10,10);
}

void PaymentApplicationInterface::run()
{
    gtk_widget_show_all(oknoProgramu);
}

GtkWidget* PaymentApplicationInterface::getBtnPlatnoscKarta()
{
    return this->btnPlatnoscKarta;
}

void PaymentApplicationInterface::buildButtons()
{
    btnPlatnoscKarta = gtk_button_new_with_label("Platnosc karta");
    gtk_grid_attach(GTK_GRID(boxNaWidgety),btnPlatnoscKarta,12,12,2,2);
}
