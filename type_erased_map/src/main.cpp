#include "any_map.h"
#include <iostream>
#include <vector>

int main() {
    ds::AnyMap<std::string> map{};

    map.set("age",    42);
    map.set("name",   std::string{"Mazen"});
    map.set("scores", std::vector<int>{95, 87, 100, 73});
    map.set("ratio",  3.14);
    map.set("widen",  (int)3);

    // correct type
    if (std::expected<int, std::string> age = map.get("age"); age)
        std::cout << "age: " << *age << "\n";
    else
        std::cout << "age error: " << age.error() << "\n";

    // correct type
    if (std::expected<std::string, std::string> name = map.get("name"); name)
        std::cout << "name: " << *name << "\n";
    else
        std::cout << "name error: " << name.error() << "\n";

    // wrong type, int stored as double
    if (std::expected<int, std::string> ratio = map.get("ratio"); ratio)
        std::cout << "ratio as int: " << *ratio << "\n";
    else
        std::cout << "ratio as int error: " << ratio.error() << "\n";

    // correct type
    if (std::expected<std::vector<int>, std::string> scores = map.get("scores"); scores) {
        std::cout << "scores: ";
        for (int s : *scores) std::cout << s << " ";
        std::cout << "\n";
    } else {
        std::cout << "scores error: " << scores.error() << "\n";
    }

    // wrong type, string stored, asking for int
    if (std::expected<int, std::string> bad = map.get("name"); bad)
        std::cout << "name as int: " << *bad << "\n";
    else
        std::cout << "name as int error: " << bad.error() << "\n";


    // fails, any_cast tracks exact type, cannot widen int to double
    if (std::expected<double, std::string> bad = map.get("widen"); bad)
        std::cout << "int as double: " << *bad << "\n";
    else
        std::cout << "int as double error: " << bad.error() << "\n";

    return 0;
}
