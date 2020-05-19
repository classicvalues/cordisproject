#include "restricted_object.cpp"
#include "restricted_object_obstacle.cpp"
#include "rewarding_events_handlers.cpp"
#include "rewarding_state_events.cpp"
#include "reward_event_generator.cpp"
#include "reward_manager.cpp"
#include "RocketLauncher.cpp"
#include "RustyHairArtifact.cpp"
#include "saved_game_wrapper.cpp"
#include "saved_game_wrapper_script.cpp"
#include "ScientificOutfit.cpp"
#include "Scope.cpp"
#include "screenshots_common.cpp"
#include "screenshots_writer.cpp"
#include "screenshot_manager.cpp"
#include "screenshot_server.cpp"
#include "ScriptXMLInit.cpp"
#include "script_abstract_action.cpp"
#include "script_action_condition.cpp"
#include "script_action_condition_script.cpp"
#include "script_action_planner_action_wrapper.cpp"
#include "script_action_planner_wrapper.cpp"
#include "script_action_wrapper.cpp"
#include "script_animation_action.cpp"
#include "script_animation_action_script.cpp"
#include "script_binder.cpp"

#pragma region Cordis Scheme Binders
#include "Script_Binder_Actor.cpp"
#include "Script_Binder_AnomalField.cpp"
#include "Script_Binder_AnomalZone.cpp"
#include "Script_Binder_Artefact.cpp"
#include "Script_Binder_Camp.cpp"
#include "Script_Binder_Campfire.cpp"
#include "Script_Binder_Crow.cpp"
#include "Script_Binder_DoorLabx8.cpp"
#include "Script_Binder_Faction.cpp"
#include "Script_Binder_Helicopter.cpp"
#include "Script_Binder_LevelChanger.cpp"
#include "Script_Binder_Monster.cpp"
#include "script_binder_object.cpp"
#include "script_binder_object_script.cpp"
#include "script_binder_object_wrapper.cpp"
#include "Script_Binder_Phantom.cpp"
#include "Script_Binder_PhysicObject.cpp"
#include "Script_Binder_Restrictor.cpp"
#include "Script_Binder_SignalLight.cpp"
#include "Script_Binder_SmartCover.cpp"
#include "Script_Binder_SmartTerrain.cpp"
#include "Script_Binder_Stalker.cpp"

#include "Script_DataBase.cpp"
#pragma endregion

#include "script_effector.cpp"
#include "script_effector_script.cpp"
#include "script_effector_wrapper.cpp"
#include "script_entity.cpp"
#include "Script_EntitySounds.cpp"
#include "script_entity_action.cpp"
#include "script_entity_action_script.cpp"
#include "script_game_object.cpp"
#include "script_game_object2.cpp"
#include "script_game_object3.cpp"
#include "script_game_object4.cpp"
#include "script_game_object_inventory_owner.cpp"
#include "script_game_object_script.cpp"
#include "script_game_object_script2.cpp"
#include "script_game_object_script3.cpp"
#include "script_game_object_script_trader.cpp"
#include "script_game_object_smart_covers.cpp"
#include "script_game_object_trader.cpp"
#include "script_game_object_use.cpp"
#include "script_game_object_use2.cpp"
#include "script_hit.cpp"
#include "script_hit_script.cpp"
#include "Script_ISchemeMonster.cpp"
#include "Script_ISchemeStalker.cpp"
#include "script_lanim.cpp"
#include "script_monster_action.cpp"
#include "script_monster_action_script.cpp"
#include "script_monster_hit_info_script.cpp"
#include "Script_MoveManager.cpp"
#include "script_movement_action.cpp"
#include "script_movement_action_script.cpp"
#include "Script_NewsManager.cpp"
#include "script_object.cpp"
#include "script_object_action.cpp"
#include "script_object_action_script.cpp"
#include "script_particles.cpp"
#include "script_particles_script.cpp"
#include "script_particle_action.cpp"
#include "script_particle_action_script.cpp"
#include "script_property_evaluator_wrapper.cpp"
#include "script_render_device_script.cpp"
#include "Script_SchemeHelicopterMove.cpp"

#pragma region Cordis Scheme MOB
#include "Script_SchemeMobCamp.cpp"
#include "Script_SchemeMobCombat.cpp"
#include "Script_SchemeMobDeath.cpp"
#include "Script_SchemeMobHome.cpp"
#include "Script_SchemeMobJump.cpp"
#include "Script_SchemeMobRemark.cpp"
#include "Script_SchemeMobWalker.cpp"
#pragma endregion

