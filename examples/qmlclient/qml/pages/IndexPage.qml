import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import BrainIM 0.1

ScrollablePage {
    id: page

    Column {
        focus: true
        width: parent.width

        Repeater {
            model: ListModel {
                id: indexModel
                ListElement { title: "Account management"; source: "AccountManagement.qml" }
            }

            ItemDelegate {
                width: parent.width
                text: model.title
                highlighted: ListView.isCurrentItem
                onClicked: {
                    stackView.push(model.source, { } )
                }
            }
        }
    }
}
