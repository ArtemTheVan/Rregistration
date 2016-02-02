import QtQuick 1.1

import "qrc:/ui_components/qml/"
import "qrc:/ui_components/qml/switch_view/"
import "qrc:/mods/military_staff/ui/qml/components/"


Rectangle {
    id: root
    width: 75
    color: "transparent" //"#a8787878"

    signal itemChoosed();

    Rectangle {
        id: line
        width: 1
        color: "#98eaeaea"
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
    }

    Column {
        id: toolbarColumn
        spacing: 6
        width: parent.width - 16;
        height: parent.height - 4;
        anchors {
            top: parent.top
            topMargin: 3
            horizontalCenter: parent.horizontalCenter
        }

        ImageButton {
            id: chooseItemButton
            source: "/mods/military_staff/ui/images/checkmark.png"
            width: parent.width
            height: width
            onClicked: root.itemChoosed();
        }

        ImageButton {
            id: infoButton
            source: "/mods/military_staff/ui/images/info.png"
            width: parent.width
            height: width
            enabled: false;
        }

        ImageButton {
            id: editButton
            source: "/mods/military_staff/ui/images/pencil.png"
            width: parent.width
            height: width
            enabled: false;
        }

        ImageButton {
            id: deleteButton
            source: "/mods/military_staff/ui/images/delete_icon.png"
            width: parent.width
            height: width
            enabled: false;
        }
    }
}
