#ifndef KARTAPLATNICZA_H
#define KARTAPLATNICZA_H
#include "SerialPort.h"
#include <iostream>
#include "gtk/gtk.h"

class KartaPlatnicza
{
    private:
        char *port_name;
        std::string kodUdanejPlatnosci;
        std::string kodNieudanejPlatnosci;
        std::string kodUplynieciaCzasu;

        GtkWidget* komunikat;
    public:
        KartaPlatnicza();
        virtual ~KartaPlatnicza();
        int OpenConnection();
        void operator()();

        void komunikatUdanejPlatnosci();
};

#endif // KARTAPLATNICZA_H
