import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0
import BrainIM 0.1

Item {
    id: messageDelegate
    height: contentHeight + previousEventSpacing + nextEventSpacing + details.anchors.topMargin + details.anchors.bottomMargin
    readonly property int contentHeight: {
        if (!pictureFrame.visible) {
            return detailsColumn.height
        }
        var pictureNeededHeight = pictureFrame.height
        if (joinWithPrev) {
            // We can use the space in previous message for picture.
            // It is hard to get the exact available space, but there is at least the height of
            // the peer name and one line of text (use double margin instead of line height for now)
            pictureNeededHeight -= nameLabel.height + defaultMargin * 2
        }
        return Math.max(pictureNeededHeight, detailsColumn.height)
    }

    property var model
    property bool joinWithNext: typeof(model.nextMessage) != "undefined" && model.nextMessage.peer.id === model.peer.id
    property bool joinWithPrev: typeof(model.previousMessage) != "undefined" && model.previousMessage.peer.id === model.peer.id

    property color backgroundColor: Material.color(Material.background, Material.theme === Material.Light ? Material.Shade200 : Material.ShadeA200)

    readonly property int defaultMargin: 8
    readonly property int halfMargin: defaultMargin / 2
    property int previousEventSpacing: joinWithPrev ? 0 : halfMargin
    property int nextEventSpacing: joinWithNext ? 0 : halfMargin

    Item {
        id: content
        anchors.fill: messageDelegate
        anchors.topMargin: messageDelegate.previousEventSpacing
        anchors.bottomMargin: messageDelegate.nextEventSpacing

        Rectangle {
            id: roundDecoration
            visible: !messageDelegate.joinWithNext || !messageDelegate.joinWithPrev
            anchors.fill: parent
            radius: defaultMargin
            color: messageDelegate.backgroundColor
        }

        Rectangle {
            id: rectDecoration
            anchors.topMargin: messageDelegate.joinWithPrev ? 0 : roundDecoration.radius
            anchors.bottomMargin: messageDelegate.joinWithNext ? 0 : roundDecoration.radius
            visible: messageDelegate.joinWithNext || messageDelegate.joinWithPrev
            anchors.fill: parent
            color: messageDelegate.backgroundColor
        }

        Item {
            id: pictureFrame
            height: picture.height
            width: height
            visible: !messageDelegate.joinWithNext

            anchors.bottom: content.bottom
            anchors.bottomMargin: defaultMargin
            anchors.left: content.left
            anchors.leftMargin: defaultMargin

            Rectangle {
                id: picture
                readonly property var colors: [
                    Material.Purple,
                    Material.DeepPurple,
                    Material.Blue,
                    Material.LightBlue,
                    Material.Cyan,
                    Material.Teal,
                    Material.LightGreen,
                    Material.Lime,
                    Material.Amber,
                    Material.Orange,
                    Material.DeepOrange,
                    Material.Brown,
                    Material.Grey,
                    Material.BlueGrey,
                ]
                function getColor(username) {
                    return colors[Qt.md5(username).charCodeAt(0) % colors.length]
                }

                color: Material.color(getColor(messageDelegate.model.peer.id))
                width: 48
                height: 48
                radius: defaultMargin

                Text {
                    anchors.centerIn: parent
                    font.pixelSize: parent.width - defaultMargin
                    text: messageDelegate.model.peer.id[0]
                    font.capitalization: Font.AllUppercase
                }
            }
        }

        ItemDelegate {
            id: details
            anchors {
                left: pictureFrame.right
                leftMargin: defaultMargin
                right: content.right
                rightMargin: defaultMargin
                top: content.top
                topMargin: messageDelegate.joinWithPrev ? 0 : defaultMargin
                bottom: content.bottom
                bottomMargin: messageDelegate.joinWithNext ? 0 : defaultMargin
            }

            Column {
                id: detailsColumn
                width: parent.width
                Label {
                    id: nameLabel
                    text: messageDelegate.model.peer.id
                    visible: !messageDelegate.joinWithPrev
                    font.bold: true
                }
                Label {
                    id: messageLabel
                    text: messageDelegate.model.message.text
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }
            }
            Label {
                id: deliveryStatus
                anchors.bottom: details.bottom
                anchors.right: details.right
                property bool showSentTimestamp: true
                readonly property date printDate: showSentTimestamp ? modelData.sentTimestamp : modelData.receivedTimestamp
                readonly property bool printDateIsToday: {
                    var today = new Date
                    today.setHours(0,0,0,0)
                    return printDate >= today;
                }

                text: printDateIsToday ? Qt.formatTime(printDate, "hh:mm") : Qt.formatDateTime(printDate, "d MMM hh:mm")
            }
        }
    }
}
