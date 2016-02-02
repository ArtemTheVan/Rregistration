import QtQuick 1.1

Rectangle {
    id: button
    clip: true
    width: 200
    height: 80
    color: (buttonArea.pressed ? ("#a83a92cb"):("#a8787878"));
    border.width: 1
    border.color: "#98eaeaea"
    radius: 6

    property string text: "button_text"

    signal clicked();

    Text {
        id: buttonText
        anchors.fill: parent
        color: "white"
        text: button.text
        font.family: "Arial"
        font.pixelSize: 15
        font.bold: true
        clip: true
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    MouseArea {
        id: buttonArea
        anchors.fill: parent
        onClicked: button.clicked();
    }
}
