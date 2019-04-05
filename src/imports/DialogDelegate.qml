import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0

import BrainIM 0.1

ItemDelegate {
    id: chatEntryDelegate
    width: 200

    property string displayName
    property int unreadMessageCount
    property var timestamp
    property var peer
    property var draft
    property bool isPinned
    readonly property bool isGroupChat: (chatType == ChatsModel.ChatTypeGroup)
                                        || (chatType == ChatsModel.ChatTypeMegaGroup)
    property int chatType

    property var lastMessage

    property bool debugGeometry: false
    property int smallSpacing: spacing / 2

    contentItem: Item {
        id: content
        implicitHeight: Math.max(peerPicture.height, contentColumn.height)

        PeerPicture {
            id: peerPicture
            height: 42
            width: height
            anchors.verticalCenter: parent.verticalCenter
            visible: content.width > width * 4

            displayName: chatEntryDelegate.displayName
            peer: chatEntryDelegate.peer
        }

        Item {
            id: contentColumn
            anchors.left: peerPicture.visible ? peerPicture.right : content.left
            anchors.leftMargin: peerPicture.visible ? chatEntryDelegate.spacing : 0
            anchors.right: content.right
            anchors.top: content.top
            anchors.bottom: content.bottom

            Item {
                anchors.top: parent.top
                width: parent.width
                height: displayNameLabel.implicitHeight
                Row {
                    spacing: chatEntryDelegate.smallSpacing
                    height: displayNameLabel.implicitHeight
                    Rectangle {
                        id: chatTypeIcon
                        radius: 4
                        width: height
                        height: displayNameLabel.height
                        visible: chatEntryDelegate.isGroupChat
                        color: "blue"
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    InlineHeader {
                        id: displayNameLabel
                        text: chatEntryDelegate.displayName
                    }
                    anchors.left: parent.left
                    anchors.right: deliveryIcon.visible ? deliveryIcon.left : lastMessageDateTime.left
                    anchors.rightMargin: chatEntryDelegate.spacing
                }

                Rectangle {
                    id: deliveryIcon
                    width: height
                    height: lastMessageDateTime.font.pixelSize
                    color: "green"
                    visible: !draft && (typeof(chatEntryDelegate.lastMessage) != "undefined" && chatEntryDelegate.lastMessage.flags & 1)
                    anchors.right: lastMessageDateTime.left
                    anchors.rightMargin: chatEntryDelegate.smallSpacing
                    anchors.verticalCenter: parent.verticalCenter
                }

                Label {
                    id: lastMessageDateTime
                    color: "gray"
                    horizontalAlignment: Qt.AlignRight
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 14

                    function formatRelativeCurrentDay(date)
                    {
                        var referenceDate = new Date
                        referenceDate.setHours(0,0,0,0)
                        if (chatEntryDelegate.timestamp >= referenceDate) {
                            // Today
//                            return Qt.formatTime(dialogDelegate.timestamp, "hh:mm")
                            return chatEntryDelegate.timestamp.toLocaleTimeString(Qt.locale(), Locale.ShortFormat);
                        }
                        referenceDate.setDate(referenceDate.getDate() - 1)
                        if (chatEntryDelegate.timestamp >= referenceDate) {
                            return qsTr("Yesterday")
                        }
                        referenceDate.setDate(referenceDate.getDate() - 6)
                        if (chatEntryDelegate.timestamp >= referenceDate) {
                            // Less than a week ago
                            return chatEntryDelegate.timestamp.toLocaleString(Qt.locale(), "ddd")
                        }
                        //return Qt.formatDate(dialogDelegate.timestamp)
                        //return Qt.formatDateTime(dialogDelegate.timestamp, "d MMM hh:mm")
                        return chatEntryDelegate.timestamp.toLocaleDateString(Qt.locale(), Locale.ShortFormat)
                    }

                    //text: typeof(dialogDelegate.timestamp) === "undefined" ? "<none>" : formatRelativeCurrentDay(dialogDelegate.timestamp)
                    text: chatEntryDelegate.chatType
                }
                Rectangle {
                    opacity: 0.1
                    color: "blue"
                    anchors.fill: parent
                    visible: chatEntryDelegate.debugGeometry
                }
            }

            Item {
                anchors.bottom: parent.bottom
                width: contentColumn.width
                height: messagePreviewLabel.implicitHeight

                Label {
                    id: messagePreviewLabel
                    elide: Text.ElideRight
                    text: prefixStyledText + contentText
                    anchors.left: parent.left
                    anchors.right: dialogIndicator.visible ? dialogIndicator.left : parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    maximumLineCount: 1

                    readonly property string prefixStyledText: prefixText ? "<font color=\"" + prefixColor + "\">" + prefixText + "</font>" : ""
                    readonly property color prefixColor: model.draft ? "red" : palette.link
                    readonly property string prefixText: {
                        if (model.draft) {
                            return "Draft: "
                        }
                        if (typeof(model.activeMessageActions) != "undefined") {
                            return ""
                        }
                        if (lastMessage) {
                            if (lastMessage.flags & 1) {
                                return "You: "
                            }
                            if (chatEntryDelegate.isGroupChat) {
                                return lastMessage.senderFirstName + ": "
                            }
                        }
                        return ""
                    }

                    readonly property string contentText: {
                        if (model.draft)
                            return model.draft

                        var emphasedText
                        if (typeof(model.activeMessageActions) != "undefined" && model.activeMessageActions.count > 0) {
                            var firstAction = model.activeMessageActions.get(0)
                            if (firstAction.type === "typing") {
                                emphasedText = "..." + firstAction.contact + " is typing"
                            }
                            if (emphasedText) {
                                return "<font color=\"" + emphasedContentColor + "\">" + emphasedText + "</font>"
                            }
                            // Ignore unknown actions
                        }
                        if (typeof(lastMessage) === "undefined") {
                            return ""
                        }
                        var text = lastMessage.text
                        if (lastMessage.type === Telegram.Namespace.MessageTypeText) {
                            return text
                        }
                        if (lastMessage.type === Telegram.Namespace.MessageTypeWebPage) {
                            return text
                        }
                        if (lastMessage.type === Telegram.Namespace.MessageTypePhoto) {
                            emphasedText = "Photo"
                        } else if (lastMessage.type === Telegram.Namespace.MessageTypeVideo) {
                            emphasedText = "Video"
                        } else if (lastMessage.type === Telegram.Namespace.MessageTypeSticker) {
                            emphasedText = "Sticker"
                        } else if (lastMessage.type === Telegram.Namespace.MessageTypeAnimation) {
                            emphasedText = "GIF"
                        } else if (lastMessage.type === Telegram.Namespace.MessageTypeDocument) {
                            emphasedText = "File"
                        } else {
                            // Do *not* ignore unknown messages
                            emphasedText = "Unsupported content" + lastMessage.type
                        }
                        if (text.length !== 0) {
                            emphasedText += ", "
                        }

                        return "<font color=\"" + emphasedContentColor + "\">" + emphasedText + "</font>" + text
                    }
                    readonly property color emphasedContentColor: palette.link
                }

                Flow {
                    id: dialogIndicator
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter

                    UnreadMessageIndicator {
                        id: unreadIndicator
                        count: chatEntryDelegate.unreadMessageCount
                    }

                    Text {
                        id: pinnedIndicator
                        visible: chatEntryDelegate.isPinned
                        text: "<pin>"
                        color: "gray"
                        font: lastMessageDateTime.font
                        height: font.pixelSize
                        textFormat: Text.PlainText
                    }
                }

                Rectangle {
                    opacity: 0.1
                    color: "red"
                    anchors.fill: parent
                    visible: chatEntryDelegate.debugGeometry
                }
            }
        }
    }
}
