#pragma once

namespace Cordis
{
namespace Scripts
{
namespace DataBase
{
class Storage_Scheme;
}
} // namespace Scripts
} // namespace Cordis

#include "script_sound.h"
#include "Script_ISchemeEntity.h"
#include "Script_SchemePHDoor.h"
#include "Script_MoveManager.h"
#include "Script_Binder_AnomalZone.h"
#include "Script_Binder_AnomalField.h"
#include "Script_Binder_DoorLabx8.h"
#include "Script_TaskManager.h"

namespace Cordis
{
namespace Scripts
{
namespace DataBase
{
// Lord: зачем оно и что это в итоге?
inline void add_enemy(CSE_Abstract* object)
{
    if (!object)
    {
        R_ASSERT2(false, "can't be null!");
        return;
    }
}

class Data_Overrides
{
public:
    Data_Overrides(void) = default;
    ~Data_Overrides(void) {}

    inline std::uint32_t getMinPostCombatTime(void) const noexcept { return this->m_min_post_combat_time; }
    inline void setMinPostCombatTime(const std::uint32_t value) noexcept { this->m_min_post_combat_time = value; }

    inline std::uint32_t getMaxPostCombatTime(void) const noexcept { return this->m_max_post_combat_time; }
    inline void setMaxPostCombatTime(const std::uint32_t value) noexcept { this->m_max_post_combat_time = value; }

    inline const xr_map<std::uint32_t, CondlistData>& getOnOfflineCondlist(void) const noexcept
    {
        return this->m_on_offline_condlist;
    }
    inline void setOnOfflineCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        this->m_on_offline_condlist = condlist;
    }

    inline const LogicData& getCombatIgnore(void) const noexcept { return this->m_combat_ignore; }
    inline void setCombatIgnore(const LogicData& data) noexcept { this->m_combat_ignore = data; }

    inline bool getCombatIgnoreKeepWhenAttacked(void) const noexcept
    {
        return this->m_combat_ignore_keep_when_attacked;
    }
    inline void setCombatIgnoreKeepWhenAttacked(const bool value) noexcept
    {
        this->m_combat_ignore_keep_when_attacked = value;
    }

    inline const LogicData& getCombatType(void) const noexcept { return this->m_combat_type; }
    inline void setCombatType(const LogicData& data) noexcept { this->m_combat_type = data; }

    inline const LogicData& getOnCombat(void) const noexcept { return this->m_on_combat; }
    inline void setOnCombat(const LogicData& data) noexcept { this->m_on_combat = data; }

    inline const xr_map<std::uint32_t, CondlistData>& getHelicopterHunterCondlist(void) const noexcept
    {
        return this->m_helicopter_hunter_condlist;
    }

    inline void setHelicopterHunterCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        this->m_helicopter_hunter_condlist = condlist;
    }

    inline const xr_string& getSoundGroupName(void) const noexcept { return this->m_sound_group_name; }
    inline void setSoundGroupName(const xr_string& sound_group_name) noexcept
    {
        if (sound_group_name.empty())
        {
            Msg("[Scripts/Data_Overrides/setSoundGroupName(sound_group_name)] WARNING: sound_group_name.empty() == "
                "true! You set an empty string!");
        }

        this->m_sound_group_name = sound_group_name;
    }

    inline bool isEmpty(void) const noexcept
    {
        return ((!this->m_combat_ignore_keep_when_attacked) && (!this->m_min_post_combat_time) &&
            (!this->m_max_post_combat_time) && (this->m_helicopter_hunter_condlist.empty()) &&
            (this->m_on_offline_condlist.empty()) && (this->m_sound_group_name.empty()) &&
            (this->m_combat_ignore.IsEmpty()) && (this->m_combat_type.IsEmpty()) && (this->m_on_combat.IsEmpty()));
    }

private:
    bool m_combat_ignore_keep_when_attacked = false;
    std::uint32_t m_min_post_combat_time = 0;
    std::uint32_t m_max_post_combat_time = 0;
    xr_map<std::uint32_t, CondlistData> m_helicopter_hunter_condlist;
    xr_map<std::uint32_t, CondlistData> m_on_offline_condlist;
    xr_string m_sound_group_name;
    LogicData m_combat_ignore;
    LogicData m_combat_type;
    LogicData m_on_combat;
};

class Script_XRAbuseManager
{
public:
    Script_XRAbuseManager(void) = delete;
    Script_XRAbuseManager(CScriptGameObject* const p_client_object, DataBase::Storage_Scheme& storage)
        : m_p_npc(p_client_object), m_p_storage(&storage), m_is_enable(true), m_is_hit_done(false), m_abuse_rate(2.0f),
          m_abuse_threshold(5.0f), m_abuse_value(0.0f), m_last_update(0.0f)
    {
    }
    ~Script_XRAbuseManager(void) {}

    inline void setAbuseRate(const float value) noexcept { this->m_abuse_rate = value; }
    inline bool IsAbused(void) const noexcept { return (this->m_abuse_value >= this->m_abuse_threshold); }
    inline bool update(void) noexcept
    {
        if (!this->m_last_update)
            this->m_last_update = static_cast<float>(Globals::get_time_global());

        if (this->m_abuse_value)
            this->m_abuse_value =
                this->m_abuse_value - (static_cast<float>(Globals::get_time_global()) - this->m_last_update) * 0.00005f;
        else
            this->m_abuse_value = 0.0f;

        if (this->m_abuse_value > this->m_abuse_threshold * 1.1f)
            this->m_abuse_value = this->m_abuse_threshold * 1.1f;

        if (this->m_is_hit_done && (this->m_abuse_value < this->m_abuse_threshold * 2.0f / 3.0f))
            this->m_is_hit_done = false;

        this->m_last_update = static_cast<float>(Globals::get_time_global());

        if (this->IsAbused())
            return true;

        return false;
    }

    inline void AddAbuse(const float value) noexcept
    {
        if (this->m_is_enable)
            this->m_abuse_value = this->m_abuse_value + (value * this->m_abuse_rate);
    }

    inline void ClearAbuse(void) noexcept { this->m_abuse_value = 0.0f; }
    inline void EnableAbuse(void) noexcept { this->m_is_enable = true; }
    inline void DisableAbuse(void) noexcept { this->m_is_enable = false; }

private:
    bool m_is_enable;
    bool m_is_hit_done;
    float m_last_update;
    float m_abuse_value;
    float m_abuse_threshold;
    float m_abuse_rate;
    CScriptGameObject* m_p_npc;
    DataBase::Storage_Scheme* m_p_storage;
};

// Lord: сгруппировать всё по pragma region!!!
class Storage_Scheme
{
public:
    ~Storage_Scheme(void)
    {
        this->m_p_ini =
            nullptr; // @ Контролиться в Script_SE_SmartTerrain, либо от this->m_ltx, либо от this->m_job_data

        if (!this->m_actions.empty())
        {
            for (Script_ISchemeEntity* it : this->m_actions)
            {
                if (it)
                {
                    Msg("[Scripts/DataBase/Storage_Scheme/~dtor()] deleting scheme %s from actions",
                        it->getSchemeName().c_str());
                    xr_delete(it);
                }
            }
        }

        if (this->m_p_jump_path)
        {
            Msg("[Scripts/DataBase/Storage_Scheme/~dtor()] deleting CPatrolPathParams m_p_jump_path %s",
                this->m_p_jump_path->m_path_name);
            xr_delete(this->m_p_jump_path);
        }

        if (this->m_p_abuse_manager)
        {
            Msg("[Scripts/DataBase/Storage_Scheme/~dtor()] deleting abuse manager from %s", this->m_p_npc->Name());
            xr_delete(this->m_p_abuse_manager);
        }

        this->m_p_npc = nullptr;
    }

    inline const xr_vector<Script_ISchemeEntity*>& getActions(void) const noexcept { return this->m_actions; }

    inline void setActions(Script_ISchemeEntity* p_scheme)
    {
        if (!p_scheme)
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setActions(p_scheme)] WARNING: you can't assign nullptr object "
                "return ...!");
            return;
        }

        this->m_actions.push_back(p_scheme);
    }

    inline void setActionSchemeID(const std::uint32_t scheme_id) noexcept
    {
        if (scheme_id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setActionShemeID(scheme_id)] WARNING: something not right. overflow "
                "sheat ...");
        }

        this->m_scheme_id_for_unsubscring = scheme_id;
    }

    inline void UnSubscribeAction(void) noexcept
    {
        if (!this->m_scheme_id_for_unsubscring || this->m_scheme_id_for_unsubscring == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Scripts/DataBase/Storage_Scheme/UnSubscribeAction()] WARNING: can't unsubscribe action, which not "
                "registered with ID ... YOUR ID is [%d]",
                this->m_scheme_id_for_unsubscring);
            return;
        }

        for (Script_ISchemeEntity* it : this->m_actions)
        {
            if (it)
            {
                if (it->getSchemeID() == this->m_scheme_id_for_unsubscring)
                {
                    Msg("[Scripts/DataBase/Storage_Scheme/UnSubscribeAction()] unsubscribe action for npc %s %d",
                        it->getSchemeName().c_str(), it->getID());
                    it->unsubscribe_action();
                    break;
                }
            }
        }

        Msg("[Scripts/DataBase/Storage_Scheme/UnSubscribeAction()] WARNING: can't find action for unsubscribing YOUR "
            "ACTION ID -> %d",
            this->m_scheme_id_for_unsubscring);
    }

    /*
        inline void setAction(Script_ISchemeEntity* p_scheme)
        {
            if (!p_scheme)
            {
                Msg("[Scripts/DataBase/Storage_Scheme/setAction(p_scheme)] WARNING: you can't assign nullptr object
       return "
                    "...!");
                return;
            }

            this->m_p_action = p_scheme;
        }*/

    inline const xr_map<xr_string, bool>& getSignals(void) const noexcept { return this->m_signals; }

    inline void setSignals(const xr_map<xr_string, bool>& map) noexcept
    {
        if (map.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSignals(map)] WARNING: map.empty() == true! Can't assign an empty "
                "map "
                "return ...");
            return;
        }

        this->m_signals = map;
    }

    inline void setSignals(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (pair.first.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSignals(pair)] WARNING: pair.first.empty() == true! Can't assign "
                "an empty string return ...");
            return;
        }

        this->m_signals.insert(pair);
    }

    inline void setSignals(const xr_string& signal_name, const bool value) noexcept
    {
        if (signal_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSignals(signal_name, value)] WARNING: signal_name.empty() == "
                "true! Can't assign an empty string return ...");
            return;
        }

        this->m_signals[signal_name] = value;
    }

    inline void ClearSignals(void) noexcept
    {
        Msg("[Scripts/DataBase/Storage_Scheme/ClearSignals(void)] signals are cleared!");
        this->m_signals.clear();
    }

    inline const xr_string& getPathWalkName(void) const noexcept { return this->m_path_walk_name; }
    inline void setPathWalkName(const xr_string& path_walk_name) noexcept
    {
        if (path_walk_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPathWalkName(path_walk_name)] WARNING: path_walk_name.empty() == "
                "true! You are assign an empty string");
        }

        this->m_path_walk_name = path_walk_name;
    }

    inline const xr_string& getPathLookName(void) const noexcept { return this->m_path_look_name; }
    inline void setPathLookName(const xr_string& path_look_name) noexcept
    {
        if (path_look_name.empty())
        {
            Msg("[Scripts/DataBase/Script_Scheme/setPathLookName(path_look_name)] WARNING: path_look_name.empty() == "
                "true! You set an empty string");
        }

        this->m_path_look_name = path_look_name;
    }

    inline const xr_string& getStateName(void) const noexcept { return this->m_state_name; }
    inline void setStateName(const xr_string& state_name) noexcept
    {
        if (state_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setStateName(state_name)] WARNING: state_name.empty() == true! You "
                "set an empty string");
        }

        this->m_state_name = state_name;
    }

    inline const xr_string& getSchemeName(void) const noexcept { return this->m_scheme_name; }
    inline void setSchemeName(const xr_string& scheme_name) noexcept
    {
        if (scheme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSchemeName(scheme_name)] WARNING: scheme_name.empty() == true! "
                "You set an empty string!");
        }

        this->m_scheme_name = scheme_name;
    }

    inline const xr_string& getLogicName(void) const noexcept { return this->m_logic_name; }
    inline void setLogicName(const xr_string& section_name) noexcept
    {
        if (section_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSectionScheme(section_name)] WARNING: section_name.empty() == "
                "true! You set an empty string.");
        }

        this->m_logic_name = section_name;
    }

    inline const xr_string& getSoundName(void) const noexcept { return this->m_sound_name; }
    inline void setSoundName(const xr_string& data_name) noexcept
    {
        if (data_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSoundName(data_name)] WARNING: data_name.empty() == true! You set "
                "an empty string.");
        }

        this->m_sound_name = data_name;
    }

    inline const xr_string& getTipName(void) const noexcept { return this->m_tip_name; }
    inline void setTipName(const xr_string& message_name) noexcept
    {
        if (message_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setTipName(message_name)] WARNING: message_name.empty() == true! You "
                "set an empty string");
        }

        this->m_tip_name = message_name;
    }

    inline const xr_string& getTimeName(void) const noexcept { return this->m_time_name; }
    inline void setTimeName(const xr_string& data_name) noexcept
    {
        if (data_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setTimeName(data_name)] WARNING: data_name.empty() == true! You set "
                "an empty string");
        }

        this->m_time_name = data_name;
    }

    inline const xr_string& getAnimationName(void) const noexcept { return this->m_animation_name; }
    inline void setAnimationName(const xr_string& animation_name) noexcept
    {
        if (animation_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setAnimationName(animation_name)] WARNING: animation_name.empty() == "
                "true! You set an empty string");
        }

        this->m_animation_name = animation_name;
    }

    inline const xr_string& getAnimationHeadName(void) const noexcept { return this->m_animation_head_name; }
    inline void setAnimationHeadName(const xr_string& animation_head_name) noexcept
    {
        if (animation_head_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setAnimationHeadName(animation_head_name)] WARNING: "
                "animation_head_name.empty() == true! You set an empty string");
        }

        this->m_animation_head_name = animation_head_name;
    }

    inline const xr_string& getPathJumpName(void) const noexcept { return this->m_path_jump_name; }
    inline void setPathJumpName(const xr_string& path_name) noexcept
    {
        if (path_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPathJumpName(path_name)] WARNING: path_name.empty() == true! You "
                "set an empty string");
        }

        this->m_path_jump_name = path_name;
    }

    inline const xr_string& getHomeName(void) const noexcept { return this->m_home_name; }
    inline void setHomeName(const xr_string& home_name) noexcept
    {
        if (home_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setHomeNmae(home_name)] WARNING: home_name.empty() == true! You set "
                "an empty string");
        }

        this->m_home_name = home_name;
    }

    inline const CondlistWaypoints& getPathWalkInfo(void) const noexcept { return this->m_path_walk_info; }
    inline void setPathWalkInfo(const CondlistWaypoints& data) noexcept { this->m_path_walk_info = data; }

    inline const CondlistWaypoints& getPathLookInfo(void) const noexcept { return this->m_path_look_info; }
    inline void setPathLookInfo(const CondlistWaypoints& data) noexcept { this->m_path_look_info = data; }

    inline CScriptIniFile* const getIni(void) const { return this->m_p_ini; }
    inline void setIni(CScriptIniFile* const p_ini) { this->m_p_ini = p_ini; }

    inline CScriptGameObject* const getClientObject(void) const { return this->m_p_npc; }
    inline void setClientObject(CScriptGameObject* const p_client_object) { this->m_p_npc = p_client_object; }

    inline CPatrolPathParams* const getJumpPath(void) const { return this->m_p_jump_path; }
    inline void setJumpPath(CPatrolPathParams* const p_path)
    {
        if (!p_path)
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setJumpPath(p_path)] WARNING: p_path == nullptr! Can't set because "
                "deleting is in dtor()!");
            return;
        }

        this->m_p_jump_path = p_path;
    }

    inline bool IsAnimationMovement(void) const noexcept { return this->m_is_animation_movement; }
    inline void setAnimationMovement(const bool value) noexcept { this->m_is_animation_movement = value; }

    inline bool IsNoReset(void) const noexcept { return this->m_is_no_reset; }
    inline void setNoReset(const bool value) noexcept { this->m_is_no_reset = value; }

    inline const xr_map<std::uint32_t, CondlistData>& getDialogCondlist(void) const noexcept
    {
        return this->m_dialog_condlist;
    }

    inline void setDialogCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        this->m_dialog_condlist = condlist;
    }

    inline const Fvector& getOffset(void) const noexcept { return this->m_offset; }
    inline void setOffset(const Fvector& data) noexcept { this->m_offset = data; }

    inline float getPHJumpFactor(void) const noexcept { return this->m_ph_jump_factor; }
    inline void setPHJumpFactor(const float value) noexcept { this->m_ph_jump_factor = value; }

    inline std::uint32_t getHomeMinRadius(void) const noexcept { return this->m_home_min_radius; }
    inline void setHomeMinRadius(const std::uint32_t value) noexcept { this->m_home_min_radius = value; }

    inline std::uint32_t getHomeMidRadius(void) const noexcept { return this->m_home_mid_radius; }
    inline void setHomeMidRadius(const std::uint32_t value) noexcept { this->m_home_mid_radius = value; }

    inline std::uint32_t getHomeMaxRadius(void) const noexcept { return this->m_home_max_radius; }
    inline void setHomeMaxRadius(const std::uint32_t value) noexcept { this->m_home_max_radius = value; }

    inline bool IsAggresive(void) const noexcept { return this->m_is_aggresive; }
    inline void setAggresive(const bool value) noexcept { this->m_is_aggresive = value; }

    inline bool IsGulagPoint(void) const noexcept { return this->m_is_gulag_point; }
    inline void setGulagPoint(const bool value) noexcept { this->m_is_gulag_point = value; }

    inline bool IsEnabled(void) const noexcept { return this->m_is_enabled; }
    inline void setEnabled(const bool value) noexcept { this->m_is_enabled = value; }

    inline const xr_string& getLookPointName(void) const noexcept { return this->m_look_point_name; }
    inline void setLookPointName(const xr_string& point_name) noexcept
    {
        if (point_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setLookPointName(point_name)] WARNING: point_name.empty() == true! "
                "You set an empty string");
        }

        this->m_look_point_name = point_name;
    }

    inline const xr_string& getHomePointName(void) const noexcept { return this->m_home_point_name; }
    inline void setHomePointName(const xr_string& home_name) noexcept
    {
        if (home_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setHomePointName(home_name)] WARNING: home_name.empty() == true! You "
                "set an empty string");
        }

        this->m_home_point_name = home_name;
    }

    inline std::uint32_t getTimeChangePoint(void) const noexcept { return this->m_time_change_point; }
    inline void setTimeChangePoint(const std::uint32_t value) noexcept { this->m_time_change_point = value; }

    inline bool IsSkipTransferEnemy(void) const noexcept { return this->m_is_skip_transfer_enemy; }
    inline void setSkipTransferEnemy(const bool value) noexcept { this->m_is_skip_transfer_enemy = value; }

    inline const xr_string& getHelicopterPathMoveName(void) const noexcept { return this->m_helicopter_path_move_name; }
    inline void setHelicopterPathMoveName(const xr_string& path_name) noexcept
    {
        if (path_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setHelicopterPathMoveName(path_name)] WARNING: path_name.empty() == "
                "true! You set an empty string");
        }

        this->m_helicopter_path_move_name = path_name;
    }

    inline const xr_string& getHelicopterPathLookName(void) const noexcept { return this->m_helicopter_path_look_name; }
    inline void setHelicopterPathLookName(const xr_string& path_name) noexcept
    {
        if (path_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setHelicopterPathLookName(path_name)] WARNING: path_name.empty() == "
                "true! You set an empty string");
        }

        this->m_helicopter_path_look_name = path_name;
    }

    inline const xr_string& getHelicopterEnemyName(void) const noexcept { return this->m_helicopter_enemy_name; }
    inline void setHelicopterEnemyName(const xr_string& enemy_name) noexcept
    {
        if (enemy_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setHelicopterEnemyName(enemy_name)] WARNING: enemy_name.empty() == "
                "true! You set an empty string");
        }

        this->m_helicopter_enemy_name = enemy_name;
    }

    inline const xr_string& getHelicopterFirePointName(void) const noexcept
    {
        return this->m_helicopter_fire_point_name;
    }
    inline void setHelicopterFirePointName(const xr_string& fire_point_name) noexcept
    {
        if (fire_point_name.empty())
        {
            Msg("[scripts/DataBase/Storage_Scheme/setHelicopterFirePointName(fire_point_name)] WARNING: "
                "fire_point_name.empty() == true! You set an empty string");
        }

        this->m_helicopter_fire_point_name = fire_point_name;
    }

    inline bool IsHelicopterUseRocket(void) const noexcept { return this->m_is_helicopter_use_rocket; }
    inline void setHelicopterUseRocket(const bool value) noexcept { this->m_is_helicopter_use_rocket = value; }

    inline bool IsHelicopterUseMinigun(void) const noexcept { return this->m_is_helicopter_use_minigun; }
    inline void setHelicopterUseMinigun(const bool value) noexcept { this->m_is_helicopter_use_minigun = value; }

    inline bool IsHelicopterFireTrail(void) const noexcept { return this->m_is_helicopter_fire_trail; }
    inline void setHelicopterFireTrail(const bool value) noexcept { this->m_is_helicopter_fire_trail = value; }

    inline bool IsHelicopterEngineSound(void) const noexcept { return this->m_is_helicopter_engine_sound; }
    inline void setHelicopterEngineSound(const bool value) noexcept { this->m_is_helicopter_engine_sound = value; }

    inline bool IsHelicopterShowHealth(void) const noexcept { return this->m_is_helicopter_show_health; }
    inline void setHelicopterShowHealth(const bool value) noexcept { this->m_is_helicopter_show_health = value; }

    inline std::uint32_t getHelicopterUpdVis(void) const noexcept { return this->m_helicopter_upd_vis; }
    inline void setHelicopterUpdVis(const std::uint32_t value) noexcept { this->m_helicopter_upd_vis = value; }

    inline float getHelicopterMaxRocketDistance(void) const noexcept { return this->m_helicopter_max_rocket_distance; }
    inline void setHelicopterMaxRocketDistance(const float value) noexcept
    {
        this->m_helicopter_max_rocket_distance = value;
    }

    inline float getHelicopterMaxMinigunDistance(void) const noexcept
    {
        return this->m_helicopter_max_minigun_distance;
    }
    inline void setHelicopterMaxMinigunDistance(const float value) noexcept
    {
        this->m_helicopter_max_minigun_distance = value;
    }

    inline float getHelicopterMinRocketDistance(void) const noexcept { return this->m_helicopter_min_rocket_distance; }
    inline void setHelicopterMinRocketDistance(const float value) noexcept
    {
        this->m_helicopter_min_rocket_distance = value;
    }

    inline float getHelicopterMinMinigunDistance(void) const noexcept
    {
        return this->m_helicopter_min_minigun_distance;
    }
    inline void setHelicopterMinMinigunDistance(const float value) noexcept
    {
        this->m_helicopter_min_minigun_distance = value;
    }

    inline float getHelicopterVelocity(void) const noexcept { return this->m_helicopter_velocity; }
    inline void setHelicopterVelocity(const float value) noexcept { this->m_helicopter_velocity = value; }

    inline bool IsHelicopterStopFire(void) const noexcept { return this->m_is_helicopter_stop_fire; }
    inline void setHelicopterStopFire(const bool value) noexcept { this->m_is_helicopter_stop_fire = value; }

    inline const xr_vector<LogicData>& getLogic(void) const noexcept { return this->m_logic; }
    inline void setLogic(const xr_vector<LogicData>& data) noexcept { this->m_logic = data; }

    inline const xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& getHitOnBone(void) const noexcept
    {
        return this->m_hit_on_bone;
    }
    inline void setHitOnBone(const xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& data) noexcept
    {
        this->m_hit_on_bone = data;
    }
    inline const xr_map<std::uint32_t, CondlistData>& getOnUseCondlist(void) const noexcept
    {
        return this->m_on_use_condlist;
    }
    inline void setOnUseCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        this->m_on_use_condlist = condlist;
    }

    inline float getForce(void) const noexcept { return this->m_force; }
    inline void setForce(const float value) noexcept { this->m_force = value; }

    inline float getAngle(void) const noexcept { return this->m_angle; }
    inline void setAngle(const float value) noexcept { this->m_angle = value; }

    inline std::uint16_t getSelectedID(void) const noexcept { return this->m_selected_id; }
    inline void setSelectedID(const std::uint16_t value) noexcept { this->m_selected_id = value; }

    inline std::uint32_t getDangerTime(void) const noexcept { return this->m_danger_time; }
    inline void setDangerTime(const std::uint32_t value) noexcept { this->m_danger_time = value; }

    inline const xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>& getApprovedActions(
        void) const noexcept
    {
        return this->m_approved_actions;
    }

    inline void setApprovedActions(const std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>& pair) noexcept
    {
        if (pair.first == nullptr)
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setApprovedActions(pair)] WARNING: can't add pair, because the first "
                "element (a function) is nullptr!");
            return;
        }

        this->m_approved_actions.push_back(pair);
    }

