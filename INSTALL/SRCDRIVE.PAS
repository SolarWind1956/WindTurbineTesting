{ Диалог определения установочного диска и копирования файлов }
unit srcdrive;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls, ComCtrls, Buttons, FmxUtils;

type
  TSetSourceDriveForm = class(TForm)
	BitBtnHelp		: TBitBtn;
	BitBtnOK		: TBitBtn;
	BitBtnCancel	: TBitBtn;
	EditSourceDrive : TEdit;
	Bevel1			: TBevel;
	ProgressBar		: TProgressBar;
    MessageString: TLabel;
    MessageNumDisk: TLabel;
	procedure FormActivate		(Sender: TObject);
	procedure BitBtnCancelClick	(Sender: TObject);
	procedure FormClose			(Sender: TObject; var Action: TCloseAction);
	procedure BitBtnOKClick		(Sender: TObject);
  private
	{ Private declarations }
  public
	{ Public declarations }
	  Ready  : Integer; { Флаг окончания копирования дискеты }
  end;

var
  SetSourceDriveForm: TSetSourceDriveForm;

implementation

uses mainform;

{$R *.DFM}

{ Активация формы }
procedure TSetSourceDriveForm.FormActivate(Sender: TObject);
var
	sNumDisk : string[2];
begin
	Ready := 0; 	                // Сбросим флаг окончания копирования
	MainFormInstall.SrcDrive := EditSourceDrive.Text;
	Str(MainFormInstall.NumDisk, sNumDisk);
									// Очистим строку сообщения о копировании файла
	MessageString.Caption   := '';
	MessageNumDisk.RePaint;
									// Попросим следующий диск
	MessageNumDisk.Caption  := 'Установите Диск ' + sNumDisk + ' в устройство';
	MessageNumDisk.RePaint;
end;

{ Нажатие кнопки "Отменить" для прекращения установки пакета }
procedure TSetSourceDriveForm.BitBtnCancelClick(Sender: TObject);
begin
	if MessageDlg('Прервать установку пакета ?', mtInformation,
				 [mbYes, mbNo], 0) = mrYes then
	begin
		Ready := -1;	// Флаг отмены установки
		Close;
	end;
end;

{ Закрытие окна }
procedure TSetSourceDriveForm.FormClose(Sender: TObject;
										  var Action: TCloseAction);
begin
	Action := caHide;
end;

{ Нажатие кнопки "Прололжить" }
procedure TSetSourceDriveForm.BitBtnOKClick(Sender: TObject);
var
	SearchRec 	: TSearchRec;	// структура-информация файла
	FilesCount 	: Integer;		// количество файлов на диске
	Result      : Integer;		// результат поиска файлов
	FileFrom,                   // Диск + путь + Имя исходного файла
	FileTo		: string;		// Диск + путь + Имя получаемого файла
	sNumDisk	: string[2];
Begin
	str(MainFormInstall.NumDisk, sNumDisk);
						// Получим путь, с которого устанавливаем пакет
	MainFormInstall.SrcDrive := EditSourceDrive.Text;
	if FindFirst(MainFormInstall.SrcDrive+'disk'+sNumDisk,
			faArchive, SearchRec) = 0 then
	begin
	   FilesCount := 1;			// первичная инициализация кол-ва файлов
	   { Подсчитаем число файлов на дискете }
	   Result     := FindFirst(MainFormInstall.SrcDrive+'*.*',
							   faArchive, SearchRec);
	   while Result = 0 do
		   begin
			   Result      := FindNext(SearchRec);
			   FilesCount  := FilesCount + 1;
		   end; { while }
	   FindClose(SearchRec);
	   { и установим максимальный  размер индикатора }
	   ProgressBar.Max := FilesCount;

	   ProgressBar.Position := 1;		// Начальная порция полоски
	   Result   := FindFirst(MainFormInstall.SrcDrive+'*.*', faArchive, SearchRec);
	   while Result = 0 do             	// Непосредственно копирование
		   begin                       	// файлов
			   FileFrom := MainFormInstall.SrcDrive + SearchRec.Name;
			   FileTo	:= MainFormInstall.InstPath;
										// Сообщаем имя копируемого файла
			   MessageString.Caption := 'Копирование ' + SearchRec.Name + ' ...';
			   MessageString.RePaint;
			   CopyFile(FileFrom, FileTo);
			   ProgressBar.Position := ProgressBar.Position + 1;
			   Result   := FindNext(SearchRec);
		   end; { while }
	   ProgressBar.Position := 0;		// Сбросим полоски
	   Ready := 1;                      // взведем флаг успешного копирования
	end;		{ if }
End;

end.
