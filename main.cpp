#include <cassert>
#include <algorithm>
#include "src/FavoriteCommandRegistry.h"
#include "src/unique_ptr_constexpr.h"
#include <iostream>

void testFavoriteCommandRegistry() {
    auto &cm = FavoriteCommandRegistry::Instance();

    assert(!cm.IsFavorite(1));

    cm.SetFavorite(1, true);
    assert(cm.IsFavorite(1));

    const auto &favs = cm.GetListOfFavoriteCommands();
    assert(std::find(favs.begin(), favs.end(), 1L) != favs.end());

    cm.SetFavorite(1, false);
    assert(!cm.IsFavorite(1));

    cm.SetFavorite(10, true);
    cm.SetFavorite(11, true);
    cm.SetFavorite(12, true);
    cm.SetFavorite(11, false);
    assert(!cm.IsFavorite(11));
    assert(cm.IsFavorite(10));
    assert(cm.IsFavorite(12));
    assert(cm.GetListOfFavoriteCommands().size() == 2);

    std::cout << "Tests have passsed\n";
}

consteval void testConstexprUniquePtr() {
    [] consteval {
        unique_ptr<int> x(new int(21));
        unique_ptr<int> y(new int(42));
        y = std::move(x);
    }();
}

int main()
{
    testConstexprUniquePtr();
    return 0;
}