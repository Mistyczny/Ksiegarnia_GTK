#include "M_Dialog_pracownik.h"

/*********************************************************************************************************
*
* \brief constructor
* \brief open connection to database
* \brief creates an instance of GTK_dialog
* \brief depends on call open window to create or modify employee
* \brief Modify employee doesn't have rights to change login and password of employee
* \brief false,sets default window settings and create window-settings.xml
* \brief Creates an GTK_TABLE to storage Widgets and then attach it to GTK_DIALOG content_area
* \function runs GTK_DIALOG and shows widgets
*
**********************************************************************************************************/
M_Dialog_pracownik::M_Dialog_pracownik(std::string scheme)
{
    baza = new Baza_danych();
    message = gtk_dialog_new_with_buttons("Message",NULL,GTK_DIALOG_DESTROY_WITH_PARENT,"_ACCEPT", GTK_BUTTONS_OK, "_CANCEL",GTK_BUTTONS_CANCEL,NULL);
    GtkWidget* content_area;
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(message));

    if(scheme=="add")
    {
        tabela = gtk_table_new(12, 2, TRUE);
        gtk_table_set_row_spacings(GTK_TABLE(tabela), 2);
        gtk_table_set_col_spacings(GTK_TABLE(tabela), 2);

        gtk_container_add(GTK_CONTAINER(content_area),tabela);
        ///DOdawanie nagłówka
        L_naglowek = gtk_label_new("Dodawanie nowego pracownika");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_naglowek, 0, 2, 0, 1);

        ///Dodawanie Label Imie i nazwisko
        L_imie_i_nazwisko = gtk_label_new("Imie i nazwisko:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_imie_i_nazwisko, 0, 1, 1, 2);
        ///Dodawanie Entry na imie
        E_imie_i_nazwisko = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_imie_i_nazwisko, 1, 2,1,2);

        ///Dodawanie Label pesel
        L_pesel = gtk_label_new("Pesel:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_pesel, 0, 1, 2, 3);
        ///Dodawanie Entry na pesel
        E_pesel = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_pesel, 1, 2,2,3);

        ///Dodanie Label miasto zamieszkania
        L_miasto_zamieszkania = gtk_label_new("Miasto zamieszakania:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_miasto_zamieszkania, 0, 1,3,4);
        ///Dodanie Entry na zamieszkanie
        E_miasto_zamieszkania = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_miasto_zamieszkania, 1, 2,3,4);

        ///Dodanie Label Ulica i numer mieszkania
        L_adres_zamieszkania = gtk_label_new("Ulica i numer mieszkania:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_adres_zamieszkania, 0, 1,4,5);
        ///Dodanie Entry na Ulice i numer mieszkania
        E_adres_zamieszkania = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_adres_zamieszkania, 1, 2,4,5);

        ///Dodanie Label Ulica i numer mieszkania
        L_kod_pocztowy = gtk_label_new("Kod Pocztowy:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_kod_pocztowy, 0, 1,5,6);
        ///Dodanie Entry na Ulice i numer mieszkania
        E_kod_pocztowy = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_kod_pocztowy, 1, 2,5,6);

        ///Dodanie Label email
        L_email = gtk_label_new("Adres e-mail:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_email, 0, 1,6,7);
        ///Dodanie Entry na email
        E_email = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_email, 1, 2,6,7);

        ///Dodanie Label email
        L_tel = gtk_label_new("Nr Telefonu:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_tel, 0, 1,7,8);
        ///Dodanie Entry na email
        E_tel = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_tel, 1, 2,7,8);

        ///Dodanie Label administator
        L_administrator = gtk_label_new("Prawa Administatora(TAK/NIE):");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_administrator, 0, 1,8,9);
        ///Dodanie Entry administrator
        E_administrator = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_administrator, 1, 2,8,9);

        ///Dodanie Label login
        L_login = gtk_label_new("Login:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_login, 0, 1,9,10);
        ///Dodanie Entry login
        E_login = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_login, 1, 2,9,10);

        ///Dodanie Label hasło
        L_haslo = gtk_label_new("Hasło:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_haslo, 0, 1,10,11);
        ///Dodanie Entry hasło
        E_haslo = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_haslo, 1, 2,10,11);

        InfoAboutWrongData= gtk_label_new("");
        gtk_table_attach_defaults(GTK_TABLE(tabela),InfoAboutWrongData, 0, 2,11,12);
    }
    else if(scheme=="modify")
    {
        tabela = gtk_table_new(10, 2, TRUE);
        gtk_table_set_row_spacings(GTK_TABLE(tabela), 2);
        gtk_table_set_col_spacings(GTK_TABLE(tabela), 2);

        gtk_container_add(GTK_CONTAINER(content_area),tabela);
        ///DOdawanie nagłówka
        L_naglowek = gtk_label_new("Modyfikacja danych pracownika");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_naglowek, 0, 2, 0, 1);

        ///Dodawanie Label Imie i nazwisko
        L_imie_i_nazwisko = gtk_label_new("Imie i nazwisko:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_imie_i_nazwisko, 0, 1, 1, 2);
        ///Dodawanie Entry na imie
        E_imie_i_nazwisko = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_imie_i_nazwisko, 1, 2,1,2);

        ///Dodawanie Label pesel
        L_pesel = gtk_label_new("Pesel:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_pesel, 0, 1, 2, 3);
        ///Dodawanie Entry na pesel
        E_pesel = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_pesel, 1, 2,2,3);

        ///Dodanie Label miasto zamieszkania
        L_miasto_zamieszkania = gtk_label_new("Miasto zamieszakania:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_miasto_zamieszkania, 0, 1,3,4);
        ///Dodanie Entry na zamieszkanie
        E_miasto_zamieszkania = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_miasto_zamieszkania, 1, 2,3,4);

        ///Dodanie Label Ulica i numer mieszkania
        L_adres_zamieszkania = gtk_label_new("Ulica i numer mieszkania:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_adres_zamieszkania, 0, 1,4,5);
        ///Dodanie Entry na Ulice i numer mieszkania
        E_adres_zamieszkania = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_adres_zamieszkania, 1, 2,4,5);

        ///Dodanie Label Ulica i numer mieszkania
        L_kod_pocztowy = gtk_label_new("Kod Pocztowy:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_kod_pocztowy, 0, 1,5,6);
        ///Dodanie Entry na Ulice i numer mieszkania
        E_kod_pocztowy = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_kod_pocztowy, 1, 2,5,6);

        ///Dodanie Label email
        L_email = gtk_label_new("Adres e-mail:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_email, 0, 1,6,7);
        ///Dodanie Entry na email
        E_email = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_email, 1, 2,6,7);

        ///Dodanie Label email
        L_tel = gtk_label_new("Nr Telefonu:");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_tel, 0, 1,7,8);
        ///Dodanie Entry na email
        E_tel = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_tel, 1, 2,7,8);

        ///Dodanie Label administator
        L_administrator = gtk_label_new("Prawa Administatora(TAK/NIE):");
        gtk_table_attach_defaults(GTK_TABLE(tabela), L_administrator, 0, 1,8,9);
        ///Dodanie Entry administrator
        E_administrator = gtk_entry_new();
        gtk_table_attach_defaults(GTK_TABLE(tabela), E_administrator, 1, 2,8,9);

        InfoAboutWrongData= gtk_label_new("");
        gtk_table_attach_defaults(GTK_TABLE(tabela),InfoAboutWrongData, 0, 2,9,10);
    }
    else throw std::string("ZLY PARAMETR");
    gtk_widget_show_all(message);
}

