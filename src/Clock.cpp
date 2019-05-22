#include "Clock.h"

std::unique_ptr<Clock> Clock::Clock_init(GtkWidget* box_)
{
    std::unique_ptr<Clock> p(new Clock(box_));
return p;
}
Clock::Clock(GtkWidget* box_)
{
    box_glowny = box_;
}

Clock::~Clock()
{
    //dtor
}