#pragma region Cordis Scheme PH
#include "Script_SchemePHButton.cpp"
#include "Script_SchemePHCode.cpp"
#include "Script_SchemePHDeath.cpp"
#include "Script_SchemePHDoor.cpp"
#include "Script_SchemePHForce.cpp"
#include "Script_SchemePHHit.cpp"
#include "Script_SchemePHIdle.cpp"
#include "Script_SchemePHMinigun.cpp"
#include "Script_SchemePHOnHit.cpp"
#include "Script_SchemePHOscillate.cpp"
#include "Script_SchemePHSound.cpp"
#pragma endregion

#pragma region Cordis Scheme SR
#include "Script_SchemeSRDeimos.cpp"
#include "Script_SchemeSRIdle.cpp"
#include "Script_SchemeSRLight.cpp"
#include "Script_SchemeSRNoWeapon.cpp"
#include "Script_SchemeSRParticle.cpp"
#include "Script_SchemeSRPostProcess.cpp"
#include "Script_SchemeSRPsyAntenna.cpp"
#include "Script_SchemeSRSilenceZone.cpp"
#include "Script_SchemeSRTeleport.cpp"
#include "Script_SchemeSRTimer.cpp"
#pragma endregion

#pragma region Cordis Schemes XR
#include "Script_SchemeXRAbuse.cpp"
#include "Script_SchemeXRCombat.cpp"
#include "Script_SchemeXRMeet.cpp"
#include "Script_SchemeXRCorpseDetection.cpp"
#include "Script_SchemeXRDanger.cpp"
#include "Script_SchemeXRDeath.cpp"
#include "Script_SchemeXRGatherItems.cpp"
#include "Script_SchemeXRHelpWounded.cpp"
#include "Script_SchemeXRHit.cpp"
#include "Script_SchemeXRRemark.cpp"
#include "Script_SchemeXRWalker.cpp"
#include "Script_SchemeXRSleeper.cpp"
#include "Script_SchemeXRKamp.cpp"
#include "Script_SchemeXRCamper.cpp"
#include "Script_SchemeXRWounded.cpp"
#include "Script_SchemeXRCombatIgnore.cpp"
#include "Script_SchemeXRCombatZombied.cpp"
#include "Script_SchemeXRCombatCamper.cpp"
#include "Script_SchemeXRPatrol.cpp"
#include "Script_SchemeXRReachTask.cpp"
#include "Script_SchemeXRAnimpoint.cpp"
#include "Script_SchemeXRSmartCover.cpp"
#include "Script_PostCombat.cpp"
#pragma endregion


#pragma region Cordis Server Entities
#include "Script_SE_Actor.cpp"
#include "Script_SE_Ammo.cpp"
#include "Script_SE_Artefact.cpp"
#include "Script_SE_Detector.cpp"
#include "Script_SE_Eatable.cpp"
#include "Script_SE_Explosive.cpp"
#include "Script_SE_Grenade.cpp"
#include "Script_SE_Helicopter.cpp"
#include "Script_SE_Helmet.cpp"
#include "Script_SE_InventoryBox.cpp"
#include "Script_SE_Item.cpp"
#include "Script_SE_ItemTorch.cpp"
#include "Script_SE_Lamp.cpp"
#include "Script_SE_LevelChanger.cpp"
#include "Script_SE_Monster.cpp"
#include "Script_SE_NewAttachableItem.cpp"
#include "Script_SE_Outfit.cpp"
#include "Script_SE_PDA.cpp"
#include "Script_SE_Physic.cpp"
#include "Script_SE_Restrictor.cpp"
#include "Script_SE_SimulationSquad.cpp"
#include "Script_SE_SmartCover.cpp"
#include "Script_SE_SmartTerrain.cpp"
#include "Script_SE_Stalker.cpp"
#include "Script_SE_Weapon.cpp"
#include "Script_SE_WeaponAutomaticShotgun.cpp"
#include "Script_SE_WeaponMagazined.cpp"
#include "Script_SE_WeaponMagazinedWGL.cpp"
#include "Script_SE_WeaponShotgun.cpp"
#include "Script_SE_ZoneAnomaly.cpp"
#include "Script_SE_ZoneTorrid.cpp"
#include "Script_SE_ZoneVisual.cpp"
#pragma endregion

#include "Script_GlobalHelper.cpp"

