import QtQuick 2.2

import MKBKompas.CSR.ESUTZ.Ui 1.0
import MKBKompas.CSR.ESUTZ.Staff 1.0
import MKBKompas.CSR.ESUTZ.Registration 1.0


import "qrc:/ui_components/qml"


Rectangle {
    id: root
    width: parent.width
    height: 90; //(registered)?(86):(72);
    color: ListView.isCurrentItem ? (itemChoosedColor):(itemSteadyColor);
    //color: activeArea.pressed ? ("#983a92cb"):("transparent");

    property color itemSteadyColor: (!registered)?("transparent"):("#98519726");
    property color itemChoosedColor: "#983a92cb"

    property bool checkable: true;
    property bool checked: false;
    property bool registered: false;

    property string iconSource: ""

    property string roleFullName: ""
    property string role: ""
    property string name: ""
    property string netAddress: ""
    property string registrationInfo: ""

    property bool profileRegistered: false
    property string descrFirst: ""

    property int itemIndex: 0
    property int type: EsuStaff.UnitType
    property int textSize: 15

    signal changeModel();
    signal updateModel();
    signal itemChoosed();

    Item {
        id: layout
        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
            bottom: lineBottom.top
            topMargin: 4
            bottomMargin: 4
            leftMargin: 10
            rightMargin: 10
        }

        MouseArea {
            id: activeArea
            anchors.fill: parent
            onPressed: activeAreaTimer.start();
            onExited: activeAreaTimer.stop();
            onCanceled: activeAreaTimer.stop();
            onClicked: {
                root.ListView.view.currentIndex = index;
            }

            Timer {
                id: activeAreaTimer
                interval: 1000
                repeat: false
                onTriggered: {
                    registrationStaffModel.checkable = !(registrationStaffModel.checkable);
                }
            }
        }

        // [icon-checkbox]
        Row {
            id: iconCheckboxLay
            height: 68
            width: 110
            anchors {
                //top: parent.top
                //bottom: parent.bottom
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
            spacing: 5
            state: "default"

            states: [
                State {
                    when: root.checkable === true;
                    name: "checkable"
                    PropertyChanges { target: checkboxLoader; scale: 1.0; visible: true; }
                },
                State {
                    when: root.checkable === false
                    name: "default"
                    PropertyChanges { target: checkboxLoader; scale: 0.01; visible: false; }
                },
                State {
                    when: root.registered === true
                    name: "registered"
                    PropertyChanges { target: registeredIconLoader; scale: 1.0; visible: true; }
                }
            ]

            transitions: [
                Transition {
                    from: "default"
                    to: "checkable"
                    SequentialAnimation {
                        PropertyAction { target: checkboxLoader; property: "visible"; value: true }
                        NumberAnimation { target: checkboxLoader; property: "scale"; duration: 400; }
                    }
                },
                Transition {
                    from: "checkable"
                    to: "default"
                    SequentialAnimation {
                        NumberAnimation { target: checkboxLoader; property: "scale"; duration: 400; }
                        PropertyAction { target: checkboxLoader; property: "visible"; value: false; }
                    }
                }
            ]

            // [checkbox]
            Loader {
                id: checkboxLoader
                active: root.checkable
                width: 47
                height: width
                visible: root.checkable
                asynchronous: true
                sourceComponent: Item {
                    id: checkboxContainer
                    width: 47
                    height: parent.height
                    visible: root.checkable

                    Rectangle {
                        id: checkbox
                        width: 47
                        height: width
                        anchors.centerIn: parent
                        color: "#a8787878"
                        border.width: 1
                        border.color: "#98f0f0f0"
                        radius: 6

                        Image {
                            id: checkboxMarkImg
                            source: "qrc:/ui_components/ui_components/confirm.png"
                            anchors.centerIn: parent
                            width: parent.width-6
                            height: parent.height-6
                            fillMode: Image.PreserveAspectCrop
                            smooth: true
                            opacity: (root.checkable && root.checked) ? (1.0): (0.0);
                        }
                    }

                    MouseArea {
                        id: checkboxArea
                        anchors.fill: parent
                        enabled: root.checkable
                        onClicked: root.checked = !root.checked;
                    }
                }
            }
            // [/checkbox]

            // [itemicon]
            Rectangle {
                id: iconFrame
                color: "#a8787878"
                border.width: 1
                border.color: "#98f0f0f0"
                radius: 6
                height: parent.height-6;
                width: height+2
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: itemIconContainer
                    source: Qt.resolvedUrl(root.iconSource);
                    anchors.fill: parent
                    antialiasing: true
                    smooth: true
                }
            }
            // [/itemicon]

            // [Registered Icon]
            Loader {
                id: registeredIconLoader
                active: root.registered
                anchors.verticalCenter: parent.verticalCenter
                width: 47
                height: width
                visible: root.registered
                asynchronous: true
                sourceComponent: Button {
                    id: registeredStateButton
                    width: parent.width
                    height: parent.height
                    iconSteady: "qrc:/ui/staff/staff/start_120_2.png"
                    iconHeight: height*3/4
                    iconWidth: iconHeight
                    iconAlign: Qt.AlignCenter
                    radius: 3
                    borderWidth: 0
                    antialiasing: true
                }
            }
            // [/Registered Icon]
        }
        // [/icon-checkbox]

        Rectangle {
            id: buttonsSection
            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right
                rightMargin: 5
            }
            width: 130
            color: "transparent"
            //color: "#a8787878"
            //border.width: 1
            //border.color: "#98f0f0f0"
            radius: 6

            Button {
                id: ctrlButton
                radius: 6
                anchors.centerIn: parent
                anchors.margins: 5
                animated: true
                animateBackground: true
                height: parent.height - 10
                width: parent.width - 10
                textSize: 15
                borderWidth: 1
                label: (root.type === EsuStaff.DivisionType)?("РАСКРЫТЬ"):("ВЫБРАТЬ");
                visible: !root.registered;
                onClicked: {
                    mainView.clickedItem = root;
                    if( root.type === EsuStaff.DivisionType ) {                        
                        mainView.currentIndex = root.itemIndex;
                        registrationStaffModel.toChildDivision(role);
                    } else {
                        root.itemChoosed();
                    }
                }
            }
        }

        // [ Descriptopn ]
        Rectangle {
            id: descrContainer
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: iconCheckboxLay.right
                right: buttonsSection.left
                leftMargin: 5
                rightMargin: 5
            }
            width: parent.width/3+150
            color: "#a8787878"
            border.width: 1
            border.color: "#98f0f0f0"
            radius: 6

            Column {
                id: descrColumn
                spacing: 4
                anchors {
                    fill: parent
                    leftMargin: 4
                    topMargin: 4
                    bottomMargin: 2
                    rightMargin: 4
                }
                property int paramsCount: root.registered?(4):(3);
                property int textHeight: descrContainer.height/4 - 5;

                Text {
                    id: registrationStateText
                    height: descrColumn.textHeight
                    width: parent.width
                    color: "white"
                    text: root.registered?("Зарегистрирован: " + root.registrationInfo):("Не занят");
                    font.family: "Arial"
                    font.pixelSize: root.textSize
                    font.bold: true
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                }
                Text {
                    id: addressText
                    enabled: root.netAddress !== "" && root.netAddress !== undefined;
                    visible: enabled
                    height: (enabled)?(descrColumn.textHeight):(0);
                    width: parent.width
                    color: "white"
                    text: root.netAddress
                    font.family: "Arial"
                    font.pixelSize: root.textSize
                    font.bold: true
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                }
                Text {
                    id: roleFullNameText
                    height: descrColumn.textHeight
                    width: parent.width
                    color: "white"
                    text: root.roleFullName
                    font.family: "Arial"
                    font.pixelSize: root.textSize
                    font.bold: true                    
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                }
                Text {
                    id: descrFirstText
                    height: descrColumn.textHeight*2
                    width: parent.width
                    color: "white"
                    text: root.descrFirst
                    font.family: "Arial"
                    font.pixelSize: root.textSize
                    font.bold: true
                    //clip: true
                    wrapMode: Text.WordWrap
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                }
            }
        }
        // [/description]
        // [toolbar]
        Rectangle {
            id: toolbar
            anchors {
                top: parent.top
                bottom: parent.bottom
                right: descrContainer.left
                rightMargin: 20
            }
            width: 190
            color: "#a8787878"
            border.width: 1
            border.color: "#98f0f0f0"
            radius: 6
            visible: false

            Row {
                id: toolbarRow
                spacing: 3
                anchors {
                    centerIn: parent
                }
                width: parent.width - 8;
                height: parent.height - 4;

                Button {
                    id: infoButton
                    iconSteady: "qrc:/ui_components/ui_components/info.png"
                    height: parent.height
                    width: height
                }
                Button {
                    id: editButton
                    iconSteady: "qrc:/ui_components/ui_components/pencil.png"
                    height: parent.height
                    width: height
                }
                Button {
                    id: deleteButton
                    iconSteady: "qrc:/ui_components/ui_components/delete_icon.png"
                    height: parent.height
                    width: height
                }
            }
        }
        // [/toolbar]
    }

    Rectangle {
        id: lineBottom
        height: 2
        color: "#a8f0f0f0"
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }
}
