#include <iostream>
#include <string>
#include <ranges>
#include <cmath>
#include <optional>
#include <vector>
#include <windows.h>
#include <numeric>

namespace problem {
  unsigned problem1() {
    std::string input_number;
    std::cin >> input_number;
    if (input_number.empty()) {
        return 0;
    }

    unsigned result = 1;
    for (const auto& input_char : input_number | std::views::filter([](const char& chr) { return '0' <= chr && chr <= '9'; })) {
        result *= (input_char - '0');
    }
    return result;
  }

  std::string problem2() {
    std::string input_number;
    std::cin >> input_number;
    if (input_number.empty()) {
        return "";
    }

    unsigned odd_sum = 0;
    unsigned even_sum = 0;
    bool is_odd = true;
    for (const auto& input_char : input_number | std::views::filter([](const char& chr) { return '0' <= chr && chr <= '9'; })) {
        unsigned digit = input_char - '0';
        if (is_odd) {
            odd_sum += digit;
        } else {
            even_sum += digit;
        }
        is_odd = !is_odd;
    }
    return std::to_string(odd_sum) + std::to_string(even_sum);
  }

  double problem3() {
    unsigned n;
    double result = 0;
    int three_devisible_count = 0;
    std::cin >> n;
    for (unsigned i = 0; i < n; i++) {
        int digit;
        std::cin >> digit;
        if (std::abs(digit) % 3 == 0) {
            result += digit;
            three_devisible_count += 1;
        }
    }
    return three_devisible_count == 0 ? -1 : (result / three_devisible_count);
  }

  int problem4() {
    unsigned n;
    std::cin >> n;
    
    // first, second, third fib values are 0, 1, 1
    // we know that n > 1.
    unsigned fib_position = 3;
    unsigned prev_fib_number = 1;
    for(unsigned fib_number = 2; fib_number <= n;) {
        if (fib_number == n) {
            return fib_position;
        }
        auto cur_fib_number = fib_number;
        fib_number += prev_fib_number;
        prev_fib_number = cur_fib_number;
        fib_position += 1;
    }
    return -1;
  }

  std::string problem5() {
    int N;
    std::cin >> N;

    std::string result;
    for(int i = 1; i <= N; i *=2) {
        result += std::to_string(i) + " ";
    }
    return result;
  }

  unsigned problem6() {
    int N;
    std::cin >> N;

    unsigned positive_count = 0;
    for(int i = 0; i < N; i++) {
        int value;
        std::cin >> value;
        positive_count += (value > 0);
    }
    return positive_count;
  }

  unsigned problem7() {
    unsigned sequence_element;
    std::cin >> sequence_element;

    unsigned max_element = 0;
    unsigned max_count = 0;
    while (sequence_element != 0) {
        if (sequence_element > max_element) {
            max_element = sequence_element;
            max_count = 1;
        } else if (sequence_element == max_element) {
            max_count += 1;
        }
        std::cin >> sequence_element;
    }
    return max_count;
  }
};

namespace problem8 {
  enum LaptopBrand {
    HP,
    Dell,
    Lenovo,
    MacBook,
    Asus
  };

  class LaptopBrandConverter {
    public:
        static std::string to_string(LaptopBrand brand) {
            switch (brand) {
                case HP:
                    return "HP";
                case Dell:
                    return "Dell";
                case Lenovo:
                    return "Lenovo";
                case MacBook:
                    return "MacBook";
                case Asus:
                    return "Asus";
                default:
                    return "NotFoundBrand";
            }
        }
    };

  class Laptop {
    public:
        Laptop(LaptopBrand brand, std::string model, double price_in_rubles)
        : _brand(brand), _model(model), _price_in_rubles(price_in_rubles) {
            _laptop_name = LaptopBrandConverter::to_string(brand) + " " + model;
        }

        std::string laptop_name() const {
            return _laptop_name;
        }
    private:
        LaptopBrand _brand;
        std::string _model;
        std::string _laptop_name;
        double _price_in_rubles;
  };
};

namespace problem9 {
    class Promise {
    public:
        Promise(unsigned id, double salary)
            : _id(id), _salary(salary), _is_salary_filled(false) {}

        double salary() const {
            return _salary;
        }

