/*
 * Copyright (C) 2016 Jens Drescher, Germany
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

Page
{
    allowedOrientations: Orientation.All
    property int iCountWhtRbbt: 1;

    SilicaFlickable
    {
        anchors.fill: parent
        contentHeight: id_Column_Main.height

        VerticalScrollDecorator {}

        Column
        {
            id: id_Column_Main

            anchors.top: parent.top
            width: parent.width

            PageHeader
            {
                title: qsTr("About OBDFish")
            }
            MouseArea
            {
                width : parent.width /1.8
                height: parent.width / 1.8
                anchors.horizontalCenter: parent.horizontalCenter
                Image
                {
                    anchors.fill: parent
                    source: "../obdfish.png"
                }
                onClicked:
                {
                    if (iCountWhtRbbt == 1)
                        fncShowMessage(1,"First iteration: At the earliest drawing of the fractal curve, few clues to the underlying mathematical structure will be seen.<br>Ian Malcolm", 6000);
                    else if (iCountWhtRbbt == 2)
                        fncShowMessage(1,"Second iteration: With subsequent drawings of the fractal curve, sudden changes may appear.<br>Ian Malcolm", 6000);
                    else if (iCountWhtRbbt == 3)
                        fncShowMessage(1,"Third iteration: Details emerge more clearly as the fractal curve is redrawn.<br>Ian Malcolm", 6000);
                    else if (iCountWhtRbbt == 4)
                        fncShowMessage(1,"Fourth iteration: Inevitably, underlying instabilities begin to appear.<br>Ian Malcolm", 6000);
                    else if (iCountWhtRbbt == 5)
                        fncShowMessage(1,"Fifth iteration: Flaws in the system will now become severe.<br>Ian Malcolm", 6000);
                    else if (iCountWhtRbbt == 6)
                        fncShowMessage(1,"Sixth iteration: System recovery may prove impossible.<br>Ian Malcolm", 6000);
                    else if (iCountWhtRbbt == 7)
                        fncShowMessage(1,"Seventh iteration: Increasingly, the mathematics will demand the courage to face its implications.<br>Ian Malcolm", 6000);
                    else if (iCountWhtRbbt == 8)
                        fncShowMessage(3,"STOP NOW or system will crash!!!", 6000);
                    else if (iCountWhtRbbt == 9)
                        fncShowMessage(0,"loading whte_rbt.obj to sailfish device, please wait...", 3000);
                    else if (iCountWhtRbbt == 10)
                    {
                        fncShowMessage(4,"Developed by Integrated Computer Systems, Inc. Cambridge Mass<br>Project Supervisor: Dennis Nedry<br>Chief Programmer: Mike Backes<br>\u00A9 Jurassic Parc Inc. All Rights Reserved", 16000);
                        iCountWhtRbbt = 0;
                    }

                    iCountWhtRbbt++;
                }
            }

            Item
            {
                width: parent.width
                height: Theme.paddingLarge
            }
            Label
            {
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: Theme.fontSizeExtraSmall
                wrapMode: Text.WordWrap
                maximumLineCount: 2
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("OBD ELM327 car diagnostic reader application for Sailfish OS")
            }
            Item
            {
                width: parent.width
                height: Theme.paddingLarge
            }
            Label
            {
                width: parent.width - 2 * Theme.horizontalPageMargin
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                text: "Copyright © 2018 Jens Drescher, Germany"
            }

            SectionHeader { text: qsTr("Contributions by") }

            Repeater
            {
                model: [
                    { nick: "deepbluev7", name: "", role: "BluezQt port" },
                    { nick: "poetaster", name: "Mark Washeim", role: "Packaging and Sailjail" },
                    { nick: "ExPLIT", name: "Pawel Radomychelski", role: "Sailfish OS 5 fixes" },
                    { nick: "kan-ibal", name: "", role: "Socket lifecycle research" },
                    { nick: "kimmoli", name: "Kimmo Lindholm", role: "Messagebox" },
                    { nick: "cfr34k", name: "Thomas Kolb", role: "Plotwidget" },
                    { nick: "lainwir3d", name: "Richard Rondu", role: "QtConnectivity backport" }
                ]

                delegate: Column
                {
                    width: id_Column_Main.width - 2 * Theme.horizontalPageMargin
                    x: Theme.horizontalPageMargin
                    spacing: Theme.paddingSmall / 2

                    Label
                    {
                        width: parent.width
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.primaryColor
                        wrapMode: Text.WordWrap
                        text: modelData.nick + (modelData.name ? " (" + modelData.name + ")" : "")
                    }
                    Label
                    {
                        width: parent.width
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: Theme.fontSizeExtraSmall
                        color: Theme.secondaryColor
                        wrapMode: Text.WordWrap
                        text: modelData.role
                    }
                    Item { width: parent.width; height: Theme.paddingSmall }
                }
            }

            Item { width: parent.width; height: Theme.paddingMedium }

            Label
            {
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                text: "Version: " + Qt.application.version + "  |  GPLv3"
            }
            Item
            {
                width: parent.width
                height: Theme.paddingLarge
            }
            Label
            {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                text: qsTr("Source code:")
            }
            Label
            {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: Theme.fontSizeExtraSmall
                width: parent.width - 2 * Theme.horizontalPageMargin
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WrapAnywhere
                textFormat: Text.RichText
                property string urlstring: "https://github.com/explit28/OBDFish"
                text: "<a href=\"" + urlstring + "\">" +  urlstring + "</a>"
                onLinkActivated: Qt.openUrlExternally(link)
            }
            Item
            {
                width: parent.width
                height: Theme.paddingLarge
            }
            Label
            {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                text: qsTr("Feedback, bugs:")
            }
            Label
            {
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: Theme.fontSizeExtraSmall
                width: parent.width - 2 * Theme.horizontalPageMargin
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WrapAnywhere
                textFormat: Text.RichText
                property string urlstring: "https://github.com/explit28/OBDFish/issues"
                text: "<a href=\"" + urlstring + "\">" +  urlstring + "</a>"
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }
}


