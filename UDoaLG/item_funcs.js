{
	"options": {
		"ReimuLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"MarisaLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"SanaeLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"RanLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"AunnLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"NazrinLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"SeiranLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"OrinLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"TsukasaLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"MamizouLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"YachieLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"SakiLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"YuumaLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"SuikaLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"BitenLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"EnokoLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"ChiyariLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"HisamiLifeFragments": {
			"type": "u32",
			"val": 3
		},
		"ZanmuLifeFragments": {
			"type": "u32",
			"val": 3
		},
		
		"ReimuBombFragments": {
			"type": "u32",
			"val": 3
		},
		"MarisaBombFragments": {
			"type": "u32",
			"val": 3
		},
		"SanaeBombFragments": {
			"type": "u32",
			"val": 3
		},
		"RanBombFragments": {
			"type": "u32",
			"val": 3
		},
		"AunnBombFragments": {
			"type": "u32",
			"val": 3
		},
		"NazrinBombFragments": {
			"type": "u32",
			"val": 3
		},
		"SeiranBombFragments": {
			"type": "u32",
			"val": 3
		},
		"OrinBombFragments": {
			"type": "u32",
			"val": 3
		},
		"TsukasaBombFragments": {
			"type": "u32",
			"val": 3
		},
		"MamizouBombFragments": {
			"type": "u32",
			"val": 3
		},
		"YachieBombFragments": {
			"type": "u32",
			"val": 3
		},
		"SakiBombFragments": {
			"type": "u32",
			"val": 3
		},
		"YuumaBombFragments": {
			"type": "u32",
			"val": 3
		},
		"SuikaBombFragments": {
			"type": "u32",
			"val": 3
		},
		"BitenBombFragments": {
			"type": "u32",
			"val": 3
		},
		"EnokoBombFragments": {
			"type": "u32",
			"val": 3
		},
		"ChiyariBombFragments": {
			"type": "u32",
			"val": 3
		},
		"HisamiBombFragments": {
			"type": "u32",
			"val": 3
		},
		"ZanmuBombFragments": {
			"type": "u32",
			"val": 3
		}
	},
	"codecaves": {
		"life_pieces_for_life": {
			"access": "r",
			"code": "<option:ReimuLifeFragments><option:MarisaLifeFragments><option:SanaeLifeFragments><option:RanLifeFragments><option:AunnLifeFragments><option:NazrinLifeFragments><option:SeiranLifeFragments><option:OrinLifeFragments><option:TsukasaLifeFragments><option:MamizouLifeFragments><option:YachieLifeFragments><option:SakiLifeFragments><option:YuumaLifeFragments><option:SuikaLifeFragments><option:BitenLifeFragments><option:EnokoLifeFragments><option:ChiyariLifeFragments><option:HisamiLifeFragments><option:ZanmuLifeFragments>"
		},
		"bomb_pieces_for_bomb": {
			"access": "r",
			"code": "<option:ReimuBombFragments><option:MarisaBombFragments><option:SanaeBombFragments><option:RanBombFragments><option:AunnBombFragments><option:NazrinBombFragments><option:SeiranBombFragments><option:OrinBombFragments><option:TsukasaBombFragments><option:MamizouBombFragments><option:YachieBombFragments><option:SakiBombFragments><option:YuumaBombFragments><option:SuikaBombFragments><option:BitenBombFragments><option:EnokoBombFragments><option:ChiyariBombFragments><option:HisamiBombFragments><option:ZanmuBombFragments>"
		},
		"globals_side_add_life_fragments": {
			"access": "re",
			"code": "\
53 \
57 \
56 \
8B79 50 \
8B71 54 \
39F7 \
7D 21 \
0341 58 \
8B59 0C \
31D2 \
F7349D <codecave:life_pieces_for_life> \
31DB \
01F8 \
39F0 \
0F4DC6 \
0F4DD3 \
8941 50 \
8951 58 \
5E \
5F \
5B \
C3 \
"
		},
		"globals_side_add_bomb_fragments": {
			"access": "re",
			"code": "\
53 \
57 \
56 \
8B79 64 \
8B71 70 \
39F7 \
73 21 \
0341 68 \
8B59 0C \
31D2 \
F7349D <codecave:bomb_pieces_for_bomb> \
31DB \
01F8 \
39F0 \
0F43C6 \
0F43D3 \
8941 64 \
8951 68 \
5E \
5F \
5B \
C3 \
"
		},
		"globals_side_sub_life_fragments": {
			"access": "re",
			"code": "\
57 \
56 \
89C2 \
8B71 0C \
8B41 58 \
8B34B5 <codecave:life_pieces_for_life> \
8B79 50 \
0FAFFE \
29D0 \
31D2 \
01F8 \
78 04 \
F7F6 \
EB 05 \
B8 FFFFFFFF \
8951 58 \
8941 50 \
5E \
5F \
C3 \
"
		},
		"globals_side_sub_bomb_fragments": {
			"access": "re",
			"code": "\
57 \
56 \
89C2 \
8B41 0C \
8B3C85 <codecave:bomb_pieces_for_bomb> \
8B71 64 \
0FAFF7 \
0371 68 \
31C0 \
29D6 \
BA 00000000 \
72 04 \
89F0 \
F7F7 \
8941 64 \
8951 68 \
5E \
5F \
C3 \
"
		}
	},
	"binhacks": {
		"add_bomb_fragment_check_A": {
			"addr": "Rx124247",
			"code": "\
56 \
57 \
89CB \
8B53 70 \
31C0 \
85D2 \
0F48D0 \
B8 07000000 \
39C2 \
0F4EC2 \
8943 70 \
8B73 68 \
46 \
8B7B 0C \
<nop:11> \
"
		},
		"add_bomb_fragment_check_B": {
			"addr": "Rx124297",
			"code": "\
8973 68 \
3B34BD <codecave:bomb_pieces_for_bomb> \
"
		}
	}
}