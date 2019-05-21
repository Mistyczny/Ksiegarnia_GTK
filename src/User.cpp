#include "User.h"

std::shared_ptr<User> User::User_init()
{
    std::shared_ptr<User> p(new User());
return p;
}

User::User()
{
    InformacjeOpracowniku = InfoAboutUser::InfoAboutUser_init();
}

User::~User()
{
    std::cout<<"ZABIJAM UZYTKOWNIKA"<<std::endl;
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
        this->InformacjeOpracowniku->setIdPracownika(atoi(row[0]));
        this->InformacjeOpracowniku->setImieNazwisko(static_cast<std::string>(row[1])+static_cast<std::string>(row[2]));
        this->InformacjeOpracowniku->setAdmin(static_cast<bool>(row[6]));
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
}

std::unique_ptr<InfoAboutUser>& User::getInformacjeOpracowniku()
{
    return this->InformacjeOpracowniku;
}