#pragma region Cordis Scheme PH FORCE
    inline std::uint32_t getPHForceTime(void) const noexcept { return this->m_ph_force_time; }
    inline void setPHForceTime(const std::uint32_t value) noexcept { this->m_ph_force_time = value; }

    inline std::uint32_t getPHForceDelay(void) const noexcept { return this->m_ph_force_delay; }
    inline void setPHForceDelay(const std::uint32_t value) noexcept { this->m_ph_force_delay = value; }

    inline const Fvector& getPHForcePoint(void) const noexcept { return this->m_ph_force_point; }
    inline void setPHForcePoint(const Fvector& point) noexcept { this->m_ph_force_point = point; }
#pragma endregion

#pragma region Cordis Scheme PH Button
    inline const xr_string& getPHButtonAnimationName(void) const noexcept { return this->m_ph_button_animation_name; }
    inline void setPHButtonAnimationName(const xr_string& animation_name) noexcept
    {
        if (animation_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPHButtonAnimationName(animation_name)] WARNING: "
                "animation_name.empty() == true! You set an empty string");
        }
        this->m_ph_button_animation_name = animation_name;
    }

    inline const xr_string& getPHButtonToolTipName(void) const noexcept { return this->m_ph_button_tooptip_name; }
    inline void setPHButtonToolTipName(const xr_string& value_name) noexcept
    {
        if (value_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPHButtonToolTipName(value_name)] WARNING: value_name.empty() == "
                "true! You set an empty string");
        }

        this->m_ph_button_tooptip_name = value_name;
    }

    inline bool IsPHButtonBlending(void) const noexcept { return this->m_is_ph_button_blending; }
    inline void setPHButtonBlending(const bool value) noexcept { this->m_is_ph_button_blending = value; }

    inline const xr_map<std::uint32_t, CondlistData>& getPHButtonOnPressCondlist(void) const noexcept
    {
        return this->m_ph_button_on_press_condlist;
    }
    inline void setPHButtonOnPressCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        this->m_ph_button_on_press_condlist = condlist;
    }

#pragma endregion

#pragma region Cordis Scheme PH Code
    inline const xr_string& getPHCodeTipName(void) const noexcept { return this->m_ph_code_tip_name; }
    inline void setPHCodeTipName(const xr_string& text_name) noexcept { this->m_ph_code_tip_name = text_name; }

    inline std::uint32_t getPHCodeCode(void) const noexcept { return this->m_ph_code_code; }
    inline void setPHCodeCode(const std::uint32_t value) noexcept { this->m_ph_code_code = value; }

    inline const xr_map<std::uint32_t, CondlistData>& getPHCodeOnCodeCondlist(void) const noexcept
    {
        return this->m_ph_code_on_code_condlist;
    }
    inline void setPHCodeOnCodeCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        this->m_ph_code_on_code_condlist = condlist;
    }

    inline const xr_map<xr_string, xr_map<std::uint32_t, CondlistData>>& getPHCodeOnCheckCode(void) const noexcept
    {
        return this->m_ph_code_on_check_code;
    }
    inline void setPHCodeOnCheckCode(const xr_map<xr_string, xr_map<std::uint32_t, CondlistData>>& data) noexcept
    {
        this->m_ph_code_on_check_code = data;
    }
#pragma endregion

#pragma region Cordis Scheme PH Door
    inline Script_SchemePHDoor* getDoorAction(void) { return this->m_p_ph_door_door_action; }
    inline void setPHDoorDoorAction(Script_SchemePHDoor* p_action) { this->m_p_ph_door_door_action = p_action; }

    inline bool IsPHDoorClosed(void) const noexcept { return this->m_is_ph_door_closed; }
    inline void setPHDoorClosed(const bool value) noexcept { this->m_is_ph_door_closed = value; }

    inline bool IsPHDoorLocked(void) const noexcept { return this->m_is_ph_door_locked; }
    inline void setPHDoorLocked(const bool value) noexcept { this->m_is_ph_door_locked = value; }

    inline bool IsPHDoorNoForce(void) const noexcept { return this->m_is_ph_door_no_force; }
    inline void setPHDoorNoForce(const bool value) noexcept { this->m_is_ph_door_no_force = value; }

    inline bool IsPHDoorNotForNpc(void) const noexcept { return this->m_is_ph_door_not_for_npc; }
    inline void setPHDoorNotForNpc(const bool value) noexcept { this->m_is_ph_door_not_for_npc = value; }

    inline bool IsPHDoorShowTips(void) const noexcept { return this->m_is_ph_door_show_tips; }
    inline void setPHDoorShowTips(const bool value) noexcept { this->m_is_ph_door_show_tips = value; }

    inline const xr_string& getPHDoorTipOpenName(void) const noexcept { return this->m_ph_door_tip_open_name; }
    inline void setPHDoorTipOpenName(const xr_string& tip_name) noexcept
    {
        if (tip_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPHDoorTipOpenName(tip_name)] WARNING: tip_name.empty() == true! "
                "You set an "
                "empty string!");
        }

        this->m_ph_door_tip_open_name = tip_name;
    }

    inline const xr_string& getPHDoorTipUnlockName(void) const noexcept { return this->m_ph_door_tip_unlock_name; }
    inline void setPHDoorTipUnlockName(const xr_string& tip_name) noexcept
    {
        if (tip_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPHDoorTipUnlockName(tip_name)] WARNING: tip_name.empty() == true! "
                "You set an empty string!");
        }

        this->m_ph_door_tip_unlock_name = tip_name;
    }

    inline const xr_string& getPHDoorTipCloseName(void) const noexcept { return this->m_ph_door_tip_close_name; }
    inline void setPHDoorTipCloseName(const xr_string& tip_name) noexcept
    {
        if (tip_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPHDoorTipCloseName(tip_name)] WARNING: tip_name.empty() == true! "
                "You set an empty string!");
        }

        this->m_ph_door_tip_close_name = tip_name;
    }

    inline bool IsPHDoorSlider(void) const noexcept { return this->m_is_ph_door_slider; }
    inline void setPHDoorSlider(const bool value) noexcept { this->m_is_ph_door_slider = value; }

    inline const xr_string& getPHDoorSoundOpenStartName(void) const noexcept
    {
        return this->m_ph_door_sound_open_start_name;
    }
    inline void setPHDoorSoundOpenStartName(const xr_string& sound_name) noexcept
    {
        if (sound_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPHDoorSoundOpenStartName(sound_name)] WARNING: sound_name.empty() "
                "== true! You set an empty string!");
        }

        this->m_ph_door_sound_open_start_name = sound_name;
    }

    inline const xr_string& getPHDoorSoundCloseStartName(void) const noexcept
    {
        return this->m_ph_door_sound_close_start_name;
    }
    inline void setPHDoorSoundCloseStartName(const xr_string& sound_name) noexcept
    {
        if (sound_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPHDoorSoundCloseStartName(sound_name)] WARNING: "
                "sound_name.empty() == true! You set an empty string!");
        }

        this->m_ph_door_sound_close_start_name = sound_name;
    }

    inline const xr_string& getPHDoorSoundCloseStopName(void) const noexcept
    {
        return this->m_ph_door_sound_close_stop_name;
    }
    inline void setPHDoorSoundCloseStopName(const xr_string& sound_name) noexcept
    {
        if (sound_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPHDoorSoundCloseStopName(sound_name)] WARNING: sound_name.empty() "
                "== true! You set an empty string!");
        }

        this->m_ph_door_sound_close_stop_name = sound_name;
    }

    inline bool IsPHDoorScriptUsedMoreThanOnce(void) const noexcept
    {
        return this->m_is_ph_door_script_used_more_than_once;
    }
    inline void setPHDoorScriptUsedMoreThanOnce(const bool value) noexcept
    {
        this->m_is_ph_door_script_used_more_than_once = value;
    }
#pragma endregion

#pragma region Cordis Scheme PH Hit
    inline float getPHHitPower(void) const noexcept { return this->m_ph_hit_power; }
    inline void setPHHitPower(const float value) noexcept { this->m_ph_hit_power = value; }

    inline float getPHHitImpulse(void) const noexcept { return this->m_ph_hit_impulse; }
    inline void setPHHitImpulse(const float value) noexcept { this->m_ph_hit_impulse = value; }

    inline const xr_string& getPHHitBoneName(void) const noexcept { return this->m_ph_hit_bone_name; }
    inline void setPHHitBoneName(const xr_string& bone_name) noexcept
    {
        if (bone_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPHHitBoneName(bone_name)] WARNING: bone_name.empty() == true! You "
                "set "
                "an empty string");
        }

        this->m_ph_hit_bone_name = bone_name;
    }

    inline const xr_string& getPHHitDirectionPathName(void) const noexcept
    {
        return this->m_ph_hit_direction_path_name;
    }
    inline void setPHHitDirectionPathName(const xr_string& path_name) noexcept
    {
        if (path_name.empty())
        {
            Msg("[Scripts/DataBase/Storaeg_Scheme/setPHHitDirectionPathName(path_name)] WARNING: path_name.empty() == "
                "true! You set an empty string!");
        }

        this->m_ph_hit_direction_path_name = path_name;
    }
#pragma endregion

#pragma region Cordis Scheme PH Idle
    inline bool IsPHIdleNonScriptUsable(void) const noexcept { return this->m_is_ph_idle_nonscript_usable; }
    inline void setPHIdleNonScriptUsable(const bool value) noexcept { this->m_is_ph_idle_nonscript_usable = value; }
#pragma endregion

#pragma region Cordis Scheme PH Oscillate
    inline const xr_string& getPHOscillateJointName(void) const noexcept { return this->m_ph_oscillate_joint_name; }
    inline void setPHOscillateJointName(const xr_string& joint_name) noexcept
    {
        if (joint_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPHOscillateJointName(joint_name)] WARNING: joint_name.empty() == "
                "true! You set an empty string!");
        }

        this->m_ph_oscillate_joint_name = joint_name;
    }

    inline std::uint32_t getPHOscillatePeriod(void) const noexcept { return this->m_ph_oscillate_period; }
    inline void setPHOscillatePeriod(const std::uint32_t value) noexcept { this->m_ph_oscillate_period = value; }

#pragma endregion

#pragma region Cordis Scheme PH Sound
    inline bool IsPHSoundLooped(void) const noexcept { return this->m_is_ph_sound_looped; }
    inline void setPHSoundLooped(const bool value) noexcept { this->m_is_ph_sound_looped = value; }

    inline bool IsPHSoundRandom(void) const noexcept { return this->m_is_ph_sound_random; }
    inline void setPHSoundRandom(const bool value) noexcept { this->m_is_ph_sound_random = value; }

    inline bool IsPHSoundNoHit(void) const noexcept { return this->m_is_ph_sound_no_hit; }
    inline void setPHSoundNoHit(const bool value) noexcept { this->m_is_ph_sound_no_hit = value; }

    inline const xr_string& getPHSoundThemeName(void) const noexcept { return this->m_ph_sound_theme_name; }
    inline void setPHSoundThemeName(const xr_string& theme_name) noexcept
    {
        if (theme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setPHSoundThemeName(theme_name)] WARNING: theme_name.empty() == "
                "true! You set an empty string");
        }

        this->m_ph_sound_theme_name = theme_name;
    }

    inline std::uint32_t getPHSoundPauseMin(void) const noexcept { return this->m_ph_sound_pause_min; }
    inline void setPHSoundPauseMin(const std::uint32_t value) noexcept { this->m_ph_sound_pause_min = value; }

    inline std::uint32_t getPHSoundPauseMax(void) const noexcept { return this->m_ph_sound_pause_max; }
    inline void setPHSoundPauseMax(const std::uint32_t value) noexcept { this->m_ph_sound_pause_max = value; }
#pragma endregion

#pragma region Cordis Scheme SR Timer
    inline const xr_string& getSRTimerTypeName(void) const noexcept { return this->m_sr_timer_type_name; }
    inline void setSRTimerTypeName(const xr_string& timer_type_name) noexcept
    {
        if (timer_type_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRTimerTypeName(timer_type_name)] WARNING: "
                "timer_type_name.empty() == true! You set an empty string");
        }

        this->m_sr_timer_type_name = timer_type_name;
    }

    inline const xr_string& getSRTimerTimerIDName(void) const noexcept { return this->m_sr_timer_timer_id_name; }
    inline void setSRTimerTimerIDName(const xr_string& timer_id_name) noexcept
    {
        if (timer_id_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRTimerTimerIDName(timer_id_name)] WARNING: timer_id_name.empty() "
                "== true! You set an empty string");
        }

        this->m_sr_timer_timer_id_name = timer_id_name;
    }

    inline const xr_string& getSRTimerStringName(void) const noexcept { return this->m_sr_timer_string_name; }
    inline void setSRTimerStringName(const xr_string& string_name) noexcept
    {
        if (string_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRTimerStringName(string_name)] WARNING: string_name.empty() == "
                "true! You set an empty string");
        }

        this->m_sr_timer_string_name = string_name;
    }

    inline std::uint32_t getSRTimerStartValue(void) const noexcept { return this->m_sr_timer_start_value; }
    inline void setSRTimerStartValue(const std::uint32_t value) noexcept { this->m_sr_timer_start_value = value; }

    inline CUIGameCustom* const getSRTimerUI(void) const { return this->m_p_sr_timer_ui; }
    inline void setSRTimerUI(CUIGameCustom* const p_ui)
    {
        if (!p_ui)
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRTimerUI(p_ui)] WARNING: p_ui == nullptr! You set an empty "
                "object");
        }

        this->m_p_sr_timer_ui = p_ui;
    }

    inline CUIStatic* const getSRTimerTimer(void) const { return this->m_p_sr_timer_timer; }
    inline void setSRTimerTimer(CUIStatic* const p_static)
    {
        if (!p_static)
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRTimerTimer(p_static)] WARNING: p_static == nullptr! You set an "
                "empty object");
        }

        this->m_p_sr_timer_timer = p_static;
    }

    inline const xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& getSRTimerOnValue(void) const noexcept
    {
        return this->m_sr_timer_on_value;
    }
    inline void setSRTimerOnValue(const xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>>& data) noexcept
    {
        this->m_sr_timer_on_value = data;
    }
