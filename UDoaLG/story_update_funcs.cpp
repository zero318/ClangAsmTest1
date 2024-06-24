enum Difficulty {
    Easy,
    Normal,
    Hard,
    Lunatic
};

struct CPUStoryInf {
    int32_t phase; // Clamped [0, 1000]
    int32_t internal_gauge; // Clamped [0, 10000]
    
    virtual void update() {}
    
    inline void advance_phase() {
        this->phase++;
        this->internal_gauge = 0;
    }
    
    inline void send_ex_attack() {
        GAME_SIDES[1].ex_attack_manager_ptr->__send_ex_attack();
    }
    
    inline void level_up_ex_attack(int32_t levels) {
        GAME_SIDES[1].globals_side_ptr->add_to_ex_attack_level(levels);
    }
    
    inline void send_story_boss(int32_t ecl_phase_var) {
        GAME_SIDES[1].ex_attack_manager_ptr->send_story_boss_attack(ecl_phase_var);
    }
    
    inline bool can_send_boss() {
        return !GAME_SIDES[1].ex_attack_manager_ptr->ex_attack_ptr->__opposite_side_boss_is_active();
    }
};

struct CPUStage1StoryInf : CPUStoryInf {
    virtual void update() {
        switch (this->phase) {
            case 0: case 1:
                if (this->internal_gauge >= 300) {
                    this->advance_phase();
                    this->send_ex_attack();
                }
                break;
            case 2:
                if (this->internal_gauge >= 400 && this->can_send_boss()) {
                    this->advance_phase();
                    this->send_story_boss(1);
                }
                break;
            case 3:
                if (this->internal_gauge >= 200 && this->can_send_boss()) {
                    this->advance_phase();
                    switch (GAME_MANAGER.globals.get_difficulty()) {
                        case Easy:
                            this->send_story_boss(2);
                            break;
                        case Normal:
                            this->send_story_boss(3);
                            break;
                        case Hard:
                            this->send_story_boss(4);
                            break;
                        case Lunatic:
                            this->send_story_boss(6);
                            break;
                    }
                }
                break;
        }
    }
};

struct CPUStage2StoryInf : CPUStoryInf {
    virtual void update() {
        switch (this->phase) {
            case 0:
                if (this->internal_gauge >= 300) {
                    this->advance_phase();
                    this->send_ex_attack();
                }
                break;
            case 1:
                if (this->internal_gauge >= 300) {
                    this->advance_phase();
                    this->level_up_ex_attack(1);
                    this->send_ex_attack();
                }
                break;
            case 2:
                if (this->internal_gauge >= 500 && this->can_send_boss()) {
                    this->advance_phase();
                    this->send_story_boss(2);
                }
                break;
            case 3:
                if (this->internal_gauge >= 400 && this->can_send_boss()) {
                    this->advance_phase();
                    switch (GAME_MANAGER.globals.get_difficulty()) {
                        case Easy:
                            this->send_story_boss(3);
                            break;
                        case Normal:
                            this->send_story_boss(4);
                            break;
                        case Hard:
                            this->send_story_boss(5);
                            break;
                        case Lunatic:
                            this->send_story_boss(7);
                            break;
                    }
                }
                break;
        }
    }
};

struct CPUStage3StoryInf : CPUStoryInf {
    virtual void update() {
        switch (this->phase) {
            case 0:
                if (this->internal_gauge >= 300) {
                    this->advance_phase();
                    this->send_ex_attack();
                }
                break;
            case 1: case 3:
                if (this->internal_gauge >= 300) {
                    this->advance_phase();
                    this->level_up_ex_attack(1);
                    this->send_ex_attack();
                }
                break;
            case 2:
                if (this->internal_gauge >= 500 && this->can_send_boss()) {
                    this->advance_phase();
                    this->send_story_boss(2);
                }
                break;
            case 4:
                if (this->internal_gauge >= 600 && this->can_send_boss()) {
                    this->advance_phase();
                    switch (GAME_MANAGER.globals.get_difficulty()) {
                        case Easy:
                            this->send_story_boss(4);
                            break;
                        case Normal:
                            this->send_story_boss(5);
                            break;
                        case Hard:
                            this->send_story_boss(6);
                            break;
                        case Lunatic:
                            this->send_story_boss(8);
                            break;
                    }
                }
                break;
        }
    }
};

