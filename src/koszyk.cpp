#include "koszyk.h"
#include "define_koszyk.h"
#include "Shop.h"

std::unique_ptr<Koszyk> Koszyk::Koszyk_init(GtkWidget *window_,GtkWidget *box_,GtkWidget* pay_app)
{
    std::unique_ptr<Koszyk> p (new Koszyk(window_,box_,pay_app));
    return p;
}

Koszyk::Koszyk(GtkWidget *window_,GtkWidget *box_,GtkWidget* pay_app)
{
    this->kontroler = koszykController::koszykController_init();
    this->view = koszykView::koszykView_init(window_,box_);
    view->pokaz_widzety();

    g_signal_connect(G_OBJECT(view->getBtnUsun()), "clicked", G_CALLBACK(&koszykView::TriggerNaUsun),this);
}

Koszyk::~Koszyk()
{
    std::cout<<"DESTRUKTOR KOSZYKA"<<std::endl;
}

std::unique_ptr<koszykView>& Koszyk::getInterface()
{
    return this->view;
}

std::unique_ptr<koszykController>& Koszyk::getController()
{
    return this->kontroler;
}
