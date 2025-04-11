{
    "codecaves": {
        "score_upper": {
            "access": "rw",
            "size": 12
        },
        "update_high_score": {
            "access": "re",
            "code": "\
57 \
56 \
8B15 <codecave:score_upper> \
8B0D C0CC4C00 \
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
        "update_gui_score_and_high_score": {
            "access": "re",
            "code": "\
57 \
56 \
A1 E0F24C00 \
8B0D FCCC4C00 \
8B15 <codecave:score_upper> \
8988 58010000 \
8915 (<codecave:score_upper>+8) \
A1 C0CC4C00 \
8B35 (<codecave:score_upper>+4) \
39C8 \
89F7 \
19D7 \
0F42F2 \
0F42C1 \
A3 C0CC4C00 \
8935 (<codecave:score_upper>+4) \
5E \
5F \
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
        "set_high_score_to_eax": {
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
        },
        "print_score_bigger": {
            "access": "re",
            "code": "\
55 \
53 \
57 \
56 \
83EC 30 \
8B5424 44 \
8B5C24 48 \
0FB64424 4C \
31C9 \
817A 04 00008042 \
0F94C1 \
8B2C8D (<codecave:score_upper>+4) \
C64424 2F 00 \
04 30 \
8D7C24 2E \
884424 2E \
89D8 \
09E8 \
0F84 C3000000 \
C64424 03 02 \
EB 3B \
<int3:9> \
8B7424 04 \
69C9 CCCCCCCC \
01CA \
69ED CDCCCCCC \
01D5 \
BA 09000000 \
3B5424 0C \
BA 00000000 \
19F2 \
8B5C24 08 \
89C6 \
89C7 \
0F83 83000000 \
896C24 04 \
89D8 \
D1E8 \
89E9 \
C1E1 1F \
09C1 \
D1ED \
89CE \
01EE \
83D6 00 \
89F0 \
BA CDCCCCCC \
F7E2 \
C1EA 02 \
8D0492 \
29C6 \
29F1 \
83DD 00 \
89C8 \
BA CDCCCCCC \
F7E2 \
89DE \
894424 08 \
69D8 F6000000 \
01F3 \
80CB 30 \
885F FF \
8D47 FF \
FE4C24 03 \
897424 0C \
0F85 72FFFFFF \
83FE 0A \
8B7424 04 \
89F3 \
83DB 00 \
0F82 64FFFFFF \
C647 FE 2C \
83C7 FE \
C64424 03 03 \
89F8 \
E9 51FFFFFF \
89FE \
8B3D ACF24C00 \
8B9F 44920100 \
83FB 06 \
74 15 \
83FB 0A \
8B5424 44 \
74 17 \
83FB 07 \
75 48 \
B8 09000000 \
EB 10 \
B8 08000000 \
8B5424 44 \
EB 05 \
B8 0B000000 \
8BAF 28920100 \
8B8F 2C920100 \
898F 28920100 \
8987 44920100 \
89F9 \
56 \
52 \
E8 [0x419B90] \
8B5424 44 \
899F 44920100 \
89AF 28920100 \
89F9 \
56 \
52 \
E8 [0x419B90] \
83C4 30 \
5E \
5F \
5B \
5D \
C2 0C00 \
"
        },
        "whatever_dang_gui_score_logic_in_the_tick_loop": {
            "access": "re",
            "code": "\
55 \
53 \
57 \
56 \
83EC 08 \
8B15 E0F24C00 \
8B3D FCCC4C00 \
8B1D <codecave:score_upper> \
8B2D (<codecave:score_upper>+8) \
8BB2 58010000 \
89D8 \
89F9 \
31E8 \
31F1 \
09C1 \
75 09 \
89F8 \
89DE \
E9 84000000 \
89F8 \
893424 \
896C24 04 \
29F0 \
89DE \
19EE \
89C5 \
89F1 \
C1ED 05 \
C1E1 1B \
09E9 \
BD 5ED50800 \
81F9 5ED50800 \
0F43CD \
F7C6 E0FFFFFF \
0F45CD \
8BAA 5C010000 \
83F9 01 \
83D1 00 \
39CD \
73 08 \
89CD \
898A 5C010000 \
39E8 \
89F1 \
83D9 00 \
73 08 \
8982 5C010000 \
EB 04 \
31F6 \
89E8 \
030424 \
137424 04 \
39F8 \
8982 58010000 \
89F1 \
8935 (<codecave:score_upper>+8) \
19D9 \
72 0A \
C782 5C010000 00000000 \
8B0D (<codecave:score_upper>+4) \
3905 C0CC4C00 \
19F1 \
73 1E \
8B0D 04CD4C00 \
800D C8CC4C00 04 \
A3 C0CC4C00 \
8935 (<codecave:score_upper>+4) \
890D C4CC4C00 \
83C4 08 \
5E \
5F \
5B \
5D \
C3 \
"
        }
    },
    "binhacks": {
        "update_high_score_scorecap": {
            "addr": 0x442A68,
            "code": "\
E8 [codecave:update_high_score] \
<nop:20> \
"
        },
        "whatever_dang_gui_score_logic_in_the_tick_loop": {
            "addr": 0x443A61,
            "code": "\
E8 [codecave:whatever_dang_gui_score_logic_in_the_tick_loop] \
<nop:153> \
"
        },
        "reset_upper_gui_score": {
            "addr": 0x43BA04,
            "code": "\
C705 (<codecave:score_upper>+8) 00000000 \
<nop:2> \
"
        },
        "set_high_score_to_eax": {
            "addr": [ 0x4426A7, 0x4426E4, 0x442711 ],
            "code": "E8 [codecave:set_high_score_to_eax]"
        },
        "copy_score_to_gui_field": {
            "addr": 0x43A829,
            "code": "E8 [codecave:copy_score_to_gui_field]"
        },
        "update_gui_score_and_high_score": {
            "addr": 0x459006,
            "code": "\
E8 [codecave:update_gui_score_and_high_score] \
<nop:0x1B> \
"
        },
        "print_score_to_gui": {
            "addr": [ 0x43D172, 0x43D1C8 ],
            "code": "E8 [codecave:print_score_bigger]"
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
            "addr": [ 0x45788C, 0x4578E9, 0x4620B4 ],
            "code": "E8 [codecave:get_score_as_32_bit_eax]"
        },
        "get_score_as_32_bit_edx": {
            "addr": 0x459045,
            "code": "E8 [codecave:get_score_as_32_bit_edx] 90"
        }
    }
}