struct CPUStage4StoryInf : CPUStoryInf {
    virtual void update() {
        switch (this->phase) {
            case 0:
                if (this->internal_gauge >= 300) {
                    this->advance_phase();
                    this->send_ex_attack();
                }
                break;
            case 1:
                if (this->internal_gauge >= 350) {
                    this->advance_phase();
                    this->level_up_ex_attack(1);
                    this->send_ex_attack();
                }
                break;
            case 2:
                if (this->internal_gauge >= 500 && this->can_send_boss()) {
                    this->advance_phase();
                    this->send_story_boss(1);
                }
                break;
            case 3:
                if (this->internal_gauge >= 300) {
                    this->advance_phase();
                    this->level_up_ex_attack(1);
                    this->send_ex_attack();
                }
                break;
            case 4:
                if (this->internal_gauge >= 500 && this->can_send_boss()) {
                    this->advance_phase();
                    this->send_story_boss(3);
                }
                break;
            case 5:
                if (this->internal_gauge >= 300 && this->can_send_boss()) {
                    this->advance_phase();
                    switch (GAME_MANAGER.globals.get_difficulty()) {
                        case Easy:
                            this->send_story_boss(4);
                            break;
                        case Normal:
                            this->send_story_boss(5);
                            break;
                        case Hard:
                            this->send_story_boss(6);
                            break;
                        case Lunatic:
                            this->send_story_boss(8);
                            break;
                    }
                }
                break;
        }
    }
};

struct CPUStage5StoryInf : CPUStoryInf {
    virtual void update() {
        switch (this->phase) {
            case 0:
                if (this->internal_gauge >= 300) {
                    this->advance_phase();
                    this->send_ex_attack();
                }
                break;
            case 1:
                if (this->internal_gauge >= 300) {
                    this->advance_phase();
                    this->send_story_boss(1);
                }
                break;
            case 2:
                if (this->internal_gauge >= 150 && this->can_send_boss()) {
                    this->advance_phase();
                    this->level_up_ex_attack(2);
                    this->send_ex_attack();
                }
                break;
            case 3: case 5:
                if (this->internal_gauge >= 300 && this->can_send_boss()) {
                    this->advance_phase();
                    this->level_up_ex_attack(1);
                    this->send_ex_attack();
                }
                break;
            case 4:
                if (this->internal_gauge >= 300) {
                    this->advance_phase();
                    this->send_story_boss(4);
                }
                break;
            case 6:
                if (this->internal_gauge >= 100 && this->can_send_boss()) {
                    this->advance_phase();
                    switch (GAME_MANAGER.globals.get_difficulty()) {
                        case Easy:
                            this->send_story_boss(5);
                            break;
                        case Normal:
                            this->send_story_boss(6);
                            break;
                        case Hard:
                            this->send_story_boss(7);
                            break;
                        case Lunatic:
                            this->send_story_boss(8);
                            break;
                    }
                }
                break;
        }
    }
};

struct CPUStage6StoryInf : CPUStoryInf {
    virtual void update() {
        switch (this->phase) {
            case 0:
                if (this->internal_gauge >= 300) {
                    this->advance_phase();
                    this->send_ex_attack();
                }
                break;
            case 1:
                if (this->internal_gauge >= 200) {
                    this->advance_phase();
                    this->send_story_boss(0);
                }
                break;
            case 2:
                if (this->internal_gauge >= 200) {
                    this->advance_phase();
                    this->send_story_boss(2);
                }
                break;
            case 3:
                if (this->internal_gauge >= 200 && this->can_send_boss()) {
                    this->advance_phase();
                    this->level_up_ex_attack(2);
                    this->send_ex_attack();
                }
                break;
            case 4:
                if (this->internal_gauge >= 200 && this->can_send_boss()) {
                    this->advance_phase();
                    this->level_up_ex_attack(1);
                    this->send_ex_attack();
                }
                break;
            case 5:
                if (this->internal_gauge >= 400) {
                    this->advance_phase();
                    this->level_up_ex_attack(1);
                    this->send_ex_attack();
                }
                break;
            case 6:
                if (this->internal_gauge >= 200) {
                    this->advance_phase();
                    this->send_story_boss(3);
                }
                break;
            case 7:
                if (this->internal_gauge >= 300 && this->can_send_boss()) {
                    this->advance_phase();
                    switch (GAME_MANAGER.globals.get_difficulty()) {
                        case Easy:
                            this->send_story_boss(10);
                            break;
                        case Normal:
                            this->send_story_boss(11);
                            break;
                        case Hard:
                            this->send_story_boss(12);
                            break;
                        case Lunatic:
                            this->send_story_boss(13);
                            break;
                    }
                }
                break;
            case 8:
                if (this->internal_gauge >= 100 && this->can_send_boss()) {
                    this->advance_phase();
                    switch (GAME_MANAGER.globals.get_difficulty()) {
                        case Easy:
                            this->send_story_boss(14);
                            break;
                        case Normal:
                            this->send_story_boss(15);
                            break;
                        case Hard:
                            this->send_story_boss(16);
                            break;
                        case Lunatic:
                            this->send_story_boss(17);
                            break;
                    }
                }
                break;
        }
    }
};