        bool is_salary_filled() const {
            return _is_salary_filled;
        }

        void set_salary_filled(bool value) {
            _is_salary_filled = value;
        }
    private:
        unsigned _id;
        double _salary;
        bool _is_salary_filled;
    };

    class Employee {
    public:
        Employee(std::string first_name, std::string second_name, unsigned id, double salary)
            : _first_name(first_name), _second_name(second_name),  _id(id), _promise(id, salary) {}
    protected:
        std::string _first_name;
        std::string _second_name;
        unsigned _id;
        Promise _promise;

        friend class Company;
    };

    class Director : public Employee {
    public:
        Director(std::string first_name, std::string second_name, unsigned id, double salary)
            : Employee(first_name, second_name, id, salary) {}

        bool check_promises() const {
            return _promise.is_salary_filled();
        }
    };

    class Company {
    public:
        Company(double balance) : _balance(balance) {}

        void create_director(std::string first_name, std::string second_name, unsigned id, double salary) {
            if (_director.has_value()) {
                throw std::out_of_range("Director may be only one");
            }

            _director.emplace(first_name, second_name, id, salary);
        }

        void create_employee(std::string first_name, std::string second_name, unsigned id, double salary) {
            _employees.emplace_back(first_name, second_name, id, salary);
        }

        Director& director() {
            return _director.value();
        }

        void set_profit(double profit) {
            _balance += profit;
        }

        bool fulfill_promise() {
            if (!_director.has_value()) {
                return false;
            }

            auto sum_salary = std::accumulate(_employees.begin(), _employees.end(),
                _director->_promise.salary(),
                [](double sum, const auto& employee) {
                    return sum + employee._promise.salary();
                }
            );

            auto is_promises_done = _balance >= sum_salary;
            _director->_promise.set_salary_filled(is_promises_done);
            for (auto& employee : _employees) {
                employee._promise.set_salary_filled(is_promises_done);
            }

            if (is_promises_done) {
                _balance -= sum_salary;
            }

            return is_promises_done;
        }
    private:
        double _balance;
        std::optional<Director> _director;
        std::vector<Employee> _employees;
    };
}

using namespace problem;
using namespace problem8;
using namespace problem9;

int main() {
  auto run_problem = [](this auto&& self) -> void
    {
        unsigned id_problem;
        std::cin >> id_problem;

        switch (id_problem) {
        case 1:
            std::cout << problem1();
            break;
        case 2:
            std::cout << problem2();
            break;
        case 3:
            std::cout << problem3();
            break;
        case 4:
            std::cout << problem4();
            break;
        case 5:
            std::cout << problem5();
            break;
        case 6:
            std::cout << problem6();
            break;
        case 7:
            std::cout << problem7();
            break;
        case 8: {
            SetConsoleOutputCP(CP_UTF8);
            SetConsoleCP(CP_UTF8);
            unsigned brand;
            std::string model;
            double price;

            std::cout << "Выберите бренд:\n"
                      << "0 — HP\n"
                      << "1 — Dell\n"
                      << "2 — Lenovo\n"
                      << "3 — MacBook\n"
                      << "4 — Asus\n";
            std::cin >> brand;

            std::cout << "Введите модель: ";
            std::getline(std::cin >> std::ws, model);

            std::cout << "Введите цену в рублях: ";
            std::cin >> price;

            auto laptop = Laptop(static_cast<LaptopBrand>(brand), model, price);
            std::cout << "Название ноутбука: " << laptop.laptop_name() << '\n';
            break;
        }
        case 9: {
            auto vk = Company(50);
            vk.create_director("Владимир", "Кириенко", 1, 15);

            vk.create_employee("Елена", "Иванова", 2, 8);
            vk.create_employee("Виктор", "Кузнецов", 3, 6);

            vk.set_profit(145.12);
            vk.fulfill_promise();

            auto& director = vk.director();
            std::cout << "In the start promise should be true, got: " << director.check_promises() << std::endl; // true

            vk.set_profit(-200);
            vk.fulfill_promise();

            std::cout << "After increasing profit, promise should be false, got: " << director.check_promises() << std::endl; // false
            break;
        }
        default:
            self();
        }
    };

  run_problem();
}
