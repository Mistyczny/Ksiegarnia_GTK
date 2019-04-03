#include "koszyk.h"
#include "define_koszyk.h"
#include "Shop.h"
Koszyk::Koszyk(GtkWidget *window_,GtkWidget *box_,GtkWidget* pay_app) :     window(window_),
                                                                            box(box_),
                                                                            pay_app_window(pay_app)
{
    do_zaplaty = 0;
    try
    {
        frame = gtk_frame_new("Koszyk");
        gtk_table_attach_defaults(GTK_TABLE(box),frame,0,2,3,8);

        title = gtk_label_new("To pay: 0");
        gtk_label_set_justify(GTK_LABEL(title), GTK_JUSTIFY_LEFT);
        gtk_table_attach_defaults(GTK_TABLE(box),title,0,2,3,4);

        TableShowingShopping = gtk_tree_view_new();
        storage_of_shopping = gtk_list_store_new(4,G_TYPE_INT, G_TYPE_STRING, G_TYPE_INT,G_TYPE_INT );
        gtk_tree_view_set_model(GTK_TREE_VIEW(TableShowingShopping), GTK_TREE_MODEL(storage_of_shopping));

        select = gtk_tree_view_get_selection(GTK_TREE_VIEW(TableShowingShopping));

        scrolled_box_shopping = gtk_scrolled_window_new(NULL, NULL);
        gtk_container_add(GTK_CONTAINER(scrolled_box_shopping),TableShowingShopping);
        gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_box_shopping), GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
        gtk_table_attach_defaults(GTK_TABLE(box),GTK_WIDGET(scrolled_box_shopping),0,2,4,9);

        Btn_zaplac = gtk_button_new();
        gtk_button_set_label(GTK_BUTTON(Btn_zaplac),"Zaplac");
        gtk_table_attach_defaults(GTK_TABLE(box),Btn_zaplac,0,1,9,10);
        g_signal_connect(G_OBJECT(Btn_zaplac),"clicked",G_CALLBACK(&Payment_application::show_Payment_app), this);

        Btn_usun = gtk_button_new();
        gtk_button_set_label(GTK_BUTTON(Btn_usun),"Usun");
        gtk_table_attach_defaults(GTK_TABLE(box),Btn_usun,1,2,9,10);
        g_signal_connect(G_OBJECT(Btn_usun),"clicked",G_CALLBACK(&Koszyk::delete_book_from_purchases), this);

        for(int  i = 0; i < 4; i++ )
        {
            shopping_cell = gtk_cell_renderer_text_new();
            shopping_column = gtk_tree_view_column_new_with_attributes( nazwy_kolumn_koszyk[ i ], shopping_cell, "text", i, NULL );
            gtk_tree_view_append_column( GTK_TREE_VIEW( TableShowingShopping ), shopping_column );
        }
    }
    catch(std::bad_alloc& alloc)
    {
        error = gtk_message_dialog_new(GTK_WINDOW(window),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "ERROR OCCURED WHILE MEMORY ALLOCATION.");
        gtk_window_set_title(GTK_WINDOW(error), "ERROR - MEMORY ALLOCATION");
        gtk_dialog_run(GTK_DIALOG(error));
        gtk_widget_destroy(error);
        exit(0);
        return;
    }
}

Koszyk::~Koszyk()
{
    g_free(title);
    g_free(scrolled_box_shopping);
    g_free(storage_of_shopping);
    g_free(TableShowingShopping);
    g_free(shopping_column);
    g_free(shopping_cell);
    g_free(Btn_zaplac);
    g_free(Btn_usun);
}

