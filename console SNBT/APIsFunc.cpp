#include "main.h"

extern std::string parameter;
extern std::string choosenProject;

void addAPIFunc() {

	if (choosenProject.size() == 0) {
		std::cerr << "project is not selected\n";
		return;
	}

	if (parameter.find("-") == std::string::npos) {
		std::cerr << "error with parameter\n";
		return;
	}

	size_t parameter1 = parameter.find("-");
	size_t parameter2 = parameter.find("-",parameter1 + 1);

	if (parameter1 != std::string::npos && parameter2 != std::string::npos && parameter1 == parameter2) {
		std::cerr << "error with parameter\n";
		return;
	}

	std::string apiName = parameter.substr(parameter1 + 1, parameter.find(" ", parameter1) - parameter1 - 1);
	std::string apiMeaning = parameter.substr(parameter2 + 1);

	if (apiName.length() == 0 || apiMeaning.length() == 0) {
		std::cerr << "error with parameter\n";
		return;
	}

	addAPIToJson(apiName, apiMeaning);
}

void addAPIToJson(const std::string& apiName, const std::string& apiMeaning) {
	std::ifstream jsonFile(choosenProject + "\\prop.json");
	if (jsonFile.is_open()) {
		std::string jsonString((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());
		jsonFile.close();

		rapidjson::Document doc;
		doc.Parse(jsonString.c_str());

		if (doc.HasMember("api")) {
			rapidjson::Value apiObject(rapidjson::kObjectType);
			rapidjson::Value apiMean(apiMeaning.c_str(), doc.GetAllocator());
			doc["api"].AddMember(rapidjson::StringRef(apiName.c_str()), apiMean, doc.GetAllocator());

			rapidjson::StringBuffer buffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
			doc.Accept(writer);

			std::ofstream outFile(choosenProject + "\\prop.json");
			outFile << buffer.GetString();
			outFile.close();
		}
	}
}

void translateFunc() {

	if (choosenProject.size() == 0) {
		std::cerr << "project is not selected\n";
		return;
	}

	if (parameter.find("-") == std::string::npos) {
		std::cerr << "error with parameters\n";
		return;
	}

	std::string nameAPI = parameter.substr(parameter.find("-") + 1, parameter.find_last_of("-") - parameter.find("-") - 2);
	std::string booleanStr = parameter.substr(parameter.find_last_of("-") + 1);
	bool boolean;

	if (nameAPI.length() == 0) {
		std::cerr << "error with parameters\n";
		return;
	}

	if (!(booleanStr == "true" || booleanStr == "false")) {
		std::cerr << "error with parameters\n";
		return;
	}
	else if (booleanStr == "true") {
		boolean = true;
	}
	else {
		boolean = false;
	}

	std::ifstream jsonFile(choosenProject + "\\prop.json");
	if (!jsonFile.is_open()) {
		std::cerr << "json file inst open\n";
		return;
	}
	std::string jsonString((std::istreambuf_iterator<char>(jsonFile)), std::istreambuf_iterator<char>());
	jsonFile.close();

	rapidjson::Document doc;
	doc.Parse(jsonString.c_str());

	if (!doc["api"].HasMember(nameAPI.c_str())) {
		std::cerr << "API doesnt exis\n";
		return;
	}

	std::string apiKey = doc["api"][nameAPI.c_str()].GetString();

	std::ifstream translateFile(choosenProject + "\\workingFiles\\translate.txt");
	if (!translateFile.is_open()) {
		std::cerr << "project isnt init\n";
		return;
	}
	translateFile.close();

	std::vector<std::wstring> translateLines = readFileByWstring(choosenProject + "\\workingFiles\\translate.txt");

	for (auto& line : translateLines) {
		if (line.find(L"path:") == std::wstring::npos) {
			//translateDeepl(line, apiKey);
			if (line.find(L"&") == std::wstring::npos) {
				std::wcerr << line;
			}
			else if (line.find(L"&") != std::wstring::npos && boolean) {
				std::wcerr << line;
			}
		}
	}
	std::cerr << "translate complete\n";
}

void translateDeepl(std::wstring& line, const std::string& apiKey) {
	size_t colon1;
	size_t colon2;
	size_t colon3;
	colon1 = line.find(L":");
	colon2 = line.find(L":", colon1 + 1);
	colon3 = line.find_last_of(L":");

	std::wstring originalString = line.substr(colon2 + 1, colon3 - colon2 - 1);
	line += cprRequestDeepl(originalString, apiKey);
}

std::wstring cprRequestDeepl(const std::wstring& string, const std::string& apiKey) {
	auto response = cpr::Post(
		cpr::Url{ "https://api-free.deepl.com/v2/translate" },
		cpr::Parameters{
			{"auth_key", apiKey},
			{"text", depozit::wstringToString(string)},
			{"source_lang", "EN"},
			{"target_lang", "RU"}
		}
	);

	std::string out;
	size_t lastColonn;
	size_t lastForging;

	if (response.status_code == 200) {
		lastColonn = response.text.find_last_of(":");
		lastForging = response.text.find_last_of("\"");
		out = response.text.substr(lastColonn + 2, lastForging - lastColonn - 2);
		return depozit::stringToWstring(out);
	}
	return L"";
}