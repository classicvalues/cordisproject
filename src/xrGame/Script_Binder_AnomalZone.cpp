#include "stdafx.h"
#include "Script_Binder_AnomalZone.h"

namespace Cordis
{
namespace Scripts
{
Script_Binder_Anomaly::Script_Binder_Anomaly(CScriptGameObject* object)
    : inherited(object), m_is_created_own_ini_file_from_cfg_section(false), m_is_disabled(false),
      m_is_turned_off(false), m_is_respawn_artefacts(true), m_is_forced_spawn(false), m_is_forced_spawn_override(false),
      m_is_custom_placement(false)
{
    this->m_p_ini = object->spawn_ini();

    if (!this->m_p_ini)
    {
        R_ASSERT2(false, "object is null!! something is wrong");
    }

    if (!this->m_p_ini->section_exist("anomal_zone"))
    {
        Msg("[Scripts/Script_Binder_Anomaly/ctor(object)] WARNING: no configuration!");
        this->m_is_disabled = true;
        return;
    }

    xr_string filename = Globals::Utils::cfg_get_string(this->m_p_ini, "anomal_zone", "cfg");
    if (!filename.empty())
    {
        this->m_is_created_own_ini_file_from_cfg_section = true;
        this->m_p_ini = new CScriptIniFile(filename.c_str());
    }

    this->m_layers_count =
        static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(this->m_p_ini, "anomal_zone", "layers_count"));

    if (!this->m_layers_count)
        this->m_layers_count = 1;

    this->m_current_layer_name = "layer_";
    this->m_current_layer_name +=
        std::to_string(Globals::Script_RandomInt::getInstance().Generate<std::uint32_t>(1, this->m_layers_count))
            .c_str();

    this->m_is_custom_placement = this->m_layers_count > 1;

    std::uint32_t default_respawn_tries =
        static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(this->m_p_ini, "anomal_zone", "respawn_tries"));
    if (!default_respawn_tries)
        default_respawn_tries = 2;

