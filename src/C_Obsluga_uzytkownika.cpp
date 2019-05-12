#include "C_Obsluga_uzytkownika.h"

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
    g_object_unref(G_OBJECT(F_o_uzytkowniku));
}

void C_Obsluga_uzytkownika::build()
{
    L_Nazwa_uzytkownika = gtk_label_new((uzytkownik->getName()+" "+uzytkownik->getSurname()).c_str());
    gtk_label_set_justify(GTK_LABEL(L_Nazwa_uzytkownika), GTK_JUSTIFY_LEFT);
    g_object_ref(G_OBJECT(L_Nazwa_uzytkownika));

    GtkWidget* F_o_uzytkowniku = gtk_frame_new("O mnie");
    gtk_frame_set_shadow_type(GTK_FRAME(F_o_uzytkowniku), GTK_SHADOW_IN);
    gtk_frame_set_label_align(GTK_FRAME(F_o_uzytkowniku),0.5,0.5);
    g_object_ref(G_OBJECT(F_o_uzytkowniku));
}

void C_Obsluga_uzytkownika::pokaz_widzety()
{
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),L_Nazwa_uzytkownika, 0, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(box_glowny),F_o_uzytkowniku, 0, 2, 0, 3);
}

void C_Obsluga_uzytkownika::schowaj_widzety()
{
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(L_Nazwa_uzytkownika));
    gtk_container_remove(GTK_CONTAINER(box_glowny),GTK_WIDGET(F_o_uzytkowniku));
}
