#pragma once
#include <unordered_map>
#include <vector>
#include <shared_mutex>

class FavoriteCommandRegistry 
{
	using CmdID = long;
	using Index = size_t;
public:
	static FavoriteCommandRegistry& Instance() 
	{
		static FavoriteCommandRegistry pInstance;
		return pInstance;
	}

	~FavoriteCommandRegistry() = default;
	FavoriteCommandRegistry(const FavoriteCommandRegistry&) = delete;
	FavoriteCommandRegistry(FavoriteCommandRegistry&&) = delete;
	FavoriteCommandRegistry& operator=(const FavoriteCommandRegistry&) = delete;
	FavoriteCommandRegistry& operator=(FavoriteCommandRegistry&&) = delete;
	
	void SetFavorite(const long cmdID, const bool bIsFavorite);
	bool IsFavorite(const long cmdID) const;
	std::vector<long> GetListOfFavoriteCommands() const;

private:
	FavoriteCommandRegistry() {}

private:
	std::unordered_map<CmdID, Index> m_commandToIndexMap;
	std::vector<CmdID> m_favoriteIndexMap;
	mutable std::shared_mutex m_mutex;
};