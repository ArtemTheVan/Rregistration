import QtQuick 2.2

import MKBKompas.CSR.ESUTZ.Registration 1.0
import MKBKompas.CSR.ESUTZ.Ui 1.0

import "qrc:/ui_components/qml"
import ".."


SwitchViewItem {
    id: root
    color: "transparent"
    border.color: "#a8f0f0f0"
    border.width: 1

    property real fieldHeight: 40

    signal close();

    onReady: {        
        registrationEsu.currentUiForm = root;        

        switch(registrationEsu.state)
        {
        case EsuRegistration.RegisterUserState:
            actionButton.label = "РЕГИСТРАЦИЯ";
            actionButton.enableButton = (registrationEsu.choosedRole !== "");
            break;
        case EsuRegistration.ChangeRegistrationState:
            actionButton.label = "СМЕНИТЬ ПРОФИЛЬ";
            actionButton.enableButton = ((registrationEsu.choosedRole !== "")
                                         && (registrationEsu.choosedRole !== registrationEsu.userRole));
            break;
        }
    }

    PageFooter {
        id: footer
        onPreviousPage: uiView.previousView();
    }

    Flickable {
        id: content
        clip: true
        anchors {
            left: parent.left
            right: parent.right //tools.left
            top: parent.top
            bottom: footer.top
        }
        interactive: true
        contentWidth: width
        contentHeight: columnWrapper.height
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        property int spacing: 15
        property real itemsHeight: 35

        Column {
            id: columnWrapper
            spacing: 15
            width: parent.width - 2*content.spacing;
            x: content.spacing
            y: 15

            Row {
                height: content.itemsHeight
                width: parent.width
                spacing: 2

                Label {
                    id: lbRegRoleText
                    text: "ПРОФИЛЬ"
                    textSize: 16
                    height: parent.height
                    width: parent.width/2 - 4
                }

                Button {
                    id: chooseRoleButton
                    height: parent.height
                    width: height
                    iconAlign: Qt.AlignTop
                    iconSteady: "qrc:/ui_components/ui_components/add_person.png"
                    radius: 4
                    borderWidth: 1
                    onClicked: {
                        uiView.nextView();
                    }
                }

                Button {
                    id: regRoleButton
                    label: (registrationEsu.choosedRole !== "")?(registrationEsu.choosedRole):("ВЫБРАТЬ ПРОФИЛЬ");
                    textSize: 16
                    height: parent.height
                    width: (parent.width/2 - chooseRoleButton.width)
                    radius: 4
                    borderWidth: 1
                    onClicked: {
                        uiView.nextView();
                    }
                    onLabelChanged: {
                        if( registrationEsu.choosedRole !== "" &&
                                registrationEsu.choosedRole !== registrationEsu.userRole ) {
                            actionButton.enableButton = true;
                        } else {
                            actionButton.enableButton = false;
                        }
                    }
                }
            }

            Row {
                height: content.itemsHeight
                width: parent.width

                Label {
                    id: lbDutyText
                    text: "ДОЛЖНОСТЬ"
                    textSize: 16
                    height: parent.height
                    width: parent.width/2
                }

                Label {
                    id: lbDutyRole
                    text: registrationEsu.choosedDuty;
                    textSize: 16
                    height: parent.height
                    width: parent.width/2
                    radius: 4
                    border.width: 1
                    border.color: "white"
                }
            }

            Row {
                height: content.itemsHeight
                width: parent.width

                Label {
                    id: lbSpecialityText
                    text: "ВУС"
                    textSize: 16
                    height: parent.height
                    width: parent.width/2
                }

                TextField {
                    id: fieldSpeciality
                    text: registrationEsu.choosedSpecialty
                    textSize: 16
                    radius: 4
                    borderWidth: 1
                    height: parent.height
                    width: parent.width/2
                    placeholderText: "Военно-учётная специальность";
                    onEditingFinished: {
                        registrationEsu.choosedSpecialty = fieldSpeciality.text;
                    }
                }
            }

            HorizontalLine { anchors.left: undefined; anchors.right: undefined; width: parent.width; }

            EsuRegistrationRanksModel { id: ranksModel; }

            ComboBox {
                id: comboRankList
                textSize: 16
                width: parent.width
                expandHeight: 170
                borderWidth: 1
                description: "ЗВАНИЕ"
                model: ranksModel // regRanksModel
                currentIndex: registrationEsu.choosedRankIndex;
                enableButton: (registrationEsu.choosedRole !== "");
                onSelected: {
                    registrationEsu.choosedRankIndex = index;
                }
            }

            Row {
                height: content.itemsHeight
                width: parent.width

                Label {
                    id: lbNameText
                    text: "ФИО"
                    textSize: 16
                    height: parent.height
                    width: parent.width/2
                }

                TextField {
                    id: fieldName
                    text: registrationEsu.choosedName;
                    textSize: 16
                    focusChain: true
                    radius: 4
                    borderWidth: 1
                    height: parent.height
                    width: parent.width/2
                    placeholderText: "ФИО";
                    onEditingFinished: {
                        registrationEsu.choosedName = fieldName.text;
                    }
                }
            }

            Row {
                height: content.itemsHeight
                width: parent.width

                Label {
                    id: lbBirthdayText
                    text: "ДАТА РОЖДЕНИЯ"
                    textSize: 16
                    height: parent.height
                    width: parent.width/2
                }

                TextField {
                    id: fieldBirthday
                    text: registrationEsu.choosedBirthday
                    textSize: 16
                    focusChain: true
                    radius: 4
                    borderWidth: 1
                    height: parent.height
                    width: parent.width/2
                    placeholderText: "Дата рождения";
                    onEditingFinished: registrationEsu.choosedBirthday = fieldBirthday.text;
                }
            }

            Row {
                height: content.itemsHeight
                width: parent.width

                Label {
                    id: lbAddressText
                    text: "АДРЕС"
                    textSize: 16
                    height: parent.height
                    width: parent.width/2
                }

                TextField {
                    id: fieldAddress
                    text: registrationEsu.choosedAddress
                    textSize: 16
                    radius: 4
                    borderWidth: 1
                    height: parent.height
                    width: parent.width/2
                    placeholderText: "Адрес (место жительства)"
                    onEditingFinished: registrationEsu.choosedAddress = fieldAddress.text;
                }
            }

            Row {
                height: 40;
                width: parent.width
                spacing: 10

                Button {
                    id: actionButton
                    label: "РЕГИСТРАЦИЯ";
                    textSize: 16                    
                    height: parent.height
                    width: 190
                    radius: 4
                    borderWidth: 1
                    enableButton: false;
                    onClicked: {                        
                        uiView.gotoLastPage();
                    }
                }

                Button {
                    id: clearButton
                    label: "ОЧИСТИТЬ";
                    textSize: 16
                    height: parent.height
                    width: 190
                    radius: 4
                    borderWidth: 1
                    onClicked: registrationEsu.clearChoosedData();
                }

                Button {
                    id: cancelOperationButton
                    label: "ОТМЕНА";
                    textSize: 16
                    height: parent.height
                    width: 190
                    radius: 4
                    borderWidth: 1
                    onClicked: {
                        registrationEsu.clearCurrentOperation();
                        registrationEsu.clearChoosedData();
                        uiView.gotoFirstPage();
                    }
                }
            }
        }
    }
}
