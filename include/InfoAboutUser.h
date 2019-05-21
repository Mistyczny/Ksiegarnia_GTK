#ifndef INFOABOUTUSER_H
#define INFOABOUTUSER_H
#include <memory>
#include <iostream>
#include <string>
#include <vector>

class InfoAboutUser
{
    private:
        InfoAboutUser();
        int IdPracownika;
        std::string ImieNazwisko;
        std::string Pesel;
        std::string MiastoZamieszkania;
        std::string UlicaNumerDomu;
        std::string KodPocztowy;
        std::string Email;
        std::string NrTel;
        bool Admin;
        std::string Login;
        std::string Haslo;
        int IdAdresuZamieszkania;
    public:
        static std::unique_ptr<InfoAboutUser> InfoAboutUser_init();
        virtual ~InfoAboutUser();

        void setIdPracownika(const int);
        void setImieNazwisko(const std::string&);
        void setPesel(const std::string&);
        void setMiastoZamieszkania(const std::string&);
        void setUlicaNumerDomu(const std::string&);
        void setKodPocztowy(const std::string&);
        void setEmail(const std::string&);
        void setNrTel(const std::string&);
        void setAdmin(bool);
        void setLogin(const std::string&);
        void setHaslo(const std::string&);
        void setIdAdresuZamieszkania(const int);

        int getIdPracownika();
        std::string getImieNazwisko();
        std::string getPesel();
        std::string getMiastoZamieszkania();
        std::string getUlicaNumerDomu();
        std::string getKodPocztowy();
        std::string getEmail();
        std::string getNrTel();
        bool getAdmin();
        std::string getLogin();
        std::string getHaslo();
        int getIdAdresuZamieszkania();
        std::vector<std::string>* returnAllInfo();
};

#endif // INFOABOUTUSER_H
