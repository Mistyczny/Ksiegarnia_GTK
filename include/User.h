#ifndef USER_H
#define USER_H

#include <iostream>
#include <gtk/gtk.h>
#include <windows.h>
#include <mysql.h>
#include <string>
#include <sstream>
#include <cstdio>
#include <boost/noncopyable.hpp>
#include <memory>

class User : boost::noncopyable
{
private:
    int my_id;
    std::string name;
    std::string surname;
    bool administator;

    GtkWidget *error;
public:
    User();
    virtual ~User();

    void setMyId(const int);
    void setName(const std::string);
    void setSurname(const std::string);
    void setAdministator(const bool);

    const int getMyId();
    const std::string getName();
    const std::string getSurname();
    const bool getAdministator();

    void logIn(int i);
    void logOff();
};

#endif // USER_H
