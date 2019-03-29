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
    MYSQL *connect = mysql_init(0);

    /// zeby moc pobierac polskie znaki diakrytyczne
    mysql_options(connect, MYSQL_SET_CHARSET_NAME, "utf8");
    mysql_options(connect, MYSQL_INIT_COMMAND, "SET NAMES utf8");

    if((connect = mysql_real_connect(connect, "127.0.0.1","root","","sklep_ksiazek",0,NULL,0)) == NULL)
    {
        std::cout<<"Nie udalo sie polaczyc z baza danych"<<std::endl;
    }
    else
    {
        std::string questionToDatabase = "SELECT * FROM pracownicy WHERE ID_Pracownika = " + std::to_string(i);

        if(mysql_query(connect,questionToDatabase.c_str()))
        {
            error = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Error 101");
            gtk_window_set_title(GTK_WINDOW(error), "Error");
            gtk_dialog_run(GTK_DIALOG(error));
            gtk_widget_destroy(error);
            return;
        }
        else
        {
            MYSQL_RES* result = mysql_store_result(connect);
            MYSQL_ROW row;

            row = mysql_fetch_row(result);
            if(row!=NULL)
            {
                this->my_id = atoi(row[0]);
                this->name = row[1];
                this->surname = row[2];
                this->administator = row[6];
            }
            else
            {
                error = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Nie posiadamy informacji o tym pracowniku");
                gtk_window_set_title(GTK_WINDOW(error), "Error");
                gtk_dialog_run(GTK_DIALOG(error));
                gtk_widget_destroy(error);
            }
            mysql_free_result(result);
        }
    }
mysql_close(connect);
}

void User::logOff()
{
    my_id = 0;
    name = "";
    surname = "";
    administator = false;
}
