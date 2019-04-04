#ifndef BAZA_DANYCH_H
#define BAZA_DANYCH_H
#include "gtk/gtk.h"
#include "windows.h"
#include "mysql.h"
#include <iostream>

class Baza_danych
{
    MYSQL *connect;
    GtkWidget* error;
    public:
        Baza_danych();
        MYSQL_RES* wyslij_zapytanie(std::string);
        virtual ~Baza_danych();

    protected:

    private:
};

#endif // BAZA_DANYCH_H
