#include "FavoriteCommandRegistry.h"
#include <algorithm>
#include <mutex>

void FavoriteCommandRegistry::SetFavorite(const long cmdID, const bool bIsFavorite)
{
	std::unique_lock lock{ m_mutex };
	bool alreadyFavorited = m_commandToIndexMap.count(cmdID) != 0;

	if (bIsFavorite && !alreadyFavorited)
	{
		// adding new element
		m_favoriteIndexMap.push_back(cmdID);
		m_commandToIndexMap[cmdID] = m_favoriteIndexMap.size() - 1;
	}
	else if (!bIsFavorite && alreadyFavorited)
	{
		// removing element
		if (m_commandToIndexMap[cmdID] == m_favoriteIndexMap.size() - 1)
		{
			m_commandToIndexMap.erase(cmdID);
			m_favoriteIndexMap.pop_back();
		}
		else
		{
			m_favoriteIndexMap.erase(std::remove(m_favoriteIndexMap.begin(), m_favoriteIndexMap.end(), cmdID), m_favoriteIndexMap.end());
			m_commandToIndexMap.clear();
			for (size_t i = 0; i < m_favoriteIndexMap.size(); i++)
			{
				m_commandToIndexMap[m_favoriteIndexMap[i]] = i;
			}
		}
	}
}

bool FavoriteCommandRegistry::IsFavorite(const long cmdID) const
{
	std::shared_lock lock{ m_mutex };
	return m_commandToIndexMap.count(cmdID) > 0;
}

std::vector<long> FavoriteCommandRegistry::GetListOfFavoriteCommands() const
{
	std::shared_lock lock{ m_mutex };
	return m_favoriteIndexMap;
}
