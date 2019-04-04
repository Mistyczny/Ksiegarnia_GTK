#include "Baza_danych.h"

Baza_danych::Baza_danych()
{
    connect = mysql_init(0);
    mysql_options(connect, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(connect, MYSQL_INIT_COMMAND, "SET NAMES utf8");
    if((connect = mysql_real_connect(connect, "127.0.0.1","root","","sklep_ksiazek",0,NULL,0))== NULL)
    {
        std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
        error = gtk_message_dialog_new(NULL,GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
        gtk_window_set_title(GTK_WINDOW(error), "Database connection error.");
        gtk_dialog_run(GTK_DIALOG(error));
        gtk_widget_destroy(error);
        exit(0);
    }
    else{
    }
}

Baza_danych::~Baza_danych()
{
    mysql_close(connect);
}

MYSQL_RES* Baza_danych::wyslij_zapytanie(std::string zapytanie)
{
    if(mysql_query(connect,zapytanie.c_str()))
    {
        std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
        error = gtk_message_dialog_new(NULL,GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error");
        gtk_window_set_title(GTK_WINDOW(error), "Database connection error.");
        gtk_dialog_run(GTK_DIALOG(error));
        gtk_widget_destroy(error);
        exit(0);
    }
    else
    {
        MYSQL_RES *idZapytania = mysql_store_result(connect);
        return idZapytania;
    }
}
