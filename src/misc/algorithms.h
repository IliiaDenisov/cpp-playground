#pragma once
/*
1. Searching & Querying
std::find, std::find_if, std::find_if_not
std::any_of, std::all_of, std::none_of
std::count, std::count_if
*/

/*
2. Modifying Sequences (the bread and butter)
std::copy, std::copy_if
std::move
std::transform ⭐ (very important)
std::fill, std::generate
std::iota
*/

/*
3. Removing Elements (the famous idiom)
std::remove, std::remove_if
std::unique
*/

/*
4. Reordering & Sorting
std::sort ⭐
std::stable_sort
std::partial_sort
std::nth_element ⭐ (very underrated)
std::reverse
std::rotate
std::shuffle
*/

/*
5. Binary Search Family (for sorted ranges)
std::binary_search
std::lower_bound ⭐
std::upper_bound
std::equal_range
💡 Senior insight:
lower_bound is everywhere in high-performance code. If you’re not using it, you’re probably missing an optimization.
*/

/*
6. Numeric Algorithms
std::accumulate ⭐
std::inner_product
std::partial_sum
std::adjacent_difference
💡 Senior insight:
accumulate is more flexible than it looks. Custom reducers turn it into a mini functional engine.
*/

/*
10. Partitioning
std::partition
std::stable_partition
std::partition_point
💡 Senior insight:
Partitioning is often faster than sorting when you only need grouping.
*/


/*
Modern C++ features:
auto (used properly)
Structured bindings
std::variant, std::optional, std::any
Lambdas (advanced usage)
Ranges ⭐
*/