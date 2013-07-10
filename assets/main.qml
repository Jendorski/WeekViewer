// List with a context menu project template
import bb.cascades 1.0
import bb.mymodel 1.0

Page {
    content: Container {
        layout: DockLayout {}
        ListView {
            id: listViewToday
            objectName: "listView"

            dataModel: _app.model
//                CustomGroupModel {
//                id: myListModel
////                _app.modelToday
//            }
            
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    Label {
                    	text : "<html><body><span style=\"color:#FF0000\">" + ListItemData + "</span></body></html>"
                    }
//                    Header {
//                        title: "<html><body><span style=\"color:#FF0000\">" + ListItemData + "</span></body></html>"
//                    }
                },
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        title: ListItemData.subject
                        description: ListItemData.timeString
                    }
                }
            ]
            onTriggered: {
                var curr_item = dataModel.data(indexPath)
                _app.loadEvent(curr_item.eventId, curr_item.account, curr_item.startTime)
            }
            
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1.0
            }
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

        }

        ImageButton {
            defaultImageSource: "asset:///images/ic_rotate.png"
            pressedImageSource: "asset:///images/ic_rotate.png"
            disabledImageSource: "asset:///images/ic_rotate.png"

            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            onClicked: {
                _app.loadEvents();
            }
        }

        ImageButton {
            defaultImageSource: "asset:///images/action_addevent.png"
            pressedImageSource: "asset:///images/action_addevent.png"
            disabledImageSource: "asset:///images/action_addevent.png"

			horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Bottom
            onClicked: {
                _app.openCalendar();
            }
        }
    }
    
    onCreationCompleted: {
        console.log("Page - onCreationCompleted()")

        // populate list view model with the sample data
        _app.loadEvents()
    }
    
}
