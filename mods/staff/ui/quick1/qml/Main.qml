import QtQuick 1.1
import MKBKompas.CSR.ESUTZ.StaffModule 1.0

import "qrc:/ui_components/qml/switch_view/"
import "qrc:/mods/military_staff/ui/qml/components/"


Item {
    id: main
    anchors.fill: parent

    SwitchView {
        id: mainView
        objectName: "mainView"
        anchors.fill: parent

        Component.onCompleted: {
            if( staffAppUI.viewModeUI === StaffUI.StaffUIMode ) {
                mainView.current = 0;
            } else {
                mainView.current = 1;
            }            
        }

        function setStaffMode() {
            if( staffAppUI.viewModeUI === StaffUI.StaffUIMode ) {
                mainView.current = 0;
            } else {
                mainView.current = 1;
            }
            //console.log("Change view mode = " + mainView.current);
        }

        //! Staff Page
        SwitchViewItem {
            id: staffDialog
            animationSpeed: 10

            PageTemplate {
                id: staffDialogTemplate
                anchors.fill: parent
                pathModel: staffPathModel
                caption: qsTr("ШТАТНЫЙ СОСТАВ");

                onClose: staffAppUI.closeUI();
                onArrowUp: staffPage.arrowUp();
                onArrowDown: staffPage.arrowDown();
                onPathDelegateAction: {
                    var cnt = staffPathModel.count;
                    var next = index+1;
                    if( cnt === 1 || next === cnt ) return;
                    if( next === (index+1) ) {
                        staffModel.parentDivision();
                        staffPathModel.removeLast();
                    } else {
                        staffModel.toParentDivision(name);
                        staffPathModel.removeFrom(next);
                    }
                }
                onConfirm: {
                    console.log("Create staff");
                    if( staffAppUI.userAction === StaffUI.CreateStaffAction )
                    {
                        staffAppUI.closeUI();
                    }
                }

                StaffPage {
                    id: staffPage
                    anchors.fill: parent
                }
            }
        }

        //! StructurePage
        SwitchViewItem {
            id: structDialog
            animationSpeed: 10

            PageTemplate {
                id: structDialogTemplate
                anchors.fill: parent
                pathModel: staffStructPathModel
                caption: qsTr("ИЕРАРХИЯ");

                onClose: staffAppUI.closeUI();
                onArrowUp: structPage.arrowUp();
                onArrowDown: structPage.arrowDown();
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
                onConfirm: {
                    if( staffAppUI.userAction === StaffUI.CreateStaffAction )
                    {
                        var type = structPage.itemType;
                        var index = structPage.itemIndex;

                        if( type !== StaffUI.DivisionType ) {
                            staffAppUI.createStaff(index);
                            staffAppUI.closeUI();
                        }
                        else console.log("Can\'t create staff from division...");
                    }
                }

                StaffStructurePage {
                    id: structPage
                    anchors.fill: parent
                }
            }
        }
    }
}
