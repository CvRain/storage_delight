import QtQuick
import "./Layout/" as Layout

import Theme 1.0

Window {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    color: "#eff1f5"

    Component.onCompleted: {
        mainWindow.color = ThemeProvider.color_peach()
    }
}
