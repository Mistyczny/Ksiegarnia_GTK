#include "LoginWindow.h"

LoginWindow::LoginWindow(boost::shared_ptr<User> &target)
{
    uzytkownik = target;
}

LoginWindow::~LoginWindow()
{
    g_free(window);
    g_free(box);
    g_free(label1);
    g_free(label2);
    g_free(textBox1);
    g_free(textBox2);
    g_free(button);
    g_free(errors);
}

void LoginWindow::init(int argc,char** argv) const
{
    gtk_init(&argc,&argv);
}

void LoginWindow::build()
{
    try{
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
        label1 = gtk_label_new("Login:");
        gtk_label_set_justify(GTK_LABEL(label1), GTK_JUSTIFY_LEFT);
        gtk_table_attach_defaults (GTK_TABLE(box), label1, 0, 1, 0, 1);

        label2 = gtk_label_new("Password:");
        gtk_label_set_justify(GTK_LABEL(label2), GTK_JUSTIFY_LEFT);
        gtk_table_attach_defaults (GTK_TABLE(box), label2, 0, 1, 1, 2);

        /// creating text boxs
        textBox1 = gtk_entry_new();
        gtk_entry_set_text (GTK_ENTRY(textBox1), "kacp12345");
        gtk_table_attach_defaults (GTK_TABLE(box), textBox1, 1, 4, 0, 1);

        textBox2 = gtk_entry_new();
        gtk_entry_set_invisible_char(GTK_ENTRY(textBox2) ,'*');
        gtk_entry_set_text (GTK_ENTRY(textBox2), "kacper12");
        gtk_entry_set_visibility(GTK_ENTRY(textBox2), FALSE);
        gtk_table_attach_defaults (GTK_TABLE(box), textBox2, 1, 4, 1, 2);
        /// creating sign in button
        button = gtk_button_new();
        gtk_button_set_label(GTK_BUTTON(button), "Sign in");
        g_signal_connect(button, "clicked", G_CALLBACK(&LoginWindow::login), this);
        gtk_table_attach_defaults (GTK_TABLE(box), button, 0, 4, 2, 3);
        g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
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
    const gchar *login = (gchar*)gtk_entry_get_text(GTK_ENTRY(temp->textBox1));
    const gchar *password = (gchar*)gtk_entry_get_text(GTK_ENTRY(temp->textBox2));

    std::ostringstream make_sentence;
    make_sentence<< "SELECT * FROM pracownicy WHERE Login = '" << login <<"' AND Hasło = PASSWORD('" << password << "')";

    std::string check = make_sentence.str();
    Baza_danych baza;
    MYSQL_RES* result = baza.wyslij_zapytanie(check);
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
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
