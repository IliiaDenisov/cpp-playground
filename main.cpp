#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <ranges>
#include <array>
#include <print>

int main() {
    std::array<int, 7> arr {1, 2, 3, 4, 5, 6, 7};

    auto cubicEvens = arr
                    | std::views::filter([](int x) { return x % 2 == 0; })
                    | std::views::transform([](int x) { return x * x * x; })
                    | std::views::take(3);

    for (auto x : cubicEvens) {
        std::println("Next number is {}", x);
    }


    std::vector<int> zipKey{0, 1, 3, 4, 5};
    std::vector<std::string> zipValue{"RND", "DWX", "SVO", "LAX"};
    for (auto [idx, airport] : std::views::zip(zipKey, zipValue)) {
        std::println("{} has index {}", airport, idx);
    }
}