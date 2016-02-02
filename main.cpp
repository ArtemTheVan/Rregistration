#include <QApplication>

#include "esu_staff_system.h"
#include "esu_registration.h"
#include "esu_registration_ui.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    esuStaff->loadData();
    esuReg->ui()->createModels();

    esuReg->initialization();

    return a.exec();
}
