import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import BrainIM 0.1
import TelepathyQt 0.1

import "../../components"
import "../accounts"

ScrollablePage {
    id: page

    readonly property int buttonWidth: Math.max(addAccount.implicitWidth,
                                                Math.min(addAccount.implicitWidth * 2, page.availableWidth / 3))

    Column {
        width: parent.width
        Repeater {
            id: accountsView
            model: AccountsModel { }
            delegate: ItemDelegate {
                id: accountsDelegate
                width: page.width
                text: displayName + " (" + cmName + "/" + protocolName + ")"
                onClicked: {
                    accountCreator.setAccount(model.uniqueIdentifier)
                    pagesView.currentView = accountCreator
                }
            }
        }
    }

    footer: Button {
        id: addAccount
        text: "Add account"
        onClicked: {
            window.subtitle = text
            stackView.push("ConnectionManagerView.qml")
        }
    }
}