/*********************************************************************************************************
*
* \brief destructor
* \brief close connection with database
*
**********************************************************************************************************/
M_Dialog_pracownik::~M_Dialog_pracownik()
{
    delete baza;
}

/*********************************************************************************************************
*
* \brief Function launch create new employee loop
* \brief function launch gtk_dialog_run(GTK_DIALOG)
* \case 1 - clicked save info
* \case 3 - clicked leave
* \case GTK_RESPONSE_DELETE_EVENT - clicked close window button
* \brief function in loop until info correct or window closed
* \return void
*
**********************************************************************************************************/
void M_Dialog_pracownik::nowy_pracownik()
{
    while(1)
    {
        switch(gtk_dialog_run(GTK_DIALOG(message)))
        {
            case 1:
                {
                    if(przetworz_dane()==true)
                    {
                        break;
                    }
                    else continue;
                }
            case 3:
                {
                    break;
                }
            case GTK_RESPONSE_DELETE_EVENT:
                {
                    break;
                }
        }
        break;
    }
    gtk_widget_destroy(message);
}

/*********************************************************************************************************
*
* \brief function figure out if employee with given pesel number exists
* \brief if no - gets all informations from entries
* \brief checks if informations are correct if so calls for function to save them
* \brief else sets label with text that shows which information is incorrect
* \struct data - holds informations from entries
* \return true - if all went good
* \return false - if there was incorrect information or employee with given pesel number exists
*
**********************************************************************************************************/
bool M_Dialog_pracownik::przetworz_dane()
{
    dane data;
    data.Pesel = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_pesel)));
    MYSQL_RES* result = baza->wyslij_zapytanie("SELECT * FROM PRACOWNICY WHERE PESEL='"+data.Pesel+"'");
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);
    if(row==NULL)
    {
        data.Imie_i_nazwisko = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_imie_i_nazwisko)));
        data.Miasta_zamieszkania = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_miasto_zamieszkania)));
        data.Ulica_i_numer = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_adres_zamieszkania)));
        data.Kod_pocztowy = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_kod_pocztowy)));
        data.A_email = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_email)));
        data.Nr_tel = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_tel)));
        data.Login = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_login)));
        data.Haslo = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_login)));
        data.Admin = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_administrator)));

        std::string sprawdzenie = sprawdz_czy_dane_poprawne(data);
        if(sprawdzenie=="OK")
        {
            wstaw_informacje(data);
            return true;
        }
        else
        {
            gtk_label_set_text(GTK_LABEL(InfoAboutWrongData),sprawdzenie.c_str());
            return false;
        }
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(InfoAboutWrongData),"Pracownik o tym numerze Pesel istnieje");
        return false;
    }
}
/*********************************************************************************************************
*
* \brief function figure out if all variables has correct format
* \argument struct dane - holds info about employee
* \variable std::regex wzorzec - is a regular expression object
* \return std::string "OK" if all variables are correct
* \return std::string Sentence that says which variable has incorrect format
*
**********************************************************************************************************/
std::string M_Dialog_pracownik::sprawdz_czy_dane_poprawne(dane data)
{
    /// sprawdzenie poprawnosci formy imienia i nazwiska
    std::regex wzorzec("([a-zA-Z]+)\\s([a-zA-Z]+)");
    if(!regex_match(data.Imie_i_nazwisko,wzorzec))
    {
        return "Podaj poprawne imie i nazwisko";
    }
    /// sprawdzenie peselu
    wzorzec.assign("[[:digit:]]{11}");
    if(!regex_match(data.Pesel,wzorzec))
    {
        return "Niepoprawny format peselu";
    }

    /// sprawdzenie poprawnosci formy kodu pocztowego
    wzorzec.assign("[[:digit:]]{2}\\-[[:digit:]]{3}");
    if(!regex_match(data.Kod_pocztowy,wzorzec))
    {
        return "Niepoprawny format kodu pocztowego";
    }

    /// sprawdzenie poprawnosci formy adresu e-mail
    wzorzec.assign("([a-zA-Z1-9]+)\\@([a-z1-9]+)\\.([a-z]+)");
    if(!regex_match(data.A_email,wzorzec))
    {
        return "Niepoprawny adres e-mail";
    }

    /// sprawdzenie poprawnosci formy numeru telefonu
    wzorzec.assign("[[:digit:]]{9}");
    if(!regex_match(data.Nr_tel,wzorzec))
    {
        return "Niepoprawny numer telefonu";
    }

    /// sprawdzenie czy login jest zajety
    std::string zapytanie="SELECT pracownicy.ID_Pracownika from pracownicy WHERE pracownicy.Login='"+data.Login+"';";
    MYSQL_RES* result = baza->wyslij_zapytanie(zapytanie);
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    mysql_free_result(result);
    if(row!=NULL)
    {
        return "Ten login jest zajety";
    }
return "OK";
}

