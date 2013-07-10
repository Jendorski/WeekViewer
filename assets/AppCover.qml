import bb.cascades 1.0

Container {
    id: mainScene
    layout: DockLayout {}
    background: Color.Black

    Label {
    	text: mainScene.getDate()
        textStyle.color: Color.create("#FF0000")
        textStyle.fontSize: FontSize.PointValue
        textStyle.fontSizeValue: 12
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
    }
            
    function getDate() {
        var currentTime = new Date()
        var month = currentTime.getMonth() + 1
        var day = currentTime.getDate()
        var year = currentTime.getFullYear()
        if (month < 10) month = "0"+month
        if (day < 10) day = "0"+day
        return day +"." +month+"."+year
    }
}
