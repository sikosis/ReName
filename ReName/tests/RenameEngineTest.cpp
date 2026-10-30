#include "RenameEngine.h"

#include <cstdlib>
#include <iostream>
#include <string>

namespace {

void ExpectEqual(const std::string& actual, const std::string& expected,
	const char* testName) {
	if (actual == expected)
		return;

	std::cerr << testName << ": expected '" << expected << "', got '"
		<< actual << "'\n";
	std::exit(1);
}
//---------------------------------------------------------------------------------------------------------------------------------//

} // namespace

int main() {
	RenameOptions options;
	options.changeBasename = true;
	options.basename = "photo";
	ExpectEqual(BuildName("old.jpg", 3, 12, options), "photo03.jpg",
		"numbered basename");

	options.basename = "edited-*";
	ExpectEqual(BuildName("old.jpg", 1, 3, options), "edited-old.jpg",
		"original basename placeholder");

	options = RenameOptions();
	options.changeExtension = true;
	options.extension = ".txt";
	ExpectEqual(BuildName("archive.tar.gz", 1, 1, options),
		"archive.tar.txt", "extension replacement");

	options = RenameOptions();
	options.changeCase = true;
	options.caseMode = CaseMode::kCapitaliseWords;
	ExpectEqual(BuildName("mary_mcdonald.txt", 1, 1, options),
		"Mary_McDonald.Txt", "capitalised words");

	options = RenameOptions();
	options.searchAndReplace = true;
	options.search = "draft";
	options.replacement = "final";
	ExpectEqual(BuildName("draft.notes.draft", 1, 1, options),
		"final.notes.final", "search and replace");

	options.search.clear();
	ExpectEqual(BuildName("unchanged.txt", 1, 1, options),
		"unchanged.txt", "empty search");

	std::cout << "All rename engine tests passed.\n";
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------//
