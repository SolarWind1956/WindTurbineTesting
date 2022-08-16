program Install;

uses
  Forms,
  mainform in 'mainform.pas' {MainFormInstall},
  choicdir in 'choicdir.pas' {ChoiceDirectoryForm},
  srcdrive in 'srcdrive.pas' {SetSourceDriveForm},
  FmxUtils in 'Fmxutils.pas',
  choigrup in 'choigrup.pas' {TChoiceGroupForm};

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'Автоматизированная система испытаний';
  Application.CreateForm(TMainFormInstall, 	   MainFormInstall);
  Application.CreateForm(TChoiceDirectoryForm, ChoiceDirectoryForm);
  Application.CreateForm(TSetSourceDriveForm,  SetSourceDriveForm);
  Application.CreateForm(TChoiceGroupForm, 	   ChoiceGroupForm);
  Application.Run;
end.
