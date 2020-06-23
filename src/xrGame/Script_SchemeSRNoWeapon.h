#pragma once

namespace Cordis
{
namespace Scripts
{
class Script_SchemeSRNoWeapon : public Script_ISchemeEntity
{
    using inherited_scheme = Script_ISchemeEntity;

public:
    Script_SchemeSRNoWeapon(void) = delete;
    Script_SchemeSRNoWeapon(CScriptGameObject* const p_client_object, DataBase::Script_ComponentScheme_SRNoWeapon* storage);
    ~Script_SchemeSRNoWeapon(void);

    virtual void reset_scheme(const bool value, CScriptGameObject* const p_client_object);
    virtual void update(const float delta);

    // @ PRIVATE uses, in XR_LOGIC
    static inline void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, DataBase::Script_IComponentScheme* storage)
    {
        if (!p_client_object)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        if (!p_ini)
        {
            R_ASSERT2(false, "object is null!");
            return;
        }

        MESSAGEI("added scheme to binder, name=%s scheme=%s section=%s",
            p_client_object->Name(), scheme_name.c_str(), section_name.c_str());

        Script_ISchemeEntity* p_scheme = new Script_SchemeSRNoWeapon(p_client_object, reinterpret_cast<DataBase::Script_ComponentScheme_SRNoWeapon*>(storage));
        DataBase::Storage::getInstance().setStorageSchemesActions(p_client_object->ID(), scheme_name, p_scheme);
    }

    // @ PRIVATE, uses in XR_LOGIC
    static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
        const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

private:
    void zone_enter(void);
    void zone_leave(void);
    void switch_state(CScriptGameObject* const p_client_actor);

private:
    std::uint32_t m_state;
    DataBase::Script_ComponentScheme_SRNoWeapon* m_p_storage;
    xrTime m_inited_time;
};
} // namespace Scripts
} // namespace Cordis
