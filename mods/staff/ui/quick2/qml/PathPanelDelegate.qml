import QtQuick 1.1

Item {
    id: pathDelegate
    height: 38
    width: 120

    signal clicked();
    property bool arrow: false
    property color color: "#a8787878"
    property alias text: pathDelegateText.text
    property string nameFull: ""
    property int itemIndex: 0

    Rectangle {
        id: pathDelegateFrame
        anchors {
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        clip: true
        width: pathDelegate.width - pathDelegateArrow.width;
        color: pathDelegate.color;
        border.width: 1
        border.color: "#98eaeaea"
        radius: 6

        Text {
            id: pathDelegateText
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
            onClicked: pathDelegate.clicked();
        }

        states: State {
            when: pathDelegateArea.pressed
            name: "pressed"
            PropertyChanges { target: pathDelegateFrame; color: "#a83a92cb"; }
            //PropertyChanges { target: pathDelegateText; color: "#fafafa"; }
            //PropertyChanges { target: pathDelegateText; scale: 1.1; }
        }
        /*
        transitions: Transition {
            to: "pressed"
            ParallelAnimation {
                ColorAnimation { targets: [pathDelegateFrame, pathDelegateText]; duration: 200 }
                NumberAnimation { target: pathDelegateText; property: "scale"; duration: 100 }
            }
        }*/
    }

    Image {
        id: pathDelegateArrow
        anchors {
            verticalCenter: pathDelegateFrame.verticalCenter
            left: pathDelegateFrame.right
        }
        source: "/mods/military_staff/ui/images/arrow_right_v1.png"
        width: 25
        height: 22
        smooth: true
        visible: pathDelegate.arrow
        opacity: (pathDelegate.arrow) ? (1.0): (0.0);
    }
}
