import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import BrainIM 0.1

Frame {
    id: messageView
    width: 800
    height: 600

    signal activateDialog(var peer)
    property alias model: listView.model

    ListView {
        id: listView
        anchors.fill: parent
        delegate: Loader {
            readonly property var modelData: model
            active: index >= 0
            width: listView.width - listView.leftMargin - listView.rightMargin
            sourceComponent: {
                switch (modelData.entityType) {
                case Event.NewDay:
                    return newDayDelegate
                case Event.ServiceAction:
                    return serviceActionDelegate
                case Event.Message:
                    return messageDelegate
                default:
                    return
                }
            }
        }
        ScrollBar.vertical: ScrollBar {}
    }

    Component {
        id: messageDelegate
        MessageDelegate {
            model: modelData
        }
    }

    Component {
        id: newDayDelegate
        ServiceMessageDelegate {
            text: Qt.formatDate(modelData.timestamp, Qt.DefaultLocaleLongDate)
        }
    }
    Component {
        id: serviceActionDelegate
        ServiceMessageDelegate {
            text: formatActionText(modelData)
            plainText: modelData.sender + " added " + modelData.users
            textFormat: Text.StyledText
            linkColor: textColor
            function mkLinkToPeer(peer) {
                return "<a href=\"" + peer + "\">" + peer + "</a>"
            }
            onLinkActivated: {
                console.log("Link activated: " + link)
            }
        }
    }
}
