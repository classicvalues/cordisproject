#pragma once

#include "xrServer_Objects_ALife_Monsters.h"

#include "Script_SoundManager.h"

namespace Cordis
{
namespace Scripts
{
namespace Globals
{
class Script_RandomFloat;
}
} // namespace Scripts
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
struct StayReachOnTarget
{
private:
    float m_idle_time = Globals::Script_RandomFloat::getInstance().Generate(
        Globals::kSimulationSquadActionsStayPointIdleMin, Globals::kSimulationSquadActionsStayPointIdleMax);
    std::uint16_t m_squad_id = Globals::kUnsignedInt16Undefined;
    xrTime m_start_time = 0;
    xr_string m_name = "";

public:
    inline const xr_string& getName(void) const noexcept { return this->m_name; }
    inline const xrTime& getStartTime(void) const noexcept { return this->m_start_time; }
    inline const std::uint16_t& getSquadID(void) const noexcept { return this->m_squad_id; }
    inline const float& getIdleTime(void) const noexcept { return this->m_idle_time; }

    inline void setName(const xr_string& name) noexcept
    {
        if (!name.size())
        {
            Msg("[Scripts/StayReachOnTarget/setName(name)] WARNING: name.size() = 0! You are trying to set an empty "
                "string!");
        }

        this->m_name = name;
    }

    inline void setStartTime(const xrTime& time) noexcept { this->m_start_time = time; }

    inline void setSquadID(const std::uint16_t id) noexcept
    {
        if (id == Globals::kUnsignedInt16Undefined)
        {
            Msg("[Scripts/StayReachOnTarget/setSquadID(id)] WARNING: id is undefined! You are trying to set it!");
        }

        this->m_squad_id = id;
    }

    inline void setIdleTime(const float value) noexcept { this->m_idle_time = value; }

    inline bool update(const bool& value)
    {
        if (!this->m_name.size())
        {
            R_ASSERT2(false, "You must indentifying your class's ID!");
            return false;
        }

        if (this->m_squad_id && this->m_name == Globals::kSimulationSquadCurrentActionIDReachTarget)
        {
        }
        else
        {
            R_ASSERT2(false, "Something bad!");
        }

        if (this->m_squad_id == Globals::kUnsignedInt16Undefined)
        {
            if (!value)
                return true;

            return get_time_struct().diffSec(this->m_start_time) > this->m_idle_time;
        }
        else
        {
            R_ASSERT2(false, "Something bad!");
        }

        return false;
    }

    inline void make(const bool& value) { this->m_start_time = get_time_struct(); }

    inline void Clear(void) noexcept
    {
        this->m_name.clear();
        this->m_idle_time = 0.0f;
        this->m_start_time = 0;
    }
};
} // namespace Scripts
} // namespace Cordis

namespace Cordis
{
namespace Scripts
{
class Script_SE_SimulationSquad : public CSE_ALifeOnlineOfflineGroup
{
    using inherited = CSE_ALifeOnlineOfflineGroup;

public:
    Script_SE_SimulationSquad(LPCSTR section);
    virtual ~Script_SE_SimulationSquad(void);

    virtual Script_SE_SimulationSquad* cast_script_se_simulationsquad(void) { return this; }
    virtual CALifeSmartTerrainTask* get_current_task(void);
#pragma region Cordis Getters
    inline std::uint32_t getSmartTerrainID(void) noexcept { return this->m_smart_terrain_id; }
    inline std::uint32_t getCurrentSpotID(void) noexcept { return this->m_current_spot_id; }
    inline std::uint32_t getCurrentTargetID(void) noexcept { return this->m_current_target_id; }
    inline std::uint32_t getAssignedTargetID(void) noexcept { return this->m_assigned_target_id; }
    inline std::uint32_t getEnteredSmartID(void) noexcept { return this->m_entered_smart_id; }
    inline xr_string& getSettingsID(void) noexcept { return this->m_settings_id_name; }
    inline std::uint16_t getScriptTarget(void);
    inline const StayReachOnTarget& getCurrentAction(void) noexcept { return this->m_current_action; }
    inline std::uint32_t getPlayerID(void) noexcept
    {
        return this->m_player_id;
    } // Lord: сделать под simulationActivities in Script_SimulationBoard как айдишник на который ссылаться

