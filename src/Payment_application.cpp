#include "Payment_application.h"
void my_thread_func()
{
    char *port_name = "\\\\.\\COM4";
    char incomingData[MAX_DATA_LENGTH];
    char output[MAX_DATA_LENGTH];

    SerialPort arduino(port_name);
    if (arduino.isConnected()) std::cout << "Connection Established" << std::endl;
    else std::cout << "ERROR, check port name";

    while (arduino.isConnected())
    {
        std::string input_string = "OPEN";
        char *c_string = new char[input_string.size() + 1];
        std::copy(input_string.begin(), input_string.end(), c_string);
        c_string[input_string.size()] = '\n';
        arduino.writeSerialPort(c_string, MAX_DATA_LENGTH);
        arduino.readSerialPort(output, MAX_DATA_LENGTH);
        std::string odp = output;
        std::cout<<odp<<std::endl;
        if(odp=="12")
        {
            std::cout<<"DZIALA"<<std::endl;
            break;
        }
        else if(odp=="Karta odrzucona")
        {
            std::cout<<"NIE"<<std::endl;
            break;
        }
        else if(odp=="Koniec czasu")
        {
            std::cout<<"Koniec czasu"<<std::endl;
        }
        delete c_string;
    }
}

Payment_application::Payment_application(std::map<Ksiazka*,int> zakupy,int pieniadze)
{
    this->kontroler = new PaymentApplicationController(zakupy,pieniadze);
    view.buildTreeViewZakupow(zakupy);
    view.buildButtons();
    view.run();

    g_signal_connect(GTK_WIDGET(this->view.getBtnPlatnoscKarta()), "clicked", G_CALLBACK(&Payment_application::TriggerNaPlatnoscKarta),this);
}

Payment_application::~Payment_application()
{
    std::cout<<"NISZCZE PAYMENT APP"<<std::endl;
    delete kontroler;
}

void Payment_application::run()
{
    gtk_main();
}

void Payment_application::TriggerNaPlatnoscKarta(GtkWidget* btn_karta,gpointer argument)
{
    Payment_application* temp = static_cast<Payment_application*>(argument);
    KartaPlatnicza karta;

    std::thread t(karta);
    t.detach();
    //std::thread t(&my_thread_func);
    //t.detach();
}
