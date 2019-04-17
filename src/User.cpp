#include "User.h"

User::User()
{

}

User::~User()
{
}

void User::setMyId(int MyId_)
{
    this->my_id = MyId_;
}

void User::setName(std::string name_)
{
    this->name = name_;
}

void User::setSurname(std::string surname_)
{
    this->surname = surname_;
}

void User::setAdministator(bool adm)
{
    this->administator = adm;
}

const int User::getMyId()
{
    return this->my_id;
}

const std::string User::getName()
{
    return this->name;
}

const std::string User::getSurname()
{
    return this->surname;
}

const bool User::getAdministator()
{
    return this->administator;
}

void User::logIn(int i)
{
    Baza_danych baza;
    std::string questionToDatabase = "SELECT * FROM pracownicy WHERE ID_Pracownika = " + std::to_string(i);
    MYSQL_RES* result = baza.wyslij_zapytanie(questionToDatabase);

    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    mysql_free_result(result);

    if(row!=NULL)
    {
        this->my_id = atoi(row[0]);
        this->name = row[1];
        this->surname = row[2];
        this->administator = row[6];
    }
    else
    {
        message = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Nie posiadamy informacji o tym pracowniku");
        gtk_window_set_title(GTK_WINDOW(message), "Error");
        gtk_dialog_run(GTK_DIALOG(message));
        gtk_widget_destroy(message);
    }
}

void User::logOff()
{
    my_id = 0;
    name = "";
    surname = "";
    administator = false;
}
