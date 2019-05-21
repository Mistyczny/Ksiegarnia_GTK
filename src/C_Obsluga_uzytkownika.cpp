#include "C_Obsluga_uzytkownika.h"
#include "M_Dialog_pracownik.h"

std::unique_ptr<C_Obsluga_uzytkownika> C_Obsluga_uzytkownika::C_Obsluga_uzytkownika_init(GtkWidget* okno,GtkWidget* box,std::shared_ptr<User> uzytkownik)
{
    std::unique_ptr<C_Obsluga_uzytkownika> p(new C_Obsluga_uzytkownika(okno,box,uzytkownik));
return p;
}

C_Obsluga_uzytkownika::C_Obsluga_uzytkownika(GtkWidget* okno,GtkWidget* box,std::shared_ptr<User> uzytkownik_) :
                                                                                                        okno_glowne(okno),
                                                                                                        box_glowny(box),
                                                                                                        uzytkownik(uzytkownik_)
{
    std::cout<<"TWORZE C_OBSLUGI_UZYTKOWNIKA"<<std::endl;
}

C_Obsluga_uzytkownika::~C_Obsluga_uzytkownika()
{
    std::cout<<"NISZCZE C_OBSLUGI_UZYTKOWNIKA"<<std::endl;
    g_object_unref(G_OBJECT(L_Nazwa_uzytkownika));
    g_object_unref(G_OBJECT(ramka_uzytkownika));
    g_object_unref(G_OBJECT(Btn_wprowadz_zmiany));
}

void C_Obsluga_uzytkownika::build()
{
    L_Nazwa_uzytkownika = gtk_label_new((uzytkownik->getInformacjeOpracowniku()->getImieNazwisko()).c_str());
    gtk_label_set_markup(GTK_LABEL(L_Nazwa_uzytkownika),("<span foreground=\"blue\" font='20'>"+
                                                         ((uzytkownik->getInformacjeOpracowniku()->getImieNazwisko()))+"</span>").c_str());
    gtk_label_set_justify(GTK_LABEL(L_Nazwa_uzytkownika), GTK_JUSTIFY_LEFT);
    g_object_ref(G_OBJECT(L_Nazwa_uzytkownika));

    GtkWidget* L_tytulRamki = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(L_tytulRamki),"<span foreground=\"red\" font='13'>O mnie</span>");

    ramka_uzytkownika = gtk_frame_new(NULL);
    gtk_frame_set_label_widget(GTK_FRAME(ramka_uzytkownika),GTK_WIDGET(L_tytulRamki));
    gtk_frame_set_shadow_type(GTK_FRAME(ramka_uzytkownika), GTK_SHADOW_IN);
    gtk_frame_set_label_align(GTK_FRAME(ramka_uzytkownika),0.5,0.5);
    g_object_ref(G_OBJECT(ramka_uzytkownika));

    Btn_wprowadz_zmiany = gtk_button_new();
    gtk_button_set_label(GTK_BUTTON(Btn_wprowadz_zmiany), "Zmień hasło");
    g_signal_connect(Btn_wprowadz_zmiany, "clicked", G_CALLBACK(&C_Obsluga_uzytkownika::zmiana_hasla), this);
    g_object_ref(G_OBJECT(Btn_wprowadz_zmiany));
}

void C_Obsluga_uzytkownika::pokaz_widzety()
{
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),ramka_uzytkownika, 0, 2, 0, 3);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),L_Nazwa_uzytkownika, 0, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),Btn_wprowadz_zmiany, 0, 1, 1, 2);
}

void C_Obsluga_uzytkownika::schowaj_widzety()
{
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(L_Nazwa_uzytkownika));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(ramka_uzytkownika));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(Btn_wprowadz_zmiany));
}

void C_Obsluga_uzytkownika::wyloguj()
{
    uzytkownik->logOff();
}

void C_Obsluga_uzytkownika::zmiana_hasla(GtkWidget* target,gpointer arguments)
{
    C_Obsluga_uzytkownika* temp = static_cast<C_Obsluga_uzytkownika*>(arguments);
    Formularz_zmiany_hasla formularz(temp->uzytkownik);
    formularz.build();
    formularz.run();
}
