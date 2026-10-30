#include "MainWindow.h"

#include "AppConstants.h"

#include <Alert.h>
#include <Application.h>
#include <Button.h>
#include <CheckBox.h>
#include <Directory.h>
#include <GroupLayout.h>
#include <LayoutBuilder.h>
#include <ListView.h>
#include <Message.h>
#include <OS.h>
#include <Path.h>
#include <RadioButton.h>
#include <ScrollView.h>
#include <StringItem.h>
#include <StringView.h>
#include <TextControl.h>

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <memory>

namespace {

constexpr uint32 kOptionsChanged = 'opch';
constexpr uint32 kRename = 'renm';
constexpr uint32 kClear = 'cler';

std::string PathForReference(const entry_ref& reference) {
	BEntry entry(&reference, true);
	BPath path;
	if (entry.GetPath(&path) != B_OK)
		return "";
	return path.Path();
}
//---------------------------------------------------------------------------------------------------------------------------------//


std::string ParentPathForReference(const entry_ref& reference) {
	BEntry entry(&reference, true);
	BEntry parent;
	BPath path;
	if (entry.GetParent(&parent) != B_OK || parent.GetPath(&path) != B_OK)
		return "";
	return path.Path();
}
//---------------------------------------------------------------------------------------------------------------------------------//

} // namespace

MainWindow::MainWindow(BMessage* references)
	: BWindow(BRect(120, 120, 820, 700), kWindowTitle, B_TITLED_WINDOW,
		B_ASYNCHRONOUS_CONTROLS | B_AUTO_UPDATE_SIZE_LIMITS),
	fPreviewList(nullptr),
	fDropHint(nullptr),
	fStatus(nullptr),
	fBasenameCheckBox(nullptr),
	fBasenameText(nullptr),
	fExtensionCheckBox(nullptr),
	fExtensionText(nullptr),
	fCaseCheckBox(nullptr),
	fUpperButton(nullptr),
	fLowerButton(nullptr),
	fCapitaliseButton(nullptr),
	fReplaceCheckBox(nullptr),
	fSearchText(nullptr),
	fReplacementText(nullptr),
	fRenameButton(nullptr) {
	BuildInterface();
	if (references != nullptr)
		AddReferences(references);
	else
		UpdatePreview();
}
//---------------------------------------------------------------------------------------------------------------------------------//


void MainWindow::BuildInterface() {
	fDropHint = new BStringView("drop hint",
		"Drop files here, or select files in Tracker and use Add-ons > ReName.");
	fPreviewList = new BListView("preview");
	BScrollView* previewScroller = new BScrollView("preview scroller",
		fPreviewList, 0, false, true);

	fBasenameCheckBox = new BCheckBox("change basename", "Change basename",
		new BMessage(kOptionsChanged));
	fBasenameText = new BTextControl("basename", "Basename:", "",
		new BMessage(kOptionsChanged));
	fBasenameText->SetModificationMessage(new BMessage(kOptionsChanged));

	fExtensionCheckBox = new BCheckBox("change extension", "Change extension",
		new BMessage(kOptionsChanged));
	fExtensionText = new BTextControl("extension", "Extension:", "",
		new BMessage(kOptionsChanged));
	fExtensionText->SetModificationMessage(new BMessage(kOptionsChanged));

	fCaseCheckBox = new BCheckBox("change case", "Change case",
		new BMessage(kOptionsChanged));
	fUpperButton = new BRadioButton("upper", "UPPER",
		new BMessage(kOptionsChanged));
	fLowerButton = new BRadioButton("lower", "lower",
		new BMessage(kOptionsChanged));
	fCapitaliseButton = new BRadioButton("capitalise", "Capitalise Each Word",
		new BMessage(kOptionsChanged));

	fReplaceCheckBox = new BCheckBox("search and replace",
		"Search and replace", new BMessage(kOptionsChanged));
	fSearchText = new BTextControl("search", "Search:", "",
		new BMessage(kOptionsChanged));
	fSearchText->SetModificationMessage(new BMessage(kOptionsChanged));
	fReplacementText = new BTextControl("replacement", "Replace:", "",
		new BMessage(kOptionsChanged));
	fReplacementText->SetModificationMessage(new BMessage(kOptionsChanged));

	fStatus = new BStringView("status", "No files selected");
	BButton* clearButton = new BButton("clear", "Clear",
		new BMessage(kClear));
	fRenameButton = new BButton("rename", "Rename",
		new BMessage(kRename));
	fRenameButton->MakeDefault(true);

	fUpperButton->SetValue(B_CONTROL_ON);

	BLayoutBuilder::Group<>(this, B_VERTICAL, B_USE_DEFAULT_SPACING)
		.SetInsets(B_USE_WINDOW_INSETS)
		.Add(fDropHint)
		.Add(previewScroller, 1.0f)
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING)
			.Add(fBasenameCheckBox)
			.Add(fBasenameText, 1.0f)
		.End()
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING)
			.Add(fExtensionCheckBox)
			.Add(fExtensionText, 1.0f)
		.End()
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING)
			.Add(fCaseCheckBox)
			.Add(fUpperButton)
			.Add(fLowerButton)
			.Add(fCapitaliseButton)
			.AddGlue()
		.End()
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING)
			.Add(fReplaceCheckBox)
			.Add(fSearchText, 1.0f)
			.Add(fReplacementText, 1.0f)
		.End()
		.AddGroup(B_HORIZONTAL, B_USE_DEFAULT_SPACING)
			.Add(fStatus, 1.0f)
			.Add(clearButton)
			.Add(fRenameButton)
		.End();

	SetControlsEnabled();
}
//---------------------------------------------------------------------------------------------------------------------------------//


