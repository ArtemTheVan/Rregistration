import QtQuick 1.1

Rectangle {
    id: root
    color: "transparent"
    radius: 6
    clip: true
    visible: root.activate
    enabled: true

    signal clicked();
    property color pressedColor: "#c8686868"
    property alias source: img.source
    property bool activate: true
    property bool clickable: true

    onEnabledChanged: {
        if( enabled === false ) {
            root.clickable = false;
        } else {
            root.clickable = true;
        }
    }

    states: [
        State {
            name: "pressed"
            when: activeArea.pressed
            PropertyChanges { target: root; color: pressedColor; }
            PropertyChanges { target: img;  scale: 1.15; }
        }
    ]

    transitions: [
        Transition {
            ColorAnimation { duration: 50 }
            PropertyAnimation { property: "scale"; duration: 150  }
        }
    ]

    Image {
        id: img
        smooth: true
        anchors.fill: parent
        anchors.margins: 3
    }

    Rectangle {
        id: fader
        anchors.fill: parent
        color: "#c8686868"
        radius: 6
        clip: true
        enabled: !root.enabled
        visible: !root.enabled
    }

    MouseArea {
        id: activeArea
        enabled: root.clickable
        anchors.fill: parent
        onClicked: root.clicked();
        hoverEnabled: root.enabled
    }
}
