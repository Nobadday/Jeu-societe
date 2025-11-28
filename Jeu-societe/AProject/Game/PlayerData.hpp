#ifndef PLAYERDATA_HPP
#define PLAYERDATA_HPP

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
			CHARACTER_4_2
		};

		int m_joystickId;
		PlayerSkin m_playerSkin;

	public:
		PlayerData(void);
		~PlayerData(void);

		void SetJoystickID(int _id);
		int GetJoystickId();

		void SetPlayerSkin(PlayerSkin _skin);
		PlayerSkin GetPlayerSkin();
	private:
};
#endif // !PLAYERDATA_HPP