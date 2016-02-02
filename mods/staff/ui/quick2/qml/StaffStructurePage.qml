import QtQuick 1.1
import MKBKompas.CSR.ESUTZ.Staff 1.0

import "qrc:/ui_components/qml/"
import "qrc:/ui_components/qml/switch_view/"
import "qrc:/mods/military_staff/ui/qml/components/"


Item {
    id: root

    property alias model: mainView.model
    property int itemIndex: mainView.currentIndex
    property int itemType: mainView.currentItem.type
    property string itemName: mainView.currentItem.name
    property string itemNameFull: mainView.currentItem.nameFull
    property bool checkMode: false

    function arrowUp() {
        if( mainView.currentIndex === 0) {
            mainView.currentIndex = mainView.count - 1;
        } else {
            mainView.decrementCurrentIndex();
        }
    }

    function arrowDown() {
        if( mainView.currentIndex === (mainView.count - 1)) {
            mainView.currentIndex = 0;
        } else {
            mainView.incrementCurrentIndex();
        }
    }

    ListView {
        id: mainView
        anchors.fill: parent
        anchors.rightMargin: 0
        clip: true
        //boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.VerticalFlick

        focus: true
        Keys.enabled: true
        Keys.onPressed: {
            if( event.key === Qt.Key_F1 )
                windowManager.showFocusObject();
        }

        model: staffStructModel
        delegate: StructItemDelegate {
            id: itemDeleagte
            iconSource: Qt.resolvedUrl(imageRole);
            type: typeRole
            name: nameRole
            nameFull: nameFullRole
            descrFirst: descrFirstRole
//            descrSecond: descrSecondRole
            checkable: root.checkMode

            onChangeModel: {
                if( staffStructModel.toChildDivision(itemDeleagte.name) === true )
                    staffStructPathModel.add(itemDeleagte.name, itemDeleagte.nameFull);
            }

            onUpdateModel: {
                staffStructModel.updateData();
            }

            onItemChoosed: {
                if( staffAppUI.mode === ESUStaffUI.ChooseProfileMode ) {
                    staffAppUI.registerProfile(itemDeleagte.nameFull);
                    windowManager.closeCurrentWindow();
                }
            }
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

