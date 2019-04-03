#include "C_Zakladka_Employee.h"
#include "Shop.h"

C_Zakladka_Employee::C_Zakladka_Employee(GtkWidget* okno_glowne_,GtkWidget* box_glowny_)
{
    this->okno_glowne = okno_glowne_;
    this->box_glowny = box_glowny_;
}
C_Zakladka_Employee::~C_Zakladka_Employee()
{

}

void C_Zakladka_Employee::build()
{
    this->scr_kontener_pracownicy = gtk_scrolled_window_new(NULL, NULL);
    g_object_ref(G_OBJECT(scr_kontener_pracownicy));

    tabela_pracownikow = gtk_tree_view_new();
    gtk_tree_view_columns_autosize(GTK_TREE_VIEW(tabela_pracownikow));
    magazyn_pracownikow = gtk_list_store_new(6,G_TYPE_INT,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_STRING,G_TYPE_INT);
    gtk_tree_view_set_model(GTK_TREE_VIEW(tabela_pracownikow), GTK_TREE_MODEL(magazyn_pracownikow));
    gtk_container_add(GTK_CONTAINER(scr_kontener_pracownicy),tabela_pracownikow);
}

void C_Zakladka_Employee::run(GtkWidget *target, gpointer arguments)
{
    std::cout<<"TU"<<std::endl;
    Shop *temp = static_cast<Shop*>(arguments);
    if(temp->get_active_button()==temp->get_BtnDostepnychKsiazek())
    {
        temp->usun_widzety_dostepnych_ksiazek();
    }
    else if(temp->get_active_button()==temp->get_BtnZamowien())
    {

    }
    temp->set_ActiveButton(temp->get_BtnPracownikow());
    gtk_table_attach_defaults(GTK_TABLE(temp->zakladka_employee->box_glowny),GTK_WIDGET(temp->zakladka_employee->scr_kontener_pracownicy),2,5,2,9);
    gtk_widget_show_all(temp->zakladka_employee->okno_glowne);
}

void C_Zakladka_Employee::remove_employee_widgets()
{
    gtk_container_remove(GTK_CONTAINER(box_glowny),scr_kontener_pracownicy);
}
