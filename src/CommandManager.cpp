#include "CommandManager.h"
#include <shared_mutex>
#include <iostream>
#include <mutex>
#include <algorithm>

std::shared_mutex m_mutex;

CommandManager::CommandManager()
{
}

CommandManager::~CommandManager()
{
}

void CommandManager::SetCmdFavoriteProperty(const long cmdID, const bool bIsFavorite)
{
	std::unique_lock lock{ m_mutex };
	bool bExists = m_commandToIndexMap.count(cmdID) != 0;

	if (bIsFavorite && !bExists)
	{
		// adding new element
		m_favoriteCommandIDs.push_back(cmdID);
		m_commandToIndexMap[cmdID] = m_favoriteCommandIDs.size() - 1;
	}
	else if (!bIsFavorite && bExists)
	{
		// removing element
		if (m_commandToIndexMap[cmdID] == m_favoriteCommandIDs.size() - 1)
		{
			m_commandToIndexMap.erase(cmdID);
			m_favoriteCommandIDs.pop_back();
		}
		else
		{
			m_favoriteCommandIDs.erase(std::remove(m_favoriteCommandIDs.begin(), m_favoriteCommandIDs.end(), cmdID));
			m_commandToIndexMap.clear();
			for (auto i = 0; i < m_favoriteCommandIDs.size(); i++)
			{
				m_commandToIndexMap[m_favoriteCommandIDs[i]] = i;
			}
		}
	}
}

bool CommandManager::GetCmdFavoriteProperty(const long cmdID) const
{
	std::shared_lock lock{ m_mutex };
	return m_commandToIndexMap.count(cmdID) > 0;
}

const std::vector<long>& CommandManager::GetListOfFavoriteCommands() const
{
	std::shared_lock lock{ m_mutex };
	return m_favoriteCommandIDs;
}
