import QtQuick 2.2

import MKBKompas.CSR.ESUTZ.Registration 1.0
import MKBKompas.CSR.ESUTZ.Ui 1.0

import "qrc:/ui_components/qml"
import "../user/" as User
import ".."


Item {
    id: root
    anchors.fill: parent


    // [ Header ]
    PageHeader {
        id: header
        caption: "РЕЖИМ РЕГИСТРАТОРА"
        secondCaption: menuView.currentMenuName
        onClose: windowManager.closeCurrentWindow();
    }

    // [ Left Side Menu ]
    MenuView {
        id: menuView
        menuItemHeight: 50
        menuBarWidth: 150
        anchors {
            top: header.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        MenuItem {
            id: menuQueriesAdd
            title: "ЗАПРОСЫ"
            icon: "qrc:/ui_components/ui_components/add_to_list.png"

            RegistrationQueriesPage {
                id: queriesPage
                onClose: windowManager.closeCurrentWindow();
            }
        }

        MenuItem {
            id: menuRegistrationTable
            title: "ТАБЛИЦА РЕГИСТРАЦИИ"
            icon: "qrc:/ui_components/ui_components/persons_list.png"

            RegistrationTablePage {
                id: registrationTablePage
                onClose: windowManager.closeCurrentWindow();
            }
        }

        MenuItem {
            id: regInfoMenu
            title: "КАРТОЧКА БОЙЦА"
            icon: "qrc:/ui_components/ui_components/edit_person.png"

            User.RegistrationInfoPage {
                id: regInfoPage
                onClose: windowManager.closeCurrentWindow();
            }
        }
    }
}

