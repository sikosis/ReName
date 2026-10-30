#ifndef RENAME_RENAME_ENGINE_H
#define RENAME_RENAME_ENGINE_H

#include <cstddef>
#include <string>

enum class CaseMode {
	kUpper,
	kLower,
	kCapitaliseWords
};

struct RenameOptions {
	bool changeBasename = false;
	std::string basename;
	bool changeExtension = false;
	std::string extension;
	bool changeCase = false;
	CaseMode caseMode = CaseMode::kUpper;
	bool searchAndReplace = false;
	std::string search;
	std::string replacement;
};

struct NameParts {
	std::string basename;
	std::string extension;
};

NameParts SplitName(const std::string& name);
std::string BuildName(const std::string& originalName, std::size_t index,
	std::size_t total, const RenameOptions& options);

#endif
