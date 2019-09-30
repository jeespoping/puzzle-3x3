#include "AutoItTemplate.au3"

#NoTrayIcon

$DefaultExeName = "devcpp.exe";
$DefaultAppDir = "App\devcpp";
$DefaultDataDir = "Data\";
$DefaultAditionalParameters = "";
$DefaultDisableSplashScreen = "False";

SetAppName("Dev-C++");

Dim $exeName;
Dim $appDir;
Dim $dataDir;
Dim $aditionalParams;
Dim $disableSplashScreen;

ReadIniFile("Dev-CppPortable.ini", "Dev-CppPortable", $exeName, $appDir, $dataDir, $aditionalParams, $disableSplashScreen);

If DirGetSize(@ScriptDir & "\" & $dataDir, 2) == -1 Then
	DirCreate(@ScriptDir & "\" & $dataDir);
EndIf

CheckExe($exeName, $appDir);

Init();

Dim $execStr = '"' & StringReplace(@ScriptDir & "\" & $appDir & "\" & $exeName, '"', '""') & '" -c "' & StringReplace(@ScriptDir & '\Data\', '"', '""') & '" ' & $aditionalParams;
Dim $proc = Run($execStr, @ScriptDir & "\" & $appDir);
If Not $disableSplashScreen Then
	DirCreate(@TempDir & "\Dev-CppPortable\");
	FileInstall("Dev-CppPortable.jpg", @TempDir & "\Dev-CppPortable\", 1);
	SplashImageOn("Dev-C++ Portable", @TempDir & "\Dev-CppPortable\Dev-CppPortable.jpg", 500, 233);
	WinActivate("Dev-C++ Portable");
	Sleep(1200);
	SplashOff();
	DirRemove(@TempDir & "\Dev-CppPortable", 1);
EndIf

While ProcessExists($proc)
	Sleep(1000);
WEnd

Clean();

Func Init()
	If Not FileExists(@ScriptDir & "\Data\devcpp.ini") Then
		Return;
	EndIf
	
	Local $text;
	Local $file = FileOpen(@ScriptDir & "\Data\devcpp.ini", 0);
	Local $path = @ScriptDir & "\App\devcpp";
	
	If $file == -1 Then
		MsgBox(16, "Dev-C++ Portable Launcher Error", "The data file could not be read.");
		Exit(1);
	EndIf;
	
	$text = FileRead($file);
	
	FileClose($file);
	
	$text = StringReplace($text, "%path%", $path);
	
	$file = FileOpen(@ScriptDir & "\Data\devcpp.ini", 2);

	If $file == -1 Then
		MsgBox(16, "Dev-C++ Portable Launcher Error", "The data file could not be saved.");
		Exit 1;
	EndIf;

	FileWrite($file, $text);

	FileClose($file);
EndFunc

Func Clean()
	Local $text;
	Local $file = FileOpen(@ScriptDir & "\Data\devcpp.ini", 0);
	Local $path = @ScriptDir & "\App\devcpp";

	If $file == -1 Then
		MsgBox(16, "Dev-C++ Portable Launcher Error", "The data file could not be read.");
		Exit 1;
	EndIf;

	$text = FileRead($file);

	FileClose($file);

	$text = StringReplace($text, $path, "%path%");

	$file = FileOpen(@ScriptDir & "\Data\devcpp.ini", 2);

	If $file == -1 Then
		MsgBox(16, "Dev-C++ Portable Launcher Error", "The data file could not be saved.");
		Exit(1);
	EndIf;

	FileWrite($file, $text);

	FileClose($file);
EndFunc