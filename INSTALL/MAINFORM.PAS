unit mainform;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Dialogs,
  ExtCtrls, StdCtrls, Buttons, Forms, choicdir, srcdrive, DdeMan, choigrup,
  ComCtrls;

var
	IsWin95 :  Boolean;       	{ TRUE, если Windows 95 }
	CurStep :  Byte;            { Текущий шаг программы }
const
	COUNTDISKS = 3;				{ Количество установочных дисков }
type
								{ Класс формы главного окна }
  TMainFormInstall = class(TForm)
	Image         : TImage;
	BitBtnExit    : TBitBtn;
	BitBtnOk      : TBitBtn;
	BitBtnHelp    : TBitBtn;
	Panel1        : TPanel;
	LabelTitle1	  : TLabel;
	PathForInstall: TEdit;
	BitBtnBrowse  : TBitBtn;
	TitlePathInst : TLabel;
    PanelStatusBar: TPanel;
    LabelStatusBar: TLabel;
    Label1: TLabel;
	procedure BitBtnExitClick  	(Sender: TObject);
	procedure BitBtnBrowseClick	(Sender: TObject);
	procedure FormClose        	(Sender: TObject; var Action: TCloseAction);
	procedure BitBtnOkClick    	(Sender: TObject);
	procedure FormActivate     	(Sender: TObject);
	procedure FormCreate		(Sender: TObject);
	procedure PathForInstallEnter(Sender: TObject);
    procedure KillFocus(Sender: TObject);
    procedure BitBtnBrowseEnter(Sender: TObject);
    procedure BitBtnOkEnter(Sender: TObject);
    procedure BitBtnExitEnter(Sender: TObject);
    procedure BitBtnHelpEnter(Sender: TObject);
  private
	{ Private declarations }
  public
	{ Public declarations }
	InstPath      	: string;			{ Установочный путь }
	SrcDrive		: string;			{ Устройство, с которого идет установка }
	NumDisk			: word;				{ Номер установочного диска}
  end;

var
	MainFormInstall: TMainFormInstall;

implementation

{$R *.DFM}

{ Нажатие клавиши "Закончить" }
procedure TMainFormInstall.BitBtnExitClick(Sender: TObject);
begin
	Close;
end;

{ Нажатие клавиши "Обзор" }
{ Выбор каталога установки пакета }
procedure TMainFormInstall.BitBtnBrowseClick(Sender: TObject);
begin
	ChoiceDirectoryForm.ShowModal;
end;

{ Нажатие клавиши "Продолжить" }
{ Продолжение установки пакета - копирование файлов }
procedure TMainFormInstall.BitBtnOkClick(Sender: TObject);
begin	{ Циклический запуск установки дистрибутива по дискам }
(*	NumDisk := 1;
	while (NumDisk <> COUNTDISKS + 1) and (SetSourceDriveForm.Ready <> -1) do
	begin
		SetSourceDriveForm.ShowModal;{ Отображение диалога }
		if SetSourceDriveForm.Ready = 1 then
			NumDisk := NumDisk + 1;	 { Номер установочного диска }
	end;
*)	ChoiceGroupForm.ShowModal;       { Выбор группы }
end;

{ Закрытие главного окна - конец сеанса }
procedure TMainFormInstall.FormClose(Sender: TObject;
									 var Action: TCloseAction);
begin
	if NumDisk <= COUNTDISKS then
	begin 	{ Если установили не все диски - прервали установку }
			{ Отменим произведенные ранее действия }
(*	  case CurStep of
		1 : DoDeleteFiles;
		2 : DoKillGroup;
		3 : begin
				if IsWin95 then DoCleanReg else DoCleanINI;
			end;
	  end;	{ case }
*)	end;   	{ begin if }
	Action := caFree;
end;

{ Окончательный вариант каталога установки при активации формы }
procedure TMainFormInstall.FormActivate(Sender: TObject);
begin		{ Запомним установочных путь }
	 InstPath := PathForInstall.Text;
	 SetSourceDriveForm.Ready := 0;
end;

{ Создание главной формы }
procedure TMainFormInstall.FormCreate(Sender: TObject);
begin       { Определим версию Microsoft Windows }
	IsWin95 := False;
	If Hi(LoWord(GetVersion)) = 95 Then IsWin95 := True;
end;

procedure TMainFormInstall.PathForInstallEnter(Sender: TObject);
begin
	LabelStatusBar.Caption := PathForInstall.Hint;
end;

procedure TMainFormInstall.BitBtnBrowseEnter(Sender: TObject);
begin
	LabelStatusBar.Caption := BitBtnBrowse.Hint;
end;

procedure TMainFormInstall.KillFocus(Sender: TObject);
begin
	LabelStatusBar.Caption := '';
end;

procedure TMainFormInstall.BitBtnOkEnter(Sender: TObject);
begin
	LabelStatusBar.Caption := BitBtnOk.Hint;
end;

procedure TMainFormInstall.BitBtnExitEnter(Sender: TObject);
begin
	LabelStatusBar.Caption := BitBtnExit.Hint;
end;

procedure TMainFormInstall.BitBtnHelpEnter(Sender: TObject);
begin
	LabelStatusBar.Caption := BitBtnHelp.Hint;
end;

end.
