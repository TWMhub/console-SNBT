#include "main.h"

extern std::string parameter;
extern std::string choosenProject;

void dispalyInfo() {
    std::cerr << R"(SNBT reader: v3; console SNBT:v1
    - makeProject -"your title" (creates a project folder in the same directory as the application)
    - chooseProject -"your title" (selects a working project from those that have already been created)
    - init -"path to modpack" ("D:\CurseForge\Instances\All the Mods 9 - ATM9" for example)
    - build (build choosen project)
    - list (displays your projects)
    - closeProject (close choosen project)
    - exit (close programm)

    - addAPI -"name api" -"api key"
    - translate -"name api" -"true/false" (true if with &, false when without)
    )" << std::endl;
}

void initJsonFile(const fs::path& path, const std::string& name) {

    //getting time
    time_t now = time(nullptr);
    struct tm localTime;
    localtime_s(&localTime, &now);
    std::string time = std::to_string(localTime.tm_year + 1900) + '-' + std::to_string(localTime.tm_mon + 1) + '-' + std::to_string(localTime.tm_mday);

    //creation json file
    rapidjson::Document doc;
    doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
    //name
    rapidjson::Value projectName(name.c_str(), allocator);
    doc.AddMember("project name", projectName, allocator);
    //authors
    rapidjson::Value authors(rapidjson::kArrayType);
    rapidjson::Value author1("Slava_vas", allocator);
    rapidjson::Value author2("Depozit", allocator);
    authors.PushBack(author1, allocator);
    authors.PushBack(author2, allocator);
    doc.AddMember("authors", authors, allocator);
    //today date
    rapidjson::Value timeValue(time.c_str(), allocator);
    doc.AddMember("creation_date", timeValue, allocator);
    //program version
    rapidjson::Value programVersion("1.0", allocator);
    doc.AddMember("programm_version", programVersion, allocator);
    //add api
    rapidjson::Value apiObject(rapidjson::kObjectType);
    doc.AddMember("api", apiObject, doc.GetAllocator());

    //doc.AddMember("myObj", obj, allocator);
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);

    //writing files
    std::ofstream outFile(path);
    if (outFile.is_open()) {
        outFile << buffer.GetString();
        outFile.close();
    }
    else {
        std::cerr << "file not open\n";
    }
}

void listProjectFunc() {
    std::string path = ".";

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_directory()) {
            std::cout << "- " << entry.path().filename() << std::endl;
        }
    }
}

void chooseProjectFunc() {
    std::string path = ".\\" + parameter.substr(parameter.find("-") + 1);
    if (std::filesystem::exists(path)) {
        choosenProject = path;
        std::cerr << "project is selected\n";
    }
    else {
        std::cerr << "project does not exist\n";
    }
}

void closeProjectFunc() {
    choosenProject = "";
    std::cerr << "project is closed\n";
}

void copyDirectory(const fs::path& source, const fs::path& destination) {
    if (!fs::exists(destination)) {
        fs::create_directories(destination);
    }

    for (const auto& entry : fs::directory_iterator(source)) {
        fs::path currentPath = entry.path();
        fs::path newPath = destination / currentPath.filename();

        if (fs::is_directory(currentPath)) {
            copyDirectory(currentPath, newPath);
        }
        else if (fs::is_regular_file(currentPath)) {
            fs::copy_file(currentPath, newPath, fs::copy_options::overwrite_existing);
        }
    }
}

void initWorkingFiles(const fs::path& pathWork) {
    //creating path
    fs::path pathWorking{ L"workingFiles\\translate.txt" };
    pathWorking = choosenProject / pathWorking;

    std::wstring outputFileByWstring{ L"" };
    depozit::snbt reader;
    for (const auto& entry : fs::directory_iterator(choosenProject + "\\input\\chapters")) {
        reader.clear();
        reader.init(entry);
       
        outputFileByWstring += L"path:" + entry.path().wstring() + L"\n";
        outputFileByWstring += reader.getTextsByWstring() + L"\n";
    }

    std::wofstream out(pathWorking);
    out << outputFileByWstring;
    out.close();
}

void initFunc() {
    fs::path pathToModpack = parameter.substr(parameter.find("-") + 1);
    if (choosenProject.length() > 0) {
        if (std::filesystem::exists(pathToModpack / "config\\ftbquests\\quests")) {
            pathToModpack += "\\config\\ftbquests\\quests";
            copyDirectory(pathToModpack, choosenProject + "\\input");
            initWorkingFiles(choosenProject + "\\workingFiles");
            std::cerr << "initialization is successful\n";
        }
        else {
            std::cerr << "modpack does not exist\n";
        }
    }
    else {
        std::cerr << "project does not exist\n";
    }
}

