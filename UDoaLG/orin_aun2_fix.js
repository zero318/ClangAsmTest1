{
	"binhacks": {
		"orin_aun2_fixA": {
			"addr": "Rx1C7A6",
			"code": "\
6B46 64 32 \
F30F2A0485 <Rx22B14C> \
90 \
"
		},
		"orin_aun2_fixB": {
			"addr": "Rx1C7F6",
			"code": "\
F20F100D {x.u32:0x3F800000;0xFFFFFFFF} \
F30F5905 {u32:0x3F000000} \
F30F1146 70 \
0F114E 7C \
<nop:5> \
"
		}
	}
}