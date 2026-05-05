#include <unordered_map>
#include <vector>

class CommandManager 
{
	using CmdID = long;
	using Index = size_t;
public:
	static CommandManager& Instance() 
	{
		static CommandManager pInstance;
		return pInstance;
	}

	CommandManager(const CommandManager&) = delete;
	CommandManager(CommandManager&&) = delete;
	CommandManager& operator=(const CommandManager&) = delete;
	CommandManager& operator=(CommandManager&&) = delete;
	
	void SetCmdFavoriteProperty(const long cmdID, const bool bIsFavorite);
	bool GetCmdFavoriteProperty(const long cmdID) const;
	const std::vector<long>& GetListOfFavoriteCommands() const;

private:
	CommandManager();
	~CommandManager();

private:
	std::unordered_map<CmdID, Index> m_commandToIndexMap;
	std::vector<CmdID> m_favoriteCommandIDs;
	
};