void makeProjectFunc() {
    if (parameter.find("-") != std::string::npos) {
        std::wstring nameDir = depozit::stringToWstring(parameter.substr(parameter.find("-") + 1)).c_str();
        if (nameDir.length() > 0) {
            CreateDirectory(nameDir.c_str(), NULL);
            CreateDirectory((nameDir + L"\\input").c_str(), NULL);
            CreateDirectory((nameDir + L"\\workingFiles").c_str(), NULL);
            CreateDirectory((nameDir + L"\\output").c_str(), NULL);
            fs::path path = parameter.substr(parameter.find("-") + 1) + "\\prop.json";
            initJsonFile(path, parameter.substr(parameter.find("-") + 1));
            choosenProject = ".\\" + parameter.substr(parameter.find("-") + 1);
            std::cerr << "creation is successful\n";
        }
        else {
            std::cerr << "incorrect name project\n";
        }
    }
    else {
        std::cerr << "incorrect parameter\n";
    }
}

void readFileByWstring(const fs::path& path, std::vector<std::wstring>& out) {
    out.clear();
    std::wifstream input(path);
    if (!input.is_open()) {
        std::cerr << "file isnt open\n";
        return;
    }
    std::wstring line;
    while (std::getline(input, line)) {
        out.push_back(line);
    }
}

std::vector<std::wstring> readFileByWstring(const fs::path& path) {
    std::vector<std::wstring> out;
    std::wifstream input(path);
    if (!input.is_open()) {
        std::cerr << "file isnt open\n";
        input.close();
        return out;
    }
    std::wstring line;
    while (std::getline(input, line)) {
        out.push_back(line);
    }
    input.close();
    return out;
}

void writeWstringInFile(const fs::path& path, const std::vector<std::wstring>& file) {
    std::ofstream output(path);
    for (const auto& line : file) {
        output << depozit::wstringToString(line).c_str() << std::endl;
    }
    output.close();
}

void buildFunc() {//didnt work
    if(choosenProject.length() > 0) {
        
        std::cerr << "build start\n";

        //init vars
        size_t colon1;
        size_t colon2;
        size_t colon3;
        fs::path pathToQuestFileInput;
        fs::path pathToQuestFileOutput;
        std::pair<std::wstring, std::wstring> origLocStr;
        int posInFile;
        depozit::type typeText;

        //original String
        std::vector<std::wstring> origFile;

        //copy working project
        copyDirectory(choosenProject + "\\input", choosenProject + "\\output");
        
        //get translate
        std::vector<std::wstring> translateFile;
        readFileByWstring(choosenProject + "\\workingFiles\\translate.txt", translateFile);

        //checking whether the file has been opened and written to the vector
        if (translateFile.size() == 0) {
            std::cerr << "error with translate file\n";
            return;
        }

        try{
        //iterating through the values from translate.txt
            for (const auto& line : translateFile) {
                if (line.find(L"path:") != std::wstring::npos) {
                    pathToQuestFileInput = choosenProject + "\\input\\chapters";
                    pathToQuestFileInput += line.substr(line.find_last_of(L"\\"));

                    pathToQuestFileOutput = choosenProject + "\\output\\chapters";
                    pathToQuestFileOutput += line.substr(line.find_last_of(L"\\"));

                    readFileByWstring(pathToQuestFileInput, origFile);
                    if (origFile.size() == 0) {
                        std::cerr << "snbt file isnt open\n";
                        break;
                    }
                }
                else if (line.length() != 0) {
                    colon1 = line.find(L":");
                    colon2 = line.find(L":", colon1 + 1);
                    colon3 = line.find_last_of(L":");

                    if (colon1 != std::wstring::npos && colon2 != std::wstring::npos && colon3 != std::wstring::npos) {
                        posInFile = stoi(line.substr(0, colon1));
                        typeText = depozit::wstringToType(line.substr(colon1 + 1, colon2 - colon1 - 1));
                        origLocStr.first = line.substr(colon2 + 1, colon3 - colon2 - 1);
                        origLocStr.second = line.substr(colon3 + 1);

                        if (typeText != depozit::type::unknown && origLocStr.second.length() > 0) {
                            origFile[posInFile].replace(origFile[posInFile].find(origLocStr.first), origLocStr.first.length(), origLocStr.second);
                        }
                    }
                }
                else {
                    remove(pathToQuestFileOutput);
                    std::wofstream out(pathToQuestFileOutput);
                    
                    for (int i = 0; i < origFile.size(); i++) {
                        out << origFile[i] << L"\n";
                    }
                    out.close();
                }
            }
        }
        catch (std::string error_message) {
            std::cerr << error_message << std::endl;
            return;
        }

        std::cerr << "build complete\n";
    }
    else {
        std::cerr << "project is't selected\n";
    }
}



