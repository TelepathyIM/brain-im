import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import BrainIM 0.1

ScrollablePage {
    id: page

    property alias model: accountsView.model

    Column {
        id: column
        width: parent.width
        Repeater {
            id: accountsView
            delegate: AccountDelegate {
                id: accountsDelegate
                width: parent.width
                height: 48

                accountId: model.accountId
                accountEnabled: model.accountEnabled
                displayName: model.displayName
                managerName: model.managerName
                protocolName: model.protocolName

                onClicked: {
                    stackView.push("AccountEditor.qml", {
                                       "title": "Accounts/" + displayName
                                   })
                    stackView.currentItem.setAccount(accountsDelegate.accountId)
                }

                onSetAccountEnabled: page.model.setAccountEnabled(accountId, enabled)
                onRemoveAccount: page.model.removeAccount(accountId)
            }
        }
    }
}
