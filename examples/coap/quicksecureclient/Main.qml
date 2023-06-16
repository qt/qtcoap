// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

Window {
    visible: true
    width: 480
    height: 640
    title: qsTr("Qt Quick Secure CoAP Client")

    CoapSecureClient {
        id: client
        onFinished: {
            outputView.text = result;
            statusLabel.text = "";
            disconnectButton.enabled = true;
        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 10
        columns: 2

        Label {
            text: qsTr("Host:")
        }
        ComboBox {
            id: hostComboBox
            editable: true
            model: hostsModel
            Layout.fillWidth: true
        }

        Label {
            text: qsTr("Port:")
        }
        TextField {
            id: portField
            text: "5684"
            placeholderText: qsTr("<Port>")
            inputMethodHints: Qt.ImhDigitsOnly
            Layout.preferredWidth: 80
        }

        Label {
            text: qsTr("Resource:")
        }
        TextField {
            id: resourceField
            placeholderText: qsTr("<Resource Path>")
            inputMethodHints: Qt.ImhUrlCharactersOnly
            selectByMouse: true
            Layout.fillWidth: true
        }

        Label {
            text: qsTr("Security Mode:")
        }
        ButtonGroup {
            id: securityModeGroup
            onClicked: {
                if (securityModeGroup.checkedButton === preSharedMode)
                    client.setSecurityMode(QtCoap.SecurityMode.PreSharedKey);
                else
                    client.setSecurityMode(QtCoap.SecurityMode.Certificate);
            }
        }
        RowLayout {
            RadioButton {
                id: preSharedMode
                text: qsTr("Pre-shared Key")
                ButtonGroup.group: securityModeGroup
            }
            RadioButton {
                id: certificateMode
                text: qsTr("X.509 Certificate")
                ButtonGroup.group: securityModeGroup
            }
        }

        RowLayout {
            enabled: securityModeGroup.checkedButton === preSharedMode
            Layout.columnSpan: 2

            Label {
                text: qsTr("Key")
            }
            TextField {
                id: pskField
                placeholderText: qsTr("<Pre-shared Key>")
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Identity")
            }
            TextField {
                id: identityField
                placeholderText: qsTr("<Identity>")
                Layout.fillWidth: true
            }
        }

        FilePicker {
            id: localCertificatePicker
            dialogText: "Local Certificate"
            enabled: securityModeGroup.checkedButton === certificateMode
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        FilePicker {
            id: caCertificatePicker
            dialogText: "CA Certificate"
            enabled: securityModeGroup.checkedButton === certificateMode
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        FilePicker {
            id: privateKeyPicker
            dialogText: "Private Key"
            enabled: securityModeGroup.checkedButton === certificateMode
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        Button {
            id: requestButton
            text: qsTr("Send Request")
            enabled: securityModeGroup.checkState !== Qt.Unchecked

            onClicked: {
                outputView.text = "";
                if (securityModeGroup.checkedButton === preSharedMode)
                    client.setSecurityConfiguration(pskField.text, identityField.text);
                else
                    client.setSecurityConfiguration(localCertificatePicker.selectedFile,
                                                    caCertificatePicker.selectedFile,
                                                    privateKeyPicker.selectedFile);

                client.sendGetRequest(hostComboBox.editText, resourceField.text,
                                      parseInt(portField.text));

                statusLabel.text = qsTr("Sending request to %1%2...").arg(hostComboBox.editText)
                                                                     .arg(resourceField.text);
            }
        }

        Button {
            id: disconnectButton
            text: qsTr("Disconnect")
            enabled: false

            onClicked: {
                client.disconnect();
                statusLabel.text = qsTr("Disconnected.");
                outputView.text = "";
                disconnectButton.enabled = false;
            }
        }

        TextArea {
            id: outputView
            placeholderText: qsTr("<Client Output>")
            background: Rectangle {
                border.color: "gray"
            }
            Layout.columnSpan: 2
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
        Label {
            id: statusLabel
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }
    }
}