    std::uint32_t default_max_artefacts =
        static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(this->m_p_ini, "anomal_zone", "max_artefacts"));
    if (!default_max_artefacts)
        default_max_artefacts = 3;

    std::uint32_t default_app_force_xz =
        static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(this->m_p_ini, "anomal_zone", "applying_force_xz"));
    if (!default_app_force_xz)
        default_app_force_xz = 200;

    std::uint32_t default_app_force_y =
        static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(this->m_p_ini, "anomal_zone", "applying_force_y"));
    if (!default_app_force_y)
        default_app_force_y = 400;

    xr_string default_arts_name = Globals::Utils::cfg_get_string(this->m_p_ini, "anomal_zone", "artefacts");
    xr_string default_start_arts = Globals::Utils::cfg_get_string(this->m_p_ini, "anomal_zone", "start_artefact");
    xr_string default_ways = Globals::Utils::cfg_get_string(this->m_p_ini, "anomal_zone", "artefact_ways");
    xr_string default_field_name = Globals::Utils::cfg_get_string(this->m_p_ini, "anomal_zone", "field_name");
    xr_string default_coeff_sect_name = Globals::Utils::cfg_get_string(this->m_p_ini, "anomal_zone", "coeffs_section");
    if (default_coeff_sect_name.empty())
        default_coeff_sect_name = "{+actor_was_in_many_bad_places} coeff2, coeff";

    xr_string default_coeffs = Globals::Utils::cfg_get_string(this->m_p_ini, "anomal_zone", "coeff");

    for (std::uint32_t i = 1; i <= this->m_layers_count; ++i)
    {
        xr_string section_name = "layer_";
        section_name += std::to_string(i).c_str();

        std::uint32_t _respawn_try =
            static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(this->m_p_ini, section_name, "artefact_count"));
        if (!_respawn_try)
            _respawn_try = default_respawn_tries;

        this->m_table_respawn_tries[section_name] = _respawn_try;

        std::uint32_t _respawn_try2 =
            static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(this->m_p_ini, section_name, "respawn_tries"));
        if (!_respawn_try2)
            _respawn_try2 = this->m_table_respawn_tries[section_name];

        this->m_table_respawn_tries[section_name] = _respawn_try2;

        std::uint32_t _max_artefacts =
            static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(this->m_p_ini, section_name, "max_artefacts"));

        if (!_max_artefacts)
            _max_artefacts = default_max_artefacts;

        this->m_table_max_artefacts[section_name] = _max_artefacts;

        std::uint32_t _app_xz = static_cast<std::uint32_t>(
            Globals::Utils::cfg_get_number(this->m_p_ini, section_name, "applying_force_xz"));
        if (!_app_xz)
            _app_xz = default_app_force_xz;

        this->m_table_forces[section_name].first = _app_xz;

        std::uint32_t _app_y =
            static_cast<std::uint32_t>(Globals::Utils::cfg_get_number(this->m_p_ini, section_name, "applying_force_y"));
        if (!_app_y)
            _app_y = default_app_force_y;

        this->m_table_forces[section_name].second = _app_y;

        xr_string artefacts_name = Globals::Utils::cfg_get_string(this->m_p_ini, section_name, "artefacts");
        if (artefacts_name.empty())
            artefacts_name = default_arts_name;

        if (artefacts_name.empty())
        {
            R_ASSERT2(false, "There is no field in section!");
            return;
        }

        this->m_table_artefacts[section_name] = Globals::Utils::parse_names(artefacts_name);

        xr_string start_arts = Globals::Utils::cfg_get_string(this->m_p_ini, section_name, "start_artefact");
        if (start_arts.empty())
            start_arts = default_start_arts;

        if (!start_arts.empty())
        {
            this->m_is_forced_spawn = true;
            this->m_table_start_artefacts[section_name] = Globals::Utils::parse_names(start_arts);
        }

        xr_string coeffs_section_name = Globals::Utils::cfg_get_string(this->m_p_ini, section_name, "coeffs_section");
        if (coeffs_section_name.empty())
            coeffs_section_name = default_coeff_sect_name;

        xr_map<std::uint32_t, CondlistData> parsed_condlist =
            XR_LOGIC::parse_condlist_by_script_object("script_binder_anomaly", "coeff_condlist", coeffs_section_name);

        xr_string picked_section_name = XR_LOGIC::pick_section_from_condlist(
            DataBase::Storage::getInstance().getActor(), static_cast<CScriptGameObject*>(nullptr), parsed_condlist);

        xr_string _coeffs_name = Globals::Utils::cfg_get_string(this->m_p_ini, section_name, picked_section_name);
        if (_coeffs_name.empty())
            _coeffs_name = default_coeffs;

        if (!_coeffs_name.empty())
        {
            xr_vector<int> parsed_data;
            xr_vector<xr_string> temp = Globals::Utils::parse_names(_coeffs_name);
            for (const xr_string& it : temp)
                parsed_data.push_back(boost::lexical_cast<int>(it));

            this->m_table_artefacts_coeff[section_name] = parsed_data;
        }
        else
        {
            this->m_table_artefacts_coeff.clear();
        }

        xr_string path_name = Globals::Utils::cfg_get_string(this->m_p_ini, section_name, "artefact_ways");
        if (path_name.empty())
            path_name = default_ways;

        if (path_name.empty())
        {
            R_ASSERT2(false, "something is not right check your config!");
            return;
        }

        this->m_table_path[section_name] = Globals::Utils::parse_names(path_name);

        if (this->m_is_custom_placement)
        {
            xr_string fieldname = Globals::Utils::cfg_get_string(this->m_p_ini, section_name, "field_name");
            if (fieldname.empty())
                fieldname = default_field_name;

            if (fieldname.empty())
            {
                this->m_table_fields[section_name].clear();
            }
            else
            {
                this->m_table_fields[section_name] = Globals::Utils::parse_names(fieldname);
            }

            xr_string mines_section_name = Globals::Utils::cfg_get_string(this->m_p_ini, section_name, "field_name");
            if (mines_section_name.empty())
            {
                R_ASSERT2(false, "There is no field in section");
                return;
            }

            if (this->m_p_ini->line_count(mines_section_name.c_str()))
            {
                const char* mine_name;
                const char* value_name;
                for (std::uint32_t i = 0; i < this->m_p_ini->line_count(mines_section_name.c_str()); ++i)
                {
                    if (this->m_p_ini->r_line(mines_section_name.c_str(), i, &mine_name, &value_name))
                    {
                        this->m_table_mines[section_name] = mine_name;
                    }
                }
            }
        }
    }

    this->m_respawn_tries = this->m_table_respawn_tries[this->m_current_layer_name];
    this->m_max_artefacts = this->m_table_max_artefacts[this->m_current_layer_name];
    this->m_applying_force_xz = this->m_table_forces[this->m_current_layer_name].first;
    this->m_applying_force_y = this->m_table_forces[this->m_current_layer_name].second;
}

Script_Binder_Anomaly::~Script_Binder_Anomaly(void)
{
    if (this->m_is_created_own_ini_file_from_cfg_section)
    {
        Msg("[Scripts/Script_Binder_Anomaly/~dtor()] deleting m_p_ini %s", this->m_p_ini->fname());
        xr_delete(this->m_p_ini);
    }
}

void Script_Binder_Anomaly::reinit(void) {}

void Script_Binder_Anomaly::reload(LPCSTR section_name) {}

bool Script_Binder_Anomaly::net_Spawn(SpawnType DC) { return false; }

void Script_Binder_Anomaly::net_Destroy(void) {}

void Script_Binder_Anomaly::net_Import(NET_Packet* packet) {}

void Script_Binder_Anomaly::net_Export(NET_Packet* packet) {}

void Script_Binder_Anomaly::shedule_Update(std::uint32_t time_delta) {}

void Script_Binder_Anomaly::save(NET_Packet* output_packet) {}

void Script_Binder_Anomaly::load(IReader* input_packet) {}

bool Script_Binder_Anomaly::net_SaveRelevant(void) { return false; }

void Script_Binder_Anomaly::net_Relcase(CScriptGameObject* object) {}
} // namespace Scripts
} // namespace Cordis
