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
            model: AccountsModel { id: accountsModel }
            delegate: ItemDelegate {
                id: accountsDelegate
                width: page.width
                height: 48
                text: displayName + " (" + cmName + "/" + protocolName + ")"
                onClicked: {
                    stackView.push("AccountEditor.qml", {
                                       "title": "Accounts/" + displayName
                                   })
                    stackView.currentItem.setAccount(model.uniqueIdentifier)
                }
                indicator: CheckBox {
                    id: enabledBox
                    x: accountsDelegate.mirrored ? accountsDelegate.leftPadding : accountsDelegate.width - width - accountsDelegate.rightPadding
                    anchors.baseline: accountsDelegate.contentItem.baseline
                    checked: model.enabled
                    onCheckStateChanged: {
                        model.enabled = checked
                    }
                }
            }
        }
    }

    footer: Button {
        id: addAccount
        text: "Add account"
        onClicked: {
            stackView.push("ConnectionManagerView.qml", {
                               "title": text
                           })
        }
    }
}
