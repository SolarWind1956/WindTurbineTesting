{ Диалог выбора группы установки пакета }
unit choigrup;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls, FileCtrl, Buttons, DdeMan;

type
  TChoiceGroupForm = class(TForm)
	Label1            	: TLabel;
	Panel1            	: TPanel;
	BitBtnChoice      	: TBitBtn;
	BitBtnCancel      	: TBitBtn;
	BitBtn3           	: TBitBtn;
	ListBoxGroupsNames: TListBox;
    EditGroupName: TEdit;
	DdeClientItem: TDdeClientItem;
	DdeClientConv: TDdeClientConv;
	procedure BitBtnCancelClick(Sender: TObject);
	procedure FormClose(Sender: TObject; var Action: TCloseAction);
	procedure BitBtnChoiceClick(Sender: TObject);
	procedure FormActivate(Sender: TObject);
    procedure ListBoxGroupsNamesClick(Sender: TObject);
  private
	{ Private declarations }
  public
	{ Public declarations }
  end;

var
  ChoiceGroupForm: TChoiceGroupForm;

implementation

uses mainform;

{$R *.DFM}


{ Нажатие клавиши "Отменить" }
procedure TChoiceGroupForm.BitBtnCancelClick(Sender: TObject);
begin
	Close;
end;

{ Закроем окно диалога }
procedure TChoiceGroupForm.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
	Action := caHide;
end;

{ Нажатие клавиши "Выбрать" }
procedure TChoiceGroupForm.BitBtnChoiceClick(Sender: TObject);
var
	Cmd : array[0..255] of Char;
begin							// Формируем команду для 'Execute'
								// Создаем новую группу
	if (ListBoxGroupsNames.SelCount = -1) then
	begin
		StrPCopy (Cmd, Format('[CreateGroup(%s)]', [EditGroupName.Text]) + #13#10);
		DDEClientConv.OpenLink;
		DdeClientConv.ExecuteMacro(Cmd, False); // Создадим группу
		DDEClientConv.CloseLink;
	end;
								// Создаем икону главного модуля пакета
	StrPCopy (Cmd, Format('[AddItem(%s, %s, %s, %s)]',
			  [MainFormInstall.InstPath + 'TestSyst.exe', 'Test System',
			   MainFormInstall.InstPath + 'TestSyst.exe', '0']) + #13#10);
	DDEClientConv.OpenLink;	   	// Откроем связь с Сервером
	DdeClientConv.ExecuteMacro(Cmd, False); // Создадим икону
	DDEClientConv.CloseLink;   	// Закроем связь с Сервером

								// Создаем икону модуля испытаний
	StrPCopy (Cmd, Format('[AddItem(%s, %s, %s, %s)]',
			[MainFormInstall.InstPath + 'PowerCha.exe', 'Power Test',
			 MainFormInstall.InstPath + 'PowerCha.exe', '0']) + #13#10);
	DDEClientConv.OpenLink;	   	// Откроем связь с Сервером
	DdeClientConv.ExecuteMacro(Cmd, False); // Создадим икону
	DDEClientConv.CloseLink;   	// Закроем связь с Сервером

								// Покажем группу
	StrPCopy (Cmd, Format('[ShowGroup(%s %s)]', [EditGroupName.Text, '7']));
	DDEClientConv.OpenLink;
	DdeClientConv.ExecuteMacro(Cmd, False); // Показываем группу
	DDEClientConv.CloseLink;

	Close;						// Закроем окно диалога
end;

procedure TChoiceGroupForm.FormActivate(Sender: TObject);
var
	DDE_Data_Pointer : PChar;	// Указатель на буфер данных DDEML
begin
	DDE_Data_Pointer := DDEClientConv.RequestData('Groups');
	ListBoxGroupsNames.Items.SetText(DDE_Data_Pointer);	//
	StrDispose(DDE_Data_Pointer);		// ?????
end;
{	При выборе одной из существующих групп заносим ее имя в окно }
procedure TChoiceGroupForm.ListBoxGroupsNamesClick(Sender: TObject);
begin
	EditGroupName.Text := ListBoxGroupsNames.Items.Strings[ListBoxGroupsNames.ItemIndex];
end;

end.

