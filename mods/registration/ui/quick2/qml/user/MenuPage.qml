import QtQuick 2.2

import MKBKompas.CSR.ESUTZ.Registration 1.0
import MKBKompas.CSR.ESUTZ.Ui 1.0

import "qrc:/ui_components/qml"
import ".."


SwitchViewItem {
    id: root    
    color: "transparent"

    property real fieldHeight: 40

    onReady: registrationEsu.currentUiForm = root;

    function initUserCard() {
        if( registrationEsu.isRegistered ) {

            registrationEsu.choosedName = registrationEsu.userName;
            registrationEsu.choosedRole = registrationEsu.userRole;

            if( registrationEsu.userSpecialty !== "" )
                registrationEsu.choosedSpecialty = registrationEsu.userSpecialty;

            if( registrationEsu.userRankIndex !== 0 )
                registrationEsu.choosedRankIndex = registrationEsu.userRankIndex;

            if( registrationEsu.userBirthday !== "" )
                registrationEsu.choosedBirthday = registrationEsu.userBirthday;

            if( registrationEsu.userAddress !== "" )
                registrationEsu.choosedAddress = registrationEsu.userAddress;
        }
    }

    Item {
        id: body
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: 10
        }

        Rectangle {
            id: frame
            radius: 6
            border.color: "#a8f0f0f0"
            border.width: 2
            width: 350
            height: 400
            anchors.centerIn: parent
            color: "transparent"

            Column {
                id: column
                anchors.fill: parent
                anchors.topMargin: 20
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                spacing: 10

                /*
                Label {
                    id: info
                    width: parent.width
                    height: fieldHeight                
                    radius: 5
                    textSize: 16
                    text: "СТАТУС РЕГИСТРАЦИИ:"

                    BaseLine {
                        lineWidth: 1
                    }
                }*/

                Button {
                    id: registerButton
                    width: parent.width
                    height: fieldHeight
                    label: "РЕГИСТРАЦИЯ"
                    borderWidth: 1
                    animateBackground: true
                    radius: 4
                    enableButton: !registrationEsu.isRegistered
                    onClicked: {
                        registrationEsu.state = EsuRegistration.RegisterUserState;
                        initUserCard();
                        uiView.nextView();
                    }
                }

                Button {
                    id: registrationCheckButton
                    width: parent.width
                    height: fieldHeight
                    label: "ПРОВЕРИТЬ РЕГИСТРАЦИЮ"
                    borderWidth: 1
                    animateBackground: true
                    radius: 4
                    enableButton: registrationEsu.isRegistered
                    onClicked: {
                        uiView.nextView();
                    }
                }

                Button {
                    id: changeRegisterButton
                    width: parent.width
                    height: fieldHeight
                    borderWidth: 1
                    radius: 4
                    enableButton: registrationEsu.isRegistered
                    label: "СМЕНИТЬ ПРОФИЛЬ"
                    animateBackground: true
                    onClicked: {
                        registrationEsu.state = EsuRegistration.ChangeRegistrationState;
                        initUserCard();
                        uiView.nextView();
                    }
                }

                Button {
                    id: defaultModeButton
                    width: parent.width
                    height: fieldHeight
                    borderWidth: 1
                    radius: 4
                    label: "РАБОТА В ШТАТНОМ РЕЖИМЕ"
                    animateBackground: true
                    onClicked: windowManager.closeCurrentWindow();
                }
            }
        }
    }
}

