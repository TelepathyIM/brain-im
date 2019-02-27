import QtQuick 2.9
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import BrainIM 0.1

Frame {
    id: dialogView
    width: 800
    height: 600

    signal activateDialog(var peer)
    property alias model: listView.model

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 4
        delegate: DialogDelegate {
            width: listView.width - listView.leftMargin - listView.rightMargin
            onClicked: dialogView.activateDialog(model.peer)
        }
        ScrollBar.vertical: ScrollBar {}
    }
}