#pragma endregion

#pragma region Cordis Scheme SR Teleport
    inline std::uint32_t getSRTeleportTimeout(void) const noexcept { return this->m_sr_teleport_timeout; }
    inline void setSRTeleportTimeout(const std::uint32_t value) noexcept { this->m_sr_teleport_timeout = value; }

    inline const xr_vector<std::pair<std::uint32_t, std::pair<xr_string, xr_string>>>& getSRTeleportPoints(void) const
        noexcept
    {
        return this->m_sr_teleport_points;
    }

    inline void setSRTeleportPoints(
        const xr_vector<std::pair<std::uint32_t, std::pair<xr_string, xr_string>>>& data) noexcept
    {
        this->m_sr_teleport_points = data;
    }
#pragma endregion

#pragma region Cordis Scheme SR Psy Antenna
    inline float getSRPsyAntennaIntensity(void) const noexcept { return this->m_sr_psy_antenna_intensity; }
    inline void setSRPsyAntennaIntensity(const float value) noexcept { this->m_sr_psy_antenna_intensity = value; }

    inline float getSRPsyAntennaHitIntensity(void) const noexcept { return this->m_sr_psy_antenna_hit_intensity; }
    inline void setSRPsyAntennaHitIntensity(const float value) noexcept
    {
        this->m_sr_psy_antenna_hit_intensity = value;
    }

    inline float getSRPsyAntennaPhantomProbability(void) const noexcept
    {
        return this->m_sr_psy_antenna_phantom_probability;
    }
    inline void setSRPsyAntennaPhantomProbability(const float value) noexcept
    {
        this->m_sr_psy_antenna_phantom_probability = value;
    }

    inline float getSRPsyAntennaMuteSoundThreshold(void) const noexcept
    {
        return this->m_sr_psy_antenna_mute_sound_threshold;
    }

    inline void setSRPsyAntennaMuteSoundThreshold(const float value) noexcept
    {
        this->m_sr_psy_antenna_mute_sound_threshold = value;
    }

    inline float getSRPsyAntennaHitFrequency(void) const noexcept { return this->m_sr_psy_antenna_hit_frequency; }
    inline void setSRPsyAntennaHitFrequency(const float value) noexcept
    {
        this->m_sr_psy_antenna_hit_frequency = value;
    }

    inline bool IsSRPsyAntennaNoStatic(void) const noexcept { return this->m_is_sr_psy_antenna_no_static; }
    inline void setSRPsyAntennaNoStatic(const bool value) noexcept { this->m_is_sr_psy_antenna_no_static = value; }

    inline bool IsSRPsyAntennaNoMumble(void) const noexcept { return this->m_is_sr_psy_antenna_no_mumble; }
    inline void setSRPsyAntennaNoMumble(const bool value) noexcept { this->m_is_sr_psy_antenna_no_mumble = value; }

    inline const xr_string& getSRPsyAntennaPostProcessName(void) const noexcept
    {
        return this->m_sr_psy_antenna_postprocess_name;
    }
    inline void setSRPsyAntennaPostProcessName(const xr_string& postprocess_name) noexcept
    {
        if (postprocess_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRPsyAntennaPostProcessName(postprocess_name)] WARNING: "
                "postprocess_name.empty() == true! You set an empty string");
        }

        this->m_sr_psy_antenna_postprocess_name = postprocess_name;
    }

    inline const xr_string& getSRPsyAntennaHitTypeName(void) const noexcept
    {
        return this->m_sr_psy_antenna_hit_type_name;
    }
    inline void setSRPsyAntennaHitTypeName(const xr_string& hit_type_name) noexcept
    {
        if (hit_type_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRPsyAntennaHitTypeName(hit_type_name)] WARNING: "
                "hit_type_name.empty() == true! You set an empty string");
        }

        this->m_sr_psy_antenna_hit_type_name = hit_type_name;
    }
#pragma endregion

#pragma region Cordis Scheme SR Postprocess
    inline float getSRPostProcessIntensity(void) const noexcept { return this->m_sr_postprocess_intensity; }
    inline void setSRPostProcessIntensity(const float value) noexcept { this->m_sr_postprocess_intensity = value; }

    inline float getSRPostProcessIntensitySpeed(void) const noexcept { return this->m_sr_postprocess_intensity_speed; }
    inline void setSRPostProcessIntensitySpeed(const float value) noexcept
    {
        this->m_sr_postprocess_intensity_speed = value;
    }

    inline float getSRPostProcessHitIntensity(void) const noexcept { return this->m_sr_postprocess_hit_intensity; }
    inline void setSRPostProcessHitIntensity(const float value) noexcept
    {
        this->m_sr_postprocess_hit_intensity = value;
    }
#pragma endregion

#pragma region Cordis Scheme SR Particle
    inline bool IsSRParticleLooped(void) const noexcept { return this->m_is_sr_particle_looped; }
    inline void setSRParticleLooped(const bool value) noexcept { this->m_is_sr_particle_looped = value; }

    inline std::uint32_t getSRParticleMode(void) const noexcept { return this->m_sr_particle_mode; }
    inline void setSRParticleMode(const std::uint32_t value) noexcept { this->m_sr_particle_mode = value; }

    inline const xr_string& getSRParticleName(void) const noexcept { return this->m_sr_particle_name; }
    inline void setSRParticleName(const xr_string& name) noexcept
    {
        if (name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRParticleName(name)] WARNING: name.empty() == true! You set an "
                "empty string");
        }

        this->m_sr_particle_name = name;
    }

    inline const xr_string& getSRParticlePathName(void) const noexcept { return this->m_sr_particle_path_name; }
    inline void setSRParticlePathName(const xr_string& path_name) noexcept
    {
        if (path_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRParticlePathName(path_name)] WARNING: path_name.empty() == "
                "true! You set an empty string");
        }

        this->m_sr_particle_path_name = path_name;
    }

#pragma endregion

#pragma region Cordis Scheme SR Light
    inline bool IsSRLightLight(void) const noexcept { return this->m_is_sr_light_light; }
    inline void setSRLightLight(const bool value) noexcept { this->m_is_sr_light_light = value; }
#pragma endregion

#pragma region Cordis Scheme SR Deimos
    inline float getSRDeimosIntensity(void) const noexcept { return this->m_sr_deimos_intensity; }
    inline void setSRDeimosIntensity(const float value) noexcept { this->m_sr_deimos_intensity = value; }

    inline float getSRDeimosMovementSpeed(void) const noexcept { return this->m_sr_deimos_movement_speed; }
    inline void setSRDeimosMovementSpeed(const float value) noexcept { this->m_sr_deimos_movement_speed = value; }

    inline float getSRDeimosGrowingKoef(void) const noexcept { return this->m_sr_deimos_growing_koef; }
    inline void setSRDeimosGrowingKoef(const float value) noexcept { this->m_sr_deimos_growing_koef = value; }

    inline float getSRDeimosLoweringKoef(void) const noexcept { return this->m_sr_deimos_lowering_koef; }
    inline void setSRDeimosLoweringKoef(const float value) noexcept { this->m_sr_deimos_lowering_koef = value; }

    inline std::uint32_t getSRDeimosCameraEffectorRepeatingTime(void) const noexcept
    {
        return this->m_sr_deimos_camera_effector_repeating_time;
    }
    inline void setSRDeimosCameraEffectorRepeatingTime(const std::uint32_t value) noexcept
    {
        this->m_sr_deimos_camera_effector_repeating_time = value;
    }

    inline float getSRDeimosHealthLost(void) const noexcept { return this->m_sr_deimos_health_lost; }
    inline void setSRDeimosHealthLost(const float value) noexcept { this->m_sr_deimos_health_lost = value; }

    inline float getSRDeimosDisableBound(void) const noexcept { return this->m_sr_deimos_disable_bound; }
    inline void setSRDeimosDisableBound(const float value) noexcept { this->m_sr_deimos_disable_bound = value; }

    inline float getSRDeimosSwitchLowerBound(void) const noexcept { return this->m_sr_deimos_switch_lower_bound; }
    inline void setSRDeimosSwitchLowerBound(const float value) noexcept
    {
        this->m_sr_deimos_switch_lower_bound = value;
    }

    inline float getSRDeimosSwitchUpperBound(void) const noexcept { return this->m_sr_deimos_switch_upper_bound; }
    inline void setSRDeimosSwitchUpperBound(const float value) noexcept
    {
        this->m_sr_deimos_switch_upper_bound = value;
    }

    inline const xr_string& getSRDeimosPostProcessEffectorName(void) const noexcept
    {
        return this->m_sr_deimos_postprocess_effector_name;
    }
    inline void setSRDeimosPostProcessEffectorName(const xr_string& effector_name) noexcept
    {
        if (effector_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRDeimosPostProcessEffectorName(effector_name)] WARNING: "
                "effector_name.empty() == true! You set an empty string");
        }

        this->m_sr_deimos_postprocess_effector_name = effector_name;
    }

    inline const xr_string& getSRDeimosCameraEffectorName(void) const noexcept
    {
        return this->m_sr_deimos_camera_effector_name;
    }
    inline void setSRDeimosCameraEffectorName(const xr_string& effector_name) noexcept
    {
        if (effector_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRDeimosCameraEffectorName(effector_name)] WARNING: "
                "effector_name.empty() == true! You set an empty string");
        }

        this->m_sr_deimos_camera_effector_name = effector_name;
    }

    inline const xr_string& getSRDeimosPostProcessEffector2Name(void) const noexcept
    {
        return this->m_sr_deimos_postprocess_effector2_name;
    }
    inline void setSRDeimosPostProcessEffector2Name(const xr_string& effector_name) noexcept
    {
        if (effector_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRDeimosPostProcessEffector2Name(effector_name)] WARNING: "
                "effector_name.empty() == true! You set an empty string");
        }

        this->m_sr_deimos_postprocess_effector2_name = effector_name;
    }

    inline const xr_string& getSRDeimosNoiseSoundName(void) const noexcept
    {
        return this->m_sr_deimos_noise_sound_name;
    }
    inline void setSRDeimosNoiseSoundName(const xr_string& sound_name) noexcept
    {
        if (sound_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRDeimosNoiseSoundName(sound_name)] WARNING: sound_name.empty() "
                "== true! You set an empty string");
        }

        this->m_sr_deimos_noise_sound_name = sound_name;
    }

    inline const xr_string& getSRDeimosHeartBeetSoundName(void) const noexcept
    {
        return this->m_sr_deimos_heartbeet_sound_name;
    }
    inline void setSRDeimosHeartBeetSoundName(const xr_string& sound_name) noexcept
    {
        if (sound_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setSRDeimosHeartBeetSoundName(sound_name)] WARNING: "
                "sound_name.empty() == true! You set an empty string");
        }

        this->m_sr_deimos_heartbeet_sound_name = sound_name;
    }
#pragma endregion

#pragma region Cordis Scheme XR Death
    inline const xr_map<std::uint32_t, CondlistData>& getXRDeathInfo(void) const noexcept
    {
        return this->m_xr_death_info;
    }
    inline void setXRDeathInfo(const xr_map<std::uint32_t, CondlistData>& data) noexcept
    {
        this->m_xr_death_info = data;
    }

    inline const xr_map<std::uint32_t, CondlistData>& getXRDeathInfo2(void) const noexcept
    {
        return this->m_xr_death_info2;
    }
    inline void setXRDeathInfo2(const xr_map<std::uint32_t, CondlistData>& data) noexcept
    {
        this->m_xr_death_info2 = data;
    }
#pragma endregion

#pragma region Cordis Scheme XR Abuse
    inline Script_XRAbuseManager* const getXRAbuseManager(void) const { return this->m_p_abuse_manager; }
    inline void setXRAbuseManager(Script_XRAbuseManager* const p_object)
    {
        if (!p_object)
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setXRAbuseManager(p_object)] WARNING: you are trying to set an empty "
                "object return ...");
            return;
        }

        this->m_p_abuse_manager = p_object;
    }
#pragma endregion

#pragma region Cordis Scheme XR Corpse Detection
    inline std::uint32_t getLevelVertexID(void) const noexcept { return this->m_level_vertex_id; }
    inline void setLevelVertexID(const std::uint32_t id) noexcept { this->m_level_vertex_id = id; }

    inline std::uint16_t getXRCorpseDetectionSelectedCorpseID(void) const noexcept
    {
        return this->m_xr_corpse_detection_selected_corpse_id;
    }
    inline void setXRCorpseDetectionSelectedCorpseID(const std::uint16_t id) noexcept
    {
        this->m_xr_corpse_detection_selected_corpse_id = id;
    }

    inline const Fvector& getVertexPosition(void) const noexcept { return this->m_vertex_position; }

    inline void setVertexPosition(const Fvector& position) noexcept { this->m_vertex_position = position; }
#pragma endregion

#pragma region Cordis Scheme XR Combat

    inline const xr_string& getXRCombatScriptCombatTypeName(void) const noexcept
    {
        return this->m_xr_combat_script_combat_type_name;
    }
    inline void setXRCombatScriptCombatTypeName(const xr_string& type_name) noexcept
    {
        this->m_xr_combat_script_combat_type_name = type_name;
    }

    inline const xr_map<std::uint32_t, CondlistData>& getXRCombatCombatTypeCondlist(void) const noexcept
    {
        return this->m_xr_combat_combat_type_condlist;
    }
    inline void setXRCombatCombatTypeCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        this->m_xr_combat_combat_type_condlist = condlist;
    }
#pragma endregion

#pragma region Cordis Scheme XR Help Wounded
    inline bool IsXRHelpWoundedHelpWoundedEnabled(void) const noexcept
    {
        return this->m_is_xr_help_wounded_help_wounded_enabled;
    }
    inline void setXRHelpWoundedHelpWoundedEnabled(const bool value) noexcept
    {
        this->m_is_xr_help_wounded_help_wounded_enabled = value;
    }
#pragma endregion

#pragma region Cordis Scheme XR Gather Items
    inline bool isXRGatherItemsEnabled(void) const noexcept { return this->m_is_xr_gather_items_enabled; }
    inline void setXRGatherItemsEnabled(const bool value) noexcept { this->m_is_xr_gather_items_enabled = value; }
#pragma endregion

#pragma region Cordis Scheme XR Remark
    inline const xr_string& getXRRemarkTargetName(void) const noexcept { return this->m_xr_remark_target_name; }
    inline void setXRRemarkTargetName(const xr_string& target_name) noexcept
    {
        if (target_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setXRRemarkTargetName(target_name)] WARNING: target_name.empty() == "
                "true! You are trying to set an empty string Return ...");
            return;
        }

        this->m_xr_remark_target_name = target_name;
    }

    inline bool isXRRemarkTargetInitialized(void) const noexcept { return this->m_is_xr_remark_target_initialized; }
    inline void setXRRemarkTargetInitialized(const bool value) noexcept
    {
        this->m_is_xr_remark_target_initialized = value;
    }

    inline const Fvector& getXRRemarkTargetPosition(void) const noexcept { return this->m_xr_remark_target_position; }
    inline void setXRRemarkTargetPosition(const Fvector& target_position) noexcept
    {
        this->m_xr_remark_target_position = target_position;
    }

    inline std::uint16_t getXRRemarkTargetID(void) const noexcept { return this->m_xr_remark_target_id; }
    inline void setXRRemarkTargetID(const std::uint16_t value) noexcept { this->m_xr_remark_target_id = value; }

    inline bool isXRRemarkSoundAnimationSync(void) const noexcept { return this->m_is_xr_remark_sound_animation_sync; }
    inline void setXRRemarkSoundAnimationSync(const bool value) noexcept
    {
        this->m_is_xr_remark_sound_animation_sync = value;
    }

    inline const xr_string& getXRRemarkSoundName(void) const noexcept { return this->m_xr_remark_sound_name; }
    inline void setXRRemarkSoundName(const xr_string& sound_name) noexcept
    {
        if (sound_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setXRRemarkSoundName(sound_name)] WARNING: sound_name.empty() == "
                "true! You are set an empty string Return ...");
            return;
        }

        this->m_xr_remark_sound_name = sound_name;
    }

    inline const xr_string& getXRRemarkTipsIDName(void) const noexcept { return this->m_xr_remark_tips_id_name; }
    inline void setXRRemarkTipsIDName(const xr_string& text) noexcept
    {
        if (text.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setXRRemarkTipsIDName(text)] WARNING: text.empty() == true! You are "
                "set an empty string! Return ...");
            return;
        }

        this->m_xr_remark_tips_id_name = text;
    }

    inline const xr_string& getXRRemarkTipsSenderName(void) const noexcept
    {
        return this->m_xr_remark_tips_sender_name;
    }
    inline void setXRRemarkTipsSenderName(const xr_string& text) noexcept
    {
        if (text.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setXRRemarkTipsSenderName(text)] WARNING: text.empty() == treu! You "
                "are set an empty string! Return ...");
            return;
        }

        this->m_xr_remark_tips_sender_name = text;
    }

    inline const xr_map<std::uint32_t, CondlistData>& getXRRemarkAnimationCondlist(void) const noexcept
    {
        return this->m_xr_remark_animation_condlist;
    }

    inline void setXRRemarkAnimationCondlist(const xr_map<std::uint32_t, CondlistData>& condlist) noexcept
    {
        if (condlist.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setXRRemarkAnimationCondlist(condlist)] WARNING: condlist.empty() == "
                "true! You are trying to set an empty condlist");
            return;
        }

        this->m_xr_remark_animation_condlist = condlist;
    }
#pragma endregion