#include "Script_SimulationBoard.cpp"
#include "Script_SimulationObjects.cpp"
#include "Script_SmartTerrainControl.cpp"
#include "script_sound.cpp"
#include "Script_SoundManager.cpp"
#include "Script_SoundTheme.cpp"
#include "script_sound_action.cpp"
#include "script_sound_action_script.cpp"
#include "script_sound_info_script.cpp"
#include "script_sound_script.cpp"
#include "Script_StateAnimation.cpp"
#include "Script_StateManager.cpp"
#include "Script_StoryObject.cpp"
#include "Script_StroyObject.cpp"
#include "Script_SurgeManager.cpp"
#include "Script_TravelManager.cpp"
#include "Script_TreasureManager.cpp"
#include "Script_UI_MainMenu.cpp"
#include "Script_CampData.cpp"
#include "Script_TaskManager.cpp"
#include "Script_WeatherManager.cpp"
#include "Script_Task.cpp"
#include "script_watch_action.cpp"
#include "script_watch_action_script.cpp"
#include "script_zone.cpp"
#include "script_zone_script.cpp"
#include "SDKRender_Connection.cpp"
#include "searchlight.cpp"
#include "secure_messaging.cpp"
#include "seniority_hierarchy_holder.cpp"
#include "server_entity_wrapper.cpp"
#include "ShootingObject.cpp"
#include "shootingObject_dump_impl.cpp"
#include "sight_action.cpp"
#include "sight_manager.cpp"
#include "sight_manager_target.cpp"
#include "Silencer.cpp"
#include "silent_shots.cpp"
#include "SimpleDetector.cpp"
#include "SleepEffector.cpp"
#include "smart_cover.cpp"
#include "smart_cover_action.cpp"
#include "smart_cover_animation_planner.cpp"
#include "smart_cover_animation_selector.cpp"
#include "smart_cover_default_behaviour_planner.cpp"
#include "smart_cover_description.cpp"
#include "smart_cover_detail.cpp"
#include "smart_cover_evaluators.cpp"
#include "smart_cover_loophole.cpp"
#include "smart_cover_loophole_planner_actions.cpp"
#include "smart_cover_object.cpp"
#include "smart_cover_object_script.cpp"
#include "smart_cover_planner_actions.cpp"
#include "smart_cover_planner_target_provider.cpp"
#include "smart_cover_planner_target_selector.cpp"
#include "smart_cover_storage.cpp"
#include "smart_cover_transition.cpp"
#include "smart_cover_transition_animation.cpp"
#include "sound_collection_storage.cpp"
#include "sound_memory_manager.cpp"
#include "sound_player.cpp"
#include "space_restriction.cpp"
#include "space_restriction_base.cpp"
#include "space_restriction_bridge.cpp"
#include "space_restriction_composition.cpp"
#include "space_restriction_holder.cpp"
#include "space_restriction_manager.cpp"
#include "space_restriction_shape.cpp"
#include "space_restrictor.cpp"
#include "space_restrictor_script.cpp"
#include "Spectator.cpp"
#include "spectator_camera_first_eye.cpp"
#include "sprinter_stopper.cpp"
#include "squad_hierarchy_holder.cpp"
#include "StalkerOutfit.cpp"
#include "stalker_alife_actions.cpp"
#include "stalker_alife_planner.cpp"
#include "stalker_alife_task_actions.cpp"
#include "stalker_animation_callbacks.cpp"
#include "stalker_animation_data.cpp"
#include "stalker_animation_data_storage.cpp"
#include "stalker_animation_global.cpp"
#include "stalker_animation_head.cpp"
#include "stalker_animation_legs.cpp"
#include "stalker_animation_manager.cpp"
#include "stalker_animation_manager_debug.cpp"
#include "stalker_animation_manager_update.cpp"
#include "stalker_animation_names.cpp"
/*#include "stalker_animation_offsets.cpp"*/
#include "stalker_animation_pair.cpp"
#include "stalker_animation_script.cpp"
#include "stalker_animation_state.cpp"
#include "stalker_animation_torso.cpp"
#include "stalker_anomaly_actions.cpp"
#include "stalker_anomaly_planner.cpp"
#include "stalker_base_action.cpp"
#include "stalker_combat_actions.cpp"
#include "stalker_combat_action_base.cpp"
#include "stalker_combat_planner.cpp"
#include "stalker_danger_by_sound_actions.cpp"
#include "stalker_danger_by_sound_planner.cpp"
#include "stalker_danger_grenade_actions.cpp"
#include "stalker_danger_grenade_planner.cpp"
#include "stalker_danger_in_direction_actions.cpp"
#include "stalker_danger_in_direction_planner.cpp"
#include "stalker_danger_planner.cpp"
#include "stalker_danger_property_evaluators.cpp"
#include "stalker_danger_unknown_actions.cpp"
#include "stalker_danger_unknown_planner.cpp"
#include "stalker_death_actions.cpp"
#include "stalker_death_planner.cpp"
#include "stalker_flair.cpp"
#include "stalker_get_distance_actions.cpp"
#include "stalker_get_distance_planner.cpp"
#include "stalker_kill_wounded_actions.cpp"
#include "stalker_kill_wounded_planner.cpp"
#include "stalker_low_cover_actions.cpp"
#include "stalker_low_cover_planner.cpp"
#include "stalker_movement_manager_base.cpp"
#include "stalker_movement_manager_obstacles.cpp"
#include "stalker_movement_manager_obstacles_path.cpp"
#include "stalker_movement_manager_smart_cover.cpp"
#include "stalker_movement_manager_smart_cover_fov_range.cpp"
#include "stalker_movement_manager_smart_cover_loopholes.cpp"
#include "ai\stalker\ai_stalker.cpp"
#include "stalker_movement_params.cpp"
#include "stalker_planner.cpp"
#include "stalker_property_evaluators.cpp"
#include "stalker_search_actions.cpp"
#include "stalker_search_planner.cpp"
#include "stalker_sound_data.cpp"
#include "stalker_sound_data_visitor.cpp"
#include "stalker_velocity_collection.cpp"
#include "stalker_velocity_holder.cpp"
#include "state_arguments_functions.cpp"
/*#include "static_cast_checked_test.cpp"*/
#include "static_obstacles_avoider.cpp"
#include "stats_submitter.cpp"
#include "stats_submitter_dsa_params.cpp"
#include "StdAfx.cpp"
#include "steering_behaviour.cpp"
#include "step_manager.cpp"
#include "string_table.cpp"
#include "team_base_zone.cpp"
#include "team_hierarchy_holder.cpp"
#include "TeleWhirlwind.cpp"
#include "ThornArtifact.cpp"
#include "Torch.cpp"
#include "torch_script.cpp"
#include "TorridZone.cpp"
#include "Tracer.cpp"
#include "trade.cpp"
#include "trade2.cpp"
#include "trade_parameters.cpp"
#include "traffic_optimization.cpp"
#include "trajectories.cpp"
#include "ui\ArtefactDetectorUI.cpp"
/*#include "ui\CExtraContentFilter.cpp"*/
#include "ui\ChangeWeatherDialog.cpp"
#include "ui\FactionState.cpp"
#include "ui\FractionState.cpp"
#include "ui\map_hint.cpp"
#include "ui\MMSound.cpp"
#include "ui\Restrictions.cpp"
#include "ui\ServerList.cpp"
#include "ui\ServerList_GameSpy_func.cpp"
#include "ui\TeamInfo.cpp"
#include "ui\UIAchievements.cpp"
#include "ui\UIActorMenu.cpp"
#include "ui\UIActorMenuDeadBodySearch.cpp"
#include "ui\UIActorMenuInitialize.cpp"
#include "ui\UIActorMenuInventory.cpp"
#include "ui\UIActorMenuTrade.cpp"
#include "ui\UIActorMenuUpgrade.cpp"
#include "ui\UIActorMenu_action.cpp"
#include "ui\UIActorMenu_script.cpp"
#include "ui\UIActorStateInfo.cpp"
#include "ui\UIBoosterInfo.cpp"
#include "ui\UIBuyWeaponTab.cpp"
#include "ui\UIBuyWndShared.cpp"
#include "ui\UICarPanel.cpp"
#include "ui\UICDkey.cpp"
#include "ui\UICellCustomItems.cpp"
#include "ui\UICellItem.cpp"
#include "ui\UICellItemFactory.cpp"
#include "ui\UIChangeMap.cpp"
#include "ui\UICharacterInfo.cpp"
#include "ui\UIChatWnd.cpp"
#include "ui\UIColorAnimatorWrapper.cpp"
#include "ui\UIDebugFonts.cpp"
#include "ui\UIDemoPlayControl.cpp"
#include "ui\UIDialogWnd.cpp"
#include "ui\UIDragDropListEx.cpp"
#include "ui\UIDragDropReferenceList.cpp"
#include "ui\UIEditKeyBind.cpp"
#include "ui\UIFactionWarWnd.cpp"
/*
#include "ui\UIFrags.cpp"
#include "ui\UIFrags2.cpp"*/
/*#include "ui\UIFrameLine.cpp"*/
#include "ui\UIGameLog.cpp"
#include "ui\UIGameTutorial.cpp"
#include "ui\UIGameTutorialSimpleItem.cpp"
#include "ui\UIGameTutorialVideoItem.cpp"
#include "ui\UIHelper.cpp"
#include "ui\UIHudStatesWnd.cpp"
#include "ui\UIInventoryUpgradeWnd.cpp"
#include "ui\UIInventoryUpgradeWnd_add.cpp"
#include "ui\UIInventoryUtilities.cpp"
#include "ui\UIInvUpgrade.cpp"
#include "ui\UIInvUpgradeInfo.cpp"
#include "ui\UIInvUpgradeProperty.cpp"
#include "ui\UIItemInfo.cpp"
#include "ui\UIKeyBinding.cpp"
#include "ui\UIKickPlayer.cpp"
#include "ui\UILabel.cpp"
/*#include "ui\UIListBox_script.cpp"*/
/*
#include "ui\UIListItem.cpp"
#include "ui\UIListItemAdv.cpp"
#include "ui\UIListItemEx.cpp"*/
#include "ui\UIListItemServer.cpp"
/*
#include "ui\UIListWnd.cpp"
#include "ui\UIListWnd_script.cpp"*/
#include "ui\UILoadingScreen.cpp"
#include "ui\UILogsWnd.cpp"
#include "ui\UIMainIngameWnd.cpp"
#include "ui\UIMap.cpp"
#include "ui\UIMapDesc.cpp"
#include "ui\UIMapInfo.cpp"
#include "ui\UIMapInfo_script.cpp"
#include "ui\UIMapLegend.cpp"
#include "ui\UIMapList.cpp"
#include "ui\UIMapWnd.cpp"
#include "ui\UIMapWnd2.cpp"
#include "ui\UIMapWndActions.cpp"
#include "ui\UIMessageBoxEx.cpp"
#include "ui\UIMessagesWindow.cpp"
#include "ui\UIMMShniaga.cpp"
#include "ui\UIMoneyIndicator.cpp"
#include "ui\UIMotionIcon.cpp"
#include "ui\UIMPAdminMenu.cpp"
#include "ui\UIMPChangeMapAdm.cpp"
#include "ui\UIMpItemsStoreWnd.cpp"
#include "ui\UIMPPlayersAdm.cpp"
#include "ui\UIMPServerAdm.cpp"
#include "ui\UIMpTradeWnd.cpp"
#include "ui\UIMpTradeWnd_init.cpp"
#include "ui\UIMpTradeWnd_items.cpp"
#include "ui\UIMpTradeWnd_misc.cpp"
#include "ui\UIMpTradeWnd_trade.cpp"
#include "ui\UIMpTradeWnd_wpn.cpp"
#include "ui\UINewsItemWnd.cpp"
#include "ui\UIOptConCom.cpp"
#include "ui\UIOutfitInfo.cpp"
#include "ui\UIPdaKillMessage.cpp"
#include "ui\UIPdaMsgListItem.cpp"
#include "ui\UIPdaWnd.cpp"
#include "ui\UIRankIndicator.cpp"
#include "ui\UIRankingWnd.cpp"
#include "ui\UIScriptWnd.cpp"
/*#include "ui\UIScriptWnd_script.cpp"*/
#include "ui\UISecondTaskWnd.cpp"
#include "ui\UIServerInfo.cpp"
#include "ui\UISkinSelector.cpp"
#include "ui\UISleepStatic.cpp"
#include "ui\UISpawnWnd.cpp"
#include "ui\UISpeechMenu.cpp"
#include "ui\UIStatix.cpp"
#include "ui\UIStats.cpp"
#include "ui\UIStatsIcon.cpp"
#include "ui\UIStatsPlayerInfo.cpp"
#include "ui\UIStatsPlayerList.cpp"
#include "ui\UITabButtonMP.cpp"
#include "ui\UITalkDialogWnd.cpp"
#include "ui\UITalkWnd.cpp"
#include "ui\UITaskWnd.cpp"
/*#include "ui\UITextBanner.cpp"*/
#include "ui\UITextVote.cpp"
#include "ui\UITradeWnd.cpp"
#include "ui\UIVote.cpp"
#include "ui\UIVoteStatusWnd.cpp"
#include "ui\UIVotingCategory.cpp"
#include "ui\UIWarState.cpp"
#include "ui\UIWindow_script.cpp"
#include "ui\UIWpnParams.cpp"
#include "ui\UIXmlInit.cpp"
#include "ui\ui_af_params.cpp"
/*#include "UI.cpp"*/
#include "UIAchivementsIndicator.cpp"
#include "UIDialogHolder.cpp"
/*#include "UIFrameRect.cpp"*/
#include "UIGameAHunt.cpp"
#include "UIGameCTA.cpp"
#include "UIGameCustom.cpp"
#include "UIGameCustom_script.cpp"
#include "UIGameDM.cpp"
#include "UIGameMP.cpp"
#include "UIGameSP.cpp"
#include "UIGameTDM.cpp"
/*#include "UIGame_custom_script.cpp"*/
#include "UIPanelsClassFactory.cpp"
#include "UIPlayerItem.cpp"
#include "UITeamHeader.cpp"
#include "UITeamPanels.cpp"
#include "UITeamState.cpp"
#include "UIZoneMap.cpp"
#include "ui_export_script.cpp"
#include "vision_client.cpp"
#include "visual_memory_manager.cpp"
#include "visual_memory_params.cpp"
#include "wallmark_manager.cpp"
#include "Weapon.cpp"
#include "WeaponAK74.cpp"
#include "WeaponAmmo.cpp"
#include "WeaponAutomaticShotgun.cpp"
#include "WeaponBinoculars.cpp"
#include "WeaponBinocularsVision.cpp"
#include "weaponBM16.cpp"
#include "WeaponCustomPistol.cpp"
#include "WeaponCustomPistolAuto.cpp"
#include "WeaponDispersion.cpp"
#include "WeaponFire.cpp"
#include "WeaponFN2000.cpp"
#include "WeaponGroza.cpp"
#include "WeaponHPSA.cpp"
#include "WeaponKnife.cpp"
#include "WeaponLR300.cpp"
#include "WeaponMagazined.cpp"
#include "WeaponMagazinedWGrenade.cpp"
#include "WeaponPistol.cpp"
#include "WeaponPM.cpp"
#include "WeaponRevolver.cpp"
#include "WeaponRG6.cpp"
#include "WeaponRPG7.cpp"
#include "WeaponScript.cpp"
#include "WeaponShotgun.cpp"
#include "WeaponStatMgun.cpp"
#include "WeaponStatMgunFire.cpp"
#include "WeaponStatMgunIR.cpp"
#include "WeaponSVD.cpp"
#include "WeaponUpgrade.cpp"
#include "WeaponVal.cpp"
#include "WeaponVintorez.cpp"
#include "weapon_ammo_dump_impl.cpp"
#include "weapon_dump_impl.cpp"
#include "Wound.cpp"
#include "xrClientsPool.cpp"
#include "xrGame.cpp"
#include "xrGameSpyServer.cpp"
#include "xrGameSpyServer_callbacks.cpp"
#include "xrGameSpy_GameSpyFuncs.cpp"
#include "xrgame_dll_detach.cpp"
#include "xrServer.cpp"
#include "xrServerMapSync.cpp"
#include "xrServer_balance.cpp"
#include "xrServer_CL_connect.cpp"
#include "xrServer_CL_disconnect.cpp"
#include "xrServer_Connect.cpp"
#include "xrServer_Disconnect.cpp"
#include "xrServer_info.cpp"
#include "xrServer_perform_GameExport.cpp"
#include "xrServer_perform_migration.cpp"
#include "xrServer_perform_RPgen.cpp"
#include "xrServer_perform_sls_default.cpp"
#include "xrServer_perform_sls_load.cpp"
#include "xrServer_perform_sls_save.cpp"
#include "xrServer_perform_transfer.cpp"
#include "xrServer_process_event.cpp"
#include "xrServer_process_event_activate.cpp"
#include "xrServer_process_event_destroy.cpp"
#include "xrServer_process_event_ownership.cpp"
#include "xrServer_process_event_reject.cpp"
#include "xrServer_process_spawn.cpp"
#include "xrServer_process_update.cpp"
#include "xrServer_secure_messaging.cpp"
#include "xrServer_sls_clear.cpp"
#include "xrServer_svclient_validation.cpp"
#include "xrServer_updates_compressor.cpp"
/*#include "xr_Client_BattlEye.cpp"*/
#include "xr_level_controller.cpp"
/*#include "xr_Server_BattlEye.cpp"*/
#include "xr_time.cpp"
#include "ZoneCampfire.cpp"
#include "ZoneVisual.cpp"
#include "zone_effector.cpp"
#include "ZudaArtifact.cpp"
