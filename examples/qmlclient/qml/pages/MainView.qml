import QtQuick 2.9
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import BrainIM 0.1
//import BrainIM.Telepathy 0.1

Page {
    id: mainScreen

    title: contactsModel.getName(mainScreen.peer)

    property alias peer: messagesModel.peer

    function showDialog(peer)
    {
        console.log("Show dialog with peer(" + peer.type + ", " + peer.id + ")")
        var p = Brain.peer(peer.id, peer.type)
        var index = contactsModel.indexOfPeer(p)
        messagesModel.setPeerContact(contactsModel.contactAt(index))
    }

    Row {
        id: contentRoot
        anchors.fill: mainScreen.contentItem
        DialogView {
            id: dialogView
            width: 360
            height: parent.height
            model: contactsModel
            onActivateDialog: mainScreen.showDialog(peer)
        }

        Item {
            id: rightColumn
            visible: width > 200
            width: parent.width - dialogView.width
            height: parent.height

//            MessageToolbar {
//                id: chatToolbar
//                anchors.top: parent.top
//                width: rightColumn.width
//                height: 48
//                z: 1
//            }
            MessageView {
                id: messageView
                width: rightColumn.width
                anchors.top: rightColumn.top
                anchors.bottom: messageEditor.top
                model: messagesModel
                onActivateDialog: mainScreen.showDialog(peer)
            }
            MessageEditor {
                id: messageEditor
                peer: mainScreen.peer
                width: rightColumn.width
                height: 64
                anchors.bottom: rightColumn.bottom
            }
        }
    }

    QtObject {
        id: messageSendStubProxy
        signal messageSent(string message, var peer)
        onMessageSent: {
            var typeText = (peer.type === Peer.Contact ? "User" : "Chat")
            console.log("Message to " + typeText + " " + peer.id)
        }
        signal draftChanged(string message, var peer)
    }


    ContactsModel {
        id: contactsModel
    }

//    TelepathyMessagesModel {
//        id: messagesModel
//    }

    MessagesModel {
        id: messagesModel
        contactsModel: contactsModel
    }
}
