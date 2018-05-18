import QtQuick 2.7
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import BrainIM 0.1
import TelepathyQt 0.1

import "../components"

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
                    stackView.push(model.source, { "title": model.title} )
                }
            }
        }
    }
}
