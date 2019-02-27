import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0
import BrainIM 0.1

ToolBar {
    property alias peerName: nameLabel.text

    Label {
        id: nameLabel
        anchors.centerIn: parent
    }
}
