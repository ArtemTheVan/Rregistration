import QtQuick 1.1
import MKBKompas.CSR.ESUTZ.Staff 1.0

import "qrc:/ui_components/qml"
import "qrc:/ui_components/qml/switch_view/"
import "qrc:/mods/military_staff/ui/qml/components/"


Window {
    id: root
    width: (parent != undefined)?(parent.width):(500)
    height: (parent != undefined)?(parent.height):(350)
    windowName: "StaffForm"

    function modeChanged() {
        switch(staffAppUI.mode)
        {
        case ESUStaffUI.ViewMode:
            break;
        default: break;
        }
    }

    Component.onCompleted: staffAppUI.uiStaffForm = root;

    BorderImage {
        id: bgImage
        source: "qrc:/ui_components/images/background-transparent.png"
        width: parent.width; height: parent.height;
        border.left: 6; border.top: 6;
        border.right: 6; border.bottom: 6;
    }

    PageTemplate {
        id: staffTemplate
        anchors.fill: parent
        pathModel: staffPathModel
        //caption: staffAppUI.caption;

        onClose: root.close();
        onArrowUp: structPage.arrowUp();
        onArrowDown: structPage.arrowDown();

        onPathDelegateAction: {
            var cnt = staffPathModel.count;
            var next = index+1;
            if( cnt === 1 || next === cnt ) return;

            if( index === (cnt-2) ) {
                staffModel.parentDivision();
                staffPathModel.removeLast();
            } else {
                staffModel.toParentDivision(name);
                staffPathModel.removeFrom(next);
            }
        }

        StaffPage {
            id: staffPage
            anchors.fill: parent
        }
    }
}
