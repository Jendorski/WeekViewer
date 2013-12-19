import bb.cascades 1.0

Page {
    property alias forgroundcolor: labelid.textStyle.color
    property alias backgroundcolor: mainScene.background 
Container {

    id: mainScene
    layout: DockLayout {}
    background: Color.Black

    Label {
        id: labelid
        objectName: "TheLabel"
        text: ""
        textStyle.color: Color.create("#FF0000")
        textStyle.fontSize: FontSize.PointValue
        textStyle.fontSizeValue: 12
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
    }
}

}