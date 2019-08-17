#pragma once

#include "alife_registry_container_composition.h"

namespace Cordis
{
namespace Scripts
{
namespace Globals
{
inline std::uint32_t vertex_in_direction(
    const std::uint32_t& level_vertex_id, Fvector& direction, const float& max_distance)
{
    direction.normalize_safe();
    direction.mul(max_distance);
    Fvector start_position = ai().level_graph().vertex_position(level_vertex_id);
    Fvector finish_position = Fvector(start_position).add(direction);
    u32 result = u32(-1);
    ai().level_graph().farthest_vertex_in_direction(level_vertex_id, start_position, finish_position, result, 0);
    return (ai().level_graph().valid_vertex_id(result) ? result : level_vertex_id);
}

inline float yaw(const Fvector& vector1, const Fvector& vector2)
{
    return (acos((vector1.x * vector2.x) + (vector1.z * vector2.z)) /
        (sqrt((vector1.x * vector1.x) + (vector1.z * vector1.z)) *
            sqrt((vector2.x * vector2.x) + (vector2.z * vector2.z))));
}

inline float yaw_degree(const Fvector& vector1, const Fvector& vector2)
{
    return (acos((vector1.x * vector2.x) + (vector1.z * vector2.z)) /
               (sqrt((vector1.x * vector1.x) + (vector2.z * vector2.z)) *
                   sqrt((vector2.x * vector2.x) + (vector2.z * vector2.z)))) *
        57.2957f;
}

// @ 2D
inline Fvector vector_cross(const Fvector& vector1, const Fvector& vector2)
{
    return Fvector().set(vector1.y * vector2.z - vector1.z * vector2.y, vector1.z * vector2.x - vector1.x * vector2.z,
        vector1.x * vector2.y - vector1.y * vector2.x);
}

inline Fvector vector_rotate_y(Fvector& vector, float& angle)
{
    // Lord: если это константа то определить что это
    angle *= 0.017453292519943295769236907684886f;
    float cos_result = cos(angle);
    float sin_result = sin(angle);

    return Fvector().set(
        vector.x * cos_result - vector.z * sin_result, vector.y, vector.x * sin_result + vector.z * cos_result);
}

inline Fvector vertex_position(u32 level_vertex_id) { return (ai().level_graph().vertex_position(level_vertex_id)); }
inline bool patrol_path_exists(LPCSTR patrol_path) { return (!!ai().patrol_paths().path(patrol_path, true)); }

inline void load_sound(void)
{
    CInifile sound_ini = CInifile("misc\\script_sound.ltx");

    if (!sound_ini.section_exist("list"))
    {
        R_ASSERT2(false, "There is no list. Please check your config (misc\\script_sound.ltx)!");
    }

    std::uint32_t count_line = sound_ini.line_count("list");
    xr_string id, value, category;

    for (int i = 0; i < count_line; ++i)
    {
        xr_string section;
        xr_string value;
        pcstr s;
        pcstr v;
        bool result = !!sound_ini.r_line("list", i, &s, &v);
        if (result)
        {
            section = s;
            value = v;

            xr_string sound_type_name = Globals::Utils::cfg_get_string(&sound_ini, section, "type");

            if (sound_type_name == "npc")
                Script_SoundThemeDataBase::getInstance().getTheme()[section] = new Script_SoundNPC(sound_ini, section);
            else if (sound_type_name == "actor")
                Script_SoundThemeDataBase::getInstance().getTheme()[section] =
                    new Script_SoundActor(sound_ini, section);
            else if (sound_type_name == "3d")
                Script_SoundThemeDataBase::getInstance().getTheme()[section] =
                    new Script_SoundObject(sound_ini, section);
            else if (sound_type_name == "looped")
                Script_SoundThemeDataBase::getInstance().getTheme()[section] =
                    new Script_SoundLooped(sound_ini, section);
        }
    }
}

inline void init_npc_sound(CScriptGameObject* npc)
{
    for (std::pair<xr_string, Script_ISoundEntity*> it : Script_SoundThemeDataBase::getInstance().getTheme())
    {
        if (it.second)
        {
            if (it.second->getSoundType() == SCRIPTSOUNDTYPE_NPC)
            {
                if (it.second->getAvailCommunities()[character_community(npc)])
                {
                    it.second->initialize_npc(npc);
                }
            }
        }
    }
}

inline bool has_alife_info(LPCSTR info_id)
{
    const KNOWN_INFO_VECTOR* known_info =
        ai().alife().registry<CALifeAbstractRegistry<u16, KNOWN_INFO_VECTOR>>(NULL).object(0, true);

    if (!known_info)
    {
        R_ASSERT2(false, "object in null!");
        return false;
    }

    if (!known_info->size())
        return (false);

    if (std::find_if(known_info->begin(), known_info->end(), CFindByIDPred(info_id)) == known_info->end())
        return (false);

    return (true);
}

inline int get_general_goodwill_between(const std::uint16_t& from, const std::uint16_t& to)
{
    CHARACTER_GOODWILL presonal_goodwill = RELATION_REGISTRY().GetGoodwill(from, to);
    VERIFY(presonal_goodwill != NO_GOODWILL);

    CSE_ALifeTraderAbstract* from_obj = smart_cast<CSE_ALifeTraderAbstract*>(ai().alife().objects().object(from));
    CSE_ALifeTraderAbstract* to_obj = smart_cast<CSE_ALifeTraderAbstract*>(ai().alife().objects().object(to));

    if (!from_obj || !to_obj)
    {
        //         GEnv.ScriptEngine->script_log(LuaMessageType::Error,
        //             "RELATION_REGISTRY::get_general_goodwill_between  : cannot convert obj to
        //             CSE_ALifeTraderAbstract!");
        R_ASSERT2(false, "object is null!");
        return (0);
    }
    CHARACTER_GOODWILL community_to_obj_goodwill = RELATION_REGISTRY().GetCommunityGoodwill(from_obj->Community(), to);
    CHARACTER_GOODWILL community_to_community_goodwill =
        RELATION_REGISTRY().GetCommunityRelation(from_obj->Community(), to_obj->Community());

    return presonal_goodwill + community_to_obj_goodwill + community_to_community_goodwill;
}

inline bool check_squad_for_enemies(CSE_ALifeOnlineOfflineGroup* squad)
{
    if (!squad)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    for (std::pair<std::uint16_t, CSE_ALifeMonsterAbstract*> it : squad->squad_members())
    {
        if (it.second)
        {
            if (get_general_goodwill_between(it.second->ID, ai().alife().graph().actor()->ID))
                return true;
        }
    }

    return false;
}

inline bool IsMonster(CScriptGameObject* object, int class_id = 0)
{
    if (!object)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    int result = class_id ? class_id : object->clsid();

    return (Script_GlobalHelper::getInstance().getMonsterClasses()[result] == true);
}

inline bool IsStalker(CScriptGameObject* object, int class_id = 0)
{
    if (!object)
    {
        R_ASSERT2(false, "object in null!");
        return false;
    }

    int result = class_id ? class_id : object->clsid();

    return (Script_GlobalHelper::getInstance().getStalkerClasses()[class_id] == true);
}

inline bool IsStalker(CSE_ALifeDynamicObject* server_object, int class_id = 0)
{
    if (!server_object)
    {
        R_ASSERT2(false, "object was null!");
        return false;
    }

    int result = class_id ? class_id : server_object->m_script_clsid;

    return (Script_GlobalHelper::getInstance().getStalkerClasses()[class_id] == true);
}

inline bool IsArtefact(CScriptGameObject* object, int class_id = 0)
{
    if (!object)
    {
        R_ASSERT2(false, "object in null!");
        return false;
    }

    int result = class_id ? class_id : object->clsid();

    return (Script_GlobalHelper::getInstance().getArtefactClasses()[class_id] == true);
}

inline bool IsWeapon(CScriptGameObject* object, int class_id = 0)
{
    if (!object)
    {
        R_ASSERT2(false, "object is null!");
        return false;
    }

    int result = class_id ? class_id : object->clsid();

    return (Script_GlobalHelper::getInstance().getWeaponClasses()[class_id] == true);
}

inline xr_string character_community(CScriptGameObject* object)
{
    if (!object)
    {
        R_ASSERT2(false, "object in null!");
        return "monster";
    }

    if (IsStalker(object))
    {
        return object->CharacterCommunity();
    }

    return "monster";
}

inline bool is_level_present(void) { return (!!g_pGameLevel); }

inline bool is_device_paused(void) { return !!Device.Paused(); }

inline void set_device_paused(bool b) { Device.Pause(b, TRUE, FALSE, "set_device_paused_script"); }

inline const CInifile* get_system_ini(void) { return (pSettings); }

inline static xr_map<xr_string, unsigned int>& SaveMarkers(void) noexcept
{
    static xr_map<xr_string, unsigned int> save_markers;
    return save_markers;
}

inline static xr_map<xr_string, xr_string>& Schemes(void) noexcept
{
    static xr_map<xr_string, xr_string> schemes;
    return schemes;
}

inline static xr_map<xr_string, unsigned int>& STypes(void) noexcept
{
    static xr_map<xr_string, unsigned int> stypes;
    return stypes;
}

inline static xr_map<xr_string, Cordis::AnyCallable<void>>& getXREffects_Functions(void) noexcept
{
    static xr_map<xr_string, Cordis::AnyCallable<void>> functions;
    return functions;
}

inline void load_scheme(const xr_string& filename, const xr_string& scheme, unsigned int stype)
{
    Schemes()[scheme] = filename;
    STypes()[scheme] = stype;
}

inline void StartGameCallback(void)
{
    // Loads schemes Lord: переименовать соответвующие наименования скриптов, но если _G[schemes[scheme]] -> нужно
    // расписывать через case, потому что строка будет указывать на namespace или getInstance() самого класса То есть
    // switch (scheme_name) { case "Script_StoryObject": Script_StoryObject::getInstance().DoSomething(); etc... };
    load_scheme("xr_danger", "danger", kSTypeStalker);
    load_scheme("xr_gather_items", "gather_items", kSTypeStalker);
    load_scheme("xr_abuse", "abuse", kSTypeStalker);
    load_scheme("xr_walker", "walker", kSTypeStalker);
    load_scheme("xr_remark", "remark", kSTypeStalker);
    load_scheme("xr_cover", "cover", kSTypeStalker);
    load_scheme("xr_sleeper", "sleeper", kSTypeStalker);
    load_scheme("xr_kamp", "kamp", kSTypeStalker);
    load_scheme("xr_camper", "camper", kSTypeStalker);
    load_scheme("xr_meet", "meet", kSTypeStalker);
    load_scheme("xr_help_wounded", "help_wounded", kSTypeStalker);
    load_scheme("xr_corpse_detection", "corpse_detection", kSTypeStalker);
    load_scheme("xr_combat", "combat", kSTypeStalker);
    load_scheme("xr_death", "death", kSTypeStalker);
    load_scheme("xr_hit", "hit", kSTypeStalker);
    load_scheme("xr_wounded", "wounded", kSTypeStalker);
    load_scheme("xr_meet", "actor_dialogs", kSTypeStalker);
    load_scheme("xr_combat_ignore", "combat_ignore", kSTypeStalker);
    load_scheme("xr_combat_zombied", "combat_zombied", kSTypeStalker);
    load_scheme("xr_patrol", "patrol", kSTypeStalker);
    load_scheme("xr_smartcover", "smartcover", kSTypeStalker);
    load_scheme("xr_companion", "companion", kSTypeStalker);
    load_scheme("xr_animpoint", "animpoint", kSTypeStalker);
    load_scheme("xr_reach_task", "reach_task", kSTypeStalker);

    load_scheme("mob_remark", "mob_remark", kSTypeMobile);
    load_scheme("mob_walker", "mob_walker", kSTypeMobile);
    load_scheme("mob_combat", "mob_combat", kSTypeMobile);
    load_scheme("mob_death", "mob_death", kSTypeMobile);
    load_scheme("mob_jump", "mob_jump", kSTypeMobile);
    load_scheme("mob_home", "mob_home", kSTypeMobile);

    load_scheme("ph_door", "ph_door", kSTypeItem);
    load_scheme("ph_idle", "ph_idle", kSTypeItem);
    load_scheme("ph_hit", "ph_hit", kSTypeItem);
    load_scheme("ph_on_hit", "ph_on_hit", kSTypeItem);
    load_scheme("ph_button", "ph_button", kSTypeItem);
    load_scheme("ph_code", "ph_code", kSTypeItem);
    load_scheme("ph_death", "ph_on_death", kSTypeItem);
    load_scheme("ph_minigun", "ph_minigun", kSTypeItem);
    // load_scheme("ph_target","ph_target",kSTypeItem);
    load_scheme("ph_oscillate", "ph_oscillate", kSTypeItem);

    load_scheme("heli_move", "heli_move", kSTypeHelicopter);

    load_scheme("sr_no_weapon", "sr_no_weapon", kSTypeRestrictor);
    load_scheme("sr_teleport", "sr_teleport", kSTypeRestrictor);
    load_scheme("sr_idle", "sr_idle", kSTypeRestrictor);
    load_scheme("sr_light", "sr_light", kSTypeRestrictor);
    load_scheme("sr_timer", "sr_timer", kSTypeRestrictor);
    load_scheme("sr_psy_antenna", "sr_psy_antenna", kSTypeRestrictor);
    load_scheme("sr_postprocess", "sr_postprocess", kSTypeRestrictor);
    load_scheme("sr_particle", "sr_particle", kSTypeRestrictor);
    load_scheme("sr_cutscene", "sr_cutscene", kSTypeRestrictor);
    // load_scheme("sr_bloodsucker","sr_bloodsucker",kSTypeRestrictor);
    load_scheme("sr_monster", "sr_monster", kSTypeRestrictor);
    // load_scheme("sr_robbery","sr_robbery",kSTypeRestrictor);
    // load_scheme("sr_survival","sr_survival",kSTypeRestrictor);
    load_scheme("sr_crow_spawner", "sr_crow_spawner", kSTypeRestrictor);
    // load_scheme("sr_shooting","sr_shooting",kSTypeRestrictor);
    // load_scheme("sr_recoveritem","sr_recoveritem",kSTypeRestrictor);
    load_scheme("sr_silence", "sr_silence", kSTypeRestrictor);
    load_scheme("sr_deimos", "sr_deimos", kSTypeRestrictor);
}

inline void set_save_marker(NET_Packet& packet, const xr_string& mode, bool check, const xr_string& prefix)
{
    xr_string result = "_";
    result.append(prefix);

    if (check)
    {
        if (SaveMarkers()[result] == 0)
        {
            R_ASSERT3(false, "Trying to check without marker ", result.c_str());
        }

        if (mode == kSaveMarkerMode_Save)
        {
            unsigned int delta = packet.w_tell() - SaveMarkers()[result];
            // Lord: переделать в In-Game логгирование
            xr_string info = "";
            info.append(result);
            info.append(":SAVE DELTA: ");
            Log(info.c_str(), delta);

            if (delta >= 8000)
            {
                // Lord: переделать в In-Game логгирование
                Log("WARNING! Maybe this is problem save point");
            }

            if (delta >= 10240)
            {
                // empty code here
                // R_ASSERT2(false, "You are saving too much");
            }
            packet.w_u16(delta);
        }
        else
        {
            const unsigned int c_delta = packet.r_tell() - SaveMarkers()[result];
            unsigned int delta = packet.r_u16();

            if (delta != c_delta)
            {
                Msg("INCORRECT LOAD [%s].[%d][%d]", result.c_str(), delta, c_delta);
                R_ASSERT(false);
            }
            else
            {
                xr_string info = "";
                info.append(result);
                info.append(": LOAD DELTA: %d");
                Msg(info.c_str(), delta);
            }
        }
        return;
    }

    if (mode == kSaveMarkerMode_Save)
    {
        xr_string info = "";
        info.append(result);
        info.append(": set save marker: %d");
        Msg(info.c_str(), packet.w_tell());
        SaveMarkers()[result] = packet.w_tell();

        if (packet.w_tell() > 16000)
        {
            R_ASSERT2(false, "You are trying save too much");
        }
    }
    else
    {
        xr_string info = "";
        info.append(result);
        info.append(": set load marker: %d");
        Msg(info.c_str(), packet.r_tell());
        SaveMarkers()[result] = packet.r_tell();
    }
}

inline bool is_on_the_same_level(CSE_ALifeObject* object1, CSE_ALifeObject* object2)
{
    if (!object1 || !object2)
    {
        R_ASSERT2(false, "Object was null!");
        return false;
    }

    return (ai().game_graph().vertex(object1->m_tGraphID)->level_id() ==
        ai().game_graph().vertex(object2->m_tGraphID)->level_id());
}

inline float sim_dist_to(CSE_ALifeObject* object1, CSE_ALifeObject* object2)
{
    if (!object1 || !object2)
    {
        R_ASSERT2(false, "Object was null!");
        return 0.0f;
    }

    std::uint16_t gamevertexid_1 = object1->m_tGraphID;
    std::uint16_t gamevertexid_2 = object2->m_tGraphID;

    return Utils::graph_distance(gamevertexid_1, gamevertexid_2);
}

inline ESingleGameDifficulty get_game_difficulty() { return g_SingleGameDifficulty; }

inline std::uint32_t get_time_days()
{
    std::uint32_t year = 0, month = 0, day = 0, hours = 0, mins = 0, secs = 0, milisecs = 0;
    split_time((g_pGameLevel && Level().game) ? Level().GetGameTime() : ai().alife().time_manager().game_time(), year,
        month, day, hours, mins, secs, milisecs);
    return day;
}

inline std::uint32_t get_time_hours()
{
    std::uint32_t year = 0, month = 0, day = 0, hours = 0, mins = 0, secs = 0, milisecs = 0;
    split_time((g_pGameLevel && Level().game) ? Level().GetGameTime() : ai().alife().time_manager().game_time(), year,
        month, day, hours, mins, secs, milisecs);
    return hours;
}

inline std::uint32_t get_time_minutes()
{
    std::uint32_t year = 0, month = 0, day = 0, hours = 0, mins = 0, secs = 0, milisecs = 0;
    split_time((g_pGameLevel && Level().game) ? Level().GetGameTime() : ai().alife().time_manager().game_time(), year,
        month, day, hours, mins, secs, milisecs);
    return mins;
}

inline bool in_time_interval(const std::uint32_t& value1, const std::uint32_t& value2)
{
    std::uint32_t game_hours = get_time_hours();

    if (value1 >= value2)
    {
        return ((game_hours < value2) || (game_hours >= value1));
    }
    else
    {
        return ((game_hours < value2) && (game_hours >= value1));
    }

    return false;
}

inline xr_string get_job_restrictor(const xr_string& waypoint_name)
{ get_job_restrictor(waypoint_name.c_str()); }

inline xr_string get_job_restrictor(const char* waypoint_name) 
{
    if (!waypoint_name)
    {
        R_ASSERT2(false, "string can't be empty!");
        return xr_string("");
    }

    const Fvector& position = CPatrolPathParams(waypoint_name).point(std::uint32_t(0));

    for (const std::pair<xr_string, CScriptGameObject*>& it :
        Script_GlobalHelper::getInstance().getGameRegisteredCombatSpaceRestrictors())
    {
        if (it.second)
        {
            if (it.second->inside(position))
            {
                return it.first;
            }
        }
    }

    return xr_string("");
}

} // namespace Globals
} // namespace Scripts
} // namespace Cordis
