{
	"options": {
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
			"val": 0.32
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
			"val": 0.28
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
		}
	},
	"codecaves": {
		"Aun2CardMultipliers": {
			"access": "r",
			"code": "<option:ReimuAuraAun2Base><option:MarisaAuraAun2Base><option:SanaeAuraAun2Base><option:RanAuraAun2Base><option:AunAuraAun2Base><option:NazrinAuraAun2Base><option:SeiranAuraAun2Base><option:OrinAura1Aun2Base><option:OrinAura2Aun2Base><option:TsukasaAuraAun2Base><option:MamizouAuraAun2Base><option:YachieAuraAun2Base><option:SakiAura1Aun2Base><option:SakiAura2Aun2Base><option:ToutetsuAuraAun2Base><option:SuikaAuraAun2Base><option:BitenAuraAun2Base><option:EnokoAuraAun2Base><option:ChiyariAuraAun2Base><option:HisamiAuraAun2Base><option:ZanmuAura1Aun2Base><option:ZanmuAura2Aun2Base>"
		},
		"Aun2CardCalculate": {
			"access": "re",
			"code": "\
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
			"code": "\
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
			"code": "\
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
			"code": "\
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
		}
	},
	"binhacks": {
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
			"code": "\
6A 08 \
E8 [codecave:Aun2CardCalculate] \
<nop:57> \
"
		},
		"RanAuraAun2Multiplier": {
			"addr": "Rx173BD",
			"code": "\
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
			"code": "\
6A 1C \
E8 [codecave:Aun2CardCalculate] \
"
		},
		"OrinAura2Aun2Multiplier": {
			"addr": "Rx1A255",
			"code": "\
E8 [codecave:Aun2CardCalculateOrin2] \
<nop:2> \
"
		},
		"TsukasaAuraAun2Multiplier": {
			"addr": "Rx1B48D",
			"code": "\
6A 24 \
E8 [codecave:Aun2CardCalculate] \
"
		},
		"MamizouAuraAun2Multiplier": {
			"addr": "Rx1CBB3",
			"code": "\
6A 28 \
E8 [codecave:Aun2CardCalculate] \
"
		},
		"YachieAuraAun2Multiplier": {
			"addr": "Rx1E3BD",
			"code": "\
6A 2C \
E8 [codecave:Aun2CardCalculate] \
"
		},
		"SakiAura1Aun2Multiplier": {
			"addr": "Rx1FF75",
			"code": "\
6A 30 \
E8 [codecave:Aun2CardCalculate] \
"
		},
		"SakiAura2Aun2Multiplier": {
			"addr": "Rx1FF00",
			"code": "\
68 34000000 \
E8 [codecave:Aun2CardCalculateB] \
"
		},
		"ToutetsuAuraAun2Multiplier": {
			"addr": "Rx2137D",
			"code": "\
6A 38 \
E8 [codecave:Aun2CardCalculate] \
"
		},
		"SuikaAuraAun2Multiplier": {
			"addr": "Rx2294A",
			"code": "\
E8 [codecave:Aun2CardCalculateSuika] \
<nop:2> \
"
		},
		"BitenAuraAun2Multiplier": {
			"addr": "Rx23CBD",
			"code": "\
6A 40 \
E8 [codecave:Aun2CardCalculate] \
"
		},
		"EnokoAuraAun2Multiplier": {
			"addr": "Rx257F3",
			"code": "\
6A 44 \
E8 [codecave:Aun2CardCalculate] \
"
		},
		"ChiyariAuraAun2Multiplier": {
			"addr": "Rx26B8D",
			"code": "\
6A 48 \
E8 [codecave:Aun2CardCalculate] \
"
		},
		"HisamiAuraAun2Multiplier": {
			"addr": "Rx27C9D",
			"code": "\
6A 4C \
E8 [codecave:Aun2CardCalculate] \
"
		},
		"ZanmuAura1Aun2Multiplier": {
			"addr": "Rx290ED",
			"code": "\
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
		}
	}
}