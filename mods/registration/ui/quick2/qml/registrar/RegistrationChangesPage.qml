import QtQuick 2.2

import MKBKompas.CSR.ESUTZ.Registration 1.0
import MKBKompas.CSR.ESUTZ.Ui 1.0

import "qrc:/ui_components/qml"
import ".."

//# Эта страница для запросов изменения профиля и данных профиля

Rectangle {
    id: root
    anchors.fill: parent
    color: "transparent"
    border.width: 1
    border.color: "white"

    signal close();
    signal ready();

    onReady: { registrationEsu.currentUiForm = root; }

    ToolPanel {
        id: actionsPanel
        width: 95
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
    }

    Item {
        id: viewContent
        clip: true
        anchors {
            top: parent.top
            left: parent.left
            right: actionsPanel.left
            bottom: parent.bottom
        }

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
            property int widthOff: (requestCaption.width + timeCaption.width + fioCaption.width);

            Row {
                anchors.fill: parent
                spacing: 0

                Label {
                    id: requestCaption
                    width: 130
                    height: parent.height
                    text: "Запрос"
                    textSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    VerticalLine { anchors.right: parent.right; padding: 3; }
                }

                Label {
                    id: timeCaption
                    width: 80
                    height: parent.height
                    text: "Время запроса"
                    textSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    VerticalLine { anchors.right: parent.right; padding: 3; }
                }

                Label {
                    id: fioCaption
                    width: 170
                    height: parent.height
                    text: "ФИО"
                    textSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    VerticalLine { anchors.right: parent.right; padding: 3; }
                }

                Label {
                    id: roleCaption
                    width: (headerItem.width - headerItem.widthOff);
                    height: parent.height
                    text: "Роль"
                    textSize: 16
                    horizontalAlignment: Text.AlignHCenter
                }
            }

            HorizontalLine { anchors.bottom: parent.bottom }
        }

        ListView {
            id: mainView
            anchors {
                top: headerItem.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
            clip: true
            flickableDirection: Flickable.VerticalFlick
            model: regQueriesModel
            delegate: Rectangle {
                id: querieDelegate
                color: ListView.isCurrentItem?("#983a92cb"):("transparent");
                width:  parent.width
                height: 45
                property int index: numRole // TODO: FIXIT

                Row {
                    anchors.fill: parent

                    Label {
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        text: requestRole
                        width: requestCaption.width;
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
                        text: profileRole; //TODO: FIXIT
                        width: fioCaption.width;
                    }
                    Label {
                        height: parent.height
                        horizontalAlignment: Text.AlignHCenter
                        text: nameRole
                        width: roleCaption.width;
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
            width: 6
            position: mainView.visibleArea.yPosition
            pageSize: mainView.visibleArea.heightRatio
        }
    }
}

