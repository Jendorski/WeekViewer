import bb.cascades 1.0

Container {
    id: mainScene
    layout: DockLayout {}
    background: Color.Black

    Label {
        objectName: "TheLabel"
        text: ""
        textStyle.color: Color.create("#FF0000")
        textStyle.fontSize: FontSize.PointValue
        textStyle.fontSizeValue: 12
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
    }
}
