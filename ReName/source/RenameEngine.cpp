#include "RenameEngine.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

namespace {

void ReplaceAll(std::string& value, const std::string& search,
	const std::string& replacement) {
	if (search.empty())
		return;

	std::size_t offset = 0;
	while ((offset = value.find(search, offset)) != std::string::npos) {
		value.replace(offset, search.length(), replacement);
		offset += replacement.length();
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//


void ChangeCase(std::string& value, CaseMode mode) {
	auto lower = [](unsigned char character) {
		return static_cast<char>(std::tolower(character));
	};
	auto upper = [](unsigned char character) {
		return static_cast<char>(std::toupper(character));
	};

	if (mode == CaseMode::kUpper) {
		std::transform(value.begin(), value.end(), value.begin(), upper);
		return;
	}

	std::transform(value.begin(), value.end(), value.begin(), lower);
	if (mode == CaseMode::kLower || value.empty())
		return;

	bool startOfWord = true;
	for (std::size_t i = 0; i < value.length(); ++i) {
		unsigned char character = static_cast<unsigned char>(value[i]);
		if (startOfWord && std::isalpha(character)) {
			value[i] = upper(character);
			startOfWord = false;
		} else if (value[i] == ' ' || value[i] == '_' || value[i] == '('
			|| value[i] == ')' || value[i] == '[' || value[i] == ']'
			|| value[i] == ',' || value[i] == '<' || value[i] == '>'
			|| value[i] == '&' || value[i] == '{' || value[i] == '}'
			|| value[i] == '-') {
			startOfWord = true;
		}

		if (i >= 1 && i + 1 < value.length() && value[i - 1] == 'M'
			&& value[i] == 'c') {
			value[i + 1] = upper(static_cast<unsigned char>(value[i + 1]));
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------------//


std::string NumberedBasename(const std::string& requested,
	const std::string& original, std::size_t index, std::size_t total) {
	std::string result = requested;
	if (result.find('*') != std::string::npos) {
		ReplaceAll(result, "*", original);
		return result;
	}

	if (total <= 1)
		return result;

	std::ostringstream number;
	number << std::setw(static_cast<int>(std::to_string(total).length()))
		<< std::setfill('0') << index;
	return result + number.str();
}
//---------------------------------------------------------------------------------------------------------------------------------//

} // namespace

NameParts SplitName(const std::string& name) {
	const std::size_t dot = name.rfind('.');
	if (dot == std::string::npos)
		return {name, ""};

	return {name.substr(0, dot), name.substr(dot + 1)};
}
//---------------------------------------------------------------------------------------------------------------------------------//


std::string BuildName(const std::string& originalName, std::size_t index,
	std::size_t total, const RenameOptions& options) {
	NameParts parts = SplitName(originalName);
	if (options.changeBasename) {
		parts.basename = NumberedBasename(options.basename, parts.basename,
			index, total);
	}

	if (options.changeExtension) {
		parts.extension = options.extension;
		if (!parts.extension.empty() && parts.extension.front() == '.')
			parts.extension.erase(0, 1);
	}

	if (options.changeCase) {
		ChangeCase(parts.basename, options.caseMode);
		ChangeCase(parts.extension, options.caseMode);
	}

	if (options.searchAndReplace) {
		ReplaceAll(parts.basename, options.search, options.replacement);
		ReplaceAll(parts.extension, options.search, options.replacement);
	}

	if (parts.extension.empty())
		return parts.basename;
	return parts.basename + "." + parts.extension;
}
//---------------------------------------------------------------------------------------------------------------------------------//
