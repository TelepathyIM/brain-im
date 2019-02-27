import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import BrainIM 0.1

Item {
    id: messageEditor
    height: 64
    width: 200

    Pane { anchors.fill: parent }
    Frame { anchors.fill: parent } // Decoration

    property alias peer: sender.peer

    MessageSender {
        id: sender
//        target: telegramClient
        onMessageSent: {
            messageSendStubProxy.messageSent(message, peer)
        }
    }

    RowLayout {
        id: rowLayout

        anchors.fill: messageEditor
        ToolButton {
            id: attachButton
            text: '\uE4A0' // '\uD83D\uDCCE' ?
            font.pixelSize: messageEditor.fontSize
        }
        TextArea {
            id: textEditor
            Layout.fillWidth: true
            placeholderText: qsTr("Write a message...")
            wrapMode: TextArea.Wrap
            Keys.onPressed: {
                if (event.key === Qt.Key_Return) {
                    if (event.modifiers & Qt.CtrlModifier) {
                        textEditor.append('\n')
                    } else {
                        sender.sendMessage(textEditor.text)
                        event.accepted = true
                        textEditor.clear()
                    }
                }
            }
            onTextChanged: sender.setText(text)
        }
        ToolButton {
            id: emojiButton
            text:'\u263A'
            font.pixelSize: messageEditor.fontSize
        }
//        ToolButton {
//            id: recordMessageButton
//            text:'\uD83D\uDCCE'
//        }
    }
}
