#include "KartaPlatnicza.h"

#define portError 10
#define platnoscPotwierdzona 11
#define platnoscNieudana 12
#define runtimeError 13

KartaPlatnicza::KartaPlatnicza()
{
    port_name = "\\\\.\\COM4";
    kodUdanejPlatnosci = "11";
    kodNieudanejPlatnosci = "12";
    kodUplynieciaCzasu = "13";
}

KartaPlatnicza::~KartaPlatnicza()
{
    //dtor
}

int KartaPlatnicza::OpenConnection()
{
    char incomingData[MAX_DATA_LENGTH];
    char output[MAX_DATA_LENGTH];

    SerialPort arduino(port_name);
    if(!arduino.isConnected())
    {
        std::cout << "ERROR, check port name";
        return portError;
    }

    while (arduino.isConnected())
    {
        std::string input_string = "OPEN";
        char *c_string = new char[input_string.size() + 1];
        std::copy(input_string.begin(), input_string.end(), c_string);
        c_string[input_string.size()] = '\n';
        arduino.writeSerialPort(c_string, MAX_DATA_LENGTH);

        arduino.readSerialPort(output, MAX_DATA_LENGTH);
        std::string odpowiedzUrzadzenia = output;
        odpowiedzUrzadzenia.resize(2);

        if(odpowiedzUrzadzenia==kodUdanejPlatnosci)
        {
            return platnoscPotwierdzona;
        }
        else if(odpowiedzUrzadzenia==kodNieudanejPlatnosci)
        {
            return platnoscNieudana;
        }
        else if(odpowiedzUrzadzenia==kodUplynieciaCzasu)
        {
            return runtimeError;
        }
        delete c_string;
    }
}

void KartaPlatnicza::operator()()
{
    switch(OpenConnection())
    {
        case portError:
        {

            break;
        }
        case platnoscPotwierdzona:
        {
            this->komunikatUdanejPlatnosci();
            break;
        }
        case platnoscNieudana:
        {
            break;
        }
        case runtimeError:
        {

            break;
        }
    }
}

void KartaPlatnicza::komunikatUdanejPlatnosci()
{
    komunikat = gtk_message_dialog_new (NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Płatność dokonana pomyślnie");
    gtk_window_set_title(GTK_WINDOW(komunikat), "Komunikat");
    gtk_dialog_run(GTK_DIALOG(komunikat));
    gtk_widget_destroy(GTK_WIDGET(komunikat));
}
