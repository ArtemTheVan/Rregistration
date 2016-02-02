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

    onReady: { registrationEsu.currentUiForm = root; console.log("currentUiForm"); }

    function switchWorkPanel() {
        actionsPanel.switchState();
    }

    Rectangle {
        id: actionsPanel
        property int visiblePos: parent.width - width

        x: visiblePos
        color: "#f0909090"
        width: 95
        anchors {
            top: parent.top
            bottom: parent.bottom
        }

        property int buttonHeight: 60
        property int hiddenPos: parent.width

        state: "visible"

        function switchState() {
            if( actionsPanel.state === "visible" )
                actionsPanel.state = "hidden";
            else actionsPanel.state = "visible";
        }

        states: [
            State {
                name: "visible"
                PropertyChanges { target: actionsPanel; x: actionsPanel.visiblePos; visible: true }
            },
            State {
                name: "hidden"
                PropertyChanges { target: actionsPanel; x: actionsPanel.hiddenPos; visible: false }
            }
        ]

        transitions: [
            Transition {
                to: "visible"
                SequentialAnimation {
                    PropertyAction { target: actionsPanel; property: "visible"; value: true; }
                    NumberAnimation { target: actionsPanel; property: "x"; duration: 300; easing.type: Easing.InCubic; }
                }
            },
            Transition {
                to: "hidden"
                SequentialAnimation {
                    NumberAnimation { target: actionsPanel; property: "x"; duration: 300; easing.type: Easing.InCubic; }
                    PropertyAction { target: actionsPanel; property: "visible"; value: false; }
                }
            }
        ]

        Column {
            anchors.fill: parent
            anchors.leftMargin: 0
            spacing: 0

            Button {
                width: parent.width
                height: actionsPanel.buttonHeight
                animateBackground: true
                iconSteady: "qrc:/ui_components/ui_components/add_person.png"
                iconHeight: height*3/4
                iconWidth: iconHeight
                iconAlign: Qt.AlignCenter
                iconText: "Принять"
                iconTextSize: 10
                enableButton: (mainView.count > 0) && mainView.currentIndex >= 0;
                onClicked: {
                    // TODO: Bug HERE!!!
                    var addr = regQueriesModel.getAddress(mainView.currentIndex);
                    if( addr === "" ) return;                    
                    regQueriesModel.remove(mainView.currentIndex);
                    registrationEsu.acceptRegistration(addr);
                }

                HorizontalLine { anchors.bottom: parent.bottom; padding: 3; }
            }

            Button {
                width: parent.width
                height: actionsPanel.buttonHeight
                animateBackground: true
                iconSteady: "qrc:/ui_components/ui_components/add_persons.png"
                iconHeight: height*3/4
                iconWidth: iconHeight
                iconText: "Принять все запросы"
                iconAlign: Qt.AlignTop
                textSize: 11
                enableButton: (mainView.count > 0);
                onClicked: {
                    regQueriesModel.clearData();
                    registrationEsu.acceptRegistrationAll();
                }
                HorizontalLine { anchors.bottom: parent.bottom; padding: 3; }
            }

            Button {
                width: parent.width
                height: actionsPanel.buttonHeight
                animateBackground: true
                iconSteady: "qrc:/ui_components/ui_components/cross.png"
                iconHeight: height/2
                iconWidth: iconHeight
                iconText: "Отклонить запрос"
                iconAlign: Qt.AlignTop
                textSize: 11
                enableButton: (mainView.count > 0) && mainView.currentIndex >= 0;
                onClicked: {
                    var addr = regQueriesModel.getAddress(mainView.currentIndex);
                    if( addr === "" ) return;

                    switch(registrationEsu.state)
                    {
                    case EsuRegistration.ChangeRegistrationState:

                        break;
                    case EsuRegistration.RegisterUserState:
                        regQueriesModel.remove(mainView.currentIndex);
                        registrationEsu.rejectRegistration(addr);
                        break;
                    default: break;
                    }
                }
                HorizontalLine { anchors.bottom: parent.bottom; padding: 3; }
            }
        }

        VerticalLine {
            lineColor: "#ffffff"
            size: 1
            anchors.left: parent.left
        }
    }

    Item {
        id: viewContent
        clip: true
        anchors {
            top: parent.top
            left: parent.left
            right: actionsPanel.left
            bottom: parent.bottom
        }

        Rectangle {
            id: headerItem
            height: 40
            clip: true
            color: "transparent"
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            property int widthOff: (requestCaption.width + timeCaption.width + fioCaption.width);

            Row {
                id: captionRow
                anchors.fill: parent
                spacing: 0

                Label {
                    id: requestCaption
                    width: 120
                    height: parent.height
                    text: "Запрос"
                    textSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    VerticalLine { anchors.right: parent.right; padding: 3; }
                }

                Label {
                    id: timeCaption
                    width: 90
                    height: parent.height
                    text: "Дата/Время"
                    textSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    VerticalLine { anchors.right: parent.right; padding: 3; }
                }

                Label {
                    id: fioCaption
                    width: 170
                    height: parent.height
                    text: "Имя/IP-адрес"
                    textSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    VerticalLine { anchors.right: parent.right; padding: 3; }
                }

                Label {
                    id: roleCaption
                    width: Math.abs(captionRow.width - (fioCaption.x + fioCaption.width));
                    height: parent.height
                    text: "Роль"
                    textSize: 16
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            HorizontalLine { anchors.bottom: parent.bottom }
        }

        ListView {
            id: mainView
            anchors {
                top: headerItem.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            clip: true
            flickableDirection: Flickable.VerticalFlick
            model: regQueriesModel            

            delegate: Rectangle {
                id: querieDelegate
                color: ListView.isCurrentItem?("#983a92cb"):("transparent");
                width:  parent.width
                height: 60

                property int itemIndex: index;
                property int request: requestRole
                property string nameAndAddressRole: (nameRole + "\n" + addressRole);

                Row {
                    anchors.fill: parent

                    Label {
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        text: requestStringRole //requestRole
                        width: requestCaption.width;
                    }
                    Label {
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        text: timeRole
                        width: timeCaption.width;
                    }
                    Label {
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.WordWrap
                        text: querieDelegate.nameAndAddressRole;
                        width: fioCaption.width;
                    }
                    Label {
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        text: baseRole
                        width: roleCaption.width;
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        mainView.currentIndex = querieDelegate.itemIndex;
                    }
                }

                HorizontalLine { anchors.bottom: parent.bottom }
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

