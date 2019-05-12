#include "Payment_application.h"

Payment_application::Payment_application(std::map<Ksiazka*,int> zakupy)
{
    this->zbior = zakupy;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_deletable(GTK_WINDOW(window),false);

    box = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), box);

    button = gtk_button_new_with_label("ALA");
    gtk_grid_attach(GTK_GRID(box), button, 0, 1, 1, 1);
    g_signal_connect(button,"clicked",G_CALLBACK(&Payment_application::hide_Payment_app),this);
}

Payment_application::~Payment_application()
{
    std::cout<<"NISZCZE PAYMENT APP"<<std::endl;
}


void Payment_application::run()
{
    wypisz_koszyk();
    gtk_widget_show_all(window);
    gtk_main();
}

void Payment_application::hide_Payment_app(GtkWidget *target, gpointer arguments)
{
    Payment_application* temp = static_cast<Payment_application*>(arguments);
    gtk_widget_destroy(temp->window);
    gtk_main_quit();
}

void Payment_application::wypisz_koszyk()
{
    for(const auto &pair : zbior)
    {
        std::cout<<zbior[pair.first];
    }
}
