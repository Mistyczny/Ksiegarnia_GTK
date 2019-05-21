#include "Formularz_zmiany_hasla.h"
#define  STARE_HASLO    zbior_hasel[0]
#define  NOWE_HASLO     zbior_hasel[1]
#define POWTORZ_HASLO   zbior_hasel[2]

Formularz_zmiany_hasla::Formularz_zmiany_hasla(std::shared_ptr<User> user_)
{
    this->uzytkownik = user_;
    okno_formularza = gtk_dialog_new_with_buttons("Message",NULL,GTK_DIALOG_DESTROY_WITH_PARENT,"_ACCEPT",
                                                  GTK_BUTTONS_OK, "_CANCEL",GTK_BUTTONS_CANCEL,NULL);
}

Formularz_zmiany_hasla::~Formularz_zmiany_hasla()
{
}

void Formularz_zmiany_hasla::build()
{
    GtkWidget* content_area = gtk_dialog_get_content_area(GTK_DIALOG(okno_formularza));

    tabela = gtk_table_new(5, 2, TRUE);
    gtk_table_set_row_spacings(GTK_TABLE(tabela), 2);
    gtk_table_set_col_spacings(GTK_TABLE(tabela), 2);

    gtk_container_add(GTK_CONTAINER(content_area),tabela);

    L_naglowek = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(L_naglowek),"<span foreground=\"blue\" font='16'>Modyfikacja hasła</span>");
    gtk_table_attach_defaults(GTK_TABLE(tabela), L_naglowek, 0, 2, 0, 1);


    L_stare_haslo = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(L_stare_haslo),"<span foreground=\"blue\" font='13'>Obecne hasło:</span>");
    gtk_table_attach_defaults(GTK_TABLE(tabela),L_stare_haslo, 0, 1, 1, 2);

    E_stare_haslo = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(tabela), E_stare_haslo, 1, 2,1,2);
    gtk_entry_set_invisible_char(GTK_ENTRY(E_stare_haslo) ,'*');
    gtk_entry_set_visibility(GTK_ENTRY(E_stare_haslo), FALSE);

    L_nowe_haslo = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(L_nowe_haslo),"<span foreground=\"blue\" font='13'>Nowe hasło:</span>");
    gtk_table_attach_defaults(GTK_TABLE(tabela),L_nowe_haslo, 0, 1, 2, 3);

    E_nowe_haslo = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(tabela),E_nowe_haslo, 1, 2,2,3);
    gtk_entry_set_invisible_char(GTK_ENTRY(E_nowe_haslo) ,'*');
    gtk_entry_set_visibility(GTK_ENTRY(E_nowe_haslo), FALSE);

    L_powtorz_haslo = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(L_powtorz_haslo),"<span foreground=\"blue\" font='13'>Powtórz hasło:</span>");
    gtk_table_attach_defaults(GTK_TABLE(tabela),L_powtorz_haslo, 0, 1, 3, 4);

    E_powtorz_haslo = gtk_entry_new();
    gtk_table_attach_defaults(GTK_TABLE(tabela), E_powtorz_haslo, 1, 2,3,4);
    gtk_entry_set_invisible_char(GTK_ENTRY(E_powtorz_haslo) ,'*');
    gtk_entry_set_visibility(GTK_ENTRY(E_powtorz_haslo), FALSE);

    L_komentarz = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(L_komentarz),"<span foreground=\"blue\" font='13'></span>");
    gtk_table_attach_defaults(GTK_TABLE(tabela),L_komentarz, 0, 2, 4, 5);

    gtk_widget_show_all(okno_formularza);
}

void Formularz_zmiany_hasla::run()
{
    while(1)
    {
        switch(gtk_dialog_run(GTK_DIALOG(okno_formularza)))
        {
            case 1:
                {
                    if(zmiana_hasla()==true) break;
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
    gtk_widget_destroy(okno_formularza);
}

bool Formularz_zmiany_hasla::zmiana_hasla()
{
    std::vector<std::string> zbior_hasel;
    pobierz_dane_z_formularza(zbior_hasel);
    if(sprawdz_stare_haslo(STARE_HASLO))
    {
        if(NOWE_HASLO==POWTORZ_HASLO)
        {
            wprowadz_zmiane_hasla(NOWE_HASLO);
            return true;
        }
        else
        {
            gtk_label_set_markup(GTK_LABEL(L_komentarz),"<span foreground=\"blue\" font='13'>Hasła nie pasują do siebie</span>");
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool Formularz_zmiany_hasla::sprawdz_stare_haslo(const std::string& haslo)
{
    std::string pytanie =   "SELECT Imie FROM pracownicy WHERE Hasło=PASSWORD('"+haslo+"') AND ID_Pracownika="
                            +std::to_string(uzytkownik->getInformacjeOpracowniku()->getIdPracownika())+";";
    Baza_danych baza;
    MYSQL_RES* result = baza.wyslij_pytanie(pytanie);
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);
    if(row==NULL)
    {
        gtk_label_set_markup(GTK_LABEL(L_komentarz),"<span foreground=\"blue\" font='13'>Podano błędne hasło</span>");
        return false;
    }
    else
    {
        return true;
    }
}

void Formularz_zmiany_hasla::pobierz_dane_z_formularza(std::vector<std::string>& hasla)
{
    hasla.push_back(static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_stare_haslo))));
    hasla.push_back(static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_nowe_haslo))));
    hasla.push_back(static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(E_powtorz_haslo))));
}

void Formularz_zmiany_hasla::wprowadz_zmiane_hasla(std::string& nowe_haslo)
{
    std::string pytanie =   "UPDATE pracownicy SET Hasło=PASSWORD('"+nowe_haslo+"') WHERE ID_Pracownika="
                            +std::to_string(uzytkownik->getInformacjeOpracowniku()->getIdPracownika());
    Baza_danych baza;
    baza.wyslij_pytanie(pytanie);
}
