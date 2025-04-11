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
83EC 34 \
8B7424 48 \
8B4C24 4C \
0FB64424 50 \
8B15 ACF24C00 \
895424 0C \
31D2 \
817E 04 00008042 \
0F94C2 \
8B1C95 (<codecave:score_upper>+4) \
C64424 33 00 \
04 30 \
884424 32 \
89C8 \
D1E8 \
89DA \
C1E2 1F \
09C2 \
89DF \
D1EF \
01D7 \
83D7 00 \
BA CDCCCCCC \
89F8 \
F7E2 \
C1EA 02 \
8D0492 \
29C7 \
01FF \
89C8 \
83E0 01 \
09F8 \
8D7424 31 \
0F84 E1000000 \
894424 08 \
B0 01 \
890424 \
EB 54 \
CC \
69C7 CCCCCCCC \
01C2 \
69DD CDCCCCCC \
01D3 \
8B4C24 10 \
89C8 \
D1E8 \
89DA \
C1E2 1F \
09C2 \
89DF \
D1EF \
01D7 \
83D7 00 \
89F8 \
BA CDCCCCCC \
F7E2 \
C1EA 02 \
8D0492 \
29C7 \
89C8 \
83E0 01 \
8D0478 \
8B7C24 04 \
89FE \
894424 08 \
85C0 \
0F84 84000000 \
89C8 \
894C24 04 \
D1E8 \
89DF \
C1E7 1F \
09C7 \
BA CDCCCCCC \
89DD \
D1ED \
89F9 \
01E9 \
83D1 00 \
89C8 \
F7E2 \
C1EA 02 \
8D0492 \
29C1 \
29CF \
83DD 00 \
8B4424 08 \
0C 30 \
8806 \
89F8 \
B9 CDCCCCCC \
F7E1 \
8B0C24 \
FEC1 \
837C24 04 0A \
83DB 00 \
8D5E FF \
895C24 04 \
894424 10 \
890C24 \
0F82 4BFFFFFF \
80F9 03 \
0F85 42FFFFFF \
C646 FF 2C \
83C6 FE \
C70424 00000000 \
897424 04 \
E9 2BFFFFFF \
89F7 \
47 \
8B6C24 0C \
8BB5 44920100 \
83FE 06 \
74 15 \
83FE 0A \
8B5C24 48 \
74 17 \
83FE 07 \
75 4A \
B8 09000000 \
EB 10 \
B8 08000000 \
8B5C24 48 \
EB 05 \
B8 0B000000 \
8B8D 28920100 \
890C24 \
8B8D 2C920100 \
898D 28920100 \
8985 44920100 \
89E9 \
57 \
53 \
E8 [0x419B90] \
89B5 44920100 \
8B0424 \
8985 28920100 \
89E9 \
57 \
53 \
E8 [0x419B90] \
83C4 34 \
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