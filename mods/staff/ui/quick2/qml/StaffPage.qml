import QtQuick 1.1
import MKBKompas.CSR.ESUTZ.Staff 1.0

import "qrc:/ui_components/qml/"
import "qrc:/mods/military_staff/ui/qml/components/"


Item {
    id: staffPage

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

        model: staffModel
        delegate: StaffItemDelegate {
            id: itemDeleagte
            iconSource: Qt.resolvedUrl(imageRole);
            type: typeRole
            name: nameRole
            nameFull: nameFullRole
            descrFirst: descrFirstRole
            descrSecond: (type !== StaffUI.DivisionType)?(addressRole):(descrSecondRole);            

            onChangeModel: {
                if( staffModel.toChildDivision(itemDeleagte.name) === true )
                    staffPathModel.add(itemDeleagte.name, itemDeleagte.nameFull);
            }

            onUpdateModel: {
                staffModel.updateData();
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

