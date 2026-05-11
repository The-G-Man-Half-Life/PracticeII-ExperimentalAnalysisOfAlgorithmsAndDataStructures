//Include of all the required elements
#pragma once
#include <fstream>
#include <string>
#include <iostream>

class ResultExporter {
public:
    static void saveJSON(const std::string& json_content) {
        // Saves the file in the same folder it will be executed
        std::ofstream file("../web/results.json");
        if (file.is_open()) {
            file << json_content;
            file.close();
            std::cout << "\n[OK] Results exported to 'results.json'.\n";
        } else {
            std::cout << "\n[ERROR] It was not possible to be created results.json.\n";
        }
    }
};