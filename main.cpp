#include <iostream>
#include <gtk/gtk.h>
#include "LoginWindow.h"
#include <User.h>
#include <memory>
#include <boost/shared_ptr.hpp>

int main(int argc,char *argv[])
{
    setlocale(LC_ALL,"");
    bool loop = true;
    //while(loop)
   // {
        std::shared_ptr<User> uzytkownik(new User());

        LoginWindow okno_logowania(uzytkownik);
        okno_logowania.init(argc,argv);
        okno_logowania.build();
        okno_logowania.run();
   // }
return 0;
}
