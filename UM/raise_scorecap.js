{
    "codecaves": {
        "score_upper": {
            "access": "rw",
            "size": 12
        },
        "update_display_score": {
            "access": "re",
            "code": "\
57 \
56 \
8B15 <codecave:score_upper> \
8B0D 0CCC4C00 \
8B35 FCCC4C00 \
A1 (<codecave:score_upper>+4) \
39CE \
89D7 \
19C7 \
0F42D0 \
0F42F1 \
8935 C0CC4C00 \
8915 (<codecave:score_upper>+4) \
5E \
5F \
C3 \
"
        },
        "get_score_as_32_bit_eax": {
            "access": "re",
            "code": "\
31C0 \
3B05 <codecave:score_upper> \
19C0 \
0B05 FCCC4C00 \
C3 \
"
        },
        "copy_score_to_gui_field": {
            "access": "re",
            "code": "\
A1 <codecave:score_upper> \
A3 (<codecave:score_upper>+8) \
A1 FCCC4C00 \
C3 \
"
        },
        "get_score_as_32_bit_ecx": {
            "access": "re",
            "code": "\
31C9 \
3B0D <codecave:score_upper> \
19C9 \
0B0D FCCC4C00 \
C3 \
"
        },
        "get_score_as_32_bit_edx": {
            "access": "re",
            "code": "\
31D2 \
3B15 <codecave:score_upper> \
19D2 \
0B15 FCCC4C00 \
C3 \
"
        },
        "clear_score_to_zero": {
            "access": "re",
            "code": "\
C705 FCCC4C00 00000000 \
C705 <codecave:score_upper> 00000000 \
C3 \
"
        },
        "set_display_score_to_eax": {
            "access": "re",
            "code": "\
A3 C0CC4C00 \
C705 (<codecave:score_upper>+4) 00000000 \
C3 \
"
        },
        "current_score_div_by_10": {
            "access": "re",
            "code": "\
53 \
57 \
56 \
51 \
8B35 <codecave:score_upper> \
8B3D FCCC4C00 \
BA CDCCCCCC \
89F8 \
89F3 \
89F1 \
D1E8 \
C1E3 1F \
D1E9 \
09C3 \
01D9 \
83D1 00 \
89C8 \
F7E2 \
C1EA 02 \
8D0492 \
29C8 \
01D8 \
69C8 CDCCCCCC \
83FF F6 \
B8 FFFFFFFF \
83DE 09 \
0F42C1 \
59 \
5E \
5F \
5B \
C3 \
"
        }
    },
    // TODO: The mess at 0x443A67, 0x459017
    "binhacks": {
        "update_display_score_scorecap": {
            "addr": 0x442A68,
            "code": "\
E8 [codecave:update_display_score] \
<nop:0x18> \
"
        },
        "set_display_score_to_eax": {
            "addr": [ 0x4426A7, 0x4426E4, 0x442711 ],
            "code": "E8 [codecave:set_display_score_to_eax]"
        },
        "copy_score_to_gui_field": {
            "addr": 0x43A829,
            "code": "E8 [codecave:copy_score_to_gui_field]"
        },
        "spell_end_scorecap": {
            "addr": 0x42A7FC,
            "code": "\
8B77 7C \
F7E6 \
B8 FCCC4C00 \
B9 <codecave:score_upper> \
0110 \
8311 00 \
19D2 \
0910 \
0911 \
<nop:7> \
56 \
"
        },
        "score_add_instr_scorecap": {
            "addr": 0x430E9B,
            "code": "\
89C6 \
B8 CDCCCCCC \
F7E6 \
C1EA 03 \
A1 FCCC4C00 \
0315 <codecave:score_upper> \
83D0 00 \
19C9 \
09CA \
09C8 \
A3 FCCC4C00 \
8915 <codecave:score_upper> \
6A FF \
56 \
8D47 44 \
<nop:3> \
"
        },
        "collect_money_scorecapA": {
            "addr": 0x446D08,
            "code": "\
F7E6 \
C1EA 03 \
0115 FCCC4C00 \
8315 <codecave:score_upper> 00 \
19D2 \
0915 <codecave:score_upper> \
"
        },
        "collect_money_scorecapB": {
            "addr": 0x446D34,
            "code": "\
0915 FCCC4C00 \
5F \
5E \
C9 \
"
        },
        "collect_power_scorecap": {
            "addr": 0x446AC5,
            "code": "\
F7E7 \
C1EA 03 \
B8 <codecave:score_upper> \
B9 FCCC4C00 \
8B30 \
0311 \
83D6 00 \
19FF \
09FA \
09FE \
8930 \
8911 \
5F \
5E \
C9 \
"
        },
        "generic_scorecap4": {
            "addr": [ 0x44476A, 0x446568 ],
            "code": "\
C1EA 03 \
B8 FCCC4C00 \
B9 <codecave:score_upper> \
0110 \
8311 00 \
19D2 \
0910 \
0911 \
<nop:4> \
"
        },
        "collect_big_power_scorecapA": {
            "addr": 0x4462E5,
            "code": "\
C74424 10 204E0000 \
B8 FCCC4C00 \
BE <codecave:score_upper> \
8100 D0070000 \
8316 00 \
19D2 \
0910 \
0916 \
68 808080FF \
68 204E0000 \
51 \
<nop:3> \
"
        },
        "collect_big_power_scorecapB": {
            "addr": 0x44638D,
            "code": "\
8B4C24 10 \
E9 D0010000 \
<int3:0x24> \
"
        },
        "stage_end_scorecapA": {
            "addr": 0x444845,
            "code": "\
01F9 \
E8 [codecave:current_score_div_by_10] \
8B5424 10 \
<nop:14> \
"
        },
        "stage_end_scorecapB": {
            "addr": 0x444865,
            "code": "73"
        },
        "stage_end_scorecapC": {
            "addr": 0x444ACE,
            "code": "\
C1EA 03 \
B8 FCCC4C00 \
BF <codecave:score_upper> \
0110 \
8317 00 \
19D2 \
0910 \
0917 \
<nop:4> \
"
        },
        "stage_end_scorecapD": {
            "addr": 0x444ACE,
            "code": "\
C1EA 03 \
B8 FCCC4C00 \
BE <codecave:score_upper> \
0110 \
8316 00 \
19D2 \
0910 \
0916 \
<nop:4> \
"
        },
        "clear_score_to_zero": {
            "addr": 0x442750,
            "code": "\
E8 [codecave:clear_score_to_zero] \
90 \
"
        },
        "get_score_as_32_bit_eax": {
            "addr": [ 0x43A829, 0x45788C, 0x4578E9, 0x4620B4 ],
            "code": "E8 [codecave:get_score_as_32_bit_eax]"
        },
        "get_score_as_32_bit_ecx": {
            "addr": [ 0x45900B ],
            "code": "E8 [codecave:get_score_as_32_bit_ecx]"
        },
        "get_score_as_32_bit_edx": {
            "addr": [ 0x459045 ],
            "code": "E8 [codecave:get_score_as_32_bit_edx]"
        }
    }
}