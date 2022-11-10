// Copyright (C) 2019 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform

Item {
    id: filePicker

    property string dialogText
    property alias selectedFile: filePathField.text

    height: addFileButton.height

    FileDialog {
        id: fileDialog
        title: qsTr("Please Choose %1").arg(dialogText)
        folder: StandardPaths.writableLocation(StandardPaths.HomeLocation)
        fileMode: FileDialog.OpenFile
        onAccepted: filePathField.text = fileDialog.file
    }

    RowLayout {
        anchors.fill: parent
        TextField {
            id: filePathField
            placeholderText: qsTr("<%1>").arg(dialogText)
            inputMethodHints: Qt.ImhUrlCharactersOnly
            selectByMouse: true
            Layout.fillWidth: true
        }

        Button {
            id: addFileButton
            text: qsTr("Add %1").arg(dialogText)
            onClicked: fileDialog.open()
        }
    }
}
