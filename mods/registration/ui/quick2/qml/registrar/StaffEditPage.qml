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

    onReady: registrationEsu.currentUiForm = root;

    signal close();
}
