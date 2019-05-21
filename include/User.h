#ifndef USER_H
#define USER_H

#include <iostream>
#include <gtk/gtk.h>
#include <windows.h>
#include <mysql.h>
#include <string>
#include <boost/noncopyable.hpp>
#include "Baza_danych.h"
#include "M_Dialog_pracownik.h"
#include "InfoAboutUser.h"

class User : boost::noncopyable
{
private:
    User();
    std::unique_ptr<InfoAboutUser> InformacjeOpracowniku;

    GtkWidget *message;
public:
    static std::shared_ptr<User> User_init();
    virtual ~User();
    std::unique_ptr<InfoAboutUser>& getInformacjeOpracowniku();

    void logIn(int i);
    void logOff();
};

#endif // USER_H
