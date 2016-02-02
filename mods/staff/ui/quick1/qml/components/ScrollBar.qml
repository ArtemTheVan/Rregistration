import QtQuick 1.1

Item {
    id: scrollBar

    property int orientation: Qt.Vertical;
    property real position: 0.0;
    property real pageSize: 0.0;

    property color backgroundColor: "#b8fafafa"
    property color scrollerColor: "#b8686868"

    Rectangle {
        id: background
        anchors.fill: parent
        color: scrollBar.backgroundColor;
    }

    Rectangle {
        id: scroller
        x: (orientation === Qt.Vertical)?(1):(position*(scrollBar.width-2)+1);
        y: (orientation === Qt.Vertical)?(position*(scrollBar.height-2)+1):(1);
        width: (orientation === Qt.Vertical)?(scrollBar.width-2):(pageSize*(scrollBar.width-2));
        height: (orientation === Qt.Vertical)?(pageSize*(scrollBar.height-2)):(scrollBar.height-2)
        radius: (orientation === Qt.Vertical)?(width/2 - 1):(height/2 - 1);
        color: scrollerColor
    }
}

