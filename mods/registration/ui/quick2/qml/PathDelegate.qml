import QtQuick 2.2


Item {
    id: root
    height: 38
    width: 120

    signal clicked();
    property bool arrow: false
    property color color: "#a8787878"
    property alias text: frameText.text
    property int itemIndex: 0

    Rectangle {
        id: frame
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        clip: true
        width: root.width - imgArrow.width;
        color: root.color;
        border.width: 1
        border.color: "#98eaeaea"
        radius: 6

        Text {
            id: frameText
            anchors.fill: parent
            color: "white"
            text: qsTr("undefined")
            font.family: "Arial"
            font.pixelSize: 15
            font.bold: true
            clip: true
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            id: pathDelegateArea
            anchors.fill: parent
            onClicked: root.clicked();
        }

        states: State {
            when: pathDelegateArea.pressed
            name: "pressed"
            PropertyChanges { target: frame; color: "#a83a92cb"; }
            PropertyChanges { target: frameText; scale: 1.1; }
        }

        Behavior on scale { NumberAnimation { target: frame; duration: 400 } }
    }

    Image {
        id: imgArrow
        anchors {
            verticalCenter: frame.verticalCenter
            left: frame.right
        }
        source: "qrc:/ui_components/ui_components/arrows/arrow_right.png"
        width: 25
        height: 22
        smooth: true
        visible: root.arrow
        opacity: (root.arrow) ? (1.0): (0.0);
    }
}

