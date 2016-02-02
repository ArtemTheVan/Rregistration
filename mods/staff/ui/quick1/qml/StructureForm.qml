import QtQuick 1.1
import MKBKompas.CSR.ESUTZ.Staff 1.0

import "qrc:/ui_components/qml"
import "qrc:/ui_components/qml/switch_view/"
import "qrc:/mods/military_staff/ui/qml/components/"


Window {
    id: root
    width: (parent != undefined)?(parent.width):(500)
    height: (parent != undefined)?(parent.height):(350)
    windowName: "StructureForm"

    function modeChanged() {
        switch(staffAppUI.mode)
        {
        case ESUStaffUI.ChooseProfileMode:
            break;
        case ESUStaffUI.ViewMode:
            break;
        default: break;
        }
    }

    Component.onCompleted: staffAppUI.uiStructureForm = root;

    BorderImage {
        id: bgImage
        source: "qrc:/ui_components/images/background-transparent.png"
        width: parent.width; height: parent.height;
        border.left: 6; border.top: 6;
        border.right: 6; border.bottom: 6;
    }

    PageTemplate {
        id: structDialogTemplate
        anchors.fill: parent
        pathModel: staffStructPathModel
        caption: staffAppUI.caption;

        onClose: root.close();
        onArrowUp: structPage.arrowUp();
        onArrowDown: structPage.arrowDown();

        onItemChoosed: {
            var type = structPage.itemType;
            var name = structPage.itemName;
            var nameFull = structPage.itemNameFull;

            if( type === ESUStaffUI.DivisionObject ) {
                if( staffStructModel.toChildDivision(name) === true )
                    staffStructPathModel.add(name, nameFull);
            } else {
                if( staffAppUI.mode === ESUStaffUI.ChooseProfileMode ) {
                    staffAppUI.registerProfile(nameFull);
                    windowManager.closeCurrentWindow();
                }
            }
        }

        onPathDelegateAction: {
            var cnt = staffStructPathModel.count;
            var next = index+1;
            if( cnt === 1 || next === cnt ) return;

            if( index === (cnt-2) ) {
                staffStructModel.parentDivision();
                staffStructPathModel.removeLast();
            } else {
                staffStructModel.toParentDivision(name);
                staffStructPathModel.removeFrom(next);
            }
        }

        StaffStructurePage {
            id: structPage
            anchors.fill: parent
        }
    }
}
