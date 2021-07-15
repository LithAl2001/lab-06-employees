#include "employees.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace employees {
std::string insert_tilda(std::string name) {
    std::replace(name.begin(), name.end(), ' ', '~');
    return name;
}

Employee::Employee(std::string full_name_arg,
                   unsigned int base_salary_usd_per_year_arg)
    : full_name(std::move(full_name_arg)),
      base_salary_usd_per_year(base_salary_usd_per_year_arg) {
}

Developer::Developer(const std::string &full_name_arg,
                     unsigned int base_salary_usd_per_year_arg,
                     std::string github_account_arg)
    : Employee(full_name_arg, base_salary_usd_per_year_arg),
      github_account(std::move(github_account_arg)) {
}

Manager::Manager(const std::string &full_name_arg,
                 unsigned int base_salary_usd_per_year_arg,
                 std::string project_name_arg)
    : Employee(full_name_arg, base_salary_usd_per_year_arg),
      project_name(std::move(project_name_arg)) {
}

LeadDeveloper::LeadDeveloper(const std::string &full_name_arg,
                             unsigned int base_salary_usd_per_year_arg,
                             const std::string &github_account_arg,
                             const std::string &project_name_arg)
    : Employee(full_name_arg, base_salary_usd_per_year_arg),
      Developer(full_name_arg,
                base_salary_usd_per_year_arg,
                github_account_arg),
      Manager(full_name_arg, base_salary_usd_per_year_arg, project_name_arg) {
}

std::string tail_serialized(const std::string &full_name,
                            unsigned int base_salary_usd_per_year,
                            const std::string &information) {
    std::stringstream ss;
    ss << insert_tilda(full_name) << " " << base_salary_usd_per_year << " "
       << information;
    return ss.str();
}

std::string Developer::serialized() const {
    return "Developer " +
           tail_serialized(full_name, base_salary_usd_per_year, github_account);
}

std::string Manager::serialized() const {
    return "Manager " +
           tail_serialized(full_name, base_salary_usd_per_year, project_name);
}

std::string LeadDeveloper::serialized() const {
    return "LeadDeveloper " +
           tail_serialized(full_name, base_salary_usd_per_year,
                           github_account + " " + project_name);
}

[[nodiscard]] unsigned int Developer::salary_usd_per_year() const {
    return base_salary_usd_per_year + 100 * github_account.length();
}

[[nodiscard]] unsigned int Manager::salary_usd_per_year() const {
    return base_salary_usd_per_year + 100 * project_name.length();
}

[[nodiscard]] unsigned int LeadDeveloper::salary_usd_per_year() const {
    return base_salary_usd_per_year + 150 * github_account.length() +
           80 * project_name.length();
}

std::unique_ptr<Employee> Employee::read_from(std::istream &in) {
    std::string position;
    std::string full_name;
    unsigned int base_salary_usd_per_year{};
    in >> position >> full_name >> base_salary_usd_per_year;
    std::replace(full_name.begin(), full_name.end(), '~', ' ');
    if (position == "Developer") {
        return Developer::read_from(in, base_salary_usd_per_year, full_name);
    }
    if (position == "Manager") {
        return Manager::read_from(in, base_salary_usd_per_year, full_name);
    }
    if (position == "LeadDeveloper") {
        return LeadDeveloper::read_from(in, base_salary_usd_per_year,
                                        full_name);
    }
    return nullptr;
}

std::unique_ptr<Employee> Developer::read_from(
    std::istream &in,
    unsigned int base_salary_usd_per_year,
    const std::string &full_name) {
    std::string github_account;
    in >> github_account;
    return std::unique_ptr<Employee>(
        new Developer(full_name, base_salary_usd_per_year, github_account));
}

std::unique_ptr<Employee> Manager::read_from(
    std::istream &in,
    unsigned int base_salary_usd_per_year,
    const std::string &full_name) {
    std::string project_name;
    in >> project_name;
    return std::unique_ptr<Employee>(
        new Manager(full_name, base_salary_usd_per_year, project_name));
}

std::unique_ptr<Employee> LeadDeveloper::read_from(
    std::istream &in,
    unsigned int base_salary_usd_per_year,
    const std::string &full_name) {
    std::string github_account;
    std::string project_name;
    in >> github_account >> project_name;
    return std::unique_ptr<Employee>(new LeadDeveloper(
        full_name, base_salary_usd_per_year, github_account, project_name));
}

std::ostream &operator<<(std::ostream &out, const Employee &employee) {
    out << employee.serialized();
    return out;
}
}  // namespace employees
