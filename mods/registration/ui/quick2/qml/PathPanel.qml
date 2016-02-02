import QtQuick 2.2


Item {
    id: root
    clip: true

    property alias model: view.model

    ListView {
        id: view;
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: line.top
            topMargin: 2
            bottomMargin: 2
            leftMargin: 5
        }
        clip: true
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.HorizontalFlick

        model: staffPathModel
        delegate: PathDelegate {
            id: pathDelegate
            height: view.height
            color: "#a8787878"
            text: nameRole
            itemIndex: indexRole
            arrow: arrowRole

            Component.onCompleted: {
                if( arrow === false ) {
                    pathDelegate.color = "#a8585858";
                } else pathDelegate.color = "#a8787878";
            }

            onClicked: {
                var count = staffPathModel.count;
                var next = index+1;
                if( count === 1 || next === count ) return;

                if( index === (count-2) ) {
                    registrationStaffModel.setParentDivision();
                    staffPathModel.removeLast();
                } else {
                    registrationStaffModel.toParentDivision(name);
                    staffPathModel.removeFrom(next);
                }
            }
        }
    }

    Rectangle {
        id: line
        height: 2
        color: "#a8f0f0f0"
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }
}
