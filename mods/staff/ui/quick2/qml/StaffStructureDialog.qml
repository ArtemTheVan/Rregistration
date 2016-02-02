import QtQuick 1.1
import "qrc:/ui_components/qml/switch_view/"
import "qrc:/mods/military_staff/ui/qml/components/"


Rectangle {
    id: main    
    color: "#b8a3a3a3"
    // [signals]
    signal close();

    // [HEADER]
    Rectangle {
        id: header
        color: "#98a3a3a3"
        height: 55
        clip: true
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        // [left]
        Row {
            id: hrLeft
            anchors {
                top: parent.top
                topMargin: 2
                bottom: hrLine1.top
                bottomMargin: 2
                left: parent.left
                leftMargin: 5
            }
            spacing: 5

            IconButton {
                id: closeButton
                height: hrLeft.height
                width: height
                image: "/ui_components/images/cross.png"
                onClicked: main.close();
            }

            Text {
                id: hrText
                height: hrLeft.height
                color: "white"
                text: qsTr("ШТАТНЫЙ СОСТАВ - ИЕРАРХИЯ")
                font.family: "Arial"
                font.pixelSize: 20
                font.bold: true
                clip: true
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
        }
        // [/left]
        // [right]
        Row {
            id: hrRight
            anchors {
                top: parent.top
                topMargin: 2
                bottom: hrLine1.top
                bottomMargin: 2
                right: parent.right
                rightMargin: 5
            }
            spacing: 5

            IconButton {
                id: downButton
                height: hrLeft.height
                width: height
                image: "/mods/military_staff/ui/images/arrow_down.png"
                onClicked: structurePage.children[0].incrementCurrentIndex();
            }
            IconButton {
                id: upButton
                height: hrLeft.height
                width: height
                image: "/mods/military_staff/ui/images/arrow_up.png"
                onClicked: structurePage.children[0].decrementCurrentIndex();
            }
        }
        // [/right]
        Line {
            id: hrLine1
            lineColor: "#a8f0f0f0"
            lineWidth: 2
        }
    }
    // [/HEADER]
    // [PATH PANEL]
    Rectangle {
        id: pathPanel
        color: "#98a3a3a3"
        height: 47
        clip: true
        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
        }

        ListView {
            id: pathModelView;
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                bottom: pathPanelLine1.top
                topMargin: 2
                bottomMargin: 3
                leftMargin: 5
            }
            clip: true
            orientation: ListView.Horizontal
            boundsBehavior: Flickable.StopAtBounds
            flickableDirection: Flickable.HorizontalFlick

            model: staffStructPathModel
            delegate: PathPanelDelegate {
                id: pathDelegate
                height: pathModelView.height
                text: nameRole;
                nameFull: nameFullRole;
                itemIndex: indexRole;
                color: "#a8787878"
                arrow: arrowRole
                onClicked: {
                    var cnt = staffStructPathModel.count;
                    var next = itemIndex+1;
                    if( cnt === 1 || next === cnt ) return;
                    staffStructModel.setParentModel(text);
                    staffStructPathModel.removeFrom(next);
                }
                Component.onCompleted: {
                    if( arrow === false ) {
                        pathDelegate.color = "#a8585858";
                    } else pathDelegate.color = "#a8787878";
                }
            }
        }

        Line {
            id: pathPanelLine1
            lineColor: "#a8f0f0f0"
            lineWidth: 2
        }
    }
    // [/PATH PANEL]
    // [BODY]
    Rectangle {
        id: body
        color: "#98a3a3a3"
        anchors {
            top: pathPanel.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        // [PAGES]
        SwitchView {
            id: bodyPages
            anchors.fill: parent
            StaffStructurePage {
                id: structurePage
            }
            SwitchViewItem {
                id: editPage
            }
        }
        // [/PAGES]
    }
    // [/BODY]
}
