import QtQuick 1.1

Rectangle {
    id: button
    width: 100
    height: 100
    color: "#a8787878"
    border.width: 1
    border.color: "#98f0f0f0"
    radius: 6

    property alias image: buttonImage.source;
    signal clicked()

    Image {
        id: buttonImage
        source: image
        anchors.fill: parent
        anchors.margins: 8
        smooth: true
        scale: activeArea.pressed ? (1.25) : (1.0);
        Behavior on scale { PropertyAnimation { duration: 150 } }
    }

    MouseArea {
        id: activeArea
        anchors.fill: parent
        onClicked: button.clicked();
    }
}