void MainWindow::AddReferences(BMessage* message) {
	std::set<std::string> paths;
	for (const entry_ref& reference : fReferences)
		paths.insert(PathForReference(reference));

	entry_ref reference;
	for (int32 index = 0;
		message->FindRef("refs", index, &reference) == B_OK; ++index) {
		AddReference(reference, paths);
	}

	UpdatePreview();
}
//---------------------------------------------------------------------------------------------------------------------------------//


void MainWindow::AddReference(const entry_ref& reference,
	std::set<std::string>& paths) {
	BEntry entry(&reference, true);
	if (entry.InitCheck() != B_OK)
		return;
	entry_ref resolvedReference;
	if (entry.GetRef(&resolvedReference) != B_OK)
		return;
	const std::string path = PathForReference(resolvedReference);
	if (path.empty() || !paths.insert(path).second)
		return;

	if (entry.IsDirectory()) {
		BDirectory directory(&entry);
		entry_ref child;
		while (directory.GetNextRef(&child) == B_OK)
			AddReference(child, paths);
		return;
	}

	if (!entry.IsFile())
		return;

	fReferences.push_back(resolvedReference);
}
//---------------------------------------------------------------------------------------------------------------------------------//


RenameOptions MainWindow::CurrentOptions() const {
	RenameOptions options;
	options.changeBasename = fBasenameCheckBox->Value() == B_CONTROL_ON;
	options.basename = fBasenameText->Text();
	options.changeExtension = fExtensionCheckBox->Value() == B_CONTROL_ON;
	options.extension = fExtensionText->Text();
	options.changeCase = fCaseCheckBox->Value() == B_CONTROL_ON;
	if (fLowerButton->Value() == B_CONTROL_ON)
		options.caseMode = CaseMode::kLower;
	else if (fCapitaliseButton->Value() == B_CONTROL_ON)
		options.caseMode = CaseMode::kCapitaliseWords;
	options.searchAndReplace = fReplaceCheckBox->Value() == B_CONTROL_ON;
	options.search = fSearchText->Text();
	options.replacement = fReplacementText->Text();
	return options;
}
//---------------------------------------------------------------------------------------------------------------------------------//


std::vector<MainWindow::RenameItem> MainWindow::MakePlan(
	std::string& error) const {
	std::vector<RenameItem> items;
	std::set<std::string> sourcePaths;
	std::set<std::string> targetPaths;
	const RenameOptions options = CurrentOptions();
	const std::size_t total = fReferences.size();

	for (const entry_ref& reference : fReferences)
		sourcePaths.insert(PathForReference(reference));

	for (std::size_t index = 0; index < total; ++index) {
		const entry_ref& reference = fReferences[index];
		RenameItem item;
		item.reference = reference;
		item.originalName = reference.name;
		item.newName = BuildName(item.originalName, index + 1, total, options);

		if (item.newName.empty() || item.newName == "." || item.newName == ".."
			|| item.newName.find('/') != std::string::npos) {
			error = "The proposed name for " + item.originalName + " is invalid.";
			return {};
		}

		const std::string parent = ParentPathForReference(reference);
		const std::string targetPath = parent + "/" + item.newName;
		if (!targetPaths.insert(targetPath).second) {
			error = "More than one file would be named " + item.newName + ".";
			return {};
		}

		BEntry target(targetPath.c_str(), false);
		if (target.Exists() && sourcePaths.find(targetPath) == sourcePaths.end()) {
			error = item.newName + " already exists.";
			return {};
		}

		items.push_back(item);
	}

	return items;
}
//---------------------------------------------------------------------------------------------------------------------------------//


