#pragma once

namespace Cordis
{

namespace Scripts
{
class Script_Binder_Helicopter : public CScriptBinderObject
{
    using inherited = CScriptBinderObject;

public:
    Script_Binder_Helicopter(void) = delete;
    Script_Binder_Helicopter(CScriptGameObject* const p_client_object);
    ~Script_Binder_Helicopter(void);

    virtual void reinit();
    virtual void reload(LPCSTR section);
    virtual bool net_Spawn(SpawnType DC);
    virtual void net_Destroy();
    virtual void shedule_Update(u32 time_delta);
    virtual void save(NET_Packet* output_packet);
    virtual void load(IReader* input_packet);
    virtual bool net_SaveRelevant();
    
    //void on_hit(float power, float impulse, )

private:
    bool m_is_initialized;
    bool m_is_loaded;
    CScriptIniFile* m_p_ini;
    Script_HelicopterFire* m_p_heli_logic;
};
}
} // namespace Cordis

