#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

int main() {
    std::ifstream fs("asset/material/gold.json");
    std::stringstream ss;
    ss << fs.rdbuf();
    std::string jsonString = ss.str();

    rapidjson::Document document;
    document.Parse(jsonString.c_str());

    std::cout << document["albedo"].GetString() << std::endl;
}