#include "InfoAboutUser.h"

std::unique_ptr<InfoAboutUser> InfoAboutUser::InfoAboutUser_init()
{
    std::unique_ptr<InfoAboutUser> p(new InfoAboutUser());
return p;
}

InfoAboutUser::InfoAboutUser()
{
    ImieNazwisko =" ";
    Pesel = " ";
    MiastoZamieszkania = " ";
    UlicaNumerDomu = " ";
    KodPocztowy  = " ";
    Email  = " ";
    NrTel  = " ";
    Admin = false;
    Login  = " ";
    Haslo  = " ";
    IdAdresuZamieszkania = 0;
}

InfoAboutUser::~InfoAboutUser()
{
    //dtor
}

void InfoAboutUser::setIdPracownika(const int myID)
{
        this-> IdPracownika = myID;
}

void InfoAboutUser::setImieNazwisko(const std::string& Imie_Nazwisko)
{
    this->ImieNazwisko = Imie_Nazwisko;
}
void InfoAboutUser::setPesel(const std::string& pes)
{
    this->Pesel = pes;
}
void InfoAboutUser::setMiastoZamieszkania(const std::string& miasto)
{
    this->MiastoZamieszkania = miasto;
}
void InfoAboutUser::setUlicaNumerDomu(const std::string& Ulica_numerDomu)
{
    this->UlicaNumerDomu = Ulica_numerDomu;
}
void InfoAboutUser::setKodPocztowy(const std::string& kod)
{
    this->KodPocztowy = kod;
}
void InfoAboutUser::setEmail(const std::string& mail)
{
    this->Email = mail;
}
void InfoAboutUser::setNrTel(const std::string& tel)
{
    this->NrTel = tel;
}
void InfoAboutUser::setAdmin(bool ad)
{
    this->Admin = ad;
}
void InfoAboutUser::setLogin(const std::string& log)
{
    this->Login = log;
}
void InfoAboutUser::setHaslo(const std::string& password)
{
    this->Haslo = password;
}
void InfoAboutUser::setIdAdresuZamieszkania(const int id)
{
    this->IdAdresuZamieszkania = id;
}

int InfoAboutUser::getIdPracownika()
{
    return this->IdPracownika;
}

std::string InfoAboutUser::getImieNazwisko()
{
    return this->ImieNazwisko;
}
std::string InfoAboutUser::getPesel()
{
    return this->Pesel;
}
std::string InfoAboutUser::getMiastoZamieszkania()
{
    return this->MiastoZamieszkania;
}
std::string InfoAboutUser::getUlicaNumerDomu()
{
    return this->UlicaNumerDomu;
}
std::string InfoAboutUser::getKodPocztowy()
{
    return this->KodPocztowy;
}
std::string InfoAboutUser::getEmail()
{
    return this->Email;
}
std::string InfoAboutUser::getNrTel()
{
    return this->NrTel;
}
bool InfoAboutUser::getAdmin()
{
    return this->Admin;
}
std::string InfoAboutUser::getLogin()
{
    return this->Login;
}
std::string InfoAboutUser::getHaslo()
{
    return this->Haslo;
}
int InfoAboutUser::getIdAdresuZamieszkania()
{
    return this->IdAdresuZamieszkania;
}

std::vector<std::string>* InfoAboutUser::returnAllInfo()
{
    std::vector<std::string>* informacje = new std::vector<std::string>;
    informacje->push_back(std::to_string(IdPracownika));
    informacje->push_back(ImieNazwisko);
    informacje->push_back(Pesel);
    informacje->push_back(MiastoZamieszkania);
    informacje->push_back(UlicaNumerDomu);
    informacje->push_back(KodPocztowy);
    informacje->push_back(Email);
    informacje->push_back(NrTel);
    informacje->push_back(std::to_string(Admin));
    informacje->push_back(Login);
    informacje->push_back(Haslo);
    informacje->push_back(std::to_string(IdAdresuZamieszkania));
return informacje;
}
