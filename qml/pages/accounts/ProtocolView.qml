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

    property alias managers: protocolsModel.managers

    ListView {
        id: protocolsList
        anchors.fill: parent
        model: protocolsModel
        delegate: ItemDelegate {
            id: protocolsDelegate
            width: protocolsList.width - protocolsList.leftMargin - protocolsList.rightMargin
            text: "Name: " + model.name
            onClicked: {
                window.subtitle += "/" + model.name
                stackView.push("AccountEditor.qml")
                stackView.currentItem.create(model.cmName, model.name)
            }
        }
        ProtocolsModel {
            id: protocolsModel
        }
    }
}
