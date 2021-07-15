#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include "employees.h"

bool is_in(employees::Employee *employee, const std::string &type) {
    if (type == "all") {
        return dynamic_cast<employees::Employee *>(employee) != nullptr;
    }
    if (type == "developers") {
        return dynamic_cast<employees::Developer *>(employee) != nullptr;
    }
    if (type == "managers") {
        return dynamic_cast<employees::Manager *>(employee) != nullptr;
    }
    if (type == "leads") {
        return dynamic_cast<employees::LeadDeveloper *>(employee) != nullptr;
    }
    return false;
}

int main() {
    std::vector<std::unique_ptr<employees::Employee>> database;
    while (true) {
        std::cout << "> ";
        std::string s;
        std::cin >> s;
        if (s == "load") {
            std::string name;
            std::cin >> name;
            database.clear();
            std::ifstream file(name);
            while (!file.eof()) {
                database.push_back(employees::Employee::read_from(file));
            }
            database.pop_back();
            file.close();
            continue;
        }
        if (s == "save") {
            std::string type;
            std::string name;
            std::cin >> type >> name;
            std::ofstream file(name);
            for (auto &i : database) {
                if (is_in(i.get(), type)) {
                    file << *i << std::endl;
                }
            }
            file.close();
            continue;
        }
        if (s == "list-devs") {
            std::vector<std::string> answer;
            for (auto &i : database) {
                if (is_in(i.get(), "developers") || is_in(i.get(), "leads")) {
                    auto *new_i = dynamic_cast<employees::Developer *>(i.get());
                    std::stringstream ss;
                    ss << answer.size() + 1 << ". " << new_i->full_name
                       << " aka @" << new_i->github_account;
                    answer.push_back(ss.str());
                }
            }
            std::cout << answer.size() << " developer(s) found:" << std::endl;
            for (auto &str : answer) {
                std::cout << str << std::endl;
            }
            continue;
        }
        return 0;
    }
}
