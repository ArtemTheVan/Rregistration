import QtQuick 1.1

Rectangle {
    id: line
    property alias lineWidth: line.height
    property color lineColor: "white"
    color: line.lineColor
    anchors {
        left: parent.left
        right: parent.right
        bottom: parent.bottom
    }
}
