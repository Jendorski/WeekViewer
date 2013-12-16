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
            
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    Label {
                    	text : ListItemData
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                            fontWeight: if (ListItemData == "Heute") FontWeight.Bold; else FontWeight.Normal;
                            fontSize: if (ListItemData == "Heute") FontSize.Medium; else FontSize.Small;
                            color: Color.Red 
                        }
                        
                    }
                },
                ListItemComponent {
                        type: "item"
                        StandardListItem {
                        title: "<html><body><span style=\"color:#" + ListItemData.color24 + "\">o </span>" +  ListItemData.subject + "</body></html>"
//                        title: ListItemData.subject 
                        description: ListItemData.timeString
//                            imageSource: if (ListItemData.account == 1) "asset:///images/grey.png";
//                                        else if (ListItemData.account == 2) "asset:///images/blue.png";
//                                        else if (ListItemData.account == 3) "asset:///images/pink.png";
//                                        else if (ListItemData.account == 4) "asset:///images/green.png";
//                                        else if (ListItemData.account == 5) "asset:///images/darkgreen.png";
//                                        else if (ListItemData.account == 6) "asset:///images/braun.png"
                        }
//                    Container {
//                        layout: StackLayout {
//                            orientation: LayoutOrientation.TopToBottom
//                        }
//                        Container {
//                            layout: StackLayout {
//                                orientation: LayoutOrientation.LeftToRight
//                            }
//                            
////                            ImageView {
////                                imageSource: if (ListItemData.account == 1) "asset:///images/grey.png";
////                                			else if (ListItemData.account == 2) "asset:///images/blue.png";
////                                			else if (ListItemData.account == 3) "asset:///images/pink.png";
////                                			else if (ListItemData.account == 4) "asset:///images/green.png";
////                                			else if (ListItemData.account == 5) "asset:///images/darkgreen.png";
////                                			else if (ListItemData.account == 6) "asset:///images/braun.png"
////                            }
//                            Label {
//                                text: "  " + ListItemData.subject
//                                textStyle {
//                                    base: SystemDefaults.TextStyles.BodyText
//                                    fontWeight: FontWeight.Normal
//                                    color: if (ListItemData.account == 1) Color.Magenta; else Color.Blue
//                                }
//
//                            }
//                        }
//                        Label {
//                            text: "  " + ListItemData.timeString
//                            textStyle {
//                                base: SystemDefaults.TextStyles.SmallText
//                            }
//                        }
//                    } // end of Container
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
