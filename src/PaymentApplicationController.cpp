#include "PaymentApplicationController.h"

PaymentApplicationController::PaymentApplicationController(std::map<Ksiazka*,int> zbior,int pieniadze)
{
    this->zakupy = zbior;
    this->doZaplaty = pieniadze;
}

PaymentApplicationController::~PaymentApplicationController()
{
    std::cout<<"JA TEZ"<<std::endl;
}
