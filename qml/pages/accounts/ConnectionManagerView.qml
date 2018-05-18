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

    Column {
        width: parent.width
        Repeater {
            model: ConnectionManagersModel { }
            delegate: ItemDelegate {
                id: managerDelegate
                text: "Name: " + model.name
                width: page.width
                onClicked: {
                    stackView.push("ProtocolView.qml", {
                                       "title": model.name,
                                       "managers": model.name
                                   })
                }
            }
        }
    }
}
