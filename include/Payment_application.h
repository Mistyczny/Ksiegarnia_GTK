#ifndef PAYMENT_APPLICATION_H
#define PAYMENT_APPLICATION_H
#include <iostream>
#include "gtk/gtk.h"
#include "Ksiazka.h"
#include <map>
#include "PaymentApplicationInterface.h"
#include "PaymentApplicationController.h"
#include "SerialPort.h"
#include <thread>
#include <string>
#include <algorithm>
#include "KartaPlatnicza.h"

class Payment_application
{
    private:
        PaymentApplicationInterface view;
        PaymentApplicationController* kontroler;
    public:
        Payment_application(std::map<Ksiazka*,int>,int);
        virtual ~Payment_application();

        void run();
        void wypisz_koszyk();

        static void TriggerNaPlatnoscKarta(GtkWidget*,gpointer);
};

#endif // PAYMENT_APPLICATION_H
