#ifndef PLAYERDATA_HPP
#define PLAYERDATA_HPP

#include <string>


#include "Bot/BotAI.hpp"


class PlayerData
{
	public:
		enum PlayerSkin
		{
			CHARACTER_1_1,
			CHARACTER_2_1,
			CHARACTER_3_1,
			CHARACTER_4_1,

			CHARACTER_1_2,
			CHARACTER_2_2,
			CHARACTER_3_2,
			CHARACTER_4_2,

			PLAYERSKINS_COUNT
		};

		int m_joystickId;
		PlayerSkin m_playerSkin;

		bool m_isBot = false;
		BotAI m_botAI;

	public:
		PlayerData(void);
		~PlayerData(void);

		void SetJoystickID(int _id);
		int GetJoystickId();

		void SetPlayerSkin(PlayerSkin _skin);
		PlayerSkin GetPlayerSkin();
		bool IsBot();
		
		// NOUVEAU : Initialiser le bot avec sa difficulté
		void InitializeAsBot(int playerIndex, BotDifficulty difficulty);

		static const std::string& GetTextureName(PlayerData::PlayerSkin _skin);
	private:

};
#endif // !PLAYERDATA_HPP