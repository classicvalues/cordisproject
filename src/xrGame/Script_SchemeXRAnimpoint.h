#pragma once

namespace Cordis
{
	namespace Scripts
	{

		class Script_Animpoint : public Script_ISchemeEntity
		{
		public:
			Script_Animpoint(void) = delete;
			Script_Animpoint(const std::uint16_t npc_id, DataBase::Script_ComponentScheme_XRAnimPoint* storage);
			~Script_Animpoint(void);

			void initialize(void);
			virtual void activate_scheme(const bool is_loading, CScriptGameObject* const p_client_object);
			void calculate_position(void);
			void get_animation_params(Fvector& position, Fvector& smart_direction);
			bool position_riched(void);
			void fill_approved_actions(void);
			void start(void);
			void stop(void);
			virtual void update(const float delta);
			const xr_string& getActionName(void) noexcept { return this->m_action_name; }
            inline const Fvector& getSmartDirection(void) const noexcept { return this->m_smart_direction; }
            inline const Fvector& getVertexPosition(void) const noexcept { return this->m_vertex_position; }
            inline const Fvector& getLookPosition(void) const noexcept { return this->m_look_position; }
            inline bool isStarted(void) const noexcept { return this->m_is_started; }
            inline std::uint32_t getPositionVertex(void) const noexcept { return this->m_position_vertex; }
			
		private:
			bool m_is_started;
			std::uint16_t m_npc_id;
			std::uint32_t m_position_vertex;
			DataBase::Script_ComponentScheme_XRAnimPoint* m_p_storage;
			Script_CampData* m_p_camp;
			Fvector m_position;
			Fvector m_smart_direction;
			Fvector m_vertex_position;
			Fvector m_look_position;
			xr_vector<std::pair<std::function<bool(std::uint16_t, bool)>, xr_string>> m_avail_actions;
			xr_string m_cover_name;
			xr_string m_action_name;
		};

		class Script_SchemeXRAnimpoint : public Script_ISchemeStalker
		{
		public:
			Script_SchemeXRAnimpoint(void) = delete;
			Script_SchemeXRAnimpoint(const xr_string& name, DataBase::Script_ComponentScheme_XRAnimPoint* storage);
			~Script_SchemeXRAnimpoint(void);

			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);
			virtual void net_destroy(CScriptGameObject* const p_client_object);

			static void add_to_binder(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& scheme_name, const xr_string& section_name, DataBase::Script_IComponentScheme* storage);

			static void set_scheme(CScriptGameObject* const p_client_object, CScriptIniFile* const p_ini,
				const xr_string& scheme_name, const xr_string& section_name, const xr_string& gulag_name);

		private:
			DataBase::Script_ComponentScheme_XRAnimPoint* m_p_storage;
		};

		class Script_ActionReachAnimpoint : public Script_ISchemeStalker
		{
		public:
			Script_ActionReachAnimpoint(void) = delete;
			Script_ActionReachAnimpoint(const xr_string& name, DataBase::Script_ComponentScheme_XRAnimPoint* storage);
			~Script_ActionReachAnimpoint(void);

			virtual void initialize(void);
			virtual void execute(void);
			virtual void finalize(void);

		private:
			DataBase::Script_ComponentScheme_XRAnimPoint* m_p_storage;
		};

		class Script_EvaluatorReachAnimpoint : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorReachAnimpoint(const xr_string& name, DataBase::Script_ComponentScheme_XRAnimPoint* storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(storage) {}
			~Script_EvaluatorReachAnimpoint(void) {}

			virtual _value_type evaluate(void);

		private:
			DataBase::Script_ComponentScheme_XRAnimPoint* m_p_storage;
		};

		class Script_EvaluatorNeedAnimpoint : public CScriptPropertyEvaluator
		{
		public:
			Script_EvaluatorNeedAnimpoint(const xr_string& name, void* storage) : CScriptPropertyEvaluator(nullptr, name.c_str()), m_p_storage(reinterpret_cast<DataBase::Script_ComponentScheme_XRAnimPoint*>(storage)) {}

			~Script_EvaluatorNeedAnimpoint(void) {}

			virtual _value_type evaluate(void);

		private:
			DataBase::Script_ComponentScheme_XRAnimPoint* m_p_storage;
		};
	}
}