void Koszyk::add_book_to_purchases(GtkWidget *target, gpointer arguments)
{
    Shop *temp = static_cast<Shop*>(arguments);

    GtkTreeModel *model;
    GtkTreeIter iter;

    if(temp->get_active_button()==temp->get_BtnDostepnychKsiazek())
    {
        if( gtk_tree_selection_get_selected( temp->get_selected(0), &model, & iter ) )
        {
            gint *id_ksiazki;
            gchar * nazwa;
            gint *cena;
            gtk_tree_model_get(model, & iter,ID_KSIAZKI,&id_ksiazki, NAZWA_KSIAZKI, & nazwa,CENA,&cena,- 1 );
            gtk_label_set_text(GTK_LABEL(temp->get_koszyczek()->title),temp->get_koszyczek()->add_money(GPOINTER_TO_INT(cena)).c_str());

            std::ostringstream name;
            name<<nazwa;
            std::string named = name.str();

            Ksiazka *ks = new Ksiazka(GPOINTER_TO_INT(id_ksiazki),named,"autor",GPOINTER_TO_INT(cena),"miekka");

            if(temp->get_koszyczek()->check_purchases(ks)==false)
            {
                temp->get_koszyczek()->zbior[ks] = 1;
                gtk_list_store_append(temp->get_koszyczek()->storage_of_shopping,&iter);
                gtk_list_store_set(temp->get_koszyczek()->storage_of_shopping, &iter,ID_KSIAZKI_KOSZYK,id_ksiazki,
                NAZWA_KSIAZKI_KOSZYK, nazwa,CENA_KOSZYK, cena, ILOSC_KOSZYK,1, - 1 );
            }
            else
            {
                gtk_list_store_clear(temp->get_koszyczek()->storage_of_shopping);
                for(const auto &pair : temp->get_koszyczek()->zbior)
                {
                    std::string nameee = pair.first->get_nazwaKsiazki();
                    gtk_list_store_append(temp->get_koszyczek()->storage_of_shopping,&iter);
                    gtk_list_store_set(temp->get_koszyczek()->storage_of_shopping, &iter,ID_KSIAZKI_KOSZYK,pair.first->get_idKsiazki(),
                    NAZWA_KSIAZKI_KOSZYK, nameee.c_str(),CENA_KOSZYK, pair.first->get_cenaKsiazki(), ILOSC_KOSZYK,pair.second, - 1 );
                }
            }
            g_free(id_ksiazki);
            g_free(nazwa);
            g_free(cena);
        }
    }
    else if(temp->get_active_button()==temp->get_BtnZamowien())
    {
      if(gtk_tree_selection_get_selected(temp->get_selected(1), &model, & iter))
      {
            gint* id_paczki;
            gtk_tree_model_get(model,&iter,ID_PACZKI,&id_paczki,-1);

            MYSQL *connect = mysql_init(0);
            mysql_options(connect, MYSQL_SET_CHARSET_NAME, "utf8");
            mysql_options(connect, MYSQL_INIT_COMMAND, "SET NAMES utf8");
            connect = mysql_real_connect(connect, "127.0.0.1","root","","sklep_ksiazek",0,NULL,0);
            std::string sentence ="SELECT książka.ID_Ksiązki,książka.Nazwa_książki,książka.cena*zamowienia.ilosc,zamowienia.ilosc FROM((zamowienia INNER JOIN książka ON zamowienia.ID_ksiązki=książka.ID_ksiązki) INNER JOIN klienci ON zamowienia.ID_Klienta=klienci.ID_Klienta) WHERE ID_Zamówienia ='"+std::to_string(GPOINTER_TO_INT(id_paczki))+"';";
            mysql_query(connect,sentence.c_str());

            MYSQL_RES* idZapytania = mysql_store_result(connect);
            MYSQL_ROW row;
            while((row = mysql_fetch_row(idZapytania)) != NULL)
            {
                gtk_label_set_text(GTK_LABEL(temp->get_koszyczek()->title),temp->get_koszyczek()->add_money((gint) atoi(row[2])).c_str());

                int id_ksiazki = atoi(row[0]);
                std::string nazwa_ksiazki = row[1];
                int ilosc = atoi(row[3]);
                int cena = atoi(row[2])/ilosc;

                Ksiazka *ks = new Ksiazka(id_ksiazki,nazwa_ksiazki,"autor",cena,"miekka");

                if(temp->get_koszyczek()->check_purchases(ks,ilosc)==false)
                {
                    temp->get_koszyczek()->zbior[ks] = ilosc;
                    gtk_list_store_append(temp->get_koszyczek()->storage_of_shopping,&iter);
                    gtk_list_store_set(temp->get_koszyczek()->storage_of_shopping, &iter,ID_KSIAZKI_KOSZYK,id_ksiazki,
                    NAZWA_KSIAZKI_KOSZYK,nazwa_ksiazki.c_str(),CENA_KOSZYK, cena, ILOSC_KOSZYK,ilosc, - 1 );
                }
                else
                {
                    gtk_list_store_clear(temp->get_koszyczek()->storage_of_shopping);
                    for(const auto &pair : temp->get_koszyczek()->zbior)
                    {
                        std::string nameee = pair.first->get_nazwaKsiazki();
                        gtk_list_store_append(temp->get_koszyczek()->storage_of_shopping,&iter);
                        gtk_list_store_set(temp->get_koszyczek()->storage_of_shopping, &iter,ID_KSIAZKI_KOSZYK,pair.first->get_idKsiazki(),
                        NAZWA_KSIAZKI_KOSZYK, nameee.c_str(),CENA_KOSZYK, pair.first->get_cenaKsiazki(), ILOSC_KOSZYK,pair.second, - 1 );
                    }
                }
            }
        }
    }
}

