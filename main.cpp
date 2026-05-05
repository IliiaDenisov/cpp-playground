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