#pragma region Cordis Scheme XR Walker
    inline const xr_string& getXRWalkerPathWalkName(void) const noexcept { return this->m_xr_walker_path_walk_name; }
    inline void setXRWalkerPathWalkName(const xr_string& path_name) noexcept
    {
        if (path_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setXRWalkerPathWalkName(path_name)] WARNING: path_walk.empty() == "
                "true! You are set an empty string");
            return;
        }

        this->m_xr_walker_path_walk_name = path_name;
    }

    inline const xr_string& getXRWalkerPathLookName(void) const noexcept { return this->m_xr_walker_path_look_name; }
    inline void setXRWalkerPathLookName(const xr_string& path_name) noexcept
    {
        if (path_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setXRWalkerPathLookName(path_name)] WARNING: path_name.empty() == "
                "true! You are set an empty string");
            return;
        }

        this->m_xr_walker_path_look_name = path_name;
    }

    inline const xr_string& getXRWalkerTeamName(void) const noexcept { return this->m_xr_walker_team_name; }
    inline void setXRWalkerTeamName(const xr_string& team_name) noexcept
    {
        if (team_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setXRWalkerTeamName(team_name)] WARNING: team_name.empty() == true! "
                "You are set an empty string");
            return;
        }

        this->m_xr_walker_team_name = team_name;
    }

    inline const xr_string& getXRWalkerDescriptionName(void) const noexcept
    {
        return this->m_xr_walker_description_name;
    }
    inline void setXRWalkerDescriptionName(const xr_string& description_name) noexcept
    {
        if (description_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setXRWalkerDescriptionName(description_name)] WARNING: "
                "description_name.empty() == true! You are set an empty string!");
            return;
        }

        this->m_xr_walker_description_name = description_name;
    }

    inline bool isXRWalkerUseCamp(void) const noexcept { return this->m_is_xr_walker_use_camp; }
    inline void setXRWalkerUseCamp(const bool is_using) noexcept { this->m_is_xr_walker_use_camp = is_using; }

    inline const xr_map<xr_string, xr_string>& getXRWalkerSuggestedStates(void) const noexcept
    {
        return this->m_xr_walker_suggested_states;
    }

    inline void setXRWalkerSuggestedStates(const xr_string& type_name, const xr_string& animation_name) noexcept
    {
        if (type_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setXRWalkerSuggestedStates(type_name, animation_name)] WARNING: "
                "type_name.empty() == true! You are set an empty string!");
            return;
        }

        if (this->m_xr_walker_suggested_states.find(type_name) != this->m_xr_walker_suggested_states.end())
        {
            Msg("[Scripts/DataBase/Storage_Scheme/setXRWalkerSuggestedStates(type_name, animation_name)] WARNING: you "
                "are trying to change the existing value %s to %s",
                this->m_xr_walker_suggested_states.at(type_name).c_str(), animation_name.c_str());
        }

        this->m_xr_walker_suggested_states[type_name] = animation_name;
    }
#pragma endregion

#pragma region Cordis Scheme SR / XR Camp / Animpoint
    inline const xr_string& getBaseActionName(void) const noexcept { return this->m_base_action_name; }
    inline void setBaseActionName(const xr_string& action_name) noexcept
    {
        if (action_name.empty())
        {
            Msg("[Scripts/Storage_Scheme/setBaseActionName(action_name)] WARNING: action_name.empty() == true! You set "
                "an empty string return");
            return;
        }

        this->m_base_action_name = action_name;
    }

    inline const xr_string& getDescriptionName(void) const noexcept { return this->m_description_name; }
    inline void setDescriptionName(const xr_string& description_name) noexcept
    {
        if (description_name.empty())
        {
            Msg("[Scripts/Storage_Scheme/setDescriptionName(description_name)] WARNING: description_name.empty() == "
                "true! You set an empty string return");
            return;
        }

        this->m_description_name = description_name;
    }
#pragma endregion

private:
    // @ Не понятно зачем в итоге но так у ПЫС, если в итоге оно находится в самом сторадже где уже зарегистрирован
    // сам НПС
    bool m_is_enabled = false;
    bool m_is_animation_movement = false;
    bool m_is_no_reset = false;
    bool m_is_aggresive = false;
    bool m_is_gulag_point = false;
    bool m_is_skip_transfer_enemy = false;
    bool m_is_helicopter_use_rocket = false;
    bool m_is_helicopter_show_health = false;
    bool m_is_helicopter_fire_trail = false;
    bool m_is_helicopter_engine_sound = false;
    bool m_is_helicopter_use_minigun = false;
    bool m_is_helicopter_stop_fire = false;
    bool m_is_ph_button_blending = false;
    bool m_is_ph_door_closed = false;
    bool m_is_ph_door_locked = false;
    bool m_is_ph_door_no_force = false;
    bool m_is_ph_door_not_for_npc = false;
    bool m_is_ph_door_show_tips = false;
    bool m_is_ph_door_slider = false;
    bool m_is_ph_door_script_used_more_than_once = false;
    bool m_is_ph_idle_nonscript_usable = false;
    bool m_is_ph_sound_no_hit = false;
    bool m_is_ph_sound_looped = false;
    bool m_is_ph_sound_random = false;
    bool m_is_sr_psy_antenna_no_static = false;
    bool m_is_sr_psy_antenna_no_mumble = false;
    bool m_is_sr_particle_looped = false;
    bool m_is_sr_light_light = false;
    bool m_is_xr_help_wounded_help_wounded_enabled = false;
    bool m_is_xr_gather_items_enabled = false;
    bool m_is_xr_remark_target_initialized = false;
    bool m_is_xr_remark_sound_animation_sync = false;
    bool m_is_xr_walker_use_camp = false;
    std::uint16_t m_xr_corpse_detection_selected_corpse_id = 0;
    std::uint16_t m_selected_id = 0;
    std::uint16_t m_xr_remark_target_id = 0;
    std::uint32_t m_home_min_radius = 0;
    std::uint32_t m_home_mid_radius = 0;
    std::uint32_t m_home_max_radius = 0;
    std::uint32_t m_time_change_point = 0;
    std::uint32_t m_helicopter_upd_vis = 0;
    std::uint32_t m_ph_force_time = 0;
    std::uint32_t m_ph_force_delay = 0;
    std::uint32_t m_ph_code_code = 0;
    std::uint32_t m_ph_oscillate_period = 0;
    std::uint32_t m_ph_sound_pause_min = 0;
    std::uint32_t m_ph_sound_pause_max = 0;
    std::uint32_t m_sr_timer_start_value = 0;
    std::uint32_t m_sr_teleport_timeout = 0;
    std::uint32_t m_sr_particle_mode = 0;
    std::uint32_t m_sr_deimos_camera_effector_repeating_time = 0;
    std::uint32_t m_level_vertex_id = 0;
    std::uint32_t m_danger_time = 0;
    std::uint32_t m_scheme_id_for_unsubscring = 0;
    float m_ph_jump_factor = 0.0f;
    float m_helicopter_min_rocket_distance = 0.0f;
    float m_helicopter_min_minigun_distance = 0.0f;
    float m_helicopter_max_rocket_distance = 0.0f;
    float m_helicopter_max_minigun_distance = 0.0f;
    float m_helicopter_velocity = 0.0f;
    float m_force = 0.0f;
    float m_angle = 0.0f;
    float m_ph_hit_power = 0.0f;
    float m_ph_hit_impulse = 0.0f;
    float m_sr_psy_antenna_intensity = 0.0f;
    float m_sr_psy_antenna_hit_intensity = 0.0f;
    float m_sr_psy_antenna_phantom_probability = 0.0f;
    float m_sr_psy_antenna_mute_sound_threshold = 0.0f;
    float m_sr_psy_antenna_hit_frequency = 0.0f;
    float m_sr_postprocess_intensity = 0.0f;
    float m_sr_postprocess_intensity_speed = 0.0f;
    float m_sr_postprocess_hit_intensity = 0.0f;
    float m_sr_deimos_intensity = 0.0f;
    float m_sr_deimos_growing_speed = 0.0f;
    float m_sr_deimos_growing_koef = 0.0f;
    float m_sr_deimos_lowering_koef = 0.0f;
    float m_sr_deimos_disable_bound = 0.0f;
    float m_sr_deimos_switch_lower_bound = 0.0f;
    float m_sr_deimos_switch_upper_bound = 0.0f;
    float m_sr_deimos_health_lost = 0.0f;
    float m_sr_deimos_movement_speed = 0.0f;
    CScriptGameObject* m_p_npc = nullptr;
    /*
        Script_ISchemeEntity* m_p_action =
            nullptr; // @ для XR_LOGIC::unsubscrive_action, используется в очень редких схемах!*/
    CScriptIniFile* m_p_ini = nullptr;
    CPatrolPathParams* m_p_jump_path = nullptr;
    Script_SchemePHDoor* m_p_ph_door_door_action =
        nullptr; // @ Используется исключительно когда у нас схема -> ph_door, удаляется как обычный зарегистрированный
                 // action, но сам pointer зануляется в деструкторе схемы!
    CUIGameCustom* m_p_sr_timer_ui = nullptr;
    CUIStatic* m_p_sr_timer_timer = nullptr;
    Script_XRAbuseManager* m_p_abuse_manager = nullptr;
    Fvector m_offset;
    Fvector m_ph_force_point;
    Fvector m_vertex_position;
    Fvector m_xr_remark_target_position;
    xr_map<xr_string, bool> m_signals;
    xr_map<std::uint32_t, CondlistData> m_dialog_condlist;
    xr_map<std::uint32_t, CondlistData> m_ph_button_on_press_condlist;
    xr_map<std::uint32_t, CondlistData> m_ph_code_on_code_condlist;
    xr_map<std::uint32_t, CondlistData> m_on_use_condlist;
    xr_map<std::uint32_t, CondlistData> m_xr_death_info;
    xr_map<std::uint32_t, CondlistData> m_xr_death_info2;
    xr_map<std::uint32_t, CondlistData> m_xr_remark_animation_condlist;
    xr_map<std::uint32_t, CondlistData> m_xr_combat_combat_type_condlist;
    xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>> m_hit_on_bone;
    xr_map<std::uint32_t, xr_map<std::uint32_t, CondlistData>> m_sr_timer_on_value;
    xr_map<xr_string, xr_string> m_xr_walker_suggested_states;
    xr_map<xr_string, xr_map<std::uint32_t, CondlistData>> m_ph_code_on_check_code;
    xr_vector<Script_ISchemeEntity*> m_actions;
    xr_vector<LogicData> m_logic;
    xr_vector<std::pair<std::uint32_t, std::pair<xr_string, xr_string>>> m_sr_teleport_points;
    xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>> m_approved_actions;
    xr_string m_path_walk_name;
    xr_string m_path_look_name;
    xr_string m_path_jump_name;
    xr_string m_state_name;
    xr_string m_scheme_name;
    xr_string m_logic_name;
    xr_string m_tip_name;
    xr_string m_sound_name;
    xr_string m_time_name;
    xr_string m_animation_name;
    xr_string m_animation_head_name;
    xr_string m_home_name;
    xr_string m_look_point_name;
    xr_string m_home_point_name;
    xr_string m_base_action_name;
    xr_string m_description_name;
    xr_string m_helicopter_path_move_name;
    xr_string m_helicopter_path_look_name;
    xr_string m_helicopter_enemy_name;
    xr_string m_helicopter_fire_point_name;
    xr_string m_ph_button_animation_name;
    xr_string m_ph_button_tooptip_name;
    xr_string m_ph_code_tip_name;
    xr_string m_ph_door_tip_open_name;
    xr_string m_ph_door_tip_unlock_name;
    xr_string m_ph_door_tip_close_name;
    xr_string m_ph_door_sound_open_start_name;
    xr_string m_ph_door_sound_close_start_name;
    xr_string m_ph_door_sound_close_stop_name;
    xr_string m_ph_hit_bone_name;
    xr_string m_ph_hit_direction_path_name;
    xr_string m_ph_oscillate_joint_name;
    xr_string m_ph_sound_theme_name;
    xr_string m_sr_timer_type_name;
    xr_string m_sr_timer_timer_id_name;
    xr_string m_sr_timer_string_name;
    xr_string m_sr_psy_antenna_postprocess_name;
    xr_string m_sr_psy_antenna_hit_type_name;
    xr_string m_sr_particle_name;
    xr_string m_sr_particle_path_name;
    xr_string m_sr_deimos_postprocess_effector_name;
    xr_string m_sr_deimos_camera_effector_name;
    xr_string m_sr_deimos_postprocess_effector2_name;
    xr_string m_sr_deimos_noise_sound_name;
    xr_string m_sr_deimos_heartbeet_sound_name;
    xr_string m_xr_combat_script_combat_type_name;
    xr_string m_xr_remark_target_name;
    xr_string m_xr_remark_sound_name;
    xr_string m_xr_remark_tips_id_name;
    xr_string m_xr_remark_tips_sender_name;
    xr_string m_xr_walker_description_name;
    xr_string m_xr_walker_path_walk_name;
    xr_string m_xr_walker_team_name;
    xr_string m_xr_walker_sound_idle;
    xr_string m_xr_walker_path_look_name;
    CondlistWaypoints m_path_walk_info;
    CondlistWaypoints m_path_look_info;
};

// Lord: не забудь что здесь ещё MANAGER для WOUNDED!!!!!!
class Data_Wounded
{
public:
    Data_Wounded(void) = default;
    ~Data_Wounded(void) = default;

    inline bool IsAutoHeal(void) const noexcept { return this->m_is_autoheal; }
    inline void setAutoHeal(const bool value) noexcept { this->m_is_autoheal = value; }

    inline bool IsUseMedkit(void) const noexcept { return this->m_is_use_medkit; }
    inline void setUseMedkit(const bool value) noexcept { this->m_is_use_medkit = value; }

    inline bool IsEnableTalk(void) const noexcept { return this->m_is_enable_talk; }
    inline void setEnableTalk(const bool value) noexcept { this->m_is_enable_talk = value; }

    inline bool IsForHelp(void) const noexcept { return this->m_is_not_for_help; }
    inline void setForHelp(const bool value) noexcept { this->m_is_not_for_help = value; }

    inline bool IsWoundedInitialized(void) const noexcept { return this->m_is_wounded_initialized; }
    inline void setWoundedInitialized(const bool value) noexcept { this->m_is_wounded_initialized = value; }

    inline const xr_string& getHPStateName(void) const noexcept { return this->m_hp_state_name; }
    inline void setHPStateName(const xr_string& state_name) noexcept
    {
        if (state_name.empty())
        {
            Msg("[Scripts/DataBase/Data_Wounded/setHPStateName(state_name)] WARNING: state_name.empty() == true! "
                "Trying to set an empty string!");
        }

        this->m_hp_state_name = state_name;
    }

    inline const xr_string& getHPStateSeeName(void) const noexcept { return this->m_hp_state_see_name; }
    inline void setHPStateSeeName(const xr_string& state_see_name) noexcept
    {
        if (state_see_name.empty())
        {
            Msg("[Scripts/DataBase/Data_Wounded/setHPStateSeeName(state_see_name)] WARNING: state_see_name.empty() == "
                "true! Trying to set an empty string!");
        }

        this->m_hp_state_see_name = state_see_name;
    }

    inline const xr_string& getHPVictimName(void) const noexcept { return this->m_hp_victim_name; }
    inline void setHPVictimName(const xr_string& victim_name) noexcept { this->m_hp_victim_name = victim_name; }

    inline const xr_string& getHPCoverName(void) const noexcept { return this->m_hp_cover_name; }
    inline void setHPCoverName(const xr_string& cover_name) noexcept
    {
        if (cover_name.empty())
        {
            Msg("[Scripts/DataBase/Data_Wounded/setHPCoverName(cover_name)] WARNING: cover_name.empty() == true! "
                "Trying to set an empty string!");
        }

        this->m_hp_cover_name = cover_name;
    }

    inline const xr_string& getHPFightName(void) const noexcept { return this->m_hp_fight_name; }
    inline void setHPFightName(const xr_string& fight_name) noexcept
    {
        if (fight_name.empty())
        {
            Msg("[Scripts/DataBase/Data_Wounded/setHPFightName(fight_name)] WARNING: fight_name.empty() == true! "
                "Trying to set an empty string!");
        }

        this->m_hp_fight_name = fight_name;
    }

    inline const xr_string& getPsyStateName(void) const noexcept { return this->m_psy_state_name; }
    inline void setPsyStateName(const xr_string& psy_state_name) noexcept
    {
        if (psy_state_name.empty())
        {
            Msg("[Scripts/DataBase/Data_Wounded/setPsyStateName(psy_state_name)] WARNING: psy_state_name.empty() == "
                "true! Trying to set an empty string!");
        }

        this->m_psy_state_name = psy_state_name;
    }

    inline const xr_string& getSynDataName(void) const noexcept { return this->m_syndata_name; }
    inline void setSynDataName(const xr_string& syndata_name) noexcept
    {
        if (syndata_name.empty())
        {
            Msg("[Scripts/DataBase/Data_Wounded/setSynDataName(syndata_name)] WARNING: syndata_name.empty() == true! "
                "Trying to set an empty string!");
        }

        this->m_syndata_name = syndata_name;
    }

    inline const xr_string& getHelpDialogName(void) const noexcept { return this->m_help_dialog_name; }
    inline void setHelpDialogName(const xr_string& dialog_name) noexcept
    {
        if (dialog_name.empty())
        {
            Msg("[Scripts/DataBase/Data_Wounded/setHelpDialogName(dialog_name)] WARNING: dialog_name.empty() == true! "
                "Trying to set an empty string!");
        }

        this->m_help_dialog_name = dialog_name;
    }

    inline const xr_string& getHelpStartDialogName(void) const noexcept { return this->m_help_start_dialog_name; }
    inline void setHelpStartDialogName(const xr_string& dialog_name) noexcept
    {
        if (dialog_name.empty())
        {
            Msg("[Scripts/DataBase/Data_Wounded/setHelpStartDialogName(dialog_name)] WARNING: dialog_name.empty() == "
                "true! Trying to set an empty string!");
        }

        this->m_help_start_dialog_name = dialog_name;
    }

    inline const xr_string& getWoundedSectionName(void) const noexcept { return this->m_wounded_section_name; }
    inline void setWoundedSectionName(const xr_string& section_name) noexcept
    {
        if (section_name.empty())
        {
            Msg("[Scripts/DataBase/Data_Wounded/setWoundedSectionName(section_name)] WARNING: section_name.empty() == "
                "true! Trying to set an empty string!");
        }

        this->m_wounded_section_name = section_name;
    }

private:
    bool m_is_autoheal;
    bool m_is_use_medkit;
    bool m_is_enable_talk;
    bool m_is_not_for_help;
    bool m_is_wounded_initialized;
    xr_string m_hp_state_name;
    xr_string m_hp_state_see_name;
    xr_string m_hp_victim_name;
    xr_string m_hp_cover_name;
    xr_string m_hp_fight_name;
    xr_string m_psy_state_name;
    xr_string m_syndata_name;
    xr_string m_help_dialog_name;
    xr_string m_help_start_dialog_name;
    xr_string m_wounded_section_name;
};

class PStor_Data
{
    friend class Storage;
    char m_boolean =
        Globals::kPstorBooleanUndefined; // Globals::kPstorBooleanFalse -> False, Globals::kPstorBooleanUndefined -> Not
                                         // initialized, Globals::kPstorBooleanTrue -> True
    float m_number = 0.0f;
    xr_string m_string = Globals::kStringUndefined;

public:
    inline bool IsInitializedBool(void) const noexcept { return (this->m_boolean != Globals::kPstorBooleanUndefined); }
    inline bool IsInitializedNumber(void) const noexcept { return (!fis_zero(this->m_number)); }
    inline bool IsInitializedString(void) const noexcept { return (this->m_string != Globals::kStringUndefined); }
    inline bool IsInitializedSomething(void) const noexcept
    {
        return (this->IsInitializedBool() || this->IsInitializedNumber() || this->IsInitializedString());
    }
    inline bool getBool(void) const noexcept
    {
        switch (this->m_boolean)
        {
        case '-': {
            Msg("[Scripts/DataBase/PStor_Data/getBool()] Returns an initialized value -> False");
            break;
        }
        case '0': {
            Msg("[Scritps/DataBase/PStor_Data/getBool()] the m_boolean doesn't initialized!");
            break;
        }
        case '+': {
            Msg("[Scripts/DataBase/PStor_Data/getBool()] Returns an initialized value -> True");
            return true;
            break;
        }
        };

        return false;
    }

    inline float getNumber(void) const noexcept
    {
        Msg("[Scripts/DataBase/PStor_Data/getNumber()] Returns value [%d], because it was initialized!",
            this->m_number);
        return this->m_number;
    }

