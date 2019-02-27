import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0
import BrainIM 0.1

Item {
    id: delegate
    height: 32
    property alias text: label.text
    property string plainText: text
    property alias textFormat: label.textFormat
    property alias textColor: label.color
    property alias linkColor: label.linkColor
    signal linkActivated(string link)
    TextMetrics {
        id: metrics
        font: label.font
        text: delegate.text
    }

    function formatActionText(actionDetails)
    {
        switch (actionDetails.actionType) {
        case ServiceAction.AddParticipant:
            if ((actionDetails.users.length === 1) && (actionDetails.users[0] === actionDetails.actor)) {
                return actionDetails.actor + " joined the chat"
            }
            return actionDetails.actor + " added " + actionDetails.users
        case ServiceAction.DeleteParticipant:
            if ((actionDetails.users.length === 1) && (actionDetails.users[0] === actionDetails.actor)) {
                return actionDetails.actor + " left the chat";
            }
            return actionDetails.actor + " removed " + actionDetails.users + "from the chat";
        case ServiceAction.Invalid:
        default:
            return "Invalid action"
        }
    }

    Rectangle {
        id: background
        visible: label.visible
        anchors.centerIn: parent
        width: metrics.width + 20
        height: metrics.height + 10
        radius: 10
        color: Material.color(Material.background, Material.theme === Material.Light ? Material.Shade100 : Material.Shade800)
    }

    Label {
        id: label
        anchors.centerIn: background
        onLinkActivated: delegate.linkActivated(link)
        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.NoButton
            cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
        }
    }
}
