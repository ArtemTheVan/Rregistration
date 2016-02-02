import QtQuick 1.1
import MKBKompas.CSR.ESUTZ.Staff 1.0


Rectangle {
    id: root
    width: parent.width
    height: 72
    color: ListView.isCurrentItem ? ("#983a92cb"):("transparent");
    //color: activeArea.pressed ? ("#983a92cb"):("transparent");

    property bool checkable: false;
    property bool checked: false;
    property string iconSource: ""
    property string nameFull: ""
    property string name: ""
    property string descrFirst: ""
    property int type: 0
    property int textSize: 16

    signal changeModel();
    signal updateModel();
    signal itemChoosed();

    Item {
        id: layout
        anchors {
            top: parent.top
            right: parent.right
            left: parent.left
            bottom: line1.top
            topMargin: 4
            bottomMargin: 4
            leftMargin: 10
            rightMargin: 10
        }

        MouseArea {
            id: activeArea
            anchors.fill: parent
            onPressed: activeAreaTimer.restart();
            onExited: activeAreaTimer.stop();
            onCanceled: activeAreaTimer.stop();
            onClicked: {
                root.ListView.view.currentIndex = index;
            }
            onDoubleClicked: {
                if( type === ESUStaffUI.DivisionObject ) {
                    root.changeModel();
                } else {
                    root.itemChoosed();
                }
            }

            Timer {
                id: activeAreaTimer
                interval: 1200
                repeat: false
                onTriggered: {
                    root.checkable != root.checkable;
                    root.updateModel();
                }
            }
        }

        // [icon-checkbox]
        Row {
            id: iconCheckboxLay
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
            }
            spacing: 10
            state: "default"

            states: [
                State {
                    when: root.checkable === true;
                    name: "checkable"
                    PropertyChanges { target: checkboxContainer; scale: 1.0; }
                },
                State {
                    when: root.checkable === false
                    name: "default"
                    PropertyChanges { target: checkboxContainer; scale: 0.01; }
                }
            ]

            transitions: [
                Transition {
                    from: "default"
                    to: "checkable"
                    SequentialAnimation {
                        PropertyAction { target: checkboxContainer; property: "visible"; value: true }
                        NumberAnimation { target: checkboxContainer; property: "scale"; duration: 400; }
                    }
                },
                Transition {
                    from: "checkable"
                    to: "default"
                    SequentialAnimation {
                        NumberAnimation { target: checkboxContainer; property: "scale"; duration: 400; }
                        PropertyAction { target: checkboxContainer; property: "visible"; value: false; }
                    }
                }
            ]

            // [checkbox]
            Item {
                id: checkboxContainer
                width: 47
                height: parent.height
                visible: false

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
                        source: "/mods/military_staff/ui/images/checkmark.png"
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
            // [/checkbox]
            // [itemicon]
            ImageButton {
                id: itemIconContainer
                source: Qt.resolvedUrl(root.iconSource);
                height: parent.height
                width: height+2
                color: "#a8787878"
                border.width: 1
                border.color: "#98f0f0f0"
                radius: 6
                clickable: false
            }
            // [/itemicon]
            // [Expand division button]
            ImageButton {
                id: expandStaffButton
                source: Qt.resolvedUrl("qrc:/ui_components/images/icon_expand_v3.png");
                height: parent.height / 1.5
                width: height+2
                //color: "#a8787878"
                border.width: 1
                border.color: "#98f0f0f0"
                radius: 6
                activate: (root.type === ESUStaffUI.DivisionObject)
                onClicked: root.changeModel();
            }
            // [/Expand division button]
        }
        // [/icon-checkbox]
        // [description]
        Rectangle {
            id: descrContainer
            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right
            }
            width: parent.width/3+150
            color: "#a8787878"
            border.width: 1
            border.color: "#98f0f0f0"
            radius: 6

            Column {
                id: descrColumn
                spacing: 2
                anchors {
                    fill: parent
                    leftMargin: 4
                    topMargin: 4
                    bottomMargin: 2
                    rightMargin: 4
                }
                property int textHeight: parent.height/3 - descrColumn.spacing - 4;

                Text {
                    id: fullNameText
                    height: descrColumn.textHeight
                    width: parent.width
                    color: "white"
                    text: root.nameFull
                    font.family: "Arial"
                    font.pixelSize: root.textSize
                    font.bold: true
                    //clip: true
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

                ImageButton {
                    id: infoButton
                    source: "/mods/military_staff/ui/images/info.png"
                    height: parent.height
                    width: height
                }
                ImageButton {
                    id: editButton
                    source: "/mods/military_staff/ui/images/pencil.png"
                    height: parent.height
                    width: height
                }
                ImageButton {
                    id: deleteButton
                    source: "/mods/military_staff/ui/images/delete_icon.png"
                    height: parent.height
                    width: height
                }
            }
        }
        // [/toolbar]
    }

    Line {
        id: line1
        lineWidth: 2
        lineColor: "#a8f0f0f0"
    }
}
