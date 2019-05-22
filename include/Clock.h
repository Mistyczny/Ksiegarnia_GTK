#ifndef CLOCK_H
#define CLOCK_H
#include <iostream>
#include <memory>
#include <time.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

class Clock
{
    private:
        Clock(GtkWidget*);

        GtkWidget* box_glowny;
    public:
        static std::unique_ptr<Clock> Clock_init(GtkWidget*);
        virtual ~Clock();
};

#endif // CLOCK_H
