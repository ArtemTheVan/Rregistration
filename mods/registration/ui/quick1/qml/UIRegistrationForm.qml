import QtQuick 1.1
import MKBKompas.CSR.ESUTZ.Registration 1.0

import "qrc:/ui_components/qml"


PopUpWindow {
    id: root
    anchors.centerIn: parent
    width: 400
    height: 250
    focus: true
    Keys.enabled: true
    Keys.onEscapePressed: root.closeWindow();

    signal close();

    Timer {
        id: closeTimer
        interval: 1000
        onTriggered: root.closeWindow();
    }

    Component.onCompleted: regAppUI.uiForm = root;

    function stateChanged(state) {
        switch(state)
        {
        case ESURegistration.UserRegistrationRequired:
            popUp();
            break;
        case ESURegistration.RegistrationInProcess:
            busyIndicator.start();
            break;
        case ESURegistration.BecomesRegistrar:
            busyIndicator.closeMainWindow = true;
            busyIndicator.stop();
            break;
        case ESURegistration.RegistrationSuccessfull:
            busyIndicator.stop();
            break;
        case ESURegistration.RegistrationCanceled:
            busyIndicator.stop();
            break;
            // Error block
        case ESURegistration.RegistrationError:
            busyIndicator.closeMainWindow = true;
            busyIndicator.stop();
            break;
        default: break;
        }
    }

    BorderImage {
        id: bgImage
        source: "qrc:/ui_components/images/background-transparent.png"
        width: parent.width; height: parent.height;
        border.left: 6; border.top: 6;
        border.right: 6; border.bottom: 6;
    }

    // [ Header ]

    Item {
        id: header
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        height: 50

        Item {
            id: headerContent
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: headerLine.top
            }

            Text {
                id: headerCaption
                color: "white"
                text: qsTr("РЕГИСТРАЦИЯ");
                font.family: "Arial"
                font.pixelSize: 18
                font.bold: true
                clip: true
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    horizontalCenter: parent.horizontalCenter
                }
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        HorizontalLine {
            id: headerLine
            anchors.bottom: parent.bottom
        }
    }

    // [ /Header ]
    // [ Body ]

    Item {
        id: body
        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        Column {
            anchors.centerIn: parent
            width: 180
            height: registerButton.height + closeButton.height + 10
            spacing: 5

            Button {
                id: registerButton
                height: 50
                width: parent.width
                caption: "РЕГИСТРАЦИЯ"
                onClicked: {
                    regAppUI.currentMessage = "Подготовка к регистрации";
                    busyIndicator.start();
                    regAppUI.requestRegistration();
                }
            }

            Button {
                id: closeButton
                height: 50
                width: parent.width
                caption: "ЗАКРЫТЬ"
                onClicked: root.close();
            }
        }
    }

    // [ /Body ]
    // [ Busy Indicator ]

    BusyBoxIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        message: regAppUI.currentMessage;

        property bool closeMainWindow: false
        onCloseEvent: if(busyIndicator.closeMainWindow) closeTimer.start();
    }

    // [ /Busy Indicator ]
}
