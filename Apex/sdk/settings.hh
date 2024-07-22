struct s_settings {
	bool showmenu = false;

	/// AIMBOT
	bool aimbot = true;
	float AimBotSmooth = 8;
	float AimBone = 1;
	bool misc_showfov = true;
	float aimbotfov = 100;

	//int rcs = 35; // Aimbot smoothness

	///VISUALS
	bool Player_glow = false;
	bool DistanceESP = false;
	bool LootGlow = false;
	bool LootESP = false;
	bool LootDeadbox = true;
	float LootDistance = 200;
	bool visuals_box = false;
	int box_mode = 1;
	bool SeerHealth = false;
	float DistanceESPFL = 300.f;
	bool  ArmorBox = false;
} settings;