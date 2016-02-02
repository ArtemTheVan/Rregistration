import QtQuick 2.2

import MKBKompas.CSR.ESUTZ.Registration 1.0
import MKBKompas.CSR.ESUTZ.Ui 1.0

import "qrc:/ui_components/qml"
import ".."


Item {
    id: root
    anchors.fill: parent

    signal close();
    signal ready();

    onReady: {
        registrationEsu.currentUiForm = content;
        manager.initFields();
    }

    property bool editMode: false

    QtObject {
        id: manager
        property bool hasChanges: false
        property string nameField
        property string birthdayField
        property string addressField
        property string specialityField;
        property int rankIndex: 0;

        function clear() {
            hasChanges = false;
            nameField = "";
            specialityField = "";
            birthdayField = "";
            addressField = "";

            rankIndex = 0;
            comboRankList.currentIndex = rankIndex;
        }

        function initFields() {
            hasChanges = false;
            nameField = registrationEsu.userName;
            specialityField = registrationEsu.userSpecialty;
            birthdayField = registrationEsu.userBirthday;
            addressField = registrationEsu.userAddress;
            rankIndex = (registrationEsu.userRankIndex >= 0)?(registrationEsu.userRankIndex):(0);
            comboRankList.currentIndex = rankIndex;
        }

        function restoreFields() {
            if( hasChanges ) {
                hasChanges = false;
                nameField = registrationEsu.userName;
                specialityField = registrationEsu.userSpecialty;
                birthdayField = registrationEsu.userBirthday;
                addressField = registrationEsu.userAddress;
                rankIndex = (registrationEsu.userRankIndex >= 0)?(registrationEsu.userRankIndex):(0);
                comboRankList.currentIndex = rankIndex;
            }
        }
    }

    Flickable {
        id: content
        clip: true
        anchors {
            left: parent.left
            right: parent.right //tools.left
            top: parent.top
            bottom: parent.bottom
        }
        interactive: true
        contentWidth: width
        contentHeight: columnWrapper.height
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        property int spacing: 15

        Column {
            id: columnWrapper
            spacing: 10
            width: parent.width - 2*content.spacing;
            x: content.spacing
            y: 15

            Row {
                height: 30
                width: parent.width

                Label {
                    id: lbDateText
                    text: "ЗАРЕГИСТРИРОВАН"
                    textSize: 16
                    height: parent.height
                    width: parent.width/2
                }

                Label {
                    id: lbDate
                    text: registrationEsu.isRegistered?(registrationEsu.registrationTime):("НЕ ЗАРЕГИСТРИРОВАН");
                    textSize: 16
                    height: parent.height
                    width: parent.width/2
                    radius: 4
                    border.width: 1
                    border.color: "white"
                }
            }

            Row {
                height: 30
                width: parent.width

                Label {
                    id: lbRegRoleText
                    text: "ПРОФИЛЬ"
                    textSize: 16
                    height: parent.height
                    width: parent.width/2
                }

                Label {
                    id: lbRegRole
                    text: registrationEsu.isRegistered?(registrationEsu.userRole):("НЕ ЗАРЕГИСТРИРОВАН");
                    textSize: 16
                    height: parent.height
                    width: parent.width/2
                    radius: 4
                    border.width: 1
                    border.color: "white"
                }
            }

            Row {
                height: 30
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
                    text: registrationEsu.isRegistered?(registrationEsu.userDuty):("");
                    textSize: 16
                    height: parent.height
                    width: parent.width/2
                    radius: 4
                    border.width: 1
                    border.color: "white"
                }
            }

            Row {
                height: 40
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
                    text: manager.specialityField;
                    readOnly: !root.editMode
                    textSize: 16
                    radius: 4
                    borderWidth: 1
                    height: parent.height
                    width: parent.width/2
                    placeholderText: "Военно-учётная специальность";
                    onEditingFinished: {
                        if( manager.specialityField !== fieldSpeciality.text ) {
                            manager.specialityField = fieldSpeciality.text;
                            manager.hasChanges = true;
                        }
                    }
                }
            }

            HorizontalLine { anchors.left: undefined; anchors.right: undefined; width: parent.width; }

            ComboBox {
                id: comboRankList
                textSize: 16
                width: parent.width
                borderWidth: 1
                description: "ЗВАНИЕ"
                model: regRanksModel
                currentIndex: manager.rankIndex;
                enableButton: (registrationEsu.isRegistered && root.editMode);
                onSelected: {
                    if( index !== manager.rankIndex ) {
                        manager.hasChanges = true;
                        manager.rankIndex = index;
                    }
                }
            }

            Row {
                height: 40
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
                    text: manager.nameField;
                    readOnly: !root.editMode
                    focusChain: true
                    textSize: 16
                    radius: 4
                    borderWidth: 1
                    height: parent.height
                    width: parent.width/2
                    placeholderText: "ФИО";
                    onEditingFinished: {
                        if( manager.nameField !== fieldName.text ) {
                            manager.hasChanges = true;
                            manager.nameField = fieldName.text;
                        }
                    }
                }
            }

            Row {
                height: 40
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
                    text: manager.birthdayField;
                    readOnly: !root.editMode
                    focusChain: true
                    textSize: 16
                    radius: 4
                    borderWidth: 1
                    height: parent.height
                    width: parent.width/2
                    placeholderText: "Дата рождения";
                    onEditingFinished: {
                        if( manager.birthdayField !== fieldBirthday.text ) {
                            manager.birthdayField = fieldBirthday.text;
                            manager.hasChanges = true;
                        }
                    }
                }
            }

            Row {
                height: 40
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
                    text: manager.addressField
                    readOnly: !root.editMode
                    textSize: 16
                    radius: 4
                    borderWidth: 1
                    height: parent.height
                    width: parent.width/2
                    placeholderText: "Адрес (место жительства)"
                    onEditingFinished: {
                        if( manager.addressField !== fieldAddress.text ) {
                            manager.addressField = fieldAddress.text;
                            manager.hasChanges = true;
                        }
                    }
                }
            }

            HorizontalLine { anchors.left: undefined; anchors.right: undefined; width: parent.width; }

            Row {
                height: 40
                width: parent.width
                spacing: 10

                Button {
                    id: btnEdit
                    radius: 4
                    label: checked?("ОТМЕНА"):("РЕДАКТИРОВАТЬ");
                    width: parent.width/3 - 20/3
                    height: parent.height
                    checkable: true
                    borderWidth: 1
                    enableButton: registrationEsu.isRegistered
                    onToggled: {
                        if( checked ) {
                            manager.initFields();
                            root.editMode = true;
                        } else {
                            manager.clear();
                            root.editMode = false;
                        }
                    }
                }

                Button {
                    id: btnSaveChanges
                    label: "СОХРАНИТЬ ИЗМЕНЕНИЯ"
                    width: parent.width/3 - 20/3
                    height: parent.height
                    radius: 4
                    borderWidth: 1
                    enableButton: (root.editMode && manager.hasChanges);
                    onClicked: {
                        btnEdit.checked = false;
                    }
                }

                Button {
                    id: btnCancelChanges
                    label: "ОТМЕНИТЬ ИЗМЕНЕНИЯ"
                    width: parent.width/3 - 20/3
                    height: parent.height
                    radius: 4
                    enableButton: (root.editMode && manager.hasChanges);
                    borderWidth: 1
                    onClicked: {
                        manager.clear();
                    }
                }
            }

            HorizontalLine { anchors.left: undefined; anchors.right: undefined; width: parent.width; }
        }
    }
}
