template<CriticalSectionIndex index>
struct ScopedCriticalSection {
	inline ScopedCriticalSection() {
		CRITICAL_SECTION_MANAGER.enter_section(index);
	}
	inline ~ScopedCriticalSection() {
		CRITICAL_SECTION_MANAGER.leave_section(index);
	}
};

#define UniqueCriticalSectionLock(index) auto unique_name(critical_section_scope_guard_) = ScopedCriticalSection<index>()
#define CriticalSectionBlock(index) switch (UniqueCriticalSectionLock(index); 0) default:

// Player::on_tick
#if !DEBUG_NO_GAME_OVER
					__pause_menu_game_over_screen();
#else
#if TESTING_FEATURES
					int32_t continue_credits = GAME_MANAGER.continue_credits;
					switch (Zmboxf(
						"YOU DIED\n\nContinues: %d", NULL,
						MB_TOPMOST | MB_ICONWARNING | (continue_credits ? MB_CANCELTRYCONTINUE | MB_DEFBUTTON3 : MB_RETRYCANCEL)
						, continue_credits
					)) {
						case IDRETRY:
						case IDTRYAGAIN:
							APPLY_DEBUG_CONFIG();
							return UpdateFuncRestart;
							
						case IDCONTINUE:
							if (!GAME_MANAGER.continue_credits) {
						case IDCANCEL:
								exit(-1);
							}
							// this is just copy/pasted from the pause menu code
							GAME_MANAGER.globals.life_stocks = DEFAULT_LIFE_STOCKS;
							GAME_MANAGER.globals.life_fragments = 0;
							GAME_MANAGER.globals.set_bombs(DEFAULT_BOMB_STOCKS);
							GAME_MANAGER.globals.bomb_fragments = 0;
							GAME_MANAGER.globals.set_power(0);
							GAME_MANAGER.globals.add_power(GAME_MANAGER.globals.power_per_level * DEFAULT_MAX_POWER_LEVEL);
							PLAYER_PTR->data.__update_option_power_levels();
							__update_life_ui_unsafe();
							__update_bomb_ui_unsafe();
							int32_t continues_used = GAME_MANAGER.globals.continues + 1;
							GAME_MANAGER.globals.score = 0;
							GAME_MANAGER.continue_credits = continue_credits - 1;
							GAME_MANAGER.globals.continues = __min(continues_used, MAX_CONTINUES);
							break;
					}
#endif
#endif