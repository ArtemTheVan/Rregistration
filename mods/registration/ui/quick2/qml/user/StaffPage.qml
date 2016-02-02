import QtQuick 2.2

import MKBKompas.CSR.ESUTZ.Registration 1.0
import MKBKompas.CSR.ESUTZ.Staff 1.0
import MKBKompas.CSR.ESUTZ.Ui 1.0

import "qrc:/ui_components/qml"
import ".."


SwitchViewItem {
    id: root
    color: "transparent"
    border.color: "#a8f0f0f0"
    border.width: 1

    function arrowUp() {
        if( mainView.currentIndex === 0) {
            mainView.currentIndex = mainView.count - 1;
        } else {
            mainView.decrementCurrentIndex();
        }
    }

    function arrowDown() {
        if( mainView.currentIndex === (mainView.count - 1)) {
            mainView.currentIndex = 0;
        } else {
            mainView.incrementCurrentIndex();
        }
    }

    property real fieldHeight: 40

    signal close();
    signal confirm();
    signal reject();

    // [ Dialog ]:

    ConfirmationDialog {
        id: confirmDialog
        title: "Выбор роли"
        message: ("Подтвердите выбранную роль:\n" + registrationEsu.choosedRole);
        onAccepted: uiView.previousView();
        onRejected: registrationEsu.choosedRole = "";
    }

    // [ Content ]:

    onReady: {        
        registrationEsu.currentUiForm = root;
    }

    PageFooter {
        id: footer
        onPreviousPage: { uiView.previousView(); }
    }

    PathPanel {
        id: pathPanel
        height: 50
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }

    Item {
        id: body
        anchors {
            top: pathPanel.bottom
            left: parent.left
            right: parent.right
            bottom: footer.top
            //margins: 5
        }

        ListView {
            id: mainView
            anchors.fill: parent
            anchors.rightMargin: 0
            clip: true
            //boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.VerticalFlick

            property Item clickedItem: null           

            model: registrationStaffModel
            delegate: StaffItemDelegate {
                id: itemDelegate
                checkable: registrationStaffModel.checkable
                registered: registeredRole
                iconSource: Qt.resolvedUrl(imageRole);
                type: typeRole
                role: roleNameBaseRole
                itemIndex: indexRole
                roleFullName: roleNameFullRole
                netAddress: addressRole
                name: nameRole
                descrFirst: descrFirstRole

                Component.onCompleted: {
                    if( registered && itemDelegate.name ) {
                        itemDelegate.registrationInfo = "[" + itemDelegate.name + "]";
                    }
                }

                onUpdateModel: {
                    registrationStaffModel.updateData();
                }

                onItemChoosed: {
                    registrationEsu.choosedRole = roleFullName;
                    confirmDialog.open();
                    //uiView.nextView();
                }
            }

            states: [
                State {
                    name: "scrolling"
                    when: mainView.movingVertically
                    PropertyChanges { target: scrollBar; opacity: 1.0 }
                }
            ]

            transitions: [
                Transition { NumberAnimation { target: scrollBar; properties: "opacity"; duration: 300; } }
            ]
        }

        ScrollBar {
            id: scrollBar
            opacity: 0.0
            anchors {
                top: parent.top
                right: parent.right
                bottom: parent.bottom
            }
            width: 6
            position: mainView.visibleArea.yPosition
            pageSize: mainView.visibleArea.heightRatio
        }
    }
}