    inline xr_string getString(void) const noexcept
    {
        if (this->m_string == Globals::kStringUndefined)
        {
            Msg("[Scripts/DataBase/PStor_Data/getString()] the m_string doesn't initialized. Returns an empty string.");
            return xr_string();
        }

        Msg("[Scripts/DataBase/PStor_Data/getString()] Returns value [%s], it's initialized!", this->m_string.c_str());
        return this->m_string;
    }

    inline void setBool(const bool value) noexcept
    {
        if (this->m_number != Globals::kUnsignedInt8Undefined || this->m_string != Globals::kStringUndefined)
        {
            Msg("[Scripts/DataBase/PStor_Data/setNumber(value)] You can't assigned a value to m_boolean, because some "
                "member of structure was initialized!");
            return;
        }

        if (value)
        {
            Msg("[Scripts/DataBase/PStor_Data/setBool(value)] -> True");
            this->m_boolean = Globals::kPstorBooleanTrue;
        }
        else
        {
            Msg("[Scripts/DataBase/PStor_Data/setBool(value)] -> False");
            this->m_boolean = Globals::kPstorBooleanFalse;
        }
    }

    inline void setNumber(const float value) noexcept
    {
        if (this->m_boolean != Globals::kPstorBooleanUndefined || this->m_string != Globals::kStringUndefined)
        {
            Msg("[Scripts/DataBase/PStor_Data/setNumber(value)] You can't assigned a value to m_number, because some "
                "member of structure was initialized!");
            return;
        }

        Msg("[Scripts/DataBase/PStor_Data/setNumber(value)] -> %f", value);
        this->m_number = value;
    }

    inline void setString(const xr_string& string) noexcept
    {
        if (this->m_boolean != Globals::kPstorBooleanUndefined || this->m_number != Globals::kUnsignedInt8Undefined)
        {
            Msg("[Scripts/DataBase/PStor_Data/setNumber(value)] You can't assigned a value to m_string, because some "
                "member of structure was initialized!");
            return;
        }

        Msg("[Scripts/DataBase/PStor_Data/setString(string)] -> %s", string.c_str());
        this->m_string = string;
    }
};
// Lord: удалить SubStorage
/*
struct SubStorage_Data
{
private:
    friend class Storage;

public:
    SubStorage_Data(void) = default;
    ~SubStorage_Data(void) = default;

    inline void setSignal(const xr_map<xr_string, bool>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/SubStorage_Data/setSignal(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_signals = map;
    }

    inline void setSignal(const std::pair<xr_string, bool>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("");
            return;
        }
    }
    inline void setSignal(const xr_string& id_name, const bool& value) noexcept
    {
        if (!id_name.size())
        {
            Msg("[DataBase/SubStorage_Data/setSignal(id_name, value)] WARNING: id_name.size() = 0! You are trying to "
                "set an empty string! No assignment!");
            return;
        }

        this->m_signals[id_name] = value;
    }

/ *
    inline void setAction(const xr_vector<Script_ILogicEntity*>& vector)
    {
        if (!vector.size())
        {
            Msg("[DataBase/SubStorage_Data/setAction(vector)] WARNING: vector.size() = 0! You are trying to set an "
                "empty vector! No assignment!");
            return;
        }

        this->m_actions = vector;
    }

    inline void setAction(Script_ILogicEntity* entity)
    {
        if (!entity)
        {
            Msg("[DataBase/SubStorage_Data/setAction(p_entity)] WARNING: p_entity = null! You are trying to set an "
                "empty object! No assignment!");
            return;
        }

        this->m_actions.push_back(entity);
    }* /

    inline const xr_map<xr_string, bool>& getSignals(void) const noexcept { return this->m_signals; }
/ *    inline const xr_vector<Script_ILogicEntity*>& getActions(void) const noexcept { return this->m_actions; }* /
    inline void ClearActions(void)
    {
        Msg("[DataBase/SubStorage_Data/ClearActions()] this->m_actions.clear() is called!");
        this->m_actions.clear();
    }
    inline void ClearSignals(void)
    {
        Msg("[DataBase/SubStorage_Data/ClearSignals()] this->m_signals");
        this->m_signals.clear();
    }

private:
    xr_map<xr_string, bool> m_signals;
/ *    xr_vector<Script_ILogicEntity*> m_actions;* /
};*/
// сделать private, public!
struct StorageAnimpoint_Data
{
private:
    friend class Storage;

public:
    inline void setCoverName(const xr_string& string) { this->m_cover_name = string; }
    inline bool setUseCamp(const bool& value) { this->m_is_use_camp = value; }
    inline void setAnimpoint(Script_Animpoint* object)
    {
        if (!object)
        {
            R_ASSERT2(false, "object was null!");
            return;
        }

        this->m_animpoint = object;
    }
    inline void setReachedDistance(const float& value) { this->m_reached_distance = value; }
    inline void setAvailAnimation(const xr_string& animation_name)
    {
        if (!animation_name.size())
        {
            R_ASSERT2(false, "can't be empty!");
            return;
        }

        this->m_avail_animations.push_back(animation_name);
    }
    inline void setApprovedAction(const std::function<bool(std::uint16_t, bool)> function, const xr_string& action_name)
    {
        if (!action_name.size())
        {
            R_ASSERT2(false, "can't be empty!");
            return;
        }

        if (!function)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        std::pair<std::function<bool(std::uint16_t, bool)>, xr_string> insert_pair;

        insert_pair.first = function;
        insert_pair.second = action_name;
        this->m_approved_actions.push_back(insert_pair);
    }

    inline void setDescriptionName(const xr_string& description_name) noexcept
    {
        if (!description_name.size())
        {
            Msg("[Scripts/DataBase/StorageAnimpoint_Data/setDescriptionName(description_name)] WARNING: you are trying "
                "to set an empty string!");
        }

        this->m_description_name = description_name;
    }

    inline float getReachedDistance(void) const noexcept { return this->m_reached_distance; }
    inline xr_string getCoverName(void) const noexcept { return this->m_cover_name; }
    inline xr_string getDescriptionName(void) const noexcept { return this->m_description_name; }
    inline Script_Animpoint* getAnimpoint(void) const noexcept { return this->m_animpoint; }
    inline const xr_vector<xr_string>& getAvailAnimations(void) const noexcept { return this->m_avail_animations; }
    inline const xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>>& getApprovedActions(
        void) const noexcept
    {
        return this->m_approved_actions;
    }
    inline bool IsUseCamp(void) const noexcept { return this->m_is_use_camp; }

private:
    bool m_is_use_camp;
    float m_reached_distance;
    xr_vector<xr_string> m_avail_animations;
    xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>> m_approved_actions;
    xr_string m_cover_name;
    xr_string m_description_name;
    Script_Animpoint* m_animpoint =
        nullptr; // Lord: узнать где выделяется и удаляется данный класс (xr_animpoint.script)
};

struct Storage_Data
{
private:
    friend class Storage;

public:
    Storage_Data(void) = default;

    class DeathData
    {
    public:
        DeathData(void) : m_killer_id(0) {}
        ~DeathData(void) = default;

        inline const xr_string& getKillerName(void) const noexcept { return this->m_killer_name; }
        inline void setKillerName(const xr_string& name) noexcept
        {
            if (name.empty())
            {
                Msg("[Scripts/DataBase/Storage_Data/setKillerName(name)] WARNING: name.empty() == true! You set an "
                    "empty string");
            }

            this->m_killer_name = name;
        }

        inline std::uint16_t getKillerID(void) const noexcept { return this->m_killer_id; }
        inline void setKillerID(const std::uint16_t ID) noexcept { this->m_killer_id = ID; }

    private:
        std::uint16_t m_killer_id;
        xr_string m_killer_name;
    };

    class HitData
    {
    public:
        HitData(void) : m_is_deadly_hit(false), m_who_id(0), m_bone_index(0) {}
        ~HitData(void) = default;

        inline bool IsDeadlyHit(void) const noexcept { return this->m_is_deadly_hit; }
        inline void setDeadlyHit(const bool value) noexcept { this->m_is_deadly_hit = value; }

        inline std::uint16_t getWhoID(void) const noexcept { return this->m_who_id; }
        inline void setWhoID(const std::uint16_t value) noexcept { this->m_who_id = value; }

        inline std::uint32_t getBoneIndex(void) const noexcept { return this->m_bone_index; }
        inline void setBoneIndex(const std::int16_t value) noexcept { this->m_bone_index; }

    private:
        bool m_is_deadly_hit;
        std::uint16_t m_who_id;
        std::int16_t m_bone_index;
    };

    ~Storage_Data(void)
    {
        this->m_p_client_object = nullptr;
        this->m_p_server_object = nullptr;
        this->m_p_sound_object = nullptr; // @ Lord: нужно ли удалять?
        if (this->m_p_ini)
        {
            Msg("[Scripts/DataBase/Storage_Data/~dtor()] deleting CScriptIniFile");
            if (this->m_is_allocated_ini)
                xr_delete(this->m_p_ini);
            else
                this->m_p_ini = nullptr;
        }

        if (this->m_p_state_manager)
        {
            Msg("[Scripts/DataBase/Storage_Data/~dtor()] deleting m_p_state_manager");
            xr_delete(this->m_p_state_manager);
        }

        if (this->m_p_move_manager)
        {
            Msg("[Scripts/DataBase/Storage_Data/~dtor()] deleting m_p_move_manager");
            xr_delete(this->m_p_move_manager);
        }

        // Lord: проверить деаллокацию
        for (std::pair<const xr_string, Storage_Scheme*>& it : this->m_schemes)
        {
            if (it.second)
            {
                Msg("[Scripts/DataBase/Storage_Data/~dtor()] Deleting storage_scheme of %d %s %s",
                    it.second->getClientObject()->ID(), it.second->getClientObject()->Name(),
                    it.second->getSchemeName().c_str());
                xr_delete(it.second);
            }
        }
    }
    inline bool IsInvulnerable(void) const noexcept { return this->m_is_invulnerable; }
    inline void setInvulnerable(const bool value) noexcept { this->m_is_invulnerable = value; }
    inline bool IsImmortal(void) const noexcept { return this->m_is_immortal; }
    inline void setImmortal(const bool value) noexcept { this->m_is_immortal = value; }
    inline bool IsMute(void) const noexcept { return this->m_is_mute; }
    inline void setMute(const bool value) noexcept { this->m_is_mute = value; }
    inline bool IsAnimMovement(void) const noexcept { return this->m_is_anim_movement; }
    inline void setAnimMovement(const bool value) noexcept { this->m_is_anim_movement = value; }
    inline bool IsEnabled(void) const noexcept { return this->m_is_enabled; }
    inline void setEnabled(const bool value) noexcept { this->m_is_enabled = value; }

    inline std::uint16_t getEnemyID(void) const noexcept { return this->m_enemy_id; }
    inline void setEnemyID(const std::uint16_t& value) noexcept
    {
        if (value == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage_Data/setEnemyID(value)] WARNING: value = std::uint16_t(-1)! You are trying to set "
                "an undefined value! No assignment!");
            return;
        }

        this->m_enemy_id = value;
    }

    inline CScriptGameObject* getClientObject(void) const { return this->m_p_client_object; }
    inline void setClientObject(CScriptGameObject* p_client_object)
    {
        if (!p_client_object)
        {
            Msg("[Scripts/DataBase/Storage_Data/setClientObject(p_client_object)] 'deleting' object from database -> "
                "%s",
                this->m_p_client_object ? this->m_p_client_object->Name() :
                                          "using undefined object, you must add to database at first!");
        }
        else
        {
            this->m_p_client_object = p_client_object;
            Msg("[Scripts/DataBase/Storage/setClientObject(p_client_object)] adding object to database -> %s",
                p_client_object->Name());
        }
    }

    inline const StorageAnimpoint_Data& getStorageAnimpoint(void) const noexcept { return this->m_storage_animpoint; }
    inline void setStorageAnimpoint(const StorageAnimpoint_Data& data) noexcept { this->m_storage_animpoint = data; }

    inline CSE_ALifeObject* getServerObject(void) const { return this->m_p_server_object; }
    inline void setServerObject(CSE_ALifeObject* p_server_object)
    {
        if (!p_server_object)
        {
            Msg("[DataBase/Storage_Data/setServerObject(p_server_object)] WARNING: p_server_object = null! You are "
                "trying to set an empty object! No assignment!");
            return;
        }

        this->m_p_server_object = p_server_object;
    }

    inline CScriptSound* getSoundObject(void) const { return this->m_p_sound_object; }
    inline void setSoundObject(CScriptSound* p_sound_object)
    {
        if (!p_sound_object)
        {
            Msg("[DataBase/Storage_Data/setSoundObject(p_sound_object)] WARNING: p_sound_object = null! You are trying "
                "to set an empty object! No assignment!");
            return;
        }

        this->m_p_sound_object = p_sound_object;
    }

    inline CScriptIniFile* getIni(void) const { return this->m_p_ini; }
    inline void setIni(CScriptIniFile* p_ini, const bool is_allocated)
    {
        if (!p_ini)
        {
            Msg("[DataBase/Storage_Data/setIniObject(p_ini)] WARNING: p_ini = null! You are trying to set an empty "
                "object! No assignment!");
            return;
        }
        this->m_is_allocated_ini = is_allocated;
        this->m_p_ini = p_ini;
    }

    /*    inline void ResetSignals(void) { this->m_data.clear(); }*/

    // @ Gets signals xr_map<xr_string, bool>
    /*
        inline const SubStorage_Data& operator[](const xr_string& id) { return m_data[id]; }

        inline const xr_map<xr_string, SubStorage_Data>& getData(void) const noexcept { return this->m_data; }

        inline void setData(const xr_map<xr_string, SubStorage_Data>& map)
        {
            if (!map.size())
            {
                Msg("[DataBase/Storage_Data/setData(map)] WARNING: map.size() = 0! You are trying to set an empty map!
       No " "assignment!"); return;
            }

            this->m_data = map;
        }

        inline void setData(const std::pair<xr_string, SubStorage_Data>& pair)
        {
            if (!pair.first.size())
            {
                Msg("[DataBase/Storage_Data/setData(pair)] WARNING: pair.first.size() = 0! You are trying to set an
       empty " "string! No assignment!"); return;
            }

            this->m_data.insert(pair);
        }

        inline void setData(const xr_string& id_name, const SubStorage_Data& data)
        {
            if (!id_name.size())
            {
                Msg("[DataBase/Storage_Data/setData(id_name, data)] WARNING: id_name.size() = 0! You are trying to set
       an " "empty string! No assignment!"); return;
            }

            this->m_data[id_name] = data;
        }*/

    inline const xr_map<xr_string, PStor_Data>& getPStor(void) const noexcept { return this->m_pstor; }

    inline void setPStor(const xr_map<xr_string, PStor_Data>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage_Data/setPStor(map)] WARNING: map.size() = 0! You are trying to set an empty map! No "
                "assignment!");
            return;
        }

        this->m_pstor = map;
    }

    inline void setPStor(const std::pair<xr_string, PStor_Data>& pair) noexcept
    {
        if (!pair.first.size())
        {
            Msg("[DataBase/Storage_Data/setPStor(pair)] WARNING: pair.first.size() = 0! You are trying to set an empty "
                "pair! No assignment!");
            return;
        }

        this->m_pstor.insert(pair);
    }

    inline void setPStor(const xr_string& id_name, const PStor_Data& data) noexcept
    {
        if (!id_name.size())
        {
            Msg("[DataBase/Storage_Data/setPStor(id_name, data)] WARNING: id_name.size() = 0! You are trying to set an "
                "empty string! No assignment!");
            return;
        }

        this->m_pstor[id_name] = data;
    }

    inline const xr_string& getActiveSchemeName(void) const noexcept { return this->m_active_scheme_name; }
    inline void setActiveSchemeName(const xr_string& scheme_name) noexcept { this->m_active_scheme_name = scheme_name; }

    inline const xr_string& getActiveSectionName(void) const noexcept { return this->m_active_section_name; }
    inline void setActiveSectionName(const xr_string& section_name) noexcept
    {
        this->m_active_section_name = section_name;
    }

    inline const xr_string& getSoundName(void) const noexcept { return this->m_sound_name; }
    inline void setSoundName(const xr_string& sound_name) noexcept { this->m_sound_name = sound_name; }

    inline const xr_string& getAnimationName(void) const noexcept { return this->m_animation_name; }
    inline void setAnimationName(const xr_string& animation_name) noexcept { this->m_animation_name = animation_name; }

    inline const xr_string& getAnimationHeadName(void) const noexcept { return this->m_animation_head_name; }
    inline void setAnimationHeadName(const xr_string& animation_head_name) noexcept
    {
        this->m_animation_head_name = animation_head_name;
    }

    inline const xr_string& getTipName(void) const noexcept { return this->m_tip_name; }
    inline void setTipName(const xr_string& tip_name) noexcept { this->m_tip_name = tip_name; }

    inline const xr_string& getTimeName(void) const noexcept { return this->m_time_name; }
    inline void setTimeName(const xr_string& time_name) noexcept { this->m_time_name = time_name; }

    inline const xr_string& getJobIniName(void) const noexcept { return this->m_job_ini_name; }
    inline void setJobIniName(const xr_string& job_ini_name) noexcept { this->m_job_ini_name = job_ini_name; }

    inline const xr_string& getLoadedInifilename(void) const noexcept { return this->m_loaded_ini_filename; }
    inline void setLoadedInifilename(const xr_string& filename) noexcept { this->m_loaded_ini_filename = filename; }

    inline const xr_string& getLoadedSectionLogicName(void) const noexcept { return this->m_loaded_section_logic_name; }
    inline void setLoadedSectionLogicName(const xr_string& logic_name) noexcept
    {
        this->m_loaded_section_logic_name = logic_name;
    }

    inline const xr_string& getLoadedActiveSectionName(void) const noexcept
    {
        return this->m_loaded_active_section_name;
    }
    inline void setLoadedActiveSectionName(const xr_string& section_name) noexcept
    {
        this->m_loaded_active_section_name = section_name;
    }

    inline const xr_string& getLoadedGulagName(void) const noexcept { return this->m_loaded_gulag_name; }
    inline void setLoadedGulagName(const xr_string& gulag_name) noexcept { this->m_loaded_gulag_name = gulag_name; }

    inline const xr_string& getInifilename(void) const noexcept { return this->m_ini_filename; }
    inline void setInifilename(const xr_string& filename) noexcept { this->m_ini_filename = filename; }

    inline const xr_string& getSectionLogicName(void) const noexcept { return this->m_section_logic_name; }
    inline void setSectionLogicName(const xr_string& section_logic_name) noexcept
    {
        this->m_section_logic_name = section_logic_name;
    }

    inline const xr_string& getGulagName(void) const noexcept { return this->m_gulag_name; }
    inline void setGulagName(const xr_string& gulag_name) noexcept { this->m_gulag_name = gulag_name; }

    inline const xrTime& getActivationGameTime(void) const noexcept { return this->m_activation_game_time; }
    inline void setActivationGameTime(const xrTime& time) noexcept { this->m_activation_game_time = time; }

    inline const std::int32_t getActivationTime(void) const noexcept { return this->m_activation_time; }
    inline void setActivationTime(const std::int32_t value) noexcept { this->m_activation_time = value; }

    inline const xr_map<xr_string, Storage_Scheme*>& getSchemes(void) const noexcept { return this->m_schemes; }

    inline void setScheme(const xr_map<xr_string, Storage_Scheme*>& map) noexcept
    {
        if (map.empty())
        {
            Msg("[Scripts/DataBase/Storage_Data/setScheme(map)] WARNING: map.empty() == true! Can't assign an empty "
                "map!");
            return;
        }

        this->m_schemes = map;
    }

    inline void setScheme(const std::pair<xr_string, Storage_Scheme*>& pair) noexcept
    {
        if (pair.first.empty())
        {
            Msg("[Scripts/DataBase/Storage_Data/setScheme(pair)] WARNING: pair.first.empty() == true! Can't assign an "
                "empty string return ...");
            return;
        }

        this->m_schemes.insert(pair);
    }

    inline void setScheme(const xr_string& scheme_name, Storage_Scheme* const data) noexcept
    {
        if (scheme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Data/setScheme(scheme_name, data)] WARNING: scheme_name.empty() == true! "
                "Can't "
                "assign return ...");
            return;
        }

        this->m_schemes[scheme_name] = data;
    }

    inline void setSchemesActions(const xr_string& scheme_name, Script_ISchemeEntity* p_scheme)
    {
        if (scheme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Data/setSchemeActions(scheme_name, p_scheme)] WARNING: scheme_name.empty() "
                "== true! Can't assign return ...");
            return;
        }

        this->m_schemes[scheme_name]->setActions(p_scheme);
    }

    inline void setSchemesSectionName(const xr_string& scheme_name, const xr_string& section_name) noexcept
    {
        if (scheme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Data/setSchemesSectionName(scheme_name, section_name)] WARNING: "
                "scheme_name.empty() == true! Can't assign return ...");
            return;
        }

        this->m_schemes[scheme_name]->setLogicName(section_name);
    }

    inline void setSchemesEnabled(const xr_string& scheme_name, const bool value) noexcept
    {
        if (scheme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Data/setSchemesEnabled(scheme_name, value)] WARNING: scheme_name.empty() == "
                "true! Can't assign return ...");
            return;
        }

        this->m_schemes[scheme_name]->setEnabled(value);
    }

    inline const DeathData& getDeathData(void) const noexcept { return this->m_death; }
    inline void setDeathData(const DeathData& data) noexcept { this->m_death = data; }
    inline void setDeathDataKillerName(const xr_string& name) noexcept { this->m_death.setKillerName(name); }
    inline void setDeathDataKillerID(const std::uint16_t npc_id) noexcept { this->m_death.setKillerID(npc_id); }

    inline const Data_Overrides& getOverrides(void) const noexcept { return this->m_overrides; }
    inline void setOverrides(const Data_Overrides& data) noexcept { this->m_overrides = data; }

    inline std::uint8_t getSchemeType(void) const noexcept { return this->m_scheme_type; }
    inline void setSchemeType(const std::uint8_t stype) noexcept { this->m_scheme_type = stype; }

    inline const HitData& getHit(void) const noexcept { return this->m_hit; }
    inline void setHitDeadlyHit(const bool value) noexcept { this->m_hit.setDeadlyHit(value); }
    inline void setHitWhoID(const std::uint16_t value) noexcept { this->m_hit.setWhoID(value); }
    inline void setHitBoneIndex(const std::int16_t value) noexcept { this->m_hit.setBoneIndex(value); }

    inline std::uint16_t getCorpseAlreadySelected(void) const noexcept { return this->m_corpse_already_selected; }
    inline void setCorpseAlreadySelected(const std::uint16_t value) noexcept
    {
        this->m_corpse_already_selected = value;
    }

    inline std::uint16_t getWoundedAlreadySelected(void) const noexcept { return this->m_wounded_already_selected; }
    inline void setWoundedAlreadySelected(const std::uint16_t value) noexcept
    {
        this->m_wounded_already_selected = value;
    }

    inline bool IsDangerFlag(void) const noexcept { return this->m_is_danger_flag; }
    inline void setDangerFlag(const bool value) noexcept { this->m_is_danger_flag = value; }

    inline Script_StateManager* const getStateManager(void) const noexcept { return this->m_p_state_manager; }
    inline void setStateManager(Script_StateManager* const p_state_manager)
    {
        if (!p_state_manager)
        {
            Msg("[Scripts/DataBase/Script_StorageData/setStateManager(p_state_manager)] WARNING: you can't set nullptr "
                "for object return...");
            return;
        }

        this->m_p_state_manager = p_state_manager;
    }

    inline void setStateManagerSetState(const xr_string& state_name, StateManagerCallbackData& callback,
        const std::uint32_t timeout, std::pair<Fvector, CScriptGameObject* const> target,
        const StateManagerExtraData& extra)
    {
        this->m_p_state_manager->set_state(state_name, callback, timeout, target, extra);
    }

    inline Script_MoveManager* const getMoveManager(void) const noexcept { return this->m_p_move_manager; }

    inline void setMoveManager(Script_MoveManager* const p_move_manager)
    {
        if (!p_move_manager)
        {
            Msg("[Scripts/DataBase/Storage_Data/setMoveManager(p_move_manager)] WARNING: p_move_manager == nullptr! "
                "You setan empty object is not right ... Return");
            return;
        }

        this->m_p_move_manager = p_move_manager;
    }

    inline void setClientEnemy(CScriptGameObject* const p_enemy)
    {
        if (!p_enemy)
        {
            Msg("[Scripts/DataBase/Storage_Data/setClientEnemy(p_enemy)] WARNING: p_enemy == nullptr! You set an empty "
                "object!");
        }

        this->m_p_client_enemy = p_enemy;
    }

    inline CScriptGameObject* const getClientEnemy(void) const noexcept { return this->m_p_client_enemy; }

    inline const xr_string& getScriptCombatTypeName(void) const noexcept { return this->m_script_combat_type_name; }
    inline void setScriptCombatTypeName(const xr_string& type_name) noexcept
    {
        if (type_name.empty())
        {
            Msg("[Scripts/DataBase/Storage_Data/setScriptCombatTypeName(type_name)] WARNING: type_name.empty() == "
                "true! You are set an empty string!");
        }

        this->m_script_combat_type_name = type_name;
    }

    inline std::uint32_t getDisableInputIdle(void) const noexcept { return this->m_disable_input_idle; }
    inline void setDisableInputIdle(const std::uint32_t value) noexcept { this->m_disable_input_idle = value; }

    inline const xrTime& getDisableInputTime(void) const noexcept { return this->m_disable_input_time; }
    inline void setDisableInputTime(const xrTime& time) noexcept { this->m_disable_input_time = time; }

    inline std::uint16_t getRegisteredCamp(void) const noexcept { return this->m_registered_camp; }
    inline void setRegisteredCamp(const std::uint16_t id) noexcept { this->m_registered_camp = id; }

