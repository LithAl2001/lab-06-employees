#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <iostream>
#include <memory>
#include <string>

namespace employees {
class Employee {
protected:
    Employee(std::string, unsigned int);

    [[nodiscard]] virtual std::string serialized() const = 0;

public:
    const std::string full_name;
    const unsigned int base_salary_usd_per_year;

    Employee() = delete;

    Employee(Employee &&) = delete;

    Employee(const Employee &) = delete;

    Employee &operator=(Employee &&) = delete;

    Employee &operator=(const Employee &) = delete;

    [[nodiscard]] virtual unsigned int salary_usd_per_year() const = 0;

    static std::unique_ptr<Employee> read_from(std::istream &);

    friend std::ostream &operator<<(std::ostream &, const Employee &);

    virtual ~Employee() = default;
};

class Developer : virtual public Employee {
protected:
    [[nodiscard]] std::string serialized() const override;

    static std::unique_ptr<Employee> read_from(std::istream &,
                                               unsigned int,
                                               const std::string &);

    friend Employee;

public:
    const std::string github_account;

    [[nodiscard]] unsigned int salary_usd_per_year() const override;

    Developer(const std::string &, unsigned int, std::string);
};

class Manager : virtual public Employee {
protected:
    [[nodiscard]] std::string serialized() const override;

    static std::unique_ptr<Employee> read_from(std::istream &,
                                               unsigned int,
                                               const std::string &);

    friend Employee;

public:
    const std::string project_name;

    [[nodiscard]] unsigned int salary_usd_per_year() const override;

    Manager(const std::string &, unsigned int, std::string);
};

class LeadDeveloper : public Developer, public Manager {
protected:
    [[nodiscard]] std::string serialized() const override;

    static std::unique_ptr<Employee> read_from(std::istream &,
                                               unsigned int,
                                               const std::string &);

    friend Employee;

public:
    [[nodiscard]] unsigned int salary_usd_per_year() const override;

    LeadDeveloper(const std::string &,
                  unsigned int,
                  const std::string &,
                  const std::string &);
};
}  // namespace employees

#endif
