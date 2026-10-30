#include "Application.h"
#include "MainWindow.h"

#include <Application.h>
#include <Message.h>
#include <OS.h>
#include <add-ons/tracker/TrackerAddOn.h>

extern "C" void process_refs(entry_ref directory, BMessage* message,
	void* reserved) {
	(void)directory;
	(void)reserved;

	MainWindow* window = new MainWindow(message);
	window->Show();
	status_t result;
	wait_for_thread(window->Thread(), &result);
}
//---------------------------------------------------------------------------------------------------------------------------------//


int main() {
	RenameApplication application;
	application.Run();
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------//
