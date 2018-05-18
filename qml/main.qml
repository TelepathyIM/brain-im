import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import BrainIM 0.1
import TelepathyQt 0.1

import "components"
import "pages"

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600

    title: "Brain IM"

    property alias subtitle: subtitleLabel.text

    header: ToolBar {
        ToolButton {
            text: qsTr("<")
            onClicked: stackView.pop()
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
        initialItem: IndexPage {

        }
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
        sequence: StandardKey.Quit
        onActivated: window.close()
    }
}
