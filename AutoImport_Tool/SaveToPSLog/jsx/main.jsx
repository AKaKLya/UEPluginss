function exportToPSLog() {
    try {
        // 验证文档
        if (!app.documents.length) {
            alert("请先打开文档");
            return;
        }
        
        // 获取文档信息
        var doc = app.activeDocument;
        var originalName = doc.name.replace(/\.[^\.]+$/, "");
        

        // 准备导出路径
        var exportFile = new File("D:/UEImport/" + originalName + ".png");
        if (exportFile.exists) exportFile.remove(); // 强制覆盖

        // 执行导出
        var options = new ExportOptionsSaveForWeb();
        options.format = SaveDocumentType.PNG;
        options.PNG8 = false;
        options.transparency = true;
        
        doc.exportDocument(exportFile, ExportType.SAVEFORWEB, options);
        
        // 结果验证
        if (exportFile.exists) {
            alert("导出成功:\n" + originalName);
        } else {
            throw new Error("文件未生成");
        }
        
    } catch(e) {
        alert("错误详情:\n" 
            + "消息: " + e.message + "\n"
            + "行号: " + e.line + "\n"
            + "堆栈: " + e.stack);
    }
}