private:
    bool m_is_invulnerable = false;
    bool m_is_immortal = false;
    bool m_is_mute = false;
    bool m_is_enabled = false;
    bool m_is_anim_movement = false;
    bool m_is_allocated_ini = false;
    bool m_is_danger_flag = false;
    std::uint8_t m_scheme_type = 0;
    std::uint16_t m_enemy_id = Globals::kUnsignedInt16Undefined;
    std::uint16_t m_corpse_already_selected = 0;
    std::uint16_t m_wounded_already_selected = 0;
    std::uint16_t m_registered_camp = 0;
    std::int32_t m_activation_time = 0;
    std::uint32_t m_disable_input_idle;
    xrTime m_disable_input_time;
    HitData m_hit;
    CScriptGameObject* m_p_client_object = nullptr;
    CScriptGameObject* m_p_client_enemy = nullptr;
    StorageAnimpoint_Data m_storage_animpoint;
    CSE_ALifeObject* m_p_server_object = nullptr;
    CScriptSound* m_p_sound_object = nullptr;
    CScriptIniFile* m_p_ini = nullptr;
    Script_StateManager* m_p_state_manager = nullptr;
    Script_MoveManager* m_p_move_manager = nullptr;
    xrTime m_activation_game_time;
    /*    xr_map<xr_string, SubStorage_Data> m_data;*/
    xr_map<xr_string, PStor_Data> m_pstor;
    xr_map<xr_string, Storage_Scheme*> m_schemes;
    xr_string m_active_scheme_name;
    xr_string m_active_section_name;
    xr_string m_sound_name;
    xr_string m_animation_name;
    xr_string m_animation_head_name;
    xr_string m_tip_name;
    xr_string m_time_name;
    xr_string m_job_ini_name;
    xr_string m_loaded_ini_filename; // @ Idk but this doesn't use well as planned, CUZ IT'S GSC
    xr_string m_loaded_section_logic_name; // @ Idk but this doesn't use well as planned, CUZ IT'S GSC
    xr_string m_loaded_active_section_name; // @ Idk but this doesn't use well as planned, CUZ IT'S GSC
    xr_string m_loaded_gulag_name;
    xr_string m_ini_filename;
    xr_string m_section_logic_name;
    xr_string m_gulag_name;
    xr_string m_script_combat_type_name;
    DeathData m_death;
    Data_Overrides m_overrides;
};

class Storage
{
private:
    Storage(void) = default;

public:
    inline static Storage& getInstance(void) noexcept
    {
        static Storage instance;
        return instance;
    }

    // Lord: или переместить в другой метод! Потестить
    ~Storage(void)
    {
        // @ Lord: подумать здесь нужно это удалять так или оно в другом месте?
        for (xr_map<std::uint16_t, Storage_Data>::value_type& it : this->m_storage)
        {
            if (it.second.getStorageAnimpoint().getAnimpoint())
            {
                Msg("[Scripts/DataBase/Storage/~dtor] Deleting: Animpoint -> [%s]",
                    it.second.getStorageAnimpoint().getCoverName().c_str());
                Script_Animpoint* instance = it.second.getStorageAnimpoint().getAnimpoint();
                delete instance;
                instance = nullptr;
            }

            /*
                        for (const xr_map<xr_string, SubStorage_Data>::value_type& object : it.second.getData())
                        {
                            if (object.second.getActions().size())
                            {
                                for (Script_ILogicEntity* entity : object.second.getActions())
                                {
                                    if (entity)
                                    {
                                        Msg("[Scripts/DataBase/Storage/~dtor] Deleting Script_IEntity: %s",
                                            entity->m_logic_name.c_str());
                                        delete entity;
                                        entity = nullptr;
                                    }
                                }
                            }
                        }*/

            /* Lord: удалить данный комментарий!
                        if (it.second.getClientObject())
                        {
                            CScriptGameObject* p_client_object = it.second.getClientObject();
                            Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_object: %s", p_client_object->Name());
                            delete p_client_object;
                            p_client_object = nullptr;
                        }

                        if (it.second.getServerObject())
                        {
                            CSE_ALifeObject* p_server_object = it.second.getServerObject();
                            Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_server_object: %s",
               p_server_object->name()); delete p_server_object; p_server_object = nullptr;
                        }*/

            if (it.second.getIni())
            {
                CInifile* p_ini = it.second.getIni();
                Msg("[Scripts/DataBase/Storage/~dtor] Delete the m_ini: %s", p_ini->fname());
                delete p_ini;
                p_ini = nullptr;
            }

            if (it.second.getSoundObject())
            {
                CScriptSound* p_sound = it.second.getSoundObject();
                Msg("[Scripts/DataBase/Storage/~dtor] Deleting the m_sound_object");
                delete p_sound;
                p_sound = nullptr;
            }
        }

        this->m_storage.clear();
    }

#pragma region Getters
    inline const xr_map<std::uint16_t, Storage_Data>& getStorage(void) const noexcept { return this->m_storage; }

    inline void setStorage(const xr_map<std::uint16_t, Storage_Data>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage/setStorage(map)] WARNING: map.size() = 0! You are trying to set an empty map! No "
                "assignment!");
            return;
        }

        this->m_storage = map;
    }

    inline void setStorage(const std::pair<std::uint16_t, Storage_Data>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setStorage(pair)] WARNING: pair.first = std::uint16_t(-1)! You are trying to set an "
                "undefined variable! No assignment!");
            return;
        }

        this->m_storage.insert(pair);
    }
    inline void setStorage(const std::uint16_t& id, const Storage_Data& data) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setStorage(id, data)] WARNING: id = std::uint16_t(-1)! You are trying to set an "
                "undefined variable! No assignment!");
            return;
        }

        this->m_storage[id] = data;
    }

    inline CScriptGameObject* getActor(void) const { return this->m_actor; }
    inline CScriptGameObject* getInvetoryUpgradesVictim(void) const { return this->m_p_victim; }

    // Это need_victim() в inventory_upgrades!
    inline void setInventoryUpgradesVictim(CScriptGameObject* const p_object)
    {
        if (!p_object)
        {
            Msg("[Scripts/DataBase/setInventoryUpgradesVictim(p_object)] WARNING: p_object = nullptr!");
            return;
        }

        this->m_p_victim = p_object;
    }

    // Lord: переделать сюда идёт bind_anomaly_zone
    inline const xr_map<xr_string, CScriptGameObject*>& getAnomalyByName(void) const noexcept
    {
        return this->m_anomaly_by_name;
    }

    inline void setAnomalyByName(const xr_map<xr_string, CScriptGameObject*>& map)
    {
        if (!map.size())
        {
            Msg("[Scripts/DataBase/Storage/setAnomalyByName(map)] WARNING: map.size() = 0! You are trying to set empty "
                "map! Return!");
            return;
        }

        this->m_anomaly_by_name = map;
    }

    inline void setAnomalyByName(const std::pair<xr_string, CScriptGameObject*>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Scripts/DataBase/Storage/setAnomalyByName(pair)] WARNING: pair.first.size() = 0! You are trying to "
                "set an empty string! Return");
            return;
        }

        if (!pair.second)
        {
            Msg("[Scripts/DataBase/Storage/setAnomalyByName(pair)] WARNING: pair.second = nullptr! You are trying to "
                "set an empty object! Return!");
            return;
        }

        this->m_anomaly_by_name.insert(pair);
    }

    inline void setAnomalyByName(const xr_string& anomaly_name, CScriptGameObject* object)
    {
        if (!anomaly_name.size())
        {
            Msg("[Scripts/DataBase/Storage/setAnomalyByName(anomaly_name, object)] WARNING: anomaly_name.size() = 0! "
                "You are trying to set an empty string! Return;");
            return;
        }

        if (!object)
        {
            Msg("[Scripts/DataBase/Storage/setAnomalyByName(anomaly_name, object)] WARNING: object = nullptr! You are "
                "trying to set an empty object!");
            return;
        }

        this->m_anomaly_by_name[anomaly_name] = object;
    }

    inline const xr_map<xr_string, CScriptGameObject*>& getZoneByName(void) const noexcept
    {
        return this->m_zone_by_name;
    }

    inline void setZoneByName(const xr_map<xr_string, CScriptGameObject*>& map)
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage/setZoneByName(map)] WARNING: map.size() = 0! You are trying to set an empty map! No "
                "assignment!");
            return;
        }

        this->m_zone_by_name = map;
    }

    inline void setZoneByName(const std::pair<xr_string, CScriptGameObject*>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[DataBase/Storage/setZoneByName(pair)] WARNING: pair.first.size() = 0! You are "
                "trying to set an empty string! No assignment!");
            return;
        }

        if (!pair.second)
        {
            Msg("[DataBase/Storage/setZoneByName(pair)] WARNING: pair.second = null! You are "
                "trying to set an empty object! No assignment!");
            return;
        }

        this->m_zone_by_name.insert(pair);
    }

    inline void setZoneByName(const xr_string& zone_name, CScriptGameObject* p_client_zone)
    {
        if (!zone_name.size())
        {
            Msg("[DataBase/Storage/setZoneByName(zone_name, p_client_zone)] WARNING: zone_name.size() = 0! You are "
                "trying to set an empty string! No assignment!");
            return;
        }

        if (!p_client_zone)
        {
            Msg("[DataBase/Storage/setZoneByName(zone_name, p_client_zone)] WARNING: p_client_zone = null! You are "
                "trying to set an empty object! No assignment!");
            return;
        }

        this->m_zone_by_name[zone_name] = p_client_zone;
    }

    inline const xr_map<std::uint16_t, float>& getGoodwill_Sympathy(void) const noexcept
    {
        return this->m_goodwill.first;
    }

    inline void setGoodwill_Sympathy(const xr_map<std::uint16_t, float>& first) noexcept
    {
        if (!first.size())
        {
            Msg("DataBase/Storage/setGoodwill_Sympathy(first, second)] WARNING: first.size() = 0! You are trying to "
                "set an "
                "empty map! No assignment!");
            return;
        }

        this->m_goodwill.first = first;
    }

    inline void setGoodwill_Sympathy(const std::pair<std::uint16_t, float>& pair_first) noexcept
    {
        if (pair_first.first == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setGoodwill_Sympathy(pair_first)] WARNING: pair_first.first = std::uint16_t(-1)! "
                "You are "
                "trying to set an undefined value! No assignment!");
            return;
        }

        this->m_goodwill.first.insert(pair_first);
    }

    inline void setGoodwill_Sympathy(const std::uint16_t& id, const float& value) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setGoodwill_Sympathy(id, value)] WARNING: id = std::uint16_t(-1)! You are trying to "
                "set an "
                "undefined value! No assignment!");
            return;
        }

        this->m_goodwill.first[id] = value;
    }

    inline const xr_map<std::uint16_t, xr_string>& getGoodwill_Relations(void) const noexcept
    {
        return this->m_goodwill.second;
    }

    inline void setGoodwill_Relations(const xr_map<std::uint16_t, xr_string>& second) noexcept
    {
        if (!second.size())
        {
            Msg("[DataBase/Storage/setGoodwill_Relations(first, second] WARNING: second.size() = 0! You are trying to "
                "set an "
                "empty map! No assignment!");
            return;
        }

        this->m_goodwill.second = second;
    }

    inline void setGoodwill_Relations(const std::pair<std::uint16_t, xr_string>& pair_second) noexcept
    {
        if (pair_second.first == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setGoodwill_Relations(pair_second)] WARNING: pair_second.first = std::uint16_t(-1)! "
                "You are "
                "trying to set an undefined value! No assignment!");
            return;
        }

        this->m_goodwill.second.insert(pair_second);
    }

    inline void setGoodwill_Relations(const std::uint16_t& id, const xr_string& string) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setGoodwill_Relations(id, string)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to set an "
                "undefined value! No assignment!");
            return;
        }

        this->m_goodwill.second[id] = string;
    }

    inline const xr_map<std::uint16_t, std::uint32_t>& getSpawnedVertexByID(void) const noexcept
    {
        return this->m_spawned_vertex_by_id;
    }

    inline void setSpawnedVertexByID(const xr_map<std::uint16_t, std::uint32_t>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage/setSpawnedVertexByID(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map! No assignment!");
            return;
        }

        this->m_spawned_vertex_by_id = map;
    }

    inline void setSpawnedVertexByID(const std::pair<std::uint16_t, std::uint32_t>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setSpawnedVertexByID(pair)] WARNING: pair.first = std::uint16_t(-1)! You are trying "
                "to set an undefined value! No assignment!");
            return;
        }

        this->m_spawned_vertex_by_id.insert(pair);
    }

    inline void setSpawnedVertexByID(const std::uint16_t& id, const std::uint32_t& value) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setSpawnedVertexByID(id, value)] WARNING: id = std::uint16_t(-1)! You are trying to "
                "set an undefined value! No assignment!");
            return;
        }

        this->m_spawned_vertex_by_id[id] = value;
    }

    inline const xr_map<std::uint16_t, std::pair<std::uint16_t, xr_string>>& getOfflineObjects(void) const noexcept
    {
        return this->m_offline_objects;
    }

    /*
        inline void ClearOfflineObjects_Second(const std::uint16_t& id) noexcept
        {
            if (id == Globals::kUnsignedInt16Undefined)
            {
                Msg("[DataBase/Storage/ClearOfflineObjects_Second(id)] WARNING: id = std::uint16_t(-1)! You are trying
       to get access through an undefined value! NO assignment!"); return;
            }

            this->m_offline_objects[id].second.clear();
        }*/

    inline void setOfflineObjects(const xr_map<std::uint16_t, std::pair<std::uint16_t, xr_string>>& map) noexcept
    {
        if (!map.size())
        {
            Msg("[DataBase/Storage/setOfflineObjects(map)] WARNING: map.size() = 0! You are trying to set an empty "
                "map!");
            //  return;
        }

        this->m_offline_objects = map;
    }

    inline void setOfflineObjects(const std::pair<std::uint16_t, std::pair<std::uint16_t, xr_string>>& pair) noexcept
    {
        if (pair.first == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(pair)] WARNING: pair.first = std::uint16_t(-1)! You are trying to "
                "set "
                "an empty pair!");
            //   return;
        }

        this->m_offline_objects.insert(pair);
    }

    inline void setOfflineObjects(const std::uint16_t& id, const std::pair<std::uint16_t, xr_string>& pair) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(id, pair)] WARNING: id = std::uint16_t(-1)! You are trying to set "
                "an undefined value!");
            //   return;
        }

        this->m_offline_objects[id] = pair;
    }

    // @ Lord: наименовать нормально аргументы
    inline void setOfflineObjects(const std::uint16_t& id, const std::uint16_t& _id, const xr_string& string) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(id, _id, string)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to set an undefined value!");
            //    return;
        }

        this->m_offline_objects[id] = {_id, string};
    }

    inline void setOfflineObjects(const std::uint16_t& id, const std::uint16_t& _id) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(id, _id, string)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to set an undefined value!");
            //  return;
        }

        this->m_offline_objects[id].first = _id;
    }

    inline void setOfflineObjects(const std::uint16_t& id, const xr_string& string) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setOfflineObjects(id, _id, string)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to set an undefined value!");
            //  return;
        }

        this->m_offline_objects[id].second = string;
    }

    inline const std::pair<xr_map<std::uint16_t, float>, xr_map<std::uint16_t, xr_string>>& getGoodwill(void) const
        noexcept
    {
        return this->m_goodwill;
    }

    inline void setGoodwill(
        const xr_map<std::uint16_t, float>& first, const xr_map<std::uint16_t, xr_string>& second) noexcept
    {
        if (!first.size())
        {
            Msg("DataBase/Storage/setGoodwill(first, second)] WARNING: first.size() = 0! You are trying to set an "
                "empty map! No assignment!");
            return;
        }

        if (!second.size())
        {
            Msg("[DataBase/Storage/setGoodwill(first, second] WARNING: second.size() = 0! You are trying to set an "
                "empty map! No assignment!");
            return;
        }

        this->m_goodwill.first = first;
        this->m_goodwill.second = second;
    }
