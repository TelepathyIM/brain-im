import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

ItemDelegate {
    id: root_
    width: parent.width
    height: 48

    property string accountId
    property bool accountEnabled
    property string displayName
    property string managerName
    property string protocolName

    signal setAccountEnabled(string accountId, bool enabled)
    signal removeAccount(string accountId)

    text: (displayName || qsTr("<no name>")) + " (" + managerName + "/" + protocolName + ")"
    indicator: Row {
        x: root_.mirrored
           ? root_.leftPadding
           : root_.width - implicitWidth - root_.rightPadding

        anchors.verticalCenter: root_.contentItem.verticalCenter
        CheckBox {
            id: enabledBox_
            anchors.verticalCenter: parent.verticalCenter
            checked: root_.accountEnabled
            onCheckStateChanged: {
                root_.setAccountEnabled(root_.accountId, enabledBox_.checked)
            }
        }
        ToolButton {
            id: deleteButton_
            anchors.verticalCenter: parent.verticalCenter
            icon.color: "transparent"
            icon.name: "edit-delete"

            onClicked: {
                root_.removeAccount(root_.accountId)
            }
        }
    }
}
