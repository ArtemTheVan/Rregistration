import QtQuick 2.2

import MKBKompas.CSR.ESUTZ.Registration 1.0
import MKBKompas.CSR.ESUTZ.Ui 1.0

import "qrc:/ui_components/qml"
import ".."


Item {
    id: root
    anchors.fill: parent

    signal close();
    signal ready();

    onReady: { registrationEsu.currentUiForm = root; }

    Rectangle {
        id: headerItem
        height: 40
        clip: true
        color: "transparent"
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        Row {
            id: captionRow
            anchors.fill: parent
            spacing: 0

            Label {
                id: numberCaption
                width: 30
                height: parent.height
                text: "№"
                textSize: 16
                horizontalAlignment: Text.AlignHCenter
                VerticalLine { anchors.right: parent.right; padding: 3; }
            }

            Label {
                id: timeCaption
                width: 90;
                height: parent.height
                text: "Дата/Время"
                textSize: 16
                horizontalAlignment: Text.AlignHCenter
                VerticalLine { anchors.right: parent.right; padding: 3; }
            }

            Label {
                id: nameCaption
                width: 160
                height: parent.height
                text: "ФИО"
                textSize: 16
                horizontalAlignment: Text.AlignHCenter
                VerticalLine { anchors.right: parent.right; padding: 3; }
            }

            Label {
                id: roleCaption
                width: 180;
                height: parent.height
                text: "Профиль"
                textSize: 16
                horizontalAlignment: Text.AlignHCenter
                VerticalLine { anchors.right: parent.right; padding: 3; }
            }

            Label {
                id: addressCaption
                width:  Math.abs(captionRow.width - (roleCaption.x + roleCaption.width)); //160;
                height: parent.height
                text: "IP адрес"
                textSize: 16
                horizontalAlignment: Text.AlignHCenter
                VerticalLine { anchors.right: parent.right; padding: 3; }
            }
        }

        HorizontalLine { anchors.bottom: parent.bottom }
    }

    Item {
        id: tableContent
        anchors {
            top: headerItem.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        ListView {
            id: mainView
            anchors.fill: parent
            clip: true
            flickableDirection: Flickable.VerticalFlick
            model: registrationTableModel
            delegate: Rectangle {
                id: querieDelegate
                color: ListView.isCurrentItem?("#983a92cb"):("transparent");
                width:  parent.width
                height: 50
                property int index: indexRole
                property real itemWidth: 1/6*width;
                property real itemOffset: 50

                Row {
                    anchors.fill: parent

                    Label {
                        id: indexCaption
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        text: (indexRole + 1)
                        width: numberCaption.width;
                    }
                    Label {
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        text: timeRole
                        width: timeCaption.width;
                    }
                    Label {                        
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        text: nameRole
                        width: nameCaption.width;
                    }
                    Label {
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        text: profileRole
                        width: roleCaption.width
                    }
                    Label {
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        text: addressRole
                        width: addressCaption.width
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: mainView.currentIndex = querieDelegate.index;
                }

                HorizontalLine { anchors.bottom: parent.bottom }
            }

            states: [
                State {
                    name: "scrolling"
                    when: mainView.movingVertically
                    PropertyChanges { target: scrollBar; opacity: 1.0 }
                }
            ]

            transitions: [
                Transition { NumberAnimation { target: scrollBar; properties: "opacity"; duration: 300; } }
            ]
        }

        ScrollBar {
            id: scrollBar
            opacity: 0.0
            anchors {
                top: parent.top
                right: parent.right
                bottom: parent.bottom
            }
            backgroundColor: "#b8909090"
            scrollerColor: "#b8fafafa"
            width: 8
            position: mainView.visibleArea.yPosition
            pageSize: mainView.visibleArea.heightRatio
        }
    }
}
