#ifndef PAYMENTAPPLICATIONCONTROLLER_H
#define PAYMENTAPPLICATIONCONTROLLER_H
#include <iostream>
#include <map>
#include <Ksiazka.h>

class PaymentApplicationController
{
    private:
        std::map<Ksiazka*,int> zakupy;
        int doZaplaty;
    public:
        PaymentApplicationController(std::map<Ksiazka*,int>,int);
        virtual ~PaymentApplicationController();
};

#endif // PAYMENTAPPLICATIONCONTROLLER_H
