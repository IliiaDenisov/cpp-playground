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
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <variant>
#include <string>
#include <unordered_map>
#include <optional>

struct Song
{
	std::string title;
	std::string artist;
	int rating;  // 1-5
};
template <typename ForwardIt>
	requires std::is_same_v<typename ForwardIt::value_type, Song>
void PrintSongs(ForwardIt begin, ForwardIt end)
{
	auto curr = begin;
	while (curr != end)
	{
		std::cout << curr->title << ", " << curr->artist << ", " << curr->rating << '\n';
		++curr;
	}
}


// 1. Partition the songs into "liked" and "unliked" (liked = rating ≥ 4), keeping relative order within each group
// 2. Rotate the liked songs so the highest-rated one comes first
// 3. Find all songs by a specific artist within the liked partition
// 4. Count how many unique artists are in the full playlist
// 5. Check if the liked songs are sorted by rating in descending order

std::optional<int> f() {
	return std::nullopt;
}

int main()
{
	std::vector<Song> playlist = {
		{"Bohemian Rhapsody", "Queen",       5},
		{"Blinding Lights",  "The Weeknd",  3},
		{"Starboy",          "The Weeknd",  4},
		{"Billie Jean",      "MJ",          5},
		{"Blue",             "Eiffel 65",   2},
		{"Thunderstruck",    "AC/DC",       4},
		{"Yesterday",        "Beatles",     3},
		{"Come Together",    "Beatles",     4},
		{"Shape of You",     "Ed Sheeran",  2},
		{"Perfect",          "Ed Sheeran",  5},
	};

	// 1. 
	std::stable_partition(playlist.begin(), playlist.end(), [](const Song& song)
		{
			return song.rating >= 4;
		});

	// 2. 

	// 3. 

	// 4.
	// 5.

	//auto isSorted = std::is_sorted(playlist.b)
	PrintSongs(playlist.begin(), playlist.end());

	auto res = f();

	auto arr = new int[1000]; 

	arr[10000000000] = 121;
}