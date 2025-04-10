macroScript Macro2
	category:"DragAndDrop"
	toolTip:""
(
	objName = "F:\\UEImport\\" + selection[1].name
	exportFile objName #noPrompt selectedOnly:true using:fbxexp
)
