#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <iostream>
#include <gtk/gtk.h>
#include <windows.h>
#include <mysql.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <User.h>
#include "Shop.h"
#include "define_login_window.h"
#include "errors.h"
#include "Baza_danych.h"

class LoginWindow : private boost::noncopyable
{
protected:
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *label1,*label2;
    GtkWidget *textBox1,*textBox2;
    GtkWidget *button;
    GtkWidget *errors;
private:
    std::shared_ptr<User> uzytkownik;
public:
    void init(int,char**) const;
    void build();
    void run();
    static void login(GtkWidget *, gpointer);

    LoginWindow(std::shared_ptr<User>&);
    virtual ~LoginWindow();
};

#endif // LOGINWINDOW_H
