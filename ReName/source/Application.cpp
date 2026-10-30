#include "RenameApplication.h"

#include "AppConstants.h"
#include "MainWindow.h"

#include <Entry.h>
#include <Message.h>

RenameApplication::RenameApplication()
	: BApplication(kApplicationSignature),
	fWindow(nullptr) {
}
//---------------------------------------------------------------------------------------------------------------------------------//


void RenameApplication::ArgvReceived(int32 argc, char** argv) {
	BMessage references(B_REFS_RECEIVED);
	for (int32 i = 1; i < argc; ++i) {
		BEntry entry(argv[i], true);
		entry_ref reference;
		if (entry.InitCheck() == B_OK && entry.GetRef(&reference) == B_OK)
			references.AddRef("refs", &reference);
	}

	RefsReceived(&references);
}
//---------------------------------------------------------------------------------------------------------------------------------//


void RenameApplication::ReadyToRun() {
	if (fWindow == nullptr) {
		fWindow = new MainWindow(nullptr);
		fWindow->Show();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//


void RenameApplication::RefsReceived(BMessage* message) {
	if (fWindow == nullptr) {
		fWindow = new MainWindow(message);
		fWindow->Show();
		return;
	}

	if (fWindow->Lock()) {
		fWindow->AddReferences(message);
		fWindow->Activate();
		fWindow->Unlock();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//
