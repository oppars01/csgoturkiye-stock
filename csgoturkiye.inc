stock char s_tag[ 32 ], s_tag_color[ 16 ], s_plugin_image[ PLATFORM_MAX_PATH ], s_plugin_url[ 255 ];

stock void PluginSetting(){
    Format(s_tag, sizeof(s_tag), "[ csgo-turkiye.com ]");
    Format(s_tag_color, sizeof(s_tag_color), "{orange}");
    Format(s_plugin_url, sizeof(s_plugin_url), "https://csgo-turkiye.com");
    Format(s_plugin_image, sizeof(s_plugin_image), "https://csgo-turkiye.com/site-images/icon3");
}

stock bool IsValidClient(int client, bool nobots = true)
{
	if (client <= 0 || client > MaxClients || !IsClientConnected(client) || (nobots && IsFakeClient(client)))return false;
	return IsClientInGame(client);
}

stock bool CheckAdminFlag(int client, const char[] flags)
{
    if(!StrEqual(flags,"-")){
        char s_flag_format[32],s_flag_need[32][2];
        Format(s_flag_format, sizeof(s_flag_format), flags);
        ReplaceString(s_flag_format, sizeof(s_flag_format), " ", "");
        int i_count = ExplodeString(s_flag_format, ",", s_flag_need, sizeof(s_flag_need), sizeof(s_flag_need[]));
        for (int i = 0; i < i_count; i++)if ((GetUserFlagBits(client) & ReadFlagString(s_flag_need[i])) || (GetUserFlagBits(client) & ADMFLAG_ROOT))return true;
        return false;
    }else{
        if(GetUserAdmin(client) != INVALID_ADMIN_ID)return true;
        else return false;
    }
}

stock int GetAliveTeamCount(int team)
{
    int count = 0;
    for (int i=1; i<=MaxClients; i++)if (IsValidClient(i) && !IsFakeClient(i) && IsPlayerAlive(i) && GetClientTeam(i) == team) count++;
    return count;
}

stock void GetAimCoords(int client, float vector[3])
{
    float vAngles[3];
    float vOrigin[3];
    GetClientEyePosition(client, vOrigin);
    GetClientEyeAngles(client, vAngles);
    Handle trace = TR_TraceRayFilterEx(vOrigin, vAngles, MASK_SHOT, RayType_Infinite, TraceEntityFilterPlayer);
    if (TR_DidHit(trace))
        TR_GetEndPosition(vector, trace);
    trace.Close();
}

stock bool TraceEntityFilterPlayer(int entity, int contentsMask)
{
    return entity > MaxClients;
}

stock void Discord_EscapeString(char[] string, int maxlen)
{
    ReplaceString(string, maxlen, "@", "＠");
    ReplaceString(string, maxlen, "'", "＇");
    ReplaceString(string, maxlen, "\"", "＂");
    TrimString(string);
}

stock void PluginUnload(){
    char s_plugin_name[ 256 ];
    GetPluginFilename(INVALID_HANDLE, s_plugin_name, sizeof(s_plugin_name));
    ServerCommand("sm plugins unload %s", s_plugin_name);
}

stock void weaponClear(int client)
{
	for (int j = 0; j < 5; j++)
	{
		int weapon = GetPlayerWeaponSlot(client, j);
		if (weapon != -1)
		{
			RemovePlayerItem(client, weapon);
			RemoveEdict(weapon);
		}
	}
}