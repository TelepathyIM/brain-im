import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import BrainIM 0.1
import TelepathyQt 0.1

import "components"

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600

    title: "Brain IM"

    property alias subtitle: subtitleLabel.text

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            Label {
                id: subtitleLabel
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }
        }
    }

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: Pane {
            id: pane

            ListView {
                id: listView

                focus: true
                currentIndex: -1
                anchors.fill: parent

                delegate: ItemDelegate {
                    width: parent.width
                    text: model.title
                    highlighted: ListView.isCurrentItem
                    onClicked: {
                        window.subtitle = model.title
                        stackView.push(model.source)
                    }
                }

                model: ListModel {
                    ListElement { title: "Account management"; source: "pages/AccountManagement.qml" }
                }

                ScrollIndicator.vertical: ScrollIndicator { }
            }
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
