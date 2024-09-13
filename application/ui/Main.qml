import QtQuick
import "./Layout/" as Layout

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    color: "#eff1f5"

    Layout.Login{
        anchors.centerIn: parent
    }
}
