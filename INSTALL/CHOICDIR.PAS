{ ?????? ??????????? ???? ????????? ?????? }
unit choicdir;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls, FileCtrl, Buttons;

type
  TChoiceDirectoryForm = class(TForm)
    DirectoryListBox  : TDirectoryListBox;
    DriveComboBox     : TDriveComboBox;
    Label1            : TLabel;
    Panel1            : TPanel;
	BitBtnChoice      : TBitBtn;
	BitBtnCancel      : TBitBtn;
	BitBtn3           : TBitBtn;
	procedure DriveComboBoxChange(Sender: TObject);
	procedure BitBtnCancelClick(Sender: TObject);
	procedure FormClose(Sender: TObject; var Action: TCloseAction);
	procedure BitBtnChoiceClick(Sender: TObject);
  private
	{ Private declarations }
  public
	{ Public declarations }
  end;

var
  ChoiceDirectoryForm: TChoiceDirectoryForm;

implementation

uses mainform;

{$R *.DFM}

{ ???????? ???? }
procedure TChoiceDirectoryForm.DriveComboBoxChange(Sender: TObject);
begin
	 DirectoryListBox.Drive := DriveComboBox.Drive;
end;

{ ??????? ??????? "????????" }
procedure TChoiceDirectoryForm.BitBtnCancelClick(Sender: TObject);
begin
	 Close;
end;

{ ??????? ???? ??????? }
procedure TChoiceDirectoryForm.FormClose(Sender: TObject;
  var Action: TCloseAction);
begin
	 Action := caHide;
end;

{ ??????? ??????? "???????" }
procedure TChoiceDirectoryForm.BitBtnChoiceClick(Sender: TObject);
begin	{ ???????? ???? ????????? }
	 MainFormInstall.InstPath := DirectoryListBox.Directory;
	 MainFormInstall.PathForInstall.Text := DirectoryListBox.Directory;
	 Close;
end;

end.