bool Koszyk::check_purchases(Ksiazka* temp,int ilosc)
{
    for(const auto &pair : zbior)
    {
        if(pair.first->get_idKsiazki() == temp->get_idKsiazki())
        {
            zbior[pair.first]+=ilosc;
            return true;
        }
    }
return false;
}

void Koszyk::delete_book_from_purchases(GtkWidget *target,gpointer arguments)
{
    Koszyk* temp = static_cast<Koszyk*>(arguments);
    GtkTreeIter iter;
    GtkTreeModel *model;

    if( gtk_tree_selection_get_selected( temp->select, &model, & iter ))
    {

        gint *ilosc;
        gint *cena;
        gtk_tree_model_get(model, & iter,CENA_KOSZYK,&cena,ILOSC_KOSZYK,&ilosc,- 1 );
        gtk_label_set_text(GTK_LABEL(temp->title),temp->remove_money(GPOINTER_TO_INT(cena)).c_str());
        if(GPOINTER_TO_INT(ilosc)-1!=0)
        {
            gtk_list_store_set(temp->storage_of_shopping,&iter,ILOSC_KOSZYK,GPOINTER_TO_INT(ilosc)-1,-1);
        }
        else
        {
            gtk_list_store_remove(temp->storage_of_shopping,&iter);
        }
    }
    else
    {
        temp->error = gtk_message_dialog_new(GTK_WINDOW(temp->window),GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "SELECT SOMETHING FIRST FROM PURCHASES LIST.");
        gtk_window_set_title(GTK_WINDOW(temp->error), "ERROR");
        gtk_dialog_run(GTK_DIALOG(temp->error));
        gtk_widget_destroy(temp->error);
        return;
    }
}

void Koszyk::purchase(GtkWidget *target,gpointer arguments)
{
    Koszyk* temp = static_cast<Koszyk*>(arguments);
   // gtk_widget_show_all(temp->pay)
}

std::string Koszyk::add_money(int i)
{
    do_zaplaty+=i;
    std::ostringstream pay;
    pay<<"To Pay: "<<do_zaplaty;
    std::string to_pay = pay.str();
return to_pay;
}

std::string Koszyk::remove_money(int i)
{
    do_zaplaty-=i;
    std::ostringstream pay;
    pay<<"To Pay: "<<do_zaplaty;
    std::string to_pay = pay.str();
return to_pay;
}
