import QtQuick 2.2

import MKBKompas.CSR.ESUTZ.Registration 1.0
import MKBKompas.CSR.ESUTZ.Ui 1.0

import "qrc:/ui_components/qml"
import ".."


Item {
    id: root
    anchors.fill: parent

    Connections {
        target: registrationForm
        onWindowClosed: {
            registrationEsu.clearChoosedData();
            uiView.gotoFirstPage();
        }
    }

    // [ Header ]
    PageHeader {
        id: userHeader
        caption: "РЕГИСТРАЦИЯ"
        secondCaption: menuView.currentMenuName
        onClose: windowManager.closeCurrentWindow();
    }

    // [ Left Side Menu ]
    MenuView {
        id: menuView
        menuItemHeight: 50
        menuBarWidth: 170
        anchors {
            top: userHeader.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        MenuItem {
            id: modMenu
            title: "РЕЖИМ РЕГИСТРАЦИИ"
            icon: "qrc:/ui_components/ui_components/add_to_list.png"

            SwitchView {
                id: uiView
                anchors.fill: parent
                focus: true
                Keys.enabled: true
                Keys.onEscapePressed: root.close();

                Component.onCompleted: {                    
                    uiView.forceActiveFocus();
                }

                MenuPage {
                    id: menuPage
                }

                RegistrationCardPage {
                    id: battleCardPage
                    onClose: windowManager.closeCurrentWindow();
                }

                StaffPage {
                    id: staffPage
                    onClose: windowManager.closeCurrentWindow();
                }

                ProgressPage {
                    id: progressPage
                    onClose: windowManager.closeCurrentWindow();
                }
            }
        }

        MenuItem {
            id: tableMenu
            title: "ТАБЛИЦА РЕГИСТРАЦИИ"
            icon: "qrc:/ui_components/ui_components/persons_list.png"            

            RegistrationTablePage {
                id: tableViewPage
                anchors.fill: parent
            }
        }

        MenuItem {
            id: regInfoMenu
            title: "КАРТОЧКА БОЙЦА"
            icon: "qrc:/ui_components/ui_components/edit_person.png"            

            RegistrationInfoPage {
                id: regInfoPage
                anchors.fill: parent
            }
        }
    }
}
