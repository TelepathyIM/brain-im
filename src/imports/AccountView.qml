import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import BrainIM 0.1

ScrollablePage {
    id: page

    readonly property int buttonWidth: Math.max(addAccount.implicitWidth,
                                                Math.min(addAccount.implicitWidth * 2, page.availableWidth / 3))

    property alias model: accountsView.model

    Column {
        id: column
        width: parent.width
        Repeater {
            id: accountsView
            delegate: ItemDelegate {
                id: accountsDelegate
                width: parent.width
                height: 48

                property string accountId: model.accountId
                property bool accountEnabled: model.accountEnabled
                property string displayName: model.displayName || qsTr("<no name>")
                text: displayName + " (" + managerName + "/" + protocolName + ")"
                onClicked: {
                    stackView.push("AccountEditor.qml", {
                                       "title": "Accounts/" + displayName
                                   })
                    stackView.currentItem.setAccount(accountsDelegate.accountId)
                }
                indicator: CheckBox {
                    id: enabledBox
                    x: accountsDelegate.mirrored ? accountsDelegate.leftPadding : accountsDelegate.width - width - accountsDelegate.rightPadding
                    anchors.verticalCenter: accountsDelegate.contentItem.verticalCenter
                    checked: accountsDelegate.accountEnabled
                    onToggled: {
                        page.model.setAccountEnabled(accountsDelegate.accountId, enabledBox.checked)
                    }
                }
            }
        }
    }
}
