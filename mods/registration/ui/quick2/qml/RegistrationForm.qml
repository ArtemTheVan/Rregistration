import QtQuick 2.2
import MKBKompas.CSR.ESUTZ.Registration 1.0
import MKBKompas.CSR.ESUTZ.Ui 1.0

import "qrc:/ui_components/qml"

import "./user" as User
import "./registrar" as Registrar


BaseWindow {
    id: registrationForm
    windowName: "RegistrationDialog"
    anchors.fill: parent
    animated: true
    hideWorkPanel: true
    state: "closed";
    visible: false
    opacity: 0.2
    focus: true
    Keys.enabled: true
    Keys.onEscapePressed: registrationForm.close()
    color: "#ca787878"

    property int animationSpeed: 600

    signal windowClosed;

    onShowEvent: {
        registrationForm.state = "visible";
    }

    onCloseEvent: {
        registrationForm.state = "closed";
    }

    onWindowClosed: {
//        if( registrationEsu.isRegistrar )
//            registrationEsuUI.uiRegistrarForm.current = 0;
//        else
//            registrationEsuUI.uiUserForm.current = 0;
    }

    // [ Animation ]:
    states: [
        State {
            name: "visible"
            PropertyChanges { target: registrationForm; opacity: 1.0; visible: true; }
            PropertyChanges { target: uiLoader; scale: 1.0; visible: true; }
        },
        State {
            name: "closed"
            PropertyChanges { target: registrationForm; opacity: 0.2; visible: false; }
            PropertyChanges { target: uiLoader; scale: 0.0; visible: false; }
        }
    ]

    transitions: [
        Transition {
            from: "closed"
            to: "visible"
            SequentialAnimation {
                PropertyAction { targets: [registrationForm, uiLoader]; property: "visible"; value: true; }
                ParallelAnimation {
                    NumberAnimation {
                        target: registrationForm; properties: "opacity"; duration: animationSpeed;
                    }
                    NumberAnimation {
                        target: uiLoader; properties: "scale"; duration: animationSpeed;
                        easing.type: Easing.InElastic; easing.amplitude: 2.0; easing.period: 2.0;
                    }
                }
            }
        },
        Transition {
            from: "visible"
            to: "closed"
            SequentialAnimation {
                ParallelAnimation {
                    NumberAnimation {
                        target: uiLoader; properties: "scale"; duration: animationSpeed;
                        easing.type: Easing.InExpo; easing.amplitude: 2.0; easing.period: 2.0;
                    }
                    NumberAnimation { target: registrationForm; properties: "opacity"; duration: animationSpeed; }
                }
                PropertyAction { targets: [registrationForm, uiLoader]; property: "visible"; value: false; }
                ScriptAction { script: registrationForm.windowClosed(); }
            }
        }
    ]

    // [ Animation ]

    Timer {
        id: closeTimer
        interval: 1000
        onTriggered: registrationForm.close();
    }

    // [ Protect Area ]
    MouseArea {
        id: protectArea
        anchors.fill: parent
        hoverEnabled: false
    }

    BusyIndicator {
        id: indicator
        anchors.centerIn: parent
        activate: uiLoader.status !== Loader.Ready
    }

    Loader {
        id: uiLoader
        anchors.fill: parent
        property string userViewUrl: "qrc:/mods/registration/ui/qml/user/UserView.qml"
        property string registrarViewUrl: "qrc:/mods/registration/ui/qml/registrar/RegistrarView.qml"
        source: (registrationEsu.isRegistrar)?( registrarViewUrl ):(userViewUrl);
        asynchronous: true
    }
}