#pragma endregion

#pragma region Cordis Setters
#pragma region Cordis DataBase Storage_Data setters
    inline void UnsubscribeAction(const std::uint16_t npc_id, const xr_string& scheme_name) noexcept
    {
        if (scheme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage/UnsubscribeAction(npc_id, scheme_name)] WARNING: scheme_name.empty() == "
                "true! Return ...");
            return;
        }

        if (this->m_storage[npc_id].getSchemes().find(scheme_name) == this->m_storage[npc_id].getSchemes().end())
        {
            Msg("[Scripts/DataBase/Storage/UnsubscribeAction(npc_id, scheme_name)] WARNING: can't find scheme by "
                "scheme_name %s",
                scheme_name.c_str());
            return;
        }

        this->m_storage[npc_id].m_schemes[scheme_name]->UnSubscribeAction();
    }

    void setStorageStateManager(CScriptGameObject* const p_client_object, Script_StateManager* const p_state_manager);
    void setStorageMoveManager(CScriptGameObject* const p_client_object, Script_MoveManager* const p_move_manager);

    inline void setStorageDisableInputIdle(const std::uint16_t npc_id, const std::uint32_t value) noexcept
    {
        this->m_storage[npc_id].setDisableInputIdle(value);
    }

    inline void setStorageDisableInputTime(const std::uint16_t npc_id, const xrTime& time) noexcept
    {
        this->m_storage[npc_id].setDisableInputTime(time);
    }

    inline void setStorageScriptCombatTypeName(const std::uint16_t npc_id, const xr_string& type_name)
    {
        this->m_storage[npc_id].setScriptCombatTypeName(type_name);
    }

    inline void setStorageEnemy(const std::uint16_t npc_id, CScriptGameObject* const p_enemy)
    {
        this->m_storage[npc_id].setClientEnemy(p_enemy);
    }

    inline void setStorageStateManagerSetState(const std::uint16_t npc_id, const xr_string& state_name,
        StateManagerCallbackData& callback, const std::uint32_t timeout,
        std::pair<Fvector, CScriptGameObject* const> target, const StateManagerExtraData& extra) noexcept
    {
        this->m_storage[npc_id].setStateManagerSetState(state_name, callback, timeout, target, extra);
    }

    // Lord: обновлять сеттеры покуда обновляется сам Storage_Data
    inline void setStorageInvulnerable(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setInvulnerable(value);
    }

    inline void setStorageImmortal(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setImmortal(value);
    }

    inline void setStorageMute(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setMute(value);
    }

    inline void setStorageEnabled(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setEnabled(value);
    }

    inline void setStorageAnimMovement(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setAnimMovement(value);
    }

    inline void setStorageEnemyID(const std::uint16_t npc_id, const std::uint16_t enemy_id) noexcept
    {
        this->m_storage[npc_id].setEnemyID(enemy_id);
    }

    inline void setStorageAnimpointStorage(const std::uint16_t npc_id, const StorageAnimpoint_Data& data) noexcept
    {
        this->m_storage[npc_id].setStorageAnimpoint(data);
    }

    inline void setStorageServerObject(const std::uint16_t npc_id, CSE_ALifeObject* server_object) noexcept
    {
        this->m_storage[npc_id].setServerObject(server_object);
    }

    inline void setStorageSoundObject(const std::uint16_t npc_id, CScriptSound* p_sound_object) noexcept
    {
        this->m_storage[npc_id].setSoundObject(p_sound_object);
    }

    inline void setStorageIniFile(const std::uint16_t npc_id, CScriptIniFile* ini, const bool is_allocated) noexcept
    {
        this->m_storage[npc_id].setIni(ini, is_allocated);
    }

    inline void setStorageSignal(const std::uint16_t id, const xr_string& signal_name, const bool value) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setSignal(id, signal_name, value)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to get access through an undefined value! No assignment!");
            return;
        }

        this->m_storage[id].m_schemes[this->m_storage[id].getActiveSchemeName()]->setSignals(signal_name, value);
    }

    inline void setStorageActiveSchemeName(const std::uint16_t npc_id, const xr_string& active_scheme_name) noexcept
    {
        this->m_storage[npc_id].setActiveSchemeName(active_scheme_name);
    }

    inline void setStorageActiveSectionName(const std::uint16_t npc_id, const xr_string& active_section_name) noexcept
    {
        this->m_storage[npc_id].setActiveSectionName(active_section_name);
    }

    inline void setStorageSoundName(const std::uint16_t npc_id, const xr_string& sound_name) noexcept
    {
        this->m_storage[npc_id].setSoundName(sound_name);
    }

    inline void setStorageAnimationName(const std::uint16_t npc_id, const xr_string& animation_name) noexcept
    {
        this->m_storage[npc_id].setAnimationName(animation_name);
    }

    inline void setStorageAnimationHeadName(const std::uint16_t npc_id, const xr_string& animation_head_name) noexcept
    {
        this->m_storage[npc_id].setAnimationHeadName(animation_head_name);
    }

    inline void setStorageTipName(const std::uint16_t npc_id, const xr_string& tip_name) noexcept
    {
        this->m_storage[npc_id].setTipName(tip_name);
    }

    inline void setStorageTimeName(const std::uint16_t npc_id, const xr_string& time_name) noexcept
    {
        this->m_storage[npc_id].setTimeName(time_name);
    }

    inline void setStorageJobIniName(const std::uint16_t npc_id, const xr_string& job_ini_name) noexcept
    {
        this->m_storage[npc_id].setJobIniName(job_ini_name);
    }

    inline void setStorageLoadedInifilename(const std::uint16_t npc_id, const xr_string& filename) noexcept
    {
        this->m_storage[npc_id].setLoadedInifilename(filename);
    }

    inline void setStorageLoadedSectionLogicName(const std::uint16_t npc_id, const xr_string& logic_name) noexcept
    {
        this->m_storage[npc_id].setLoadedSectionLogicName(logic_name);
    }

    inline void setStorageLoadedActiveSectionName(
        const std::uint16_t npc_id, const xr_string& active_section_name) noexcept
    {
        this->m_storage[npc_id].setLoadedActiveSectionName(active_section_name);
    }

    inline void setStorageLoadedGulagName(const std::uint16_t npc_id, const xr_string& gulag_name) noexcept
    {
        this->m_storage[npc_id].setLoadedGulagName(gulag_name);
    }

    inline void setStorageActivationGameTime(const std::uint16_t npc_id, const xrTime& time) noexcept
    {
        this->m_storage[npc_id].setActivationGameTime(time);
    }

    inline void setStorageActivationTime(const std::uint16_t npc_id, const std::int32_t value) noexcept
    {
        this->m_storage[npc_id].setActivationTime(value);
    }

    inline void setStorageInifilename(const std::uint16_t npc_id, const xr_string& filename) noexcept
    {
        this->m_storage[npc_id].setInifilename(filename);
    }

    inline void setStorageSectionLogicName(const std::uint16_t npc_id, const xr_string& section_logic_name) noexcept
    {
        this->m_storage[npc_id].setSectionLogicName(section_logic_name);
    }

    inline void setStorageGulagName(const std::uint16_t npc_id, const xr_string& gulag_name) noexcept
    {
        this->m_storage[npc_id].setGulagName(gulag_name);
    }

    inline void setStorageSchemes(const std::uint16_t npc_id, const xr_map<xr_string, Storage_Scheme*>& map) noexcept
    {
        if (map.empty())
        {
            Msg("[Scripts/DataBase/Storage/setSchemes(map)] WARNING: can't assign an empty map! Return");
            return;
        }

        this->m_storage[npc_id].setScheme(map);
    }

    inline void setStorageScheme(const std::uint16_t npc_id, const std::pair<xr_string, Storage_Scheme*>& pair) noexcept
    {
        if (pair.first.empty())
        {
            Msg("[Scripts/DataBase/Storage/setScheme(pair)] WARNING: pair.first.empty() == true! Can't assign return "
                "...");
            return;
        }

        this->m_storage[npc_id].setScheme(pair);
    }

    inline void setStorageScheme(
        const std::uint16_t npc_id, const xr_string& scheme_name, Storage_Scheme* const data) noexcept
    {
        if (scheme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage/setScheme(scheme_name, data)] WARNING: scheme_name.empty() == true! Can't "
                "assign return ...");
            return;
        }

        Msg("[Scripts/DataBase/Storage/setScheme(scheme_name, data)] Registered scheme %s", scheme_name);

        this->m_storage[npc_id].setScheme(scheme_name, data);
    }

    inline void setStorageSchemesActionSchemeIDForUnSubscribing(
        const std::uint16_t npc_id, const xr_string& scheme_name, const std::uint32_t scheme_id)
    {
        if (scheme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage/setStorageSchemesActionSchemeIDForUnSubscribing(npc_id, scheme_name, "
                "scheme_id)] WARNING: scheme_name.empty() == true! Return ...");
            return;
        }

        if (this->m_storage[npc_id].getSchemes().find(scheme_name) == this->m_storage[npc_id].getSchemes().end())
        {
            Msg("[Scripts/DataBase/Storage/setStorageSchemesActionSchemeIDForUnSubscribing(npc_id, scheme_name, "
                "scheme_id)] WARNING: can't find scheme %s return ...",
                scheme_name.c_str());
            return;
        }

        this->m_storage[npc_id].m_schemes[scheme_name]->setActionSchemeID(scheme_id);
    }

    inline void setStorageSchemesActions(
        const std::uint16_t npc_id, const xr_string& scheme_name, Script_ISchemeEntity* p_scheme)
    {
        if (scheme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage/setStorageSchemesActions(npc_id, scheme_name, p_scheme)] WARNING: "
                "scheme_name.empty() == true! Can't assign return ...");
            return;
        }
        p_scheme->subscribe_action();
        this->m_storage[npc_id].setSchemesActions(scheme_name, p_scheme);
    }

    inline void setStorageSchemesSectionName(
        const std::uint16_t npc_id, const xr_string& scheme_name, const xr_string& section_name) noexcept
    {
        if (scheme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage/setStorageSchemesSectionName(npc_id, scheme_name, section_name)] WARNING: "
                "scheme_name.empty() == true! Can't assign return ...");
            return;
        }

        this->m_storage[npc_id].setSchemesSectionName(scheme_name, section_name);
    }

    inline void setStorageSchemesEnabled(
        const std::uint16_t npc_id, const xr_string& scheme_name, const bool value) noexcept
    {
        if (scheme_name.empty())
        {
            Msg("[Scripts/DataBase/Storage/setStorageSchemesEnabled(npc_id, scheme_name, value)] WARNING: "
                "scheme_name.empty() == true! Can't assign return ...");
            return;
        }

        this->m_storage[npc_id].setSchemesEnabled(scheme_name, value);
    }

    inline void setStorageDeathData(const std::uint16_t npc_id, const Storage_Data::DeathData& data) noexcept
    {
        this->m_storage[npc_id].setDeathData(data);
    }

    // @ sets killer's name
    inline void setStorageDeathDataKillerName(const std::uint16_t npc_id, const xr_string& name) noexcept
    {
        this->m_storage[npc_id].setDeathDataKillerName(name);
    }

    // @ sets killer's id
    inline void setStorageDeathDataKillerID(const std::uint16_t npc_id, const std::uint16_t id) noexcept
    {
        this->m_storage[npc_id].setDeathDataKillerID(id);
    }

    inline void setStorageOverrides(const std::uint16_t npc_id, const Data_Overrides& data) noexcept
    {
        this->m_storage[npc_id].setOverrides(data);
    }

    inline void setStorageSType(const std::uint16_t npc_id, const std::uint8_t stype) noexcept
    {
        this->m_storage[npc_id].setSchemeType(stype);
    }

    inline void setStorageHitDeadlyHit(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setHitDeadlyHit(value);
    }

    inline void setStorageHitWhoID(const std::uint16_t npc_id, const std::uint16_t who_id) noexcept
    {
        this->m_storage[npc_id].setHitWhoID(who_id);
    }

    inline void setStorageHitBoneIndex(const std::uint16_t npc_id, const std::int16_t bone_index) noexcept
    {
        this->m_storage[npc_id].setHitBoneIndex(bone_index);
    }

    inline void setStorageCorpseAlreadySelected(const std::uint16_t npc_id, const std::uint16_t corpse_id) noexcept
    {
        this->m_storage[npc_id].setCorpseAlreadySelected(corpse_id);
    }

    inline void setStorageWoundedAlreadySelected(const std::uint16_t npc_id, const std::uint16_t wounded_id) noexcept
    {
        this->m_storage[npc_id].setWoundedAlreadySelected(wounded_id);
    }

    inline void setStorageDangerFlag(const std::uint16_t npc_id, const bool value) noexcept
    {
        this->m_storage[npc_id].setDangerFlag(value);
    }

    inline void setStorageRegisteredCamp(const std::uint16_t npc_id, const std::uint16_t camp_id) noexcept 
    {
        this->m_storage[npc_id].setRegisteredCamp(camp_id);
    }
