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
    width: 800
    height: 600

    function create(manager, protocol) {
        console.log("Create acc", manager, protocol)
        accountParameterModel.newAccount(manager, protocol)
    }

    function setAccount(account) {
        console.log("Set account", account)
        accountParameterModel.setAccount(account)
    }

    signal createAccount(string manager, string protocol, string displayName, variant parameters)

    Column {
        id: internalCol
        width: page.width
        height: submitButton.height + parametersDetailView.count * 50

        Item {
            id: displayNameEditor2
            height: 50
            width: 300
            Row {
                spacing: 4
                Label {
                    text: qsTr("DisplayName")
                    width: 200
                    height: 40
                }
                TextField {
                    id: displayNameEditor
                    width: 200
                    height: 40
                    text: accountParameterModel.displayName
                }
            }
        }

        Repeater {
            id: parametersDetailView
            model: accountParameterModel

            delegate: Item {
                id: delegate
                height: 50
                width: page.width
                Row {
                    spacing: 4
                    Label {
                        text: model.name
                        width: 200
                        height: 40
                    }
                    Loader {
                        id: paramEditor
                        property var v: model.value
                        sourceComponent: getComponentForType(model.signature)
                        width: 200
                        height: 40
                        onLoaded: item.value = v
                        onVChanged: item.value = v

                        function getComponentForType(t)
                        {
                            console.log("Get type:" + t)
                            if (t === "u") {
                                return intDelegate
                            } else if (t === "b") {
                                return boolDelegate
                            } else if (t === "s") {
                                return textDelegate
                            }

                            console.log("Fallback to text delegate")

                            return textDelegate
                        }
                    }
                }
                Connections {
                    target: paramEditor.item
                    onSubmit: {
                        console.log("Submit param", model.index, value);
                        accountParameterModel.setData(model.index, value)
                    }
                }
            }
            onCountChanged: {
                console.log("ParamsCount:", count)
            }
        }

        AccountParameterModel {
            id: accountParameterModel
        }

        Component {
            id: intDelegate
            SpinBox {
                signal submit(var value)
                width: 40
                height: 40
                onValueChanged: submit(value)
            }
        }
        Component {
            id: textDelegate
            TextField {
                signal submit(var value)
                property string value: ""
                width: 40
                height: 40
                text: value
                onTextChanged: submit(text)
            }
        }
        Component {
            id: boolDelegate
            CheckBox {
                signal submit(var value)
                property var value
                width: 40
                height: 40
                checked: value
                onTextChanged: submit(checked)
            }
        }
    }

    footer: Button {
        id: submitButton
        text: qsTr("Create account")
        onClicked: {
            console.log("Create account")
            page.createAccount(accountParameterModel.manager, accountParameterModel.protocol, displayNameEditor.text, accountParameterModel.getVariantMap())
        }
    }

}
