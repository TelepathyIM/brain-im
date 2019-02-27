import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.3
import Qt.labs.settings 1.0

import BrainIM 0.1

import "pages"

ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 535

    title: "Brain IM"

    //property alias subtitle: subtitleLabel.text
    property string subtitle

    Material.theme: theme

    property int theme: Material.Light

    header: ToolBar {
        ToolButton {
            text: qsTr("<")
            onClicked: {
                stackView.pop()
            }
            visible: stackView.depth > 1
            anchors.left: parent.left
        }
        Label {
            id: subtitleLabel
            elide: Label.ElideRight
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: stackView.currentItem.title
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: IndexPage { }
    }

    Connections {
        target: stackView.currentItem
        ignoreUnknownSignals: true
        onCreateAccount: {
            accountsModel.createAccount(manager, protocol, displayName, parameters)
            stackView.pop(stackView.find(function(item) {
                return item.objectName === "account-management";
            }));
        }
    }

    AccountsModel {
        id: accountsModel
    }

    Shortcut {
        sequences: ["Esc", "Back"]
        enabled: stackView.depth > 1
        onActivated: {
            stackView.pop()
            listView.currentIndex = -1
        }
    }

    Shortcut {
        sequence: "Menu"
        onActivated: optionsMenu.open()
    }

    Shortcut {
        sequence: "F2"
        onActivated: {
            if (window.theme == Material.Light) {
                window.theme = Material.Dark
            } else {
                window.theme = Material.Light
            }
        }
    }

    Shortcut {
        sequence: StandardKey.Quit
        onActivated: window.close()
    }

    Settings {
        id: uiSettings

        category: "ui"

        property int mainBlockWidth

        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height
    }
}
