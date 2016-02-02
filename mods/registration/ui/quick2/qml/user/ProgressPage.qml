import QtQuick 2.2

import MKBKompas.CSR.ESUTZ.Registration 1.0
import MKBKompas.CSR.ESUTZ.Ui 1.0

import "qrc:/ui_components/qml"
import ".."


SwitchViewItem {
    id: root
    color: "transparent"
    border.color: "#a8f0f0f0"
    border.width: 1
    itemName: "registrationProgress"

    property real fieldHeight: 30
    property int currentState: -1;    

    signal close();

    Timer {
        id: closeTimer
        interval: 2500
        repeat: false
        onTriggered: {
            checkmark.reset();
            uiView.gotoFirstPage();
        }
    }

    onReady: {
        registrationEsu.currentUiForm = root;
        root.currentState = registrationEsu.state;
        cancelButton.checked = false;
        checkmark.reset();

        switch(root.currentState)
        {
        case EsuRegistration.RegisterUserState:
            registrationEsu.registerUser();
            labelAction.text = "<i>Текущая задача</i>:\t\tРегистрация";
            busyIndicator.activate = true;
            break;
        case EsuRegistration.ChangeRegistrationState:
            registrationEsu.changeUser();
            labelAction.text = "<i>Текущая задача</i>:\t\tСмена профиля";
            busyIndicator.activate = true;
            break;
        default:
            break;
        }
    }

    function finishProgress() {
        busyIndicator.activate = false;

        switch(root.currentState)
        {
            case EsuRegistration.RegisterUserState:
                if( registrationEsu.isRegistered ) checkmark.success();
                else checkmark.failure();
                break;
            case EsuRegistration.ChangeRegistrationState:
                if( registrationEsu.operationState === EsuRegistration.OperationSuccess )
                    checkmark.success();
                else checkmark.failure();
                break;
            default: break;
        }

        closeTimer.start();        
    }

    function openConfirmationDialog(message, title) {
        confirmationDialog.message = message;
        confirmationDialog.title = title;
        confirmationDialog.open();
    }

    function startTaskProgress(interval) {
        progress.stopTimer();

        if( interval <= 0 || interval === undefined )
            interval = 30000;

        progress.visible = true;
        progress.startTimer(interval);
    }

    function stopTaskProgress() {
        progress.stopTimer();
        progress.visible = false;
    }

    function __taskTimeout()
    {
        // таймаут задачи
        switch( registrationEsu.currentTask )
        {
        case EsuRegistration.CheckRegistrarTask:
            registrationEsu.setTaskTimeout();
            break;
        default: break;
        }
    }

    // [ Main View ]
    Rectangle {
        id: frame
        radius: 6
        border.color: "#a8f0f0f0"
        border.width: 2
        width: 450
        height: 400
        anchors.centerIn: parent        
        color: "transparent"

        Rectangle {
            id: indicatorContent
            height: parent.height / 3 + 150
            radius: 6
            //border.width: 1
            //border.color: "#a8f0f0f0"
            color: "transparent"
            //color: "#a8707070"
            anchors {
                top: parent.top
                right: parent.right
                left: parent.left
                leftMargin: 15
                rightMargin: 15
            }

            BusyIndicator {
                id: busyIndicator                
                anchors {
                    top: parent.top
                    topMargin: 10
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Image {
                id: checkmark
                width: 40
                height: 40
                anchors {
                    top: parent.top
                    topMargin: 10
                    horizontalCenter: parent.horizontalCenter
                }
                antialiasing: true
                smooth: true
                property string successIcon: "qrc:/ui_components/ui_components/confirm.png"
                property string failureIcon: "qrc:/ui_components/ui_components/cross.png"
                opacity: 0.0;

                NumberAnimation on opacity { duration: 250 }

                function success() {
                    checkmark.source = checkmark.successIcon;
                    opacity = 1.0;
                }

                function failure() {
                    checkmark.source = checkmark.failureIcon;
                    opacity = 1.0;
                }

                function reset() {
                    checkmark.source = "";
                    opacity = 0.0;
                }
            }

            Text {
                id: notifyText
                anchors {
                    top: busyIndicator.bottom                    
                    right: parent.right
                    left: parent.left
                    margins: 3
                }
                height: 50
                color: "white"
                text: registrationEsu.currentMessage
                font.family: "Arial"
                font.pixelSize: 14
                font.bold: true
                clip: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignTop
                wrapMode: Text.WordWrap
            }

            ProgressBar {
                id: progress
                width: parent.width
                visible: false
                prefix: "";
                textPosition: Qt.AlignCenter

                anchors {
                    top: notifyText.bottom
                    topMargin: 5
                    horizontalCenter: parent.horizontalCenter
                }

                onFinished: {
                    __taskTimeout();
                }
            }

        }

        Rectangle {
            id: infoContent
            color: "transparent"
            //color: "#a8707070"
            radius: 6            
            border.width: 1
            border.color: "#a8f0f0f0"
            anchors {
                top: indicatorContent.bottom
                bottom: buttonsContainer.top
                right: parent.right
                left: parent.left
                topMargin: 5
                bottomMargin: 5
                leftMargin: 5
                rightMargin: 5
            }

            Column {
                id: infoContentColumn
                anchors.fill: parent

                Label {
                    id: labelAction
                    width: parent.width
                    height: fieldHeight
                    radius: 6
                    text: ""
                }

                Label {
                    id: labelRole
                    width: parent.width
                    height: fieldHeight
                    radius: 6
                    text: "<i>Текущий профиль</i>:\t" + registrationEsu.choosedRole;
                }
            }
        }

        Item {
            id: buttonsContainer
            height: 50
            anchors {
                right: parent.right
                left: parent.left
                bottom: parent.bottom
                leftMargin: 15
                rightMargin: 15
                bottomMargin: 2
            }

            Button {
                id: cancelButton
                height: parent.height - 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                width: 130
                checkable: true
                label: "ОТМЕНА"
                radius: 6
                borderWidth: 1
                animateBackground: true
                animated: true
                enableButton: (!checked && !closeTimer.running);
                onCheckedChanged: {
                    registrationEsu.currentMessage = "Отмена операции"
                    registrationEsu.clearCurrentOperation();
                    busyIndicator.activate = false;
                    checkmark.failure();
                    stopTaskProgress();
                    closeTimer.restart();
                }
            }
        }
    }

    ConfirmationDialog {
        id: confirmationDialog
        title: ""

        onAccepted: {
            switch( registrationEsu.state )
            {
            case EsuRegistration.RegisterUserState:
                if( registrationEsu.error === EsuRegistration.UserAlreadyRegisteredError ) {
                    registrationEsu.acceptRegistrationData();
                    busyIndicator.activate = false;
                    checkmark.success();
                    closeTimer.restart();
                }
                break;
            default: break;
            }
        }

        onRejected: {
            registrationEsu.clearCurrentOperation();
            busyIndicator.activate = false;
            checkmark.failure();
            progress.stopTimer();
            closeTimer.restart();
        }
    }
}

