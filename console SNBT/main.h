#pragma once
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <Windows.h>
#include <map>
#include <fstream>
#include <filesystem>
#include <chrono>
#include "D:\TWM dump\SNBT parser\SNBT parser\SNBT.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "cpr/cpr.h"

namespace fs = std::filesystem;

void dispalyInfo();
void initJsonFile(const fs::path&, const std::string&);
void listProjectFunc();
void chooseProjectFunc();
void closeProjectFunc();
void copyDirectory(const fs::path&, const fs::path&);
void initWorkingFiles(const fs::path&);
void initFunc();
void makeProjectFunc();
void buildFunc();
void readFileByWstring(const fs::path&, std::vector<std::wstring>&);
std::vector<std::wstring> readFileByWstring(const fs::path&);

//APIs 
void addAPIFunc();
void addAPIToJson(const std::string&, const std::string&);
void translateFunc();
void translateDeepl(std::wstring&, const std::string&);
std::wstring cprRequestDeepl(const std::wstring&, const std::string&);