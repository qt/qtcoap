// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import CoapClientModule

Window {
    visible: true
    width: 480
    height: 480
    title: qsTr("Qt Quick CoAP Multicast Discovery")

    function addResource(resource) {
        resourceModel.insert(0, {"host" : resource.host,
                                 "path" : resource.path,
                                 "title" : resource.title})
    }

    CoapMulticastClient {
        id: client
        onDiscovered: addResource(resource)

        onFinished: {
            statusLabel.text = (error === QtCoap.Error.Ok)
                    ? qsTr("Finished resource discovery.")
                    : qsTr("Resource discovery failed with error code: %1").arg(error)
        }

        onError:
            statusLabel.text = qsTr("Resource discovery failed with error code: %1").arg(error)
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 10
        columns: 2

        Label {
            text: qsTr("Host:")
        }

        RowLayout {
            spacing: 5

            ComboBox {
                id: groupComboBox
                textRole: "group"
                Layout.preferredWidth: 210
                model: ListModel {
                    id: cbItems
                    ListElement {
                        group: qsTr("IPv4 CoAP Nodes")
                        address: "224.0.1.187"
                        value: QtCoap.MulticastGroup.AllCoapNodesIPv4
                    }
                    ListElement {
                        group: qsTr("IPv6 Link Local CoAP Nodes")
                        address: "ff02::fd"
                        value: QtCoap.MulticastGroup.AllCoapNodesIPv6LinkLocal
                    }
                    ListElement {
                        group: qsTr("IPv6 Site Local CoAP Nodes")
                        address: "ff05::fd"
                        value: QtCoap.MulticastGroup.AllCoapNodesIPv6SiteLocal
                    }
                    ListElement {
                        group: qsTr("Other")
                        address: ""
                        value: -1
                    }
                }

                delegate: ItemDelegate {
                    width: groupComboBox.width
                    contentItem: Column {
                        Text { text: group }
                        Text { text: address }
                    }
                    highlighted: groupComboBox.highlightedIndex === index
                }
            }

            TextField {
                id: customGroupField
                placeholderText: qsTr("<Custom Group>")
                enabled: groupComboBox.model.get(groupComboBox.currentIndex).value === -1
                Layout.fillWidth: true
            }
        }

        Label {
            text: qsTr("Port:")
        }

        TextField {
            id: portField
            text: "5683"
            placeholderText: qsTr("<Port>")
            inputMethodHints: Qt.ImhDigitsOnly
            Layout.preferredWidth: 80
        }

        Label {
            text: qsTr("Discovery Path:")
        }

        TextField {
            id: discoveryPathField
            text: "/.well-known/core"
            placeholderText: qsTr("<Resource Discovery Path>")
            inputMethodHints: Qt.ImhUrlCharactersOnly
            Layout.fillWidth: true
        }

        Button {
            id: discoverButton
            text: qsTr("Discover")
            Layout.columnSpan: 2

            onClicked: {
                var currentGroup = groupComboBox.model.get(groupComboBox.currentIndex).value;

                var path = "";
                if (currentGroup !== - 1) {
                    client.discover(currentGroup, parseInt(portField.text),
                                    discoveryPathField.text);
                    path = groupComboBox.currentText;
                } else {
                    client.discover(customGroupField.text, parseInt(portField.text),
                                    discoveryPathField.text);
                    path = customGroupField.text + discoveryPathField.text;
                }
                statusLabel.text = qsTr("Discovering resources at %1...").arg(path);
            }
        }

        ListModel {
            id: resourceModel
        }

        ListView {
            id: resourceView
            model: resourceModel
            clip: true
            Layout.columnSpan: 2
            Layout.fillHeight: true
            Layout.fillWidth: true

            delegate: Rectangle {
                width: resourceView.width
                height: 60
                color: "lightgray"
                border.color: "darkgray"
                radius: 5

                Column {
                    topPadding: 5
                    leftPadding: 5
                    Text { text: qsTr('<b>Host:</b> %1').arg(host) }
                    Text { text: qsTr('<b>Resource:</b> %1').arg(path) }
                    Text { text: qsTr('<b>Title:</b> %1').arg(title) }
                }
            }
        }
        Label {
            id: statusLabel
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }
    }
}
