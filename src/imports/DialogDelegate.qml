import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0

ItemDelegate {
    id: dialogDelegate
    height: 64
    width: 200
    property int margin: (pictureFrame.width - picture.width) / 2
    readonly property int defaultMargin: 8

    Item {
        //border.color: "black"
        //border.width: 1
        id: pictureFrame
        height: picture.height
        width: height

        anchors.verticalCenter: parent.verticalCenter
        anchors.left: dialogDelegate.left
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

            color: Material.color(getColor(nameLabel.text))
            width: 48
            height: 48
            radius: defaultMargin

            Text {
                anchors.centerIn: parent
                font.pixelSize: parent.width - defaultMargin
                text: nameLabel.text[0]
                font.capitalization: Font.AllUppercase
            }
        }
    }

    Row {
        anchors.leftMargin: defaultMargin
        anchors.left: pictureFrame.right
        anchors.verticalCenter: parent.verticalCenter
        Label {
            id: nameLabel
            text: model.alias
        }
    }

//    Label {
//        id: nameLabel
//        text: model.identifier
//        //text: model.alias
//        // font.pixelSize: Theme.fontSizeSmall
//    }

//    Item {
//        id: details
//        anchors.left: pictureFrame.right
//        anchors.right: parent.right
//        anchors.rightMargin: dialogDelegate.margin
//        height: parent.height
//        anchors.top: parent.top

//        Label {
//            id: messageLabel
//            text: sender + ": " + messageText
//            elide: Text.ElideRight
//            //font.pixelSize: Theme.fontSizeTiny
//            readonly property string sender: "You" //: model.draft ? "Draft" : model.messageFlags & Telegram.Namespace.MessageFlagOut ? "You" : model.firstName
//            readonly property string messageText: model.draft ? model.draft : model.lastMessage
//        }
////        Label {
////            id: deliveryStatus
////            anchors.right: parent.right
////            text: {
////                if (model.messageFlags & Telegram.Namespace.MessageFlagOut) {
////                    if (model.messageFlags & Telegram.Namespace.MessageFlagRead) {
////                        return "\u2713\u2713" // "\u2714"?
////                    }
////                    return "\u2713"
////                }
////                return ""
////            }
////            //font.pixelSize: Theme.fontSizeSmall
////        }
//        Label {
//            id: pinnedIndicator
//            anchors.right: parent.right
//            anchors.bottom: parent.bottom
//            text: "\u235f" // '\uD83D\uDCCC'?
//            //font.pixelSize: Theme.fontSizeSmall
//            visible: model.pinnedOrder >= 0 & !unreadCountLabel.visible
//        }
//        Item {
//            id: unreadCountIndicator
//            anchors.right: parent.right
//            anchors.bottom: parent.bottom
//            width: metrics.width + 10
//            height: metrics.height
//            visible: model.unreadMessageNumber
//            TextMetrics {
//                id: metrics
//                font: unreadCountLabel.font
//                text: unreadCountLabel.text
//            }

//            Rectangle {
//                id: unreadBackground
//                visible: unreadCountLabel.visible
//                anchors.fill: unreadCountIndicator
//                color: "gray"
//                radius: 10
//            }
//            Label {
//                id: unreadCountLabel
//                text: model.unreadMessageNumber
//                color: "white"
//                anchors.centerIn: unreadBackground
//            }
//        }
//    }
}
