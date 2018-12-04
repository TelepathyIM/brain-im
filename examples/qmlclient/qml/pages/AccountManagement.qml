import QtQuick 2.9
import QtQuick.Controls 2.2

import BrainIM 0.1

AccountView {
    id: page

    objectName: "account-management"
    title: qsTr("Account management")

    model: accountsModel

    data: [
        Component {
            id: addAccountComponent
            ConnectionManagerView {
            }
        }
    ]

    footer: Button {
        id: addAccount
        text: "Add account"
        onClicked: {
            stackView.push(addAccountComponent, { "title": text })
        }
    }
}
