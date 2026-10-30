#ifndef RENAME_APPLICATION_H
#define RENAME_APPLICATION_H

#include <Application.h>

class BMessage;
class MainWindow;

class RenameApplication : public BApplication {
public:
	RenameApplication();

	void ArgvReceived(int32 argc, char** argv) override;
	void ReadyToRun() override;
	void RefsReceived(BMessage* message) override;

private:
	MainWindow* fWindow;
};

#endif
