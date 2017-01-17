import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import BrainIM 0.1
import TelepathyQt 0.1

import "components"

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600

    title: "Brain IM"

    AccountsModel {
        id: accountsModel
    }

    ConnectionManagersModel {
        id: connectionManagersModel
    }

    ProtocolsModel {
        id: protocolsModel
    }

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            Label {
                text: pagesView.currentView.objectName
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
        }
    }

    Item {
        id: pagesView
        anchors.fill: parent
        anchors.margins: 40
        property Item currentView: accounts
        onCurrentViewChanged: {
            console.log("CV:" + currentView.objectName)
        }

        Item {
            id: accounts
            objectName: "Accounts"
            anchors.fill: parent
            visible: pagesView.currentView == accounts

            ListView {
                id: accountsView
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: addAccount.top

                model: accountsModel
                delegate: ItemDelegate {
                    id: accountsDelegate
                    width: accountsView.width - accountsView.leftMargin - accountsView.rightMargin
                    property var acc: accountObject
        //            property Account acc: accountObject
                    text: displayName + " (" + cmName + "/" + protocolName + ")"
                    onClicked: {
                        accountCreator.setAccount(model.uniqueIdentifier)
                        pagesView.currentView = accountCreator
                    }
                }
                onCountChanged: {
                    console.warn("AccountsCount:" + count)
                }
            }

            Button {
                id: addAccount
                height: 50
                width: 120
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                text: "Add account"
                onClicked: {
                    pagesView.currentView = connectionManagers
                }
            }
        }

        Item {
            id: connectionManagers
            objectName: "connectionManagers"
            anchors.fill: parent
            visible: pagesView.currentView == connectionManagers

            ListView {
                id: connectionManagersView
                anchors.fill: parent
//                anchors.left: parent.left
//                anchors.right: parent.right
//                anchors.top: parent.top
//                anchors.bottom: backToAccounts.top

                model: connectionManagersModel
                delegate: ItemDelegate {
                    id: managerDelegate
                    text: "Name: " + model.name
                    width: connectionManagersView.width - connectionManagersView.leftMargin - connectionManagersView.rightMargin
                    onClicked: {
                        console.log("Clicked on cm" + model.name)
                        var ms = []
                        ms.push(model.name)

                        protocolsModel.managers = ms
                        pagesView.currentView = protocols
                    }
                }
                onCountChanged: {
                    console.warn("ProtocolsCount:" + count)
                }
            }

//            Item {
//                id: backToAccounts
//                height: 50
//                anchors.left: parent.left
//                anchors.right: parent.right
//                anchors.bottom: parent.bottom

//                Rectangle {
//                    color: "steelblue"
//                    anchors.fill: parent
//                    Text {
//                        anchors.centerIn: parent
//                        text: "back to accounts"
//                    }
//                }
//                MouseArea {
//                    anchors.fill: parent
//                    onClicked: {
//                        pagesView.currentView = accounts
//                    }
//                }
//            }
        }

        Item {
            id: protocols
            objectName: "protocols"
            anchors.fill: parent
            visible: pagesView.currentView == protocols

            ListView {
                id: protocolsList
                anchors.fill: parent
//                anchors.left: parent.left
//                anchors.right: parent.right
//                anchors.top: parent.top
//                anchors.bottom: backToAccounts.top

                model: protocolsModel
                delegate: ItemDelegate {
                    id: protocolsDelegate
                    width: protocolsList.width - protocolsList.leftMargin - protocolsList.rightMargin
                    text: "Name: " + model.name
                    onClicked: {
                        accountCreator.create(model.cmName, model.name)
                        pagesView.currentView = accountCreator
                    }
                }
                onCountChanged: {
                    console.warn("ProtocolsCount:" + count)
                }
            }

//            Item {
//                id: backToAccounts
//                height: 50
//                anchors.left: parent.left
//                anchors.right: parent.right
//                anchors.bottom: parent.bottom

//                Rectangle {
//                    color: "steelblue"
//                    anchors.fill: parent
//                    Text {
//                        anchors.centerIn: parent
//                        text: "back to accounts"
//                    }
//                }
//                MouseArea {
//                    anchors.fill: parent
//                    onClicked: {
//                        pagesView.currentView = accounts
//                    }
//                }
//            }
        }

        AccountEditor {
            id: accountCreator
            anchors.fill: parent
            visible: pagesView.currentView == accountCreator
            onCreateAccount: {
                console.log("On create account for " + manager)
                accountsModel.createAccount(manager, protocol, displayName, parameters)
            }
        }
    }

    Shortcut {
        sequence: StandardKey.Quit
        onActivated: window.close()
    }
}
