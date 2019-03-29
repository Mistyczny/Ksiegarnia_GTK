#include "Payment_application.h"

void wolaj()
{
    std::cout<<"ALA"<<std::endl;
}

Payment_application::Payment_application(GtkWidget* payment_window)
{
    window = payment_window;
    gtk_window_set_deletable(GTK_WINDOW(window),false);
    gtk_window_set_position(GTK_WINDOW(payment_window), GTK_WIN_POS_CENTER);
    gtk_widget_set_size_request(payment_window, 600, 600);
    gtk_widget_hide(payment_window);

    box = gtk_table_new(10,10,TRUE);
    gtk_container_add(GTK_CONTAINER(payment_window), box);

    button = gtk_button_new_with_label("ALA");
    gtk_table_attach_defaults(GTK_TABLE(box), button, 0, 1, 0, 1);
    g_signal_connect(button,"clicked",G_CALLBACK(&Payment_application::hide_Payment_app),this);
}

Payment_application::~Payment_application()
{
}


void Payment_application::run(Payment_application obj)
{
    std::cout<<"AHAA"<<std::endl;
}

void Payment_application::hide_Payment_app(GtkWidget *target, gpointer arguments)
{
    Payment_application* temp = static_cast<Payment_application*>(arguments);
    gtk_widget_hide(temp->window);
}

void Payment_application::show_Payment_app(GtkWidget *target, gpointer arguments)
{
    Payment_application* temp = static_cast<Payment_application*>(arguments);
    gtk_widget_show_all(temp->window);
}