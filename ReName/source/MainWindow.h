#ifndef RENAME_MAIN_WINDOW_H
#define RENAME_MAIN_WINDOW_H

#include "RenameEngine.h"

#include <Entry.h>
#include <Window.h>

#include <set>
#include <string>
#include <vector>

class BButton;
class BCheckBox;
class BListView;
class BRadioButton;
class BStringView;
class BTextControl;

class MainWindow : public BWindow {
public:
	explicit MainWindow(BMessage* references);

	void AddReferences(BMessage* message);
	void MessageReceived(BMessage* message) override;
	bool QuitRequested() override;

private:
	struct RenameItem {
		entry_ref reference;
		std::string originalName;
		std::string newName;
		entry_ref temporaryReference;
		entry_ref finalReference;
		bool staged = false;
		bool finalised = false;
	};

	void AddReference(const entry_ref& reference, std::set<std::string>& paths);
	void BuildInterface();
	RenameOptions CurrentOptions() const;
	bool ExecuteRename(std::vector<RenameItem>& items, std::string& error);
	std::vector<RenameItem> MakePlan(std::string& error) const;
	void RollBack(std::vector<RenameItem>& items);
	void SetControlsEnabled();
	void UpdatePreview();
	void UpdateStatus(const char* text);

	std::vector<entry_ref> fReferences;
	BListView* fPreviewList;
	BStringView* fDropHint;
	BStringView* fStatus;
	BCheckBox* fBasenameCheckBox;
	BTextControl* fBasenameText;
	BCheckBox* fExtensionCheckBox;
	BTextControl* fExtensionText;
	BCheckBox* fCaseCheckBox;
	BRadioButton* fUpperButton;
	BRadioButton* fLowerButton;
	BRadioButton* fCapitaliseButton;
	BCheckBox* fReplaceCheckBox;
	BTextControl* fSearchText;
	BTextControl* fReplacementText;
	BButton* fRenameButton;
};

#endif