    inline CALifeSmartTerrainTask* getAlifeSmartTerrainTask(void)
    {
        if (!this->m_alife_smart_terrain_task.get())
        {
            this->m_alife_smart_terrain_task =
                std::make_unique<CALifeSmartTerrainTask>(this->m_tGraphID, this->m_tNodeID);
        }

        return this->m_alife_smart_terrain_task.get();
    }

#pragma endregion

#pragma region Cordis Setters
    inline void setSmartTerrainID(const std::uint32_t& value) noexcept { this->m_smart_terrain_id = value; }
    inline void setCurrentSpotID(const std::uint32_t& value) noexcept { this->m_current_spot_id = value; }
    inline void setCurrentTargetID(const std::uint32_t& value) noexcept { this->m_current_target_id = value; }
    inline void setAssignedTargetID(const std::uint32_t& value) noexcept { this->m_assigned_target_id = value; }
    inline void setEnteredSmartID(const std::uint32_t& value) noexcept { this->m_entered_smart_id = value; }
#pragma endregion

    void set_location_types(const xr_string& new_smart_name);
    void create_npc(Script_SE_SmartTerrain* spawn_smart);
    std::uint16_t add_squad_member(const xr_string& spawn_section_name, const Fvector& spawn_position,
        const std::uint32_t& level_vertex_id, const std::uint16_t& game_vertex_id);
    void on_npc_death(CSE_ALifeDynamicObject* server_object);
    void remove_squad(void);
    void set_squad_relation(const xr_string& relation_name = "");
    void assign_smart(Script_SE_SmartTerrain* smart = nullptr);

private:
    inline xr_string& pick_next_target(void) { return this->m_parsed_targets[this->m_next_target_index]; }
    void assign_squad_member_to_smart(
        const std::uint16_t& object_id, Script_SE_SmartTerrain* smart, const std::uint16_t& old_smart_terrain_id);
    void set_location_types_section(const xr_string& section);
    void set_squad_sympathy(const float& sympathy = 0.0f);
    void set_squad_behaviour(void);
    void init_squad_on_load(void);
    void refresh(void);
    void show(void);
    void hide(void);
    bool check_squad_come_to_point(void);

private:
    bool m_is_always_walk; // @ Init by utils
    bool m_is_always_arrived; // @ Init by utils
    bool m_is_need_to_reset_location_masks;
    bool m_is_need_free_update;
    bool m_is_show_disabled;
    std::uint32_t m_smart_terrain_id;
    std::uint32_t m_current_spot_id;
    std::uint32_t m_current_target_id;
    std::uint32_t m_assigned_target_id;
    std::uint32_t m_next_target_index;
    std::uint32_t m_entered_smart_id;
    std::uint32_t m_player_id;
    float m_sympathy; // @ Init by utils
    xr_map<std::uint32_t, CondlistData> m_condlist_action; // @ Init by Utils
    xr_map<std::uint32_t, CondlistData> m_condlist_death; // @ Init by Utils
    xr_map<std::uint32_t, CondlistData> m_condlist_invulnerability; // @ Init by Utils
    xr_map<std::uint32_t, CondlistData> m_condlist_show_spot; // @ Init by Utils
    xr_map<xr_string, xr_string> m_behaviour;
    xr_vector<xr_string> m_parsed_targets;
    xr_string m_relationship_name; // @ Init by utils
    xr_string m_player_id_name; // @ Init by utils
    xr_string m_settings_id_name;
    xr_string m_last_target_name;
    xr_string m_spot_section_name;
    std::unique_ptr<CALifeSmartTerrainTask> m_alife_smart_terrain_task;
    StayReachOnTarget m_current_action;
    Script_SoundManager m_sound_manager;
};

} // namespace Scripts
} // namespace Cordis