/*********************************************************************************************************
*
* \brief function saves information about employee to database
* \brief checks if given adres already exists in database to avoid duplicate informations
* \brief runs gtk_dialog that shows if saving info went good or failed
* \return void
*
**********************************************************************************************************/
void M_Dialog_pracownik::wstaw_informacje(dane data)
{
    std::size_t pozycja_spacji = data.Imie_i_nazwisko.find(' ');
    std::string imie = data.Imie_i_nazwisko.substr(0,pozycja_spacji);
    std::string nazwisko = data.Imie_i_nazwisko.substr(pozycja_spacji+1);

    std::size_t pozycja_cyfr = data.Ulica_i_numer.find_first_of("1234567890");
    std::string ulica = data.Ulica_i_numer.substr(0,pozycja_cyfr);
    std::string digits = data.Ulica_i_numer.substr(pozycja_cyfr);

    std::string adres = "SELECT ID_adresu_zamieszkania FROM adres_zamieszkania WHERE Ulica='"+ulica+"' AND Nr_Domu='"+digits+"' AND Miejscowość='"+data.Miasta_zamieszkania+"'";
    MYSQL_RES* result = baza->wyslij_zapytanie(adres);
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);
    int id_adresu;

    if(row!=NULL)
    {
        id_adresu = atoi(row[0]);
        std::cout<<id_adresu<<std::endl;
    }
    else
    {
        baza->wyslij_zapytanie("INSERT INTO adres_zamieszkania (ID_adresu_zamieszkania,Ulica,Nr_Domu,Kod_Pocztowy,Miejscowość) VALUES(null,'"
                               +ulica+"',"+digits+",'"+data.Kod_pocztowy+"','"+data.Miasta_zamieszkania+"')");
        result = baza->wyslij_zapytanie("SELECT ID_adresu_zamieszkania FROM adres_zamieszkania WHERE Ulica='"
                                        +ulica+"' AND Nr_Domu="+digits+" AND Kod_Pocztowy='"+data.Kod_pocztowy+"' AND Miejscowość='"+data.Miasta_zamieszkania+"'");
        row = mysql_fetch_row(result);
        mysql_free_result(result);
        id_adresu = atoi(row[0]);
    }

    bool powodzenie = baza->modyfikuj_rekord("INSERT INTO pracownicy(ID_Pracownika,Imie,Nazwisko,Pesel,E_mail,Nr_tel,Login,Hasło,ID_adresu_zamieszkania,Administrator) VALUES(null,'"
                       +imie+"','"+nazwisko+"','"+data.Pesel+"','"+"','"+data.A_email+"','"+"','"+data.Nr_tel+"','"
                       +data.Login+"',PASSWORD('"+data.Haslo+"'),"+std::to_string(id_adresu)+","+((data.Admin=="TAK")?("1"):("0"))+")");

    if(powodzenie==true)
    {
        result_message = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK, "Udało sie dodać nowego pracownika");
        gtk_window_set_title(GTK_WINDOW(result_message), "Nowy pracownik");
        gtk_dialog_run(GTK_DIALOG(result_message));
        gtk_widget_destroy(result_message);
    }
    else
    {
        result_message = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Nie udało sie dodać nowego pracownika");
        gtk_window_set_title(GTK_WINDOW(result_message), "Nowy pracownik");
        gtk_dialog_run(GTK_DIALOG(result_message));
        gtk_widget_destroy(result_message);
    }
}
/*********************************************************************************************************
*
* \brief Function launch modify employee loop
* \brief function launch gtk_dialog_run(GTK_DIALOG)
* \case 1 - clicked save info
* \case 3 - clicked leave
* \case GTK_RESPONSE_DELETE_EVENT - clicked close window button
* \brief function in loop until info correct or window closed
* \return void
*
**********************************************************************************************************/
void M_Dialog_pracownik::modyfikuj_pracownika(int index)
{
    dane data;
    pobierz_informacje(index,data);
    while(1)
    {
        switch(gtk_dialog_run(GTK_DIALOG(message)))
        {
            case 1:
                {
                    if(sprawdz_dane(data,index)==true)
                    {
                        break;
                    }
                    else continue;
                }
            case 3:
                {
                    break;
                }
            case GTK_RESPONSE_DELETE_EVENT:
                {
                    break;
                }
        }
        break;
    }
    gtk_widget_destroy(message);
}
/*********************************************************************************************************
*
* \brief Function gets info about employee from database
* \argument index - employee ID number(unique)
* \argument dane& data - struct to hold employee data
* \brief function sets downloaeded informations to struct data and gtk_entry
* \return void
*
**********************************************************************************************************/
void M_Dialog_pracownik::pobierz_informacje(int index,dane& data)
{
    MYSQL_RES* result = baza->wyslij_zapytanie("SELECT p.Imie,p.Nazwisko,p.Pesel,p.Administrator,a.Miejscowość,a.Ulica,a.Nr_domu,a.Kod_Pocztowy,p.E_mail,p.Nr_tel,p.ID_adresu_zamieszkania FROM pracownicy p INNER JOIN adres_zamieszkania a ON p.ID_adresu_zamieszkania=a.ID_adresu_zamieszkania WHERE p.ID_Pracownika="+std::to_string(index));
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);

    data.Imie_i_nazwisko = static_cast<std::string>(row[0])  + " " + static_cast<std::string>(row[1]);
    data.Pesel = static_cast<std::string>(row[2]);
    data.Admin = static_cast<bool>(row[3])==true ? "TAK" : "NIE";
    data.Miasta_zamieszkania = static_cast<std::string>(row[4]);
    data.Ulica_i_numer = static_cast<std::string>(row[5])+" "+static_cast<std::string>(row[6]);
    data.Kod_pocztowy = static_cast<std::string>(row[7]);
    data.A_email = static_cast<std::string>(row[8]);
    data.Nr_tel = static_cast<std::string>(row[9]);
    data.id_adresu_zamieszkania = atoi(row[10]);

    gtk_entry_set_text(GTK_ENTRY(E_imie_i_nazwisko),data.Imie_i_nazwisko.c_str());
    gtk_entry_set_text(GTK_ENTRY(E_pesel),data.Pesel.c_str());
    gtk_entry_set_text(GTK_ENTRY(E_miasto_zamieszkania),data.Miasta_zamieszkania.c_str());
    gtk_entry_set_text(GTK_ENTRY(E_adres_zamieszkania),data.Ulica_i_numer.c_str());
    gtk_entry_set_text(GTK_ENTRY(E_kod_pocztowy),data.Kod_pocztowy.c_str());
    gtk_entry_set_text(GTK_ENTRY(E_email),data.A_email.c_str());
    gtk_entry_set_text(GTK_ENTRY(E_tel),data.Nr_tel.c_str());
    gtk_entry_set_text(GTK_ENTRY(E_administrator),data.Admin.c_str());
}
/*********************************************************************************************************
*
* \brief function gets all informations from entries
* \brief checks if informations are correct if so calls for function to save them
* \brief else sets label with text that shows which information is incorrect
* \struct data - holds informations from entries
* \return true - if all went good
* \return false - if there was incorrect information
*
**********************************************************************************************************/
bool M_Dialog_pracownik::sprawdz_dane(dane& data,int index)
{
    data.Imie_i_nazwisko = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_imie_i_nazwisko)));
    data.Miasta_zamieszkania = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_miasto_zamieszkania)));
    data.Ulica_i_numer = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_adres_zamieszkania)));
    data.Kod_pocztowy = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_kod_pocztowy)));
    data.A_email = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_email)));
    data.Nr_tel = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_tel)));
    data.Admin = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_administrator)));

    std::string sprawdzenie = sprawdz_czy_dane_poprawne(data);
    if(sprawdzenie=="OK")
    {
        zapisz_zmiany(data,index);
        return true;
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(InfoAboutWrongData),sprawdzenie.c_str());
        return false;
    }
}
/*********************************************************************************************************
*
* \brief function saves information about employee to database
* \brief checks if given adres already exists in database to avoid duplicate informations
* \brief runs gtk_dialog that shows if saving info went good or failed
* \return void
*
**********************************************************************************************************/
void M_Dialog_pracownik::zapisz_zmiany(dane data,int index)
{
    GtkWidget *question_message;
    question_message = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK_CANCEL, "Czy napewno chcesz wprowadzić zmiany?");
    gtk_window_set_title(GTK_WINDOW(question_message), "Modyfikacja pracownika");

    switch(gtk_dialog_run(GTK_DIALOG(question_message)))
    {
        case GTK_RESPONSE_OK:
        {
            std::size_t pozycja_spacji = data.Imie_i_nazwisko.find(' ');
            std::string imie = data.Imie_i_nazwisko.substr(0,pozycja_spacji);
            std::string nazwisko = data.Imie_i_nazwisko.substr(pozycja_spacji+1);

            std::size_t pozycja_cyfr = data.Ulica_i_numer.find_first_of("1234567890");
            std::string ulica = data.Ulica_i_numer.substr(0,pozycja_cyfr-1);
            std::string digits = data.Ulica_i_numer.substr(pozycja_cyfr);

            //SELECT ID_adresu_zamieszkania,COUNT(ID_adresu_zamieszkania) FROM adres_zamieszkania WHERE Ulica='Ogrodowa' AND Nr_domu='3' AND Miejscowość='Bochnia'

            std::string adres = "SELECT ID_adresu_zamieszkania FROM adres_zamieszkania WHERE Ulica='"+ulica+"' AND Nr_Domu='"+digits+"' AND Miejscowość='"+data.Miasta_zamieszkania+"'";
            MYSQL_RES* result = baza->wyslij_zapytanie(adres);
            MYSQL_ROW row = mysql_fetch_row(result);
            mysql_free_result(result);
            int id_adresu;

            if(row!=NULL)
            {
                data.id_adresu_zamieszkania = atoi(row[0]);
            }
            else
            {

            }

            baza->wyslij_zapytanie("UPDATE adres_zamieszkania SET Kod_Pocztowy='"+data.Kod_pocztowy+"',Miejscowość='"+data.Miasta_zamieszkania
                                  +"',Nr_Domu='"+digits+"',Ulica='"+ulica+"' WHERE ID_adresu_zamieszkania="+std::to_string(data.id_adresu_zamieszkania));

            bool powodzenie = baza->modyfikuj_rekord("UPDATE pracownicy SET Imie='"+imie+"',Nazwisko='"+nazwisko+"',Pesel='"+data.Pesel
                                                     +"',E_mail='"+data.A_email+"',Nr_tel='"+data.Nr_tel+"',ID_adresu_zamieszkania='"+std::to_string(data.id_adresu_zamieszkania)+
                                                     "',Administrator='"+((data.Admin=="TAK")?("1"):("0"))+"' WHERE ID_Pracownika="+std::to_string(index));

            if(powodzenie==true)
            {
                result_message = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_OTHER, GTK_BUTTONS_OK, "Udało sie wprowadzić zmiany");
                gtk_window_set_title(GTK_WINDOW(result_message), "Gotowe");
                gtk_dialog_run(GTK_DIALOG(result_message));
                gtk_widget_destroy(result_message);
            }
            else
            {
                result_message = gtk_message_dialog_new(NULL, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Nie udało sie wprowadzić zmian");
                gtk_window_set_title(GTK_WINDOW(result_message), "Błąd");
                gtk_dialog_run(GTK_DIALOG(result_message));
                gtk_widget_destroy(result_message);
            }
        break;
        }
        case GTK_RESPONSE_CANCEL:
        {
            break;
        }
        case GTK_RESPONSE_DELETE_EVENT:
        {
            break;
        }
    }
    gtk_widget_destroy(question_message);
}

void M_Dialog_pracownik::zabawa_z_adresem(std::string miasto,std::string ulica,std::string nr_domu)
{
    int id_adresu;
    int ilosc_powtorzen;
    std::string pracownicy = "SELECT pracownicy.ID_adresu_zamieszkania,COUNT(pracownicy.ID_adresu_zamieszkania) FROM pracownicy INNER JOIN adres_zamieszkania ON pracownicy.ID_adresu_zamieszkania=adres_zamieszkania.ID_adresu_zamieszkania WHERE Ulica='"
                             +ulica+"' AND Nr_domu='"+nr_domu+"' AND Miejscowość='"+miasto+"'";
    MYSQL_RES* result = baza->wyslij_zapytanie(pracownicy);
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);


}
