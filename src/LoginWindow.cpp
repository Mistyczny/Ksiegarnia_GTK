#include "LoginWindow.h"

static void apply_css (GtkWidget *widget, GtkStyleProvider *provider)
{
  gtk_style_context_add_provider (gtk_widget_get_style_context (widget), provider, G_MAXUINT);
  if (GTK_IS_CONTAINER (widget))
    gtk_container_forall (GTK_CONTAINER (widget), (GtkCallback) apply_css, provider);
}

LoginWindow::LoginWindow(std::shared_ptr<User> &target)
{
    uzytkownik = target;
}

LoginWindow::~LoginWindow()
{

}

void LoginWindow::init(int argc,char** argv) const
{
    gtk_init(&argc,&argv);
}

void LoginWindow::build()
{
    try{
        GtkStyleProvider *provider;
        GBytes *bytes;
        gsize data_size;
        gconstpointer data;

    /// Login Window settings
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_widget_set_size_request(window, LOGINWINDOW_X, LOGINWINDOW_Y);
        gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
        gtk_window_set_title(GTK_WINDOW(window), "Ksiegarnia - logowanie");

        /// creating vbox for widgets
        box = gtk_table_new(BOX_SIZE_X,BOX_SIZE_Y,TRUE);
        gtk_container_set_border_width(GTK_CONTAINER(box), BORDER_WIDTH);
        gtk_container_add(GTK_CONTAINER(window), box);

        /// creating text labels
        L_login = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(L_login),"<span foreground=\"red\" font='13'>Login:</span>");
        gtk_label_set_justify(GTK_LABEL(L_login), GTK_JUSTIFY_LEFT);
        gtk_table_attach_defaults (GTK_TABLE(box), L_login, 0, 1, 0, 1);

        L_haslo = gtk_label_new(NULL);
        gtk_label_set_markup(GTK_LABEL(L_haslo),"<span foreground=\"red\" font='13'>Hasło:</span>");
        gtk_label_set_justify(GTK_LABEL(L_haslo), GTK_JUSTIFY_LEFT);
        gtk_table_attach_defaults (GTK_TABLE(box), L_haslo, 0, 1, 1, 2);

        /// creating text boxs
        E_login = gtk_entry_new();
        gtk_entry_set_text (GTK_ENTRY(E_login), "kacp12345");
        gtk_table_attach_defaults (GTK_TABLE(box), E_login, 1, 4, 0, 1);

        E_haslo = gtk_entry_new();
        gtk_entry_set_invisible_char(GTK_ENTRY(E_haslo) ,'*');
        gtk_entry_set_text (GTK_ENTRY(E_haslo), "kacper12");
        gtk_entry_set_visibility(GTK_ENTRY(E_haslo), FALSE);
        gtk_table_attach_defaults (GTK_TABLE(box), E_haslo, 1, 4, 1, 2);
        /// creating sign in button
        Btn_zaloguj = gtk_button_new();
        gtk_button_set_label(GTK_BUTTON(Btn_zaloguj), "Zaloguj");
        g_signal_connect(Btn_zaloguj, "clicked", G_CALLBACK(&LoginWindow::login), this);
        gtk_table_attach_defaults (GTK_TABLE(box), Btn_zaloguj, 0, 4, 2, 3);
        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);


        provider = GTK_STYLE_PROVIDER (gtk_css_provider_new());
        bytes = g_resources_lookup_data ("../proba_css.css", G_RESOURCE_LOOKUP_FLAGS_NONE, NULL);
       // data = g_bytes_get_data (bytes, &data_size);
       // gtk_css_provider_load_from_data (GTK_CSS_PROVIDER (provider), (gchar *)data, data_size, NULL);
        //g_bytes_unref (bytes);
        //apply_css (window, provider);
    }
    catch(std::bad_alloc& al)
    {
        errors = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, LOGIN_MEMORY_ALLOCATE);
        gtk_window_set_title(GTK_WINDOW(errors), "Error");
        gtk_dialog_run(GTK_DIALOG(errors));
        gtk_widget_destroy(errors);
        exit(0);
        return;
    }
}

void LoginWindow::run()
{
    gtk_widget_show_all(window);
    gtk_main();
}

void LoginWindow::login(GtkWidget *target, gpointer arguments)
{
    LoginWindow* temp = static_cast<LoginWindow*>(arguments);
    std::string login = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(temp->E_login)));
    std::string password = static_cast<std::string>(gtk_entry_get_text(GTK_ENTRY(temp->E_haslo)));

    std::string sentence = "SELECT * FROM pracownicy WHERE Login = '"+login+"' AND Hasło = PASSWORD('"+password+"')";

    Baza_danych baza;
    MYSQL_RES* result = baza.wyslij_pytanie(sentence);
    MYSQL_ROW row = mysql_fetch_row(result);
    mysql_free_result(result);

    if(row!=NULL)
    {
        temp->uzytkownik->logIn(atoi(row[0]));
        gtk_widget_destroy(temp->window);
        gtk_main_quit();
        Shop sklep(temp->uzytkownik);
        sklep.run();
    }
}
