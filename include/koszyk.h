#ifndef KOSZYK_H
#define KOSZYK_H
#include <iostream>
#include <vector>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <map>
#include <thread>
#include <memory>
#include "koszykView.h"
#include "koszykController.h"

class Koszyk
{
private:
    Koszyk(GtkWidget*,GtkWidget*,GtkWidget*);
    std::unique_ptr<koszykController> kontroler;
    std::unique_ptr<koszykView> view;
public:
    GtkWidget* error;
    static std::unique_ptr<Koszyk> Koszyk_init(GtkWidget*,GtkWidget*,GtkWidget*);
    ~Koszyk();
    std::unique_ptr<koszykView>& getInterface();
    std::unique_ptr<koszykController>& getController();
};

#endif // KOSZYK_H
