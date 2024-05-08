{
	"options": {
		"ToutetsuC1EtBreakSubName": {
			"type": "s",
			"val": "Ecl_EtBreakLv1"
		}
	},
	"codecaves": {
		"ToutetsuC1EtBreakCave": {
			"access": "re",
			"code": "\
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
F20F1000 \
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
		}
	},
	"binhacks": {
		"ToutetsuC1EtBreak": {
			"addr": "Rx21ACD",
			"code": "E8 [codecave:ToutetsuC1EtBreakCave]"
		}
	}
}