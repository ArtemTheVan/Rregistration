import QtQuick 2.2

import "qrc:/ui_components/qml"


Rectangle {
    id: root
    height: 30
    clip: true
    color: "transparent"
    anchors {
        bottom: parent.bottom
        left: parent.left
        right: parent.right
    }

    signal previousPage();

    Rectangle {
        id: headerLine
        height: 1
        color: "#a8f0f0f0"
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }
    }

    Item {
        id: content        
        anchors {
            top: headerLine.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            leftMargin: 10
        }

        Button {
            id: backButton
            radius: 6
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
            width: 110
            height: parent.height
            label: "НАЗАД"
            textSize: 16
            animateBackground: true
            onClicked: root.previousPage();

            Image {
                id: arrowLeftImg
                anchors {
                    left: parent.left
                    leftMargin: 4
                    verticalCenter: parent.verticalCenter
                }
                source: "qrc:/ui_components/ui_components/marks/arrow_left_60.png"
                width: 15
                height: parent.height*3/4
                smooth: true
            }

            Image {
                id: lineImg
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    right: parent.right
                }
                source: "qrc:/ui_components/ui_components/VerticalDecorationLine.png"
                width: 15
                smooth: true
            }
        }
    }
}

