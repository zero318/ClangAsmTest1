{
  "options": {
    "ToutetsuC1EtBreakSubName": {
      "type": "s",
      "val": "Ecl_EtBreakLv1Toutetsu"
    },
    "ReimuAuraAun2Base": {
      "type": "f32",
      "val": 0.64
    },
    "MarisaAuraAun2Base": {
      "type": "f32",
      "val": 0.14
    },
    "SanaeAuraAun2Base": {
      "type": "f32",
      "val": 1.0
    },
    "RanAuraAun2Base": {
      "type": "f32",
      "val": 0.3
    },
    "AunAuraAun2Base": {
      "type": "f32",
      "val": 0.32
    },
    "NazrinAuraAun2Base": {
      "type": "f32",
      "val": 0.4
    },
    "SeiranAuraAun2Base": {
      "type": "f32",
      "val": 0.48
    },
    "OrinAura1Aun2Base": {
      "type": "f32",
      "val": 0.5
    },
    "OrinAura2Aun2Base": {
      "type": "f32",
      "val": 0.5
    },
    "TsukasaAuraAun2Base": {
      "type": "f32",
      "val": 0.5
    },
    "MamizouAuraAun2Base": {
      "type": "f32",
      "val": 0.33
    },
    "YachieAuraAun2Base": {
      "type": "f32",
      "val": 0.88
    },
    "SakiAura1Aun2Base": {
      "type": "f32",
      "val": 0.96
    },
    "SakiAura2Aun2Base": {
      "type": "f32",
      "val": 0.32
    },
    "ToutetsuAuraAun2Base": {
      "type": "f32",
      "val": 4.0
    },
    "SuikaAuraAun2Base": {
      "type": "f32",
      "val": 0.4
    },
    "BitenAuraAun2Base": {
      "type": "f32",
      "val": 0.32
    },
    "EnokoAuraAun2Base": {
      "type": "f32",
      "val": 0.48
    },
    "ChiyariAuraAun2Base": {
      "type": "f32",
      "val": 0.56
    },
    "HisamiAuraAun2Base": {
      "type": "f32",
      "val": 1.2
    },
    "ZanmuAura1Aun2Base": {
      "type": "f32",
      "val": 1.28
    },
    "ZanmuAura2Aun2Base": {
      "type": "f32",
      "val": 0.64
    },
    "BuffHisamiCard2Mul": {
      "type": "f32",
      "val": 0.5
    }
  },

  "binhacks": {
    "BulletCap": {
      "addr": "RxE5B54",
      "code": "B004"
    },
      "LaserCap": {
      "addr": "Rx11DBA9",
      "code": "9600"
    },

/*
    //302A0000
    "RankDelayA": {
      "addr": "Rx1087B7",
      "code": " \
        08070000 \
        7D 07 \
        33C0 \
        E9 98000000 \
        A1 <Rx1AE618> \
        8B 4008 \
        E9 8B000000 \
        <nop:4> \
      "
    },
    "RankDelayB": {
      "addr": "Rx108861",
      "code": "<nop:10>"
    },
*/

    "SeperateExAttackAndBossAttackCheckA": {
      "addr": "Rx1032A0",
      "code": "83C6 4C"
    },
    "SeperateExAttackAndBossAttackCheckB": {
      "addr": "Rx1032C4",
      "code": " \
        8B35 <Rx1AE4D8> \
        8B7E 4C \
        39F9 \
        0F8F 93000000 \
        0F8C 9D000000 \
        8B35 <Rx1AE49C> \
        8B4E 48 \
        8B35 <Rx1AE4D8> \
        8B7E 48 \
        39F9 \
        7F 77 \
        0F8C 81000000 \
        E9 8A000000 \
      "
    },

    "ReimuC2NoC1": {
      "addr": "Rx127675",
      "code": "\
        E9 [codecave:ReimuC2NoC1Cave] \
        90 \
      "
    },
    "ReimuC2ShootAndChargeDelayA": {
      "addr": "Rx12762A",
      "code": "E9 [codecave:ReimuC2ShootAndChargeDelayACave]"
    },
    "ReimuC2ShootAndChargeDelayB": {
        "addr": "Rx185D60",
        "code": "<codecave:ReimuC2ShootAndChargeDelayBCave>"
    },
    "ReimuC2Range": {
      "addr": "Rx127BA1",
      "code": "0F8D 06010000 \
        8D7D D8 \
        BB 03000000 \
        90 \
      "
    },
    "ReimuC2Time": {
      "addr": "Rx127D97",
      "code": "32"
    },

    "NazrinScopeSpeedIncrease": {
      "addr": "Rx188D6",
      "code": "BE"
    },
    "NazrinBombDmg": {
      "addr": "Rx12C681",
      "code": "28"
    },

    "YachieCancelDamage": {
      "addr": "Rx1F443",
      "code": "36"
    },
    "YachieC2Range": {
      "addr": "Rx1F3D5",
      "code": "28"
    },

    "ToutetsuScopeTime": {
      "addr": "Rx21368",
      "code": "20030000"
    },
    "ToutetsuC2RangeA": {
      "addr": "Rx21BA4",
      "code": "70C2"
    },
    "ToutetsuC2RangeB": {
      "addr": "Rx21BAE",
      "code": "9643"
    },
    "ToutetsuC2RangeC": {
      "addr": "Rx21BB8",
      "code": "70C2"
    },
    "ToutetsuC1TimeOffset": {
      "addr": "Rx21A56",
      "code": "11"
    },
    "ToutetsuC2EndTime": {
      "addr": "Rx21DF5",
      "code": "50"
    },
    "ToutetsuC2Dmg": {
      "addr": "Rx21E4C",
      "code": "50"
    },
    "ToutetsuExTimeCap": {
      "addr": "Rx21F07",
      "code": "1B"
    },
    "ToutetsuEraseBullets": {
      "addr": "Rx21E09",
      "code": "E9 [codecave:ToutetsuEraseBulletsCave]"
    },

    "ToutetsuC1EtBreak": {
      "addr": "Rx21ACD",
      "code": "E8 [codecave:ToutetsuC1EtBreakCave]"
    },

    "ToutetsuNoCancelA": {
      "addr": "Rx21E24",
      "code": "83C4 14 9090"
    },
    "ToutetsuNoCancelB": {
      "addr": "Rx21E3B",
      "code": "83C4 0C 9090"
    },

    "ToutetsuNoEnemyCancel": {
      "addr": "Rx21E84",
      "code": "90909090"
    },

    "CiyariScopeDmg": {
      "addr": "Rx26D03",
      "code": "06"
    },

    "ZanmuC2A": {
      "addr": "Rx299BF",
      "code": "A001" //5A00
    },
    "ZanmuC2B": {
      "addr": "Rx299C9",
      "code": "06" //04
    },
    "ZanmuC2C": {
      "addr": "Rx299D5",
      "code": "803F"
    },
    "ZanmuC2D": {
      "addr": "Rx299DF",
      "code": "A043"
    },
    "ZanmuC2E": {
      "addr": "Rx299E9",
      "code": "803F"
    },

    "AnimalSpiritIdentify": {
      "addr": "Rx29DBA",
      "code": " \
        8B86 CC140000 \
        8945BC \
        8B86 D8140000 \
        83E003 \
        8945C8 \
        F30F1086 78120000 \
        F30F1145 9C \
        F30F1086 7C120000 \
        F30F1145 A0 \
        F30F1086 80120000 \
        B8 <Rx186B8C> \
        8D4D9C \
        51 \
        8B8F D0000000 \
        F30F1145 A4 \
        C745B0 64000000 \
        C745A8 00000000 \
        31D2 \
        8955AC \
      "
    },

    "ReimuAuraAun2Multiplier": {
      "addr": "Rx14CA0",
      "code": "<codecave:Aun2CardMultipliers>"
    },
    "MarisaAuraAun2Multiplier": {
      "addr": "Rx15900",
      "code": "(<codecave:Aun2CardMultipliers>+4)"
    },
    "SanaeAuraAun2Multiplier": {
      "addr": "Rx163D1",
      "code": " \
        6A 08 \
        E8 [codecave:Aun2CardCalculate] \
        <nop:57> \
      "
    },
    "RanAuraAun2Multiplier": {
      "addr": "Rx173BD",
      "code": " \
        6A 0C \
        E8 [codecave:Aun2CardCalculate] \
      "
    },
    "AunAuraAun2Multiplier": {
      "addr": "Rx18058",
      "code": "(<codecave:Aun2CardMultipliers>+16)"
    },
    "NazrinAuraAun2Multiplier": {
      "addr": "Rx18920",
      "code": "(<codecave:Aun2CardMultipliers>+20)"
    },
    "SeiranAuraAun2Multiplier": {
      "addr": "Rx195C0",
      "code": "(<codecave:Aun2CardMultipliers>+24)"
    },
    "OrinAura1Aun2Multiplier": {
      "addr": "Rx1A11D",
      "code": " \
        6A 1C \
        E8 [codecave:Aun2CardCalculate] \
      "
    },
    "OrinAura2Aun2Multiplier": {
      "addr": "Rx1A255",
      "code": " \
        E8 [codecave:Aun2CardCalculateOrin2] \
        <nop:2> \
      "
    },
    "TsukasaAuraAun2Multiplier": {
      "addr": "Rx1B48D",
      "code": " \
        6A 24 \
        E8 [codecave:Aun2CardCalculate] \
      "
    },
    "MamizouAuraAun2Multiplier": {
      "addr": "Rx1CBB3",
      "code": " \
        6A 28 \
        E8 [codecave:Aun2CardCalculate] \
      "
    },
    "YachieAuraAun2Multiplier": {
      "addr": "Rx1E3BD",
      "code": " \
        6A 2C \
        E8 [codecave:Aun2CardCalculate] \
      "
    },
    "SakiAura1Aun2Multiplier": {
      "addr": "Rx1FF75",
      "code": " \
        6A 30 \
        E8 [codecave:Aun2CardCalculate] \
      "
    },
    "SakiAura2Aun2Multiplier": {
      "addr": "Rx1FF00",
      "code": " \
        68 34000000 \
        E8 [codecave:Aun2CardCalculateB] \
      "
    },
    "ToutetsuAuraAun2Multiplier": {
      "addr": "Rx2137D",
      "code": " \
        6A 38 \
        E8 [codecave:Aun2CardCalculate] \
      "
    },
    "SuikaAuraAun2Multiplier": {
      "addr": "Rx2294A",
      "code": " \
        E8 [codecave:Aun2CardCalculateSuika] \
        <nop:2> \
      "
    },
    "BitenAuraAun2Multiplier": {
      "addr": "Rx23CBD",
      "code": " \
        6A 40 \
        E8 [codecave:Aun2CardCalculate] \
      "
    },
    "EnokoAuraAun2Multiplier": {
      "addr": "Rx257F3",
      "code": " \
        6A 44 \
        E8 [codecave:Aun2CardCalculate] \
      "
    },
    "ChiyariAuraAun2Multiplier": {
      "addr": "Rx26B8D",
      "code": " \
        6A 48 \
        E8 [codecave:Aun2CardCalculate] \
      "
    },
    "HisamiAuraAun2Multiplier": {
      "addr": "Rx27C9D",
      "code": " \
        6A 4C \
        E8 [codecave:Aun2CardCalculate] \
      "
    },
    "ZanmuAura1Aun2Multiplier": {
      "addr": "Rx290ED",
      "code": " \
        6A 50 \
        E8 [codecave:Aun2CardCalculate] \
      "
    },
    "ZanmuAura2Aun2Multiplier": {
      "addr": "Rx2915F",
      "code": "\
        68 54000000 \
        E8 [codecave:Aun2CardCalculateB] \
      "
    },

    "BuffHisamiCard2": {
      "addr": "RxE349B",
      "code": "<codecave:BuffHisamiCard2Cave>"
    },

    "NerfZanmuCard2A": {
      "addr": "RxD83FF",
      "code": " \
        E9 [codecave:NerfZanmuCard2ACave] CC \
        90 \
      "
    },
    "NerfZanmuCard2B": {
      "addr": "RxD848C",
      "code": " \
        E9 [codecave:NerfZanmuCard2BCave] \
      "
    }
  },

  "codecaves": {
    "ReimuC2NoC1Cave": {
      "code": "\
        8B4A 64 \
        85C9 \
        74 14 \
        0F1F4000 \
        8B09 \
        8B09 \
        81F9 <Rx185DF8> \
        0F84 [Rx1276D8] \
        8B8A 98000000 \
        E9 [Rx12767B] \
      "
    },
    "ReimuC2ShootAndChargeDelayCounters": {
      "access": "RW",
      "code": "\
        FFFFFFFF \
        FFFFFFFF \
      "
    },
    "ReimuC2ShootAndChargeDelayACave": {
      "code": "\
        31C0 \
        A3 <codecave:ReimuC2ShootAndChargeDelayCounters> \
        A3 (<codecave:ReimuC2ShootAndChargeDelayCounters+4>) \
        894E 08 \
        E9 [Rx12762F] \
      "
    },
    "ReimuC2ShootAndChargeDelayBCave": {
      "code": "\
        A1 <codecave:ReimuC2ShootAndChargeDelayCounters> \
        85C0 \
        78 3E \
        0F1F4000 \
        40 \
        A3 <codecave:ReimuC2ShootAndChargeDelayCounters> \
        83E0 01 \
        75 31 \
        0F1F4000 \
        A1 (<codecave:ReimuC2ShootAndChargeDelayCounters+4>) \
        83F8 38 /* Delay Frames */ \
        7D 14 \
        0F1F4000 \
        40 \
        A3 (<codecave:ReimuC2ShootAndChargeDelayCounters+4>) \
        B8 01000000 \
        EB 12 \
        0F1F00 \
        31C0 \
        48 \
        A3 <codecave:ReimuC2ShootAndChargeDelayCounters> \
        A3 (<codecave:ReimuC2ShootAndChargeDelayCounters+4>) \
        31C0 \
        C3 \
      "
    },

    "ToutetsuEraseBulletsCave": {
      "code": " \
        50 \
        51 \
        52 \
        6A 01 \
        6A 02 \
        6A 00 \
        6A 01 \
        57 \
        B8 D0000000 \
        FF3430 \
        0F28D8 \
        E8 [Rx10AA10] \
        5A \
        59 \
        58 \
        68 9F860100 \
        E9 [Rx21E0E] \
      "
    },
    "ToutetsuC1EtBreakCave": {
      "access": "re",
      "code": " \
        83EC 54 \
        0F57C0 \
        89CE \
        F20F114424 4C \
        0F114424 3C \
        0F114424 2C \
        0F114424 1C \
        0F114424 0C \
        8B45 08 \
        8B50 08 \
        895424 08 \
        F20F1100 \
        F20F110424 \
        8B87 D0000000 \
        8B48 08 \
        89E2 \
        52 \
        68 <option:ToutetsuC1EtBreakSubName> \
        E8 [RxFB510] \
        83C4 54 \
        89F1 \
        E9 [RxAEB20] \
      "
    },

    "NerfZanmuCard2ACave": {
      "code": " \
        0F84 [RxD8429] \
        8B83 94000000 \
        3B41 30 \
        0F8C [RxD8429] \
        E8 [RxA5E0] \
        E9 [RxD8406] \
      "
    },
    "NerfZanmuCard2BCave": {
      "code": "\
        56 \
        8B4DFC \
        8BB1 94000000 \
        3B73 30 \
        5E \
        7C 14 \
        0F1F4000 \
        80BB B5000000 00 \
        74 07 \
        0F1F4000 \
        83C6 04 \
        83C6 2C \
        8B0E \
        E9 [RxD8491] \
      "
    },

    "Aun2CardMultipliers": {
      "access": "r",
      "code": "<option:ReimuAuraAun2Base><option:MarisaAuraAun2Base><option:SanaeAuraAun2Base><option:RanAuraAun2Base><option:AunAuraAun2Base><option:NazrinAuraAun2Base><option:SeiranAuraAun2Base><option:OrinAura1Aun2Base><option:OrinAura2Aun2Base><option:TsukasaAuraAun2Base><option:MamizouAuraAun2Base><option:YachieAuraAun2Base><option:SakiAura1Aun2Base><option:SakiAura2Aun2Base><option:ToutetsuAuraAun2Base><option:SuikaAuraAun2Base><option:BitenAuraAun2Base><option:EnokoAuraAun2Base><option:ChiyariAuraAun2Base><option:HisamiAuraAun2Base><option:ZanmuAura1Aun2Base><option:ZanmuAura2Aun2Base>"
    },
    "Aun2CardCalculate": {
      "access": "re",
      "code": " \
        8B5424 04 \
        8B47 68 \
        8B40 2C \
        F30F2A80 88000000 \
        0F57C9 \
        F30F5FC1 \
        F30F5D05 {f32:200.0f} \
        F30F5982 <codecave:Aun2CardMultipliers> \
        F30F1147 70 \
        C2 0400 \
      "
    },
    "Aun2CardCalculateB": {
      "access": "re",
      "code": " \
        8B5424 04 \
        8B47 68 \
        8B40 2C \
        F30F2A80 88000000 \
        0F57C9 \
        F30F5FC1 \
        F30F5D05 {f32:200.0f} \
        F30F5982 <codecave:Aun2CardMultipliers> \
        F30F1187 A0000000 \
        C2 0400 \
      "
    },
    "Aun2CardCalculateOrin2": {
      "access": "re",
      "code": " \
        8B46 68 \
        8B40 2C \
        F30F2A80 88000000 \
        0F57C9 \
        F30F5FC1 \
        F30F5D05 {f32:200.0f} \
        F30F5905 (<codecave:Aun2CardMultipliers>+32) \
        F30F1146 70 \
        C3 \
      "
    },
    "Aun2CardCalculateSuika": {
      "access": "re",
      "code": " \
        8B43 68 \
        8B40 2C \
        F30F2A80 88000000 \
        0F57C9 \
        F30F5FC1 \
        F30F5D05 {f32:200.0f} \
        F30F5905 (<codecave:Aun2CardMultipliers>+60) \
        F30F1143 70 \
        C3 \
      "
    },
    "BuffHisamiCard2Cave": {
      "code": "<option:BuffHisamiCard2Mul>"
    }
  }
}