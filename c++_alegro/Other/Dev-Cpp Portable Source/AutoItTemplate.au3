#include <Array.au3>

Dim $DefaultExeName;
Dim $DefaultAppDir;
Dim $DefaultDataDir;
Dim $DefaultAditionalParameters = "";
Dim $DefaultDisableSplashScreen = "False";

Dim $ApplicationName;
Dim $ApplicationPortableName;
Dim $DirApplicationPortableName;

Const $ACT_NONE = -2;
Const $ACT_REGISTRY_EXPORT = 0;
Const $ACT_REGISTRY_IMPORT = 1;
Const $ACT_FOLDER_COPY = 2;
Const $ACT_FOLDER_COPYBACK = 3;
Const $ACT_FILE_COPY = 4;
Const $ACT_FILE_COPYBACK = 5;

Dim $__actionList[1];
Dim $__localList[1];
Dim $__backupList[1];
Dim $__empty = True;

Func Stack_Add($actID, $local, $backup)
	If $__empty Then
		$__actionList[0] = $actID;
		$__localList[0] = $local;
		$__backupList[0] = $backup;
		$__empty = False;
		Return
	EndIf
	
	$__actionList = _ArrayAdd($__actionList, $actID);
	$__localList = _ArrayAdd($__localList, $local);
	$__backupList = _ArrayAdd($__backupList, $backup);
	
EndFunc

Func Stack_Exec()
	If Not UBound($__actionList) == UBound($__localList) And UBound($__actionList) == UBound($__backupList) Then
		Return
	EndIf
	
	Dim $i = 0;
	Dim $length = UBound($__actionList);
	While $i < $length
		
		ExecuteAction($__actionList[$i], $__localList[$i], $__backupList[$i]);
		
		$i += 1;
	WEnd
EndFunc

Func Stack_Exec_Reverse()
	If Not UBound($__actionList) == UBound($__localList) And UBound($__actionList) == UBound($__backupList) Then
		Return
	EndIf
	
	Dim $i = UBound($__actionList) - 1;
	While $i >= 0
		ExecuteAction($__actionList[$i] + 1, $__localList[$i], $__backupList[$i]);
	WEnd
	
EndFunc

Func SetAppName($name)
	$ApplicationName = $name;
	$ApplicationPortableName = $name & " Portable";
	$DirApplicationPortableName = $name & "Portable";
EndFunc

Func ReadIniFile($iniFile, $iniSection, ByRef $exeName, ByRef $appDir, ByRef $dataDir, ByRef $aditionalParameters, ByRef $disableSpashScreen)
	
	$exeName = IniRead($iniFile, $iniSection, "AppExecutable", $DefaultExeName);
	$appDir = IniRead($iniFile, $iniSection, "AppDirectory", $DefaultAppDir);
	$dataDir = IniRead($iniFile, $iniSection, "DataDirectory", $DefaultDataDir);
	$aditionalParameters = IniRead($iniFile, $iniSection, "AditionalParameters", $DefaultAditionalParameters);
	
	Dim $disableBool = IniRead($iniFile, $iniSection, "DisableSplashScreen", $DefaultDisableSplashScreen);
	
	If $disableBool == "true" Or $disableBool == "True" Then
		$disableSpashScreen = True;
	Else
		$disableSpashScreen = False;
	EndIf
	
EndFunc

Func RequireNotRunning($exeName)
	
	If ProcessExists($exeName) Then
		MsgBox(48, $ApplicationPortableName, $ApplicationName & " could not be started because it is already running.");
		Exit(1);
	EndIf
	
EndFunc

Func CheckExe($exeName, $appDir)
	
	If Not FileExists(@ScriptDir & "\" & $appDir & "\" & $exeName) Then
		MsgBox(48, "Error", $ApplicationName & " could not be started because """ & $exeName & """ could not be found.");
		Exit(1);
	EndIf
	
EndFunc

Func ExecuteAction($actID, $local, $backup)
	
	Dim $other;
	
	Select
		
		Case $actID == $ACT_REGISTRY_IMPORT
			ShellExecuteWait("regedit.exe", '/S "' & $backup & '"');
			
		Case $actID == $ACT_REGISTRY_EXPORT
			ShellExecuteWait("regedit.exe", '/E "' & $backup & '" "' & $local & '"');
			
		Case $actID == $ACT_FOLDER_COPYBACK
			DirCopy($local, $backup, 1);
			
		Case $actID == $ACT_FOLDER_COPY
			DirCopy($backup, $local, 1);
			
		Case $actID == $ACT_FILE_COPYBACK
			FileCopy($local, $backup, 1);
			
		Case $actID == $ACT_FILE_COPY
			FileCopy($backup, $local, 1);
			
	EndSelect
	
EndFunc

Func RunProgram($appDir, $exeName, $aditionalParameters, $passedParameters, $constantParameters = "")
	Return ShellExecuteWait($exeName, $aditionalParameters & " " & $passedParameters & " " & $constantParameters, $appDir);
EndFunc