bool MainWindow::ExecuteRename(std::vector<RenameItem>& items,
	std::string& error) {
	const bigtime_t nonce = system_time();
	for (std::size_t index = 0; index < items.size(); ++index) {
		RenameItem& item = items[index];
		if (item.originalName == item.newName)
			continue;

		BEntry entry(&item.reference, false);
		char temporaryName[B_FILE_NAME_LENGTH];
		std::snprintf(temporaryName, sizeof(temporaryName),
			".ReName-%lld-%zu", static_cast<long long>(nonce), index);
		const status_t status = entry.Rename(temporaryName, false);
		if (status != B_OK || entry.GetRef(&item.temporaryReference) != B_OK) {
			error = "Could not stage " + item.originalName + " for renaming.";
			RollBack(items);
			return false;
		}
		item.staged = true;
	}

	for (RenameItem& item : items) {
		if (!item.staged)
			continue;

		BEntry entry(&item.temporaryReference, false);
		const status_t status = entry.Rename(item.newName.c_str(), false);
		if (status != B_OK || entry.GetRef(&item.finalReference) != B_OK) {
			error = "Could not rename " + item.originalName + " to "
				+ item.newName + ".";
			RollBack(items);
			return false;
		}
		item.finalised = true;
	}

	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------//


void MainWindow::RollBack(std::vector<RenameItem>& items) {
	const bigtime_t nonce = system_time();
	for (std::size_t index = 0; index < items.size(); ++index) {
		RenameItem& item = items[index];
		if (!item.finalised)
			continue;

		BEntry entry(&item.finalReference, false);
		char temporaryName[B_FILE_NAME_LENGTH];
		std::snprintf(temporaryName, sizeof(temporaryName),
			".ReName-rollback-%lld-%zu", static_cast<long long>(nonce), index);
		if (entry.Rename(temporaryName, false) == B_OK
			&& entry.GetRef(&item.temporaryReference) == B_OK) {
			item.finalised = false;
			item.staged = true;
		}
	}

	for (RenameItem& item : items) {
		if (!item.staged || item.finalised)
			continue;

		BEntry entry(&item.temporaryReference, false);
		entry.Rename(item.originalName.c_str(), false);
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//


void MainWindow::SetControlsEnabled() {
	fBasenameText->SetEnabled(fBasenameCheckBox->Value() == B_CONTROL_ON);
	fExtensionText->SetEnabled(fExtensionCheckBox->Value() == B_CONTROL_ON);
	const bool changeCase = fCaseCheckBox->Value() == B_CONTROL_ON;
	fUpperButton->SetEnabled(changeCase);
	fLowerButton->SetEnabled(changeCase);
	fCapitaliseButton->SetEnabled(changeCase);
	const bool replace = fReplaceCheckBox->Value() == B_CONTROL_ON;
	fSearchText->SetEnabled(replace);
	fReplacementText->SetEnabled(replace);
}
//---------------------------------------------------------------------------------------------------------------------------------//


void MainWindow::UpdatePreview() {
	fPreviewList->MakeEmpty();
	const RenameOptions options = CurrentOptions();
	const std::size_t total = fReferences.size();
	for (std::size_t index = 0; index < total; ++index) {
		const std::string original = fReferences[index].name;
		const std::string proposed = BuildName(original, index + 1, total, options);
		const std::string line = original + "  →  " + proposed;
		fPreviewList->AddItem(new BStringItem(line.c_str()));
	}

	if (total == 0)
		UpdateStatus("No files selected");
	else if (total == 1)
		UpdateStatus("1 file selected");
	else {
		char status[64];
		std::snprintf(status, sizeof(status), "%zu files selected", total);
		UpdateStatus(status);
	}
	fRenameButton->SetEnabled(total > 0);
}
//---------------------------------------------------------------------------------------------------------------------------------//


void MainWindow::UpdateStatus(const char* text) {
	fStatus->SetText(text);
}
//---------------------------------------------------------------------------------------------------------------------------------//


void MainWindow::MessageReceived(BMessage* message) {
	switch (message->what) {
		case B_SIMPLE_DATA:
		case B_REFS_RECEIVED:
			AddReferences(message);
			break;
		case kOptionsChanged:
			SetControlsEnabled();
			UpdatePreview();
			break;
		case kClear:
			fReferences.clear();
			UpdatePreview();
			break;
		case kRename: {
			std::string error;
			std::vector<RenameItem> plan = MakePlan(error);
			if (!error.empty() || !ExecuteRename(plan, error)) {
				(new BAlert(kApplicationName, error.c_str(), "OK",
					nullptr, nullptr, B_WIDTH_AS_USUAL, B_STOP_ALERT))->Go();
				break;
			}
			fReferences.clear();
			UpdatePreview();
			UpdateStatus("Rename complete");
			break;
		}
		default:
			BWindow::MessageReceived(message);
			break;
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//


bool MainWindow::QuitRequested() {
	if (be_app != nullptr && be_app->Signature() != nullptr
		&& std::strcmp(be_app->Signature(), kApplicationSignature) == 0) {
		be_app->PostMessage(B_QUIT_REQUESTED);
	}
	return true;
}
//---------------------------------------------------------------------------------------------------------------------------------//
