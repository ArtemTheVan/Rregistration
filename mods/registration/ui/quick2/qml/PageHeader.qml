import QtQuick 2.2

import "qrc:/ui_components/qml"


Item {
    id: root
    height: 40
    clip: true
    anchors {
        top: parent.top
        left: parent.left
        right: parent.right
    }

    property alias caption: headerText.text
    property string secondCaption
    property int textSize: 18
    property int leftMargin: 6

    signal close();

    Item {
        id: headerContent
        anchors {
            top: parent.top
            bottom: headerLine.top
            left: parent.left
            right: parent.right
            topMargin: 2
            bottomMargin: 2
            rightMargin: 2
            leftMargin: root.leftMargin
        }

        Row {
            id: headerRow
            spacing: 2
            anchors {
                top: parent.top
                bottom: parent.bottom
                left: parent.left
                right: buttonContent.left
            }

            Text {
                id: headerText
                height: parent.height
                width: headerText.paintedWidth
                color: "white"
                text: ""
                font.family: "Arial"
                font.pixelSize: textSize
                font.bold: true
                clip: true
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }

            Image {
                id: splitterIcon
                height: parent.height*2/3
                width: visible?(height):(0);
                anchors.verticalCenter: parent.verticalCenter
                source: "qrc:/ui_components/ui_components/arrows/arrow_next.png"
                antialiasing: true
                visible: root.secondCaption !== "";
            }

            Text {
                id: secondHeaderText
                height: parent.height
                width: visible?(secondHeaderText.paintedWidth):(0);
                color: "white"
                text: root.secondCaption
                font.family: "Arial"
                font.pixelSize: textSize
                font.bold: true
                clip: true
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                visible: root.secondCaption !== "";
            }
        }

        Item {
            id: buttonContent
            width: 110
            anchors {
                top: parent.top
                right: parent.right
                bottom: parent.bottom
            }

            Image {
                id: lineImg
                anchors {
                    top: parent.top
                    left: parent.left
                    bottom: parent.bottom
                }
                source: "qrc:/ui_components/ui_components/VerticalDecorationLine.png"
                width: 12
                smooth: true
            }

            Button {
                id: closeButton
                anchors {
                    top: parent.top
                    right: parent.right
                    bottom: parent.bottom
                }
                width: 90
                animateBackground: true
                label: qsTr("Закрыть")
                onClicked: root.close();
                radius: 5
            }
        }
    }

    Rectangle {
        id: headerLine
        height: 2
        color: "#f5f5f5"
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }
}
