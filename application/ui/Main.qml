import QtQuick
import "./Layout/" as Layout

import Theme 1.0

Window {
    id: mainWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    color: ThemeProvider.get_current_theme().base
}