#pragma endregion

    // @ For helicopters only
    inline void addEnemy(CScriptGameObject* p_object)
    {
        this->m_helicopter_enemies[this->m_helicopter_count] = p_object;
        ++this->m_helicopter_count;
    }

    // @ For helicopters only
    inline void deleteEnemy(const std::uint32_t index)
    {
        this->m_helicopter_enemies[index] = nullptr;
        // Lord: I guess it will be much better if I minus this->m_helicopter_count, cuz the scripts doesn't handle it.
        // It means that the value this->m_helicopter_count only grows up!
    }

    inline void addObject(CScriptGameObject* object)
    {
        if (!object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        this->m_storage[object->ID()].setClientObject(object);
    }

    inline void deleteObject(CScriptGameObject* object)
    {
        if (!object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        this->m_storage[object->ID()].setClientObject(nullptr);
    }

    inline void addAnimationObject(CScriptGameObject* const p_animation_object, Script_Binder_DoorLabx8* const p_binder)
    {
        this->m_animation_object_by_name[p_animation_object->Name()] = p_binder;
        this->addObject(p_animation_object);
    }

    inline void deleteAnimationObject(CScriptGameObject* const p_animation_object)
    {
        this->m_animation_object_by_name[p_animation_object->Name()] = nullptr;
        this->deleteObject(p_animation_object);
    }

    inline void addZone(CScriptGameObject* zone)
    {
        if (!zone)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        this->m_zone_by_name[zone->Name()] = zone;

        Msg("[Scripts/DataBase/Storage/addZone(zone)] adding to database %s", zone->Name());
    }

    inline void deleteZone(CScriptGameObject* zone)
    {
        if (!zone)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        this->m_zone_by_name[zone->Name()] = nullptr;

        Msg("[Scripts/DataBase/Storage/deleteZone(zone)] 'deleting' from database %s", zone->Name());
    }

    inline void addAnomaly(CScriptGameObject* anomaly)
    {
        if (!anomaly)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        this->m_anomaly_by_name[anomaly->Name()] = anomaly;

        Msg("[Scripts/DataBase/Storage/addAnomaly(anomaly)] adding object to database %s", anomaly->Name());
    }

    inline void deleteAnomaly(CScriptGameObject* anomaly)
    {
        if (!anomaly)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        this->m_anomaly_by_name[anomaly->Name()] = nullptr;

        Msg("[Scripts/DataBase/Storage/deleteAnomaly(anomaly)] 'deleting' object from database %s", anomaly->Name());
    }

    inline void setActor(CScriptGameObject* object)
    {
        if (!object)
        {
            R_ASSERT2(false, "Can't set actor because the object was null!");
            return;
        }

        this->m_actor = object;
        this->addObject(object);
        Msg("[Scripts/DataBase/Storage/setActor(object)] registering actor to database!");
    }

    inline void deleteActor(CScriptGameObject* object)
    {
        this->m_actor = nullptr;
        this->deleteObject(object);

        Msg("[Scripts/DataBase/Storage/deleteActor(object)] 'deleting' actor from database!");
    }

    inline void setPStorBool(const std::uint16_t& id, const xr_string& varname, const bool& value) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setPStorNumber(id, varname, value)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to get access through an undefined value! No assignment!");
            return;
        }

        if (!varname.size())
        {
            Msg("[DataBase/Storage/setPStorNumber(id, varname, value)] WARNING: varname.size() = 0! You are trying to "
                "get an access through an empty string! No assignment!");
            return;
        }

        this->m_storage[id].m_pstor[varname].setBool(value);
    }

    inline void setPStorNumber(const std::uint16_t& id, const xr_string& varname, const std::uint8_t& value)
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setPStorNumber(id, varname, value)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to get access through an undefined value! No assignment!");
            return;
        }

        if (!varname.size())
        {
            Msg("[DataBase/Storage/setPStorNumber(id, varname, value)] WARNING: varname.size() = 0! You are trying to "
                "get an access through an empty string! No assignment!");
            return;
        }

        this->m_storage[id].m_pstor[varname].setNumber(value);
    }

    inline void setPStorString(const std::uint16_t& id, const xr_string& varname, const xr_string& value)
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[DataBase/Storage/setPStorString(id, varname, value)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to get an access through an undefined value! No assignment!");
            return;
        }

        if (!varname.size())
        {
            Msg("[DataBase/Storage/setPStorString(id, varname, value)] WARNING: id = std::uint16_t(-1)! You are trying "
                "to get an access through an undefined value! No assignment!");
            return;
        }

        this->m_storage[id].m_pstor[varname].setString(value);
    }

    inline const xr_map<xr_string, Script_SE_SmartTerrain*>& getGameRegisteredServerSmartTerrainsByName(void) const
        noexcept
    {
        return this->m_game_registered_server_smartterrains_by_name;
    }

    inline void setGameRegisteredServerSmartTerrainsByName(const xr_map<xr_string, Script_SE_SmartTerrain*>& map)
    {
        if (!map.size())
        {
            Msg("[Scripts/DataBase/Storage/setGameRegisteredServerSmartTerrainsByName(map)] "
                "WARNING: map.size() = 0! You "
                "are trying to set an empty map!");
            //  return;
        }

        this->m_game_registered_server_smartterrains_by_name = map;
    }

    inline void setGameRegisteredServerSmartTerrainsByName(const std::pair<xr_string, Script_SE_SmartTerrain*>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Scripts/DataBase/Storage/setGameRegisteredServerSmartTerrainsByName(name, "
                "server_smart)] WARNING: "
                "pair.first.size() = 0! You are trying to set an empty string!");
            //  return;
        }

        if (!pair.second)
        {
            Msg("[Scripts/DataBase/Storage/setGameRegisteredServerSmartTerrainsByName(name, "
                "server_smart)] WARNING: "
                "pair.second = null! You are trying to set an empty object!");
            // return;
        }

        this->m_game_registered_server_smartterrains_by_name.insert(pair);
    }

    inline void setGameRegisteredServerSmartTerrainsByName(const xr_string& name, Script_SE_SmartTerrain* server_smart)
    {
        if (!name.size())
        {
            Msg("[Scripts/DataBase/Storage/setGameRegisteredServerSmartTerrainsByName(name, "
                "server_smart)] WARNING: "
                "name.size() = 0! You are trying to set an empty string!");
            //  return;
        }

        if (!server_smart)
        {
            Msg("[Scripts/DataBase/Storage/setGameRegisteredServerSmartTerrainsByName(name, "
                "server_smart)] WARNING: "
                "server_smart = null! You are trying to set an empty object!");
            //    return;
        }

        this->m_game_registered_server_smartterrains_by_name[name] = server_smart;
    }

    // @ In-Game
    inline const xr_map<xr_string, Script_SE_SmartCover*>& getGameRegisteredServerSmartCovers(void) const noexcept
    {
        return this->m_game_registered_smartcovers;
    }

    inline void setGameRegisteredServerSmartCovers(const xr_map<xr_string, Script_SE_SmartCover*>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCovers(map)] WARNING: "
                "map.size() = 0! You are trying "
                "to set an empty map!");
            //  return;
        }

        this->m_game_registered_smartcovers = map;
    }

    inline void setGameRegisteredServerSmartCovers(const std::pair<xr_string, Script_SE_SmartCover*>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCovers(name, "
                "server_smartcover)] WARNING: "
                "pair.first.size() = 0! You are trying to set an empty string!");
            //  return;
        }

        if (!pair.second)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCovers(name, "
                "server_smartcover)] WARNING: "
                "pair.second = null! You are trying to set an empty object!");
            //    return;
        }

        this->m_game_registered_smartcovers.insert(pair);
    }

    inline void setGameRegisteredServerSmartCovers(const xr_string& name, Script_SE_SmartCover* server_smartcover)
    {
        if (!name.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCovers(name, "
                "server_smartcover)] WARNING: "
                "name.size() = 0! You are trying to set an empty string!");
            //   return;
        }

        if (!server_smartcover)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCovers(name, "
                "server_smartcover)] WARNING: "
                "server_smartcover = null! You are trying to set an empty object!");
            // return;
        }

        this->m_game_registered_smartcovers[name] = server_smartcover;
    }

    inline const xr_map<xr_string, CScriptGameObject*>& getGameRegisteredCombatSpaceRestrictors(void) const noexcept
    {
        return this->m_game_registered_combat_spacerestrictors;
    }

    inline void setGameRegisteredCombatSpaceRestrictors(const xr_map<xr_string, CScriptGameObject*>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredCombatSpaceRestrictors(space_name, "
                "client_zone)] WARNING: "
                "map.size() = 0! You are trying to set an empty map!");
            //    return;
        }

        this->m_game_registered_combat_spacerestrictors = map;
    }

    inline void setGameRegisteredCombatSpaceRestrictors(const std::pair<xr_string, CScriptGameObject*>& pair)
    {
        if (!pair.first.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredCombatSpaceRestrictors(space_name, "
                "client_zone)] WARNING: "
                "pair.first.size() = 0! You are trying to set an empty string!");
            //   return;
        }

        if (!pair.second)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredCombatSpaceRestrictors(space_name, "
                "client_zone)] WARNING: "
                "pair.second = null! You are trying to set an empty object!");
            //  return;
        }

        this->m_game_registered_combat_spacerestrictors.insert(pair);
    }

    inline void setGameRegisteredCombatSpaceRestrictors(const xr_string& space_name, CScriptGameObject* client_zone)
    {
        if (!space_name.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredCombatSpaceRestrictors(space_name, "
                "client_zone)] WARNING: "
                "space_name.size() = 0! You are trying to set an empty string!");
            //    return;
        }

        if (!client_zone)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredCombatSpaceRestrictors(space_name, "
                "client_zone)] WARNING: "
                "client_zone = null! You are trying to set an empty object!");
            //    return;
        }

        this->m_game_registered_combat_spacerestrictors[space_name] = client_zone;
    }

    // @ In-Game
    inline const xr_map<std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>>&
    getGameRegisteredServerSmartCoversByLevelID(void) const noexcept
    {
        return this->m_game_registered_smartcovers_by_level_id;
    }

    // Lord: Проверить данные сеттеры на правильность наименования аргументов!
    inline void setGameRegisteredServerSmartCoversByLevelID(
        const xr_map<std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>>& map)
    {
        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(map)] "
                "WARNING: map.size() = 0! You "
                "are trying to set an empty map!");
            //  return;
        }

        this->m_game_registered_smartcovers_by_level_id = map;
    }

    inline void setGameRegisteredServerSmartCoversByLevelID(
        const std::uint8_t& level_id, const xr_map<std::uint32_t, Script_SE_SmartCover*>& map)
    {
        if (level_id == Globals::kUnsignedInt8Undefined)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, "
                "map)] "
                "WARNING: level_id = std::uint8_t(-1)! You are trying to set an undefined "
                "number of unsigned int!");
            //  return;
        }

        if (!map.size())
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, "
                "map)] WARNING: map.size() "
                "= 0! You are trying to set an empty map!");
            //  return;
        }

        this->m_game_registered_smartcovers_by_level_id[level_id] = map;
    }

    inline void setGameRegisteredServerSmartCoversByLevelID(
        const std::uint8_t& level_id, const std::pair<std::uint32_t, Script_SE_SmartCover*>& pair)
    {
        if (level_id == Globals::kUnsignedInt8Undefined)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, "
                "pair)] "
                "WARNING: level_id = std::uint8_t(-1)! You are trying to set an undefined "
                "number of unsigned int!");
            //  return;
        }

        if (pair.first == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, "
                "pair)] "
                "WARNING: pair.first = std::uint32_t(-1)! You are trying to set an undefined "
                "number of unsigned int! ");
            //   return;
        }

        if (!pair.second)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, "
                "pair)] "
                "WARNING: pair.second = null! You are trying to set an empty object!");
            //  return;
        }

        this->m_game_registered_smartcovers_by_level_id[level_id].insert(pair);
    }

    inline void setGameRegisteredServerSmartCoversByLevelID(
        const std::uint8_t& level_id, const std::uint32_t& id, Script_SE_SmartCover* server_smartcover)
    {
        if (level_id == Globals::kUnsignedInt8Undefined)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, "
                "id, server_smartcover)] "
                "WARNING: level_id = std::uint8_t(-1)! You are trying to set an undefined "
                "number of unsigned int!");
            //  return;
        }

        if (id == Globals::kUnsignedInt32Undefined)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, "
                "id, server_smartcover)] "
                "WARNING: id = std::uint32_t(-1)! You are trying to set an undefined number of "
                "unsigned int!");
            //    return;
        }

        if (!server_smartcover)
        {
            Msg("[Script_GlobalHelper/setGameRegisteredServerSmartCoversByLevelID(level_id, "
                "id, server_smartcover)] "
                "WARNING: server_smartcover = null! You are trying to set an empty object!");
            //   return;
        }

        this->m_game_registered_smartcovers_by_level_id[level_id][id] = server_smartcover;
    }

    inline void deleteStorage(const std::uint16_t object_id)
    {
        Msg("[Scripts/DataBase/Storage/deleteStorage(object_id)] deleting storage -> %d", object_id);
        this->m_storage.erase(object_id);
    }

    inline void Deallocate(void)
    {
        this->m_actor = nullptr;
        this->m_storage.clear();
        if (!this->m_zone_by_name.empty())
        {
            for (std::pair<const xr_string, CScriptGameObject*>& it : this->m_zone_by_name)
                it.second = nullptr;

            this->m_zone_by_name.clear();
        }

        if (!this->m_anomaly_by_name.empty())
        {
            for (std::pair<const xr_string, CScriptGameObject*>& it : this->m_anomaly_by_name)
                it.second = nullptr;

            this->m_anomaly_by_name.clear();
        }

        if (!this->m_game_registered_combat_spacerestrictors.empty())
        {
            for (std::pair<const xr_string, CScriptGameObject*>& it : this->m_game_registered_combat_spacerestrictors)
                it.second = nullptr;

            this->m_game_registered_combat_spacerestrictors.clear();
        }

        if (!this->m_game_registered_server_smartterrains_by_name.empty())
        {
            for (std::pair<const xr_string, Script_SE_SmartTerrain*>& it :
                this->m_game_registered_server_smartterrains_by_name)
                it.second = nullptr;

            this->m_game_registered_server_smartterrains_by_name.clear();
        }

        if (!this->m_game_registered_smartcovers.empty())
        {
            for (std::pair<const xr_string, Script_SE_SmartCover*>& it : this->m_game_registered_smartcovers)
                it.second = nullptr;

            this->m_game_registered_smartcovers.clear();
        }

        if (!this->m_game_registered_smartcovers_by_level_id.empty())
        {
            for (std::pair<const std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>>& it :
                this->m_game_registered_smartcovers_by_level_id)
            {
                for (std::pair<const std::uint32_t, Script_SE_SmartCover*>& it_sub : it.second)
                    it_sub.second = nullptr;
            }

            this->m_game_registered_smartcovers_by_level_id.clear();
        }

        this->m_offline_objects.clear();
        this->m_spawned_vertex_by_id.clear();
        this->m_goodwill.first.clear();
        this->m_goodwill.second.clear();
        this->m_camp_storage.clear();
        this->m_noweapon_zones.clear();

        Script_TaskManager::getInstance().Deallocate();
    }

    inline const xr_map<xr_string, xr_map<std::uint32_t, bool>>& getCampStorage(void) const noexcept
    {
        return this->m_camp_storage;
    }

    inline void setCampStorage(const xr_string& point_name, const std::uint32_t index, const bool value) noexcept
    {
        this->m_camp_storage[point_name][index] = value;
    }

    inline std::uint32_t getHelicopterCount(void) const noexcept { return this->m_helicopter_count; }

    // @ ++value;
    inline void AddHelicopterCount(void) noexcept { ++this->m_helicopter_count; }

    // @ --value;
    inline void DeleteHelicopterCount(void) noexcept { --this->m_helicopter_count; }

    inline const xr_map<std::uint32_t, CScriptGameObject*>& getHelicopterEnemies(void) const noexcept
    {
        return this->m_helicopter_enemies;
    }

    inline const xr_map<xr_string, bool>& getNoWeaponZones(void) const noexcept { return this->m_noweapon_zones; }
    inline void setNoWeaponZones(const xr_string& zone_name, const bool value) noexcept
    {
        if (zone_name.empty())
        {
            Msg("[Scripts/Database/Storage/setNoWeaponZones(zone_name, value)] WARNING: zone_name.empty() == true! You "
                "are trying to set an empty string! Retunr ...");
            return;
        }

        this->m_noweapon_zones[zone_name] = value;
    }
#pragma endregion

#pragma region Cordis Script_Binder_Anomaly stuff
    inline const xr_map<std::uint16_t, xr_string>& getArtefactWaysByID(void) const noexcept
    {
        return this->m_artefact_ways_by_id;
    }
    inline void setArtefactWaysByID(const std::uint16_t id, const xr_string& path_name) noexcept
    {
        if (path_name.empty())
        {
            Msg("[Scripts/DataBase/Storage/setArtefactWaysByID(id, path_name)] WARNING: path_name.empty() == true! ");
        }

        this->m_artefact_ways_by_id[id] = path_name;
    }

    inline const xr_map<std::uint16_t, std::uint32_t>& getArtefactPointsByID(void) const noexcept
    {
        return this->m_artefact_points_by_id;
    }

    inline void setArtefactPointsByID(const std::uint16_t id, const std::uint32_t point) noexcept
    {
        this->m_artefact_points_by_id[id] = point;
    }

    inline const xr_map<std::uint16_t, Script_Binder_Anomaly*>& getParentZonesArtefactByID(void)
    {
        return this->m_parent_zones_by_artefact_id;
    }

    inline void setParentZonesArtefactByID(const std::uint16_t id, Script_Binder_Anomaly* const p_binder_object)
    {
        if (!p_binder_object)
        {
            Msg("[Scripts/DataBase/Storage/setParentZonesArtefactByID(id, p_binder_object)] WARNING: p_binder_object "
                "== nullptr!");
        }

        this->m_parent_zones_by_artefact_id[id] = p_binder_object;
    }
#pragma endregion

#pragma region Cordis Script_Binder_Field
    inline const xr_map<xr_string, Script_Binder_AnomalField*>& getFieldsByName(void) const noexcept
    {
        return this->m_fields_by_name;
    }
    inline void setFieldsByName(const xr_string& field_name, Script_Binder_AnomalField* const p_field)
    {
        if (!p_field)
        {
            Msg("[Scripts/DataBase/Storage/setFieldsByName(field_name, p_field)] WARNING: p_field == nullptr!");
        }

        this->m_fields_by_name[field_name] = p_field;
    }
#pragma endregion

#pragma region Cordis Script_Binder_DoorLabx8
    inline const xr_map<xr_string, Script_Binder_DoorLabx8*>& getAnimationObjectsByName(void) const noexcept
    {
        return this->m_animation_object_by_name;
    }
#pragma endregion

#pragma region Cordis Script_Binder_Crow

    inline const xr_map<std::uint16_t, std::uint16_t>& getCrowStorage(void) const noexcept
    {
        return this->m_crow_storage;
    }

    inline void setCrowStorage(const std::uint16_t object_id, const std::uint16_t objectid)
    { 
        this->m_crow_storage[object_id] = objectid;
    }

    inline void addCrowToCount(void) noexcept { ++this->m_crow_count; }
    inline void deleteCrowToCount(void) noexcept {--this->m_crow_count;}
#pragma endregion

    inline const xr_map<std::uint16_t, xr_string>& getScriptIDS(void) const noexcept { return this->m_script_ids; }
    inline void setScriptIDS(const std::uint16_t id, const xr_string& name) noexcept
    {
        if (name.empty())
        {
            Msg("[Scripts/DataBase/Storage/setScriptIDS(id, name)] WARNING: name.empty() == true! You set an empty "
                "string");
        }

        this->m_script_ids[id] = name;
    }

    Storage(const Storage&) = delete;
    Storage& operator=(const Storage&) = delete;
    Storage(Storage&&) = delete;
    Storage& operator=(Storage&&) = delete;

private:
    std::uint32_t m_crow_count = 0;
    std::uint32_t m_helicopter_count = 0;
    CScriptGameObject* m_actor = nullptr;

#pragma region Cordis Inventory Upgrades
    CScriptGameObject* m_p_victim = nullptr;
#pragma endregion

    xr_map<std::uint16_t, Storage_Data> m_storage;
    xr_map<std::uint16_t, xr_string> m_script_ids;
    xr_map<xr_string, xr_map<std::uint32_t, bool>> m_camp_storage; // @ Uses in mob_camp only
    xr_map<std::uint16_t, std::pair<std::uint16_t, xr_string>> m_offline_objects;
    xr_map<std::uint32_t, CScriptGameObject*> m_helicopter_enemies;
    xr_map<xr_string, bool> m_noweapon_zones;
    xr_map<std::uint16_t, std::uint16_t> m_crow_storage;
    xr_map<xr_string, CScriptGameObject*> m_zone_by_name;
    xr_map<xr_string, CScriptGameObject*> m_anomaly_by_name;
    xr_map<xr_string, CScriptGameObject*> m_game_registered_combat_spacerestrictors;
    xr_map<xr_string, Script_SE_SmartTerrain*> m_game_registered_server_smartterrains_by_name;
    xr_map<std::uint8_t, xr_map<std::uint32_t, Script_SE_SmartCover*>> m_game_registered_smartcovers_by_level_id;
    xr_map<xr_string, Script_SE_SmartCover*> m_game_registered_smartcovers;
    xr_map<std::uint16_t, std::uint32_t> m_spawned_vertex_by_id;
#pragma region Cordis Script_Binder_AnomalZone
    xr_map<std::uint16_t, xr_string> m_artefact_ways_by_id;
    xr_map<std::uint16_t, std::uint32_t> m_artefact_points_by_id;
    xr_map<std::uint16_t, Script_Binder_Anomaly*> m_parent_zones_by_artefact_id;
#pragma endregion

#pragma region Cordis Script_Binder_Field
    xr_map<xr_string, Script_Binder_AnomalField*> m_fields_by_name;
#pragma endregion

#pragma region Cordis Script_Binder_DoorLabx8
    xr_map<xr_string, Script_Binder_DoorLabx8*> m_animation_object_by_name;
#pragma endregion

    // first -> sympathy[ID] = std::uint32_t; | second -> relations[ID] = std::string;
    std::pair<xr_map<std::uint16_t, float>, xr_map<std::uint16_t, xr_string>> m_goodwill;
};

} // namespace DataBase
} // namespace Scripts
} // namespace Cordis
