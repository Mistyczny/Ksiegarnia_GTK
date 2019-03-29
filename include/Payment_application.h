#ifndef PAYMENT_APPLICATION_H
#define PAYMENT_APPLICATION_H
#include <iostream>
#include "gtk/gtk.h"
//#include "koszyk.h"
#include "Ksiazka.h"
#include <map>

class Payment_application
{
    public:
        GtkWidget* window;
        GtkWidget* box;
        GtkWidget* button;

        Payment_application();
        Payment_application(GtkWidget* window);
        Payment_application(std::map<Ksiazka*,int>);

        static void wolaj()
        {
            std::cout<<"JESTEM WATKIEM"<<std::endl;
        }
        virtual ~Payment_application();

        static void run(Payment_application object);
        static void hide_Payment_app(GtkWidget *target, gpointer arguments);
        static void show_Payment_app(GtkWidget *target, gpointer arguments);

    protected:

    private:
};

#endif // PAYMENT_APPLICATION_H
