	{
		.name = "BLANK",
		.id = BLANK_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = FreeTier, // $0
		.shop_weight = ALWAYS_IN_SHOP, // 0
		.availability = AVAILABLE_IF_UNLOCKED_STAGES_1_2_3_4_5, // 11
		.allow_duplicates = false,
		.__can_starting_equip = false,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 4,
		.sprite_small = 5
	},
	{
		.name = "EXTEND",
		.id = EXTEND_CARD,
		.__int_8 = 1,
		.__int_C = 3,
		.price_tier = CheapTier2, // $80
		.shop_weight = ALWAYS_IN_SHOP, // 0
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = true,
		.__can_starting_equip = false,
		.__default_unlock = true,
		.__render_passive_in_hud = false,
		.sprite_large = 6,
		.sprite_small = 7
	},
	{
		.name = "BOMB",
		.id = BOMB_CARD,
		.__int_8 = 1,
		.__int_C = 3,
		.price_tier = FreeTier, // $0
		.shop_weight = ALWAYS_IN_SHOP, // 0
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = true,
		.__can_starting_equip = false,
		.__default_unlock = true,
		.__render_passive_in_hud = false,
		.sprite_large = 8,
		.sprite_small = 9
	},
	{
		.name = "EXTEND2",
		.id = EXTEND2_CARD,
		.__int_8 = 1,
		.__int_C = 3,
		.price_tier = FreeTier, // $0
		.shop_weight = NEVER_IN_SHOP, // 6
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = true,
		.__can_starting_equip = false,
		.__default_unlock = true,
		.__render_passive_in_hud = false,
		.sprite_large = 10,
		.sprite_small = 11
	},
	{
		.name = "BOMB2",
		.id = BOMB2_CARD,
		.__int_8 = 1,
		.__int_C = 3,
		.price_tier = FreeTier, // $0
		.shop_weight = NEVER_IN_SHOP, // 6
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = true,
		.__can_starting_equip = false,
		.__default_unlock = true,
		.__render_passive_in_hud = false,
		.sprite_large = 12,
		.sprite_small = 13
	},
	{
		.name = "PENDULUM",
		.id = PENDULUM_CARD,
		.__int_8 = 1,
		.__int_C = 3,
		.price_tier = FreeTier, // $0
		.shop_weight = ALWAYS_IN_SHOP, // 0
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = true,
		.__can_starting_equip = false,
		.__default_unlock = true,
		.__render_passive_in_hud = false,
		.sprite_large = 16,
		.sprite_small = 17
	},
	{
		.name = "DANGO",
		.id = DANGO_CARD,
		.__int_8 = 1,
		.__int_C = 3,
		.price_tier = FreeTier, // $0
		.shop_weight = ALWAYS_IN_SHOP, // 0
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = true,
		.__can_starting_equip = false,
		.__default_unlock = true,
		.__render_passive_in_hud = false,
		.sprite_large = 18,
		.sprite_small = 19
	},
	{
		.name = "MOKOU",
		.id = MOKOU_CARD,
		.__int_8 = 1,
		.__int_C = 3,
		.price_tier = ExpensiveTier4, // $450
		.shop_weight = 4,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = false,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 14,
		.sprite_small = 15
	},
	{
		.name = "MANEKI",
		.id = MANEKI_CARD,
		.__int_8 = 1,
		.__int_C = 2,
		.price_tier = ReasonableTier2, // $140
		.shop_weight = 2,
		.availability = STAGE_1_REWARD, // 1
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 100,
		.sprite_small = 101
	},
	{
		.name = "YAMAWARO",
		.id = YAMAWARO_CARD,
		.__int_8 = 1,
		.__int_C = 2,
		.price_tier = CheapTier2, // $80
		.shop_weight = 2,
		.availability = STAGE_2_REWARD, // 2
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 102,
		.sprite_small = 103
	},
	{
		.name = "KISERU",
		.id = KISERU_CARD,
		.__int_8 = 1,
		.__int_C = 2,
		.price_tier = PriceyTier3, // $280
		.shop_weight = 2,
		.availability = STAGE_3_REWARD, // 3
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 104,
		.sprite_small = 105
	},
	{
		.name = "NARUMI",
		.id = NARUMI_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = ReasonableTier2, // $140
		.shop_weight = 4,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 20,
		.sprite_small = 21
	},
	{
		.name = "PACHE",
		.id = PACHE_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = CheapTier3, // $100
		.shop_weight = 4,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 22,
		.sprite_small = 23
	},
	{
		.name = "YOUMU_OP",
		.id = YOUMU_OP_CARD,
		.__int_8 = 1,
		.__int_C = 1,
		.price_tier = PriceyTier2, // $240
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = true,
		.__render_passive_in_hud = true,
		.sprite_large = 44,
		.sprite_small = 45
	},
	{
		.name = "REIMU_OP",
		.id = REIMU_OP_CARD,
		.__int_8 = 1,
		.__int_C = 1,
		.price_tier = PriceyTier2, // $240
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 24,
		.sprite_small = 25
	},
	{
		.name = "ALICE_OP",
		.id = ALICE_OP_CARD,
		.__int_8 = 1,
		.__int_C = 1,
		.price_tier = PriceyTier3, // $280
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 36,
		.sprite_small = 37
	},
	{
		.name = "CIRNO_OP",
		.id = CIRNO_OP_CARD,
		.__int_8 = 1,
		.__int_C = 1,
		.price_tier = PriceyTier1, // $200
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 40,
		.sprite_small = 41
	},
	{
		.name = "REIMU_OP2",
		.id = REIMU_OP2_CARD,
		.__int_8 = 0,
		.__int_C = 1,
		.price_tier = PriceyTier3, // $280
		.shop_weight = 1,
		.availability = AVAILABLE_IF_UNLOCKED, // 12
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 46,
		.sprite_small = 47
	},
	{
		.name = "MARISA_OP",
		.id = MARISA_OP_CARD,
		.__int_8 = 0,
		.__int_C = 1,
		.price_tier = PriceyTier2, // $240
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 26,
		.sprite_small = 27
	},
	{
		.name = "MARISA_OP2",
		.id = MARISA_OP2_CARD,
		.__int_8 = 0,
		.__int_C = 1,
		.price_tier = PriceyTier3, // $280
		.shop_weight = 1,
		.availability = AVAILABLE_IF_UNLOCKED, // 12
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 48,
		.sprite_small = 49
	},
	{
		.name = "SAKUYA_OP",
		.id = SAKUYA_OP_CARD,
		.__int_8 = 0,
		.__int_C = 1,
		.price_tier = PriceyTier2, // $240
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 28,
		.sprite_small = 29
	},
	{
		.name = "SAKUYA_OP2",
		.id = SAKUYA_OP2_CARD,
		.__int_8 = 0,
		.__int_C = 1,
		.price_tier = PriceyTier3, // $280
		.shop_weight = 1,
		.availability = AVAILABLE_IF_UNLOCKED, // 12
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 50,
		.sprite_small = 51
	},
	{
		.name = "SANAE_OP",
		.id = SANAE_OP_CARD,
		.__int_8 = 0,
		.__int_C = 1,
		.price_tier = PriceyTier2, // $240
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 30,
		.sprite_small = 31
	},
	{
		.name = "SANAE_OP2",
		.id = SANAE_OP2_CARD,
		.__int_8 = 0,
		.__int_C = 1,
		.price_tier = PriceyTier3, // $280
		.shop_weight = 1,
		.availability = AVAILABLE_IF_UNLOCKED, // 12
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 52,
		.sprite_small = 53
	},
	{
		.name = "OKINA_OP",
		.id = OKINA_OP_CARD,
		.__int_8 = 0,
		.__int_C = 1,
		.price_tier = PriceyTier3, // $280
		.shop_weight = 1,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 38,
		.sprite_small = 39
	},
	{
		.name = "NUE_OP",
		.id = NUE_OP_CARD,
		.__int_8 = 0,
		.__int_C = 1,
		.price_tier = PriceyTier1, // $200
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 42,
		.sprite_small = 43
	},
	{
		.name = "DBOMBEXTEND",
		.id = DBOMBEXTEND_CARD,
		.__int_8 = 1,
		.__int_C = 2,
		.price_tier = CheapTier3, // $100
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = true,
		.__render_passive_in_hud = true,
		.sprite_large = 58,
		.sprite_small = 59
	},
	{
		.name = "AUTOBOMB",
		.id = AUTOBOMB_CARD,
		.__int_8 = 1,
		.__int_C = 2,
		.price_tier = ExpensiveTier2, // $350
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 56,
		.sprite_small = 57
	},
	{
		.name = "ITEM_CATCH",
		.id = ITEM_CATCH_CARD,
		.__int_8 = 1,
		.__int_C = 2,
		.price_tier = CheapTier3, // $100
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 32,
		.sprite_small = 33
	},
	{
		.name = "MAINSHOT_PU",
		.id = MAINSHOT_PU_CARD,
		.__int_8 = 1,
		.__int_C = 2,
		.price_tier = ReasonableTier3, // $180
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 54,
		.sprite_small = 55
	},
	{
		.name = "KOISHI",
		.id = KOISHI_CARD,
		.__int_8 = 1,
		.__int_C = 2,
		.price_tier = CheapTier3, // $100
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 72,
		.sprite_small = 73
	},
	{
		.name = "ITEM_LINE",
		.id = ITEM_LINE_CARD,
		.__int_8 = 1,
		.__int_C = 2,
		.price_tier = ReasonableTier1, // $100
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 34,
		.sprite_small = 35
	},
	{
		.name = "MAGICSCROLL",
		.id = MAGICSCROLL_CARD,
		.__int_8 = 1,
		.__int_C = 2,
		.price_tier = ExpensiveTier1, // $300
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 64,
		.sprite_small = 65
	},
	{
		.name = "MAINSHOT_SP",
		.id = MAINSHOT_SP_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = ReasonableTier2, // $140
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 60,
		.sprite_small = 61
	},
	{
		.name = "SPEEDQUEEN",
		.id = SPEEDQUEEN_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = ReasonableTier1, // $100
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 62,
		.sprite_small = 63
	},
	{
		.name = "OPTION_BR",
		.id = OPTION_BR_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = ExpensiveTier2, // $350
		.shop_weight = 1,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 66,
		.sprite_small = 67
	},
	{
		.name = "DEAD_SPELL",
		.id = DEAD_SPELL_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = CheapTier3, // $100
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 68,
		.sprite_small = 69
	},
	{
		.name = "POWERMAX",
		.id = POWERMAX_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = ExpensiveTier2, // $350
		.shop_weight = 3,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 70,
		.sprite_small = 71
	},
	{
		.name = "YUYUKO",
		.id = YUYUKO_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = PriceyTier2, // $240
		.shop_weight = 1,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 74,
		.sprite_small = 75
	},
	{
		.name = "MONEY",
		.id = MONEY_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = ReasonableTier2, // $140
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 76,
		.sprite_small = 77
	},
	{
		.name = "ROKUMON",
		.id = ROKUMON_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = ReasonableTier2, // $140
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 78,
		.sprite_small = 79
	},
	{
		.name = "KOZUCHI",
		.id = KOZUCHI_CARD,
		.__int_8 = 1,
		.__int_C = 0,
		.price_tier = ReasonableTier3, // $180
		.shop_weight = 4,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = true,
		.__render_passive_in_hud = true,
		.sprite_large = 88,
		.sprite_small = 89
	},
	{
		.name = "WARP",
		.id = WARP_CARD,
		.__int_8 = 1,
		.__int_C = 0,
		.price_tier = ExpensiveTier1, // $300
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 80,
		.sprite_small = 81
	},
	{
		.name = "KANAME",
		.id = KANAME_CARD,
		.__int_8 = 1,
		.__int_C = 0,
		.price_tier = ExpensiveTier3, // $400
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 86,
		.sprite_small = 87
	},
	{
		.name = "MOON",
		.id = MOON_CARD,
		.__int_8 = 1,
		.__int_C = 0,
		.price_tier = PriceyTier2, // $240
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 82,
		.sprite_small = 83
	},
	{
		.name = "BASSDRUM",
		.id = BASSDRUM_CARD,
		.__int_8 = 0,
		.__int_C = 0,
		.price_tier = ReasonableTier2, // $140
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 96,
		.sprite_small = 97
	},
	{
		.name = "MIKOFLASH",
		.id = MIKOFLASH_CARD,
		.__int_8 = 0,
		.__int_C = 0,
		.price_tier = ExpensiveTier3, // $400
		.shop_weight = 4,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 84,
		.sprite_small = 85
	},
	{
		.name = "VAMPIRE",
		.id = VAMPIRE_CARD,
		.__int_8 = 0,
		.__int_C = 0,
		.price_tier = PriceyTier2, // $240
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 90,
		.sprite_small = 91
	},
	{
		.name = "SUN",
		.id = SUN_CARD,
		.__int_8 = 0,
		.__int_C = 0,
		.price_tier = ExpensiveTier4, // $450
		.shop_weight = 4,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 92,
		.sprite_small = 93
	},
	{
		.name = "LILY",
		.id = LILY_CARD,
		.__int_8 = 0,
		.__int_C = 0,
		.price_tier = ExpensiveTier1, // $300
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 94,
		.sprite_small = 95
	},
	{
		.name = "PSYCO",
		.id = PSYCO_CARD,
		.__int_8 = 0,
		.__int_C = 0,
		.price_tier = ReasonableTier3, // $180
		.shop_weight = 2,
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 98,
		.sprite_small = 99
	},
	{
		.name = "MAGATAMA",
		.id = MAGATAMA_CARD,
		.__int_8 = 0,
		.__int_C = 1,
		.price_tier = PriceyTier2, // $240
		.shop_weight = 4,
		.availability = STAGE_4_REWARD, // 4
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 106,
		.sprite_small = 107
	},
	{
		.name = "CYLINDER",
		.id = CYLINDER_CARD,
		.__int_8 = 0,
		.__int_C = 0,
		.price_tier = ReasonableTier2, // $140
		.shop_weight = 4,
		.availability = STAGE_5_REWARD, // 5
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 108,
		.sprite_small = 109
	},
	{
		.name = "RICEBALL",
		.id = RICEBALL_CARD,
		.__int_8 = 0,
		.__int_C = 0,
		.price_tier = PriceyTier3, // $280
		.shop_weight = 4,
		.availability = STAGE_5_REWARD, // 5
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 110,
		.sprite_small = 111
	},
	{
		.name = "MUKADE",
		.id = MUKADE_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = ExpensiveTier1, // $300
		.shop_weight = 3,
		.availability = AVAILABLE_IF_UNLOCKED, // 12
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 112,
		.sprite_small = 113
	},
	{
		.name = "MAGATAMA2",
		.id = MAGATAMA2_CARD,
		.__int_8 = 0,
		.__int_C = 2,
		.price_tier = ReasonableTier2, // $140
		.shop_weight = NEVER_IN_SHOP, // 6
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = false,
		.__default_unlock = false,
		.__render_passive_in_hud = true,
		.sprite_large = 114,
		.sprite_small = 115
	},
	{
		.name = "NULL",
		.id = NULL_CARD,
		.__int_8 = 1,
		.__int_C = 4,
		.price_tier = FreeTier, // $0
		.shop_weight = ALWAYS_IN_SHOP, // 0
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = true,
		.__default_unlock = true,
		.__render_passive_in_hud = false,
		.sprite_large = 116,
		.sprite_small = 117
	},
	{
		.name = "BACK",
		.id = BACK_CARD,
		.__int_8 = 1,
		.__int_C = 4,
		.price_tier = FreeTier, // $0
		.shop_weight = ALWAYS_IN_SHOP, // 0
		.availability = ALWAYS_AVAILABLE, // 0
		.allow_duplicates = false,
		.__can_starting_equip = false,
		.__default_unlock = true,
		.__render_passive_in_hud = false,
		.sprite_large = 2,
		.sprite_small = 3
	}