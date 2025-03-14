{
  "codecaves": {
    "AutoAttackTable": {
      "access": "r",
      "code": "03030B0B2D39"
    },
    "P2BossAttackToAutoAttack": {
      "access": "re",
      "code": "\
81FE <Rx1AE4AC> \
0F85 [RxE7B28] \
8B4E 34 \
85C9 \
0F84 [RxE7B28] \
8B51 34 \
85D2 \
0F84 [RxE7B28] \
A1 <Rx22BA80> \
0FB688 (<codecave:AutoAttackTable>-1) \
8B42 04 \
0FA3C1 \
0F82 [RxE7B28] \
8142 08 E8030000 \
8B57 34 \
2B57 28 \
2957 24 \
89F0 \
E9 [RxE83EE] \
"
    }
  },
  "binhacks": {
    "NoAutoAttacks": {
      "addr": "RxB500",
      "code": "C2 0400 CC"
    },
    "P2StoryMeterRate": {
      "addr": [ "RxFB9C0", "RxFB9D1", "RxFB9E2", "RxFB9F3", "RxFBA72", "RxFBA83", "RxFBA94", "RxFBAA5", "RxFBB24", "RxFBB35", "RxFBB46", "RxFBB57", "RxFBBD6", "RxFBBE7", "RxFBBF8", "RxFBC09", "RxFBC88", "RxFBC99", "RxFBCAA", "RxFBCBB", "RxFBD15", "RxFBD26", "RxFBD37", "RxFBD48" ],
      "code": "A0000000"
    },
    "P2StoryMeterRateUnCap": {
      "addr": "Rx121577",
      "code": "8BC2 90"
    },
    "StoryP1Input": {
      "addr": "Rx1222D5",
      "code": "00000000"
    },
    "StoryP2Input": {
      "enable": "<option:P1AI>",
      "addr": "Rx1222DF",
      "code": "02000000"
    },
    "StoryP1AI": {
      "enable": "<option:P1AI>",
      "addr": "Rx15840B",
      "code": "01000000"
    },
    "StoryNoAIInput": {
      "enable": "!<option:P1AI>",
      "addr": "RxFA3A0",
      "code": "<nop:6>"
    },
    "StoryAllowPlayerInput": {
      "enable": "!<option:P1AI>",
      "addr": "RxFB84E",
      "code": "<nop:10>"
    },
    "JumpFromBossAttack": {
      "addr": "RxE7AB4",
      "code": "\
31C0 \
8943 64 \
3943 18 \
0F8E BB0A0000 \
8BB3 CC000000 \
8B7E 2C \
F30F7E47 30 \
F30F7E1D {i32:2500;2500} \
660FEFC9 \
660F6FD0 \
660F66D1 \
660FDBD0 \
660F6FC3 \
660F66C2 \
660FDBD0 \
660FDFC3 \
660FEBC2 \
660FD647 30 \
8B93 94000000 \
3B57 30 \
0F8C D2050000 \
8B46 38 \
8B48 18 \
8B11 \
FF52 24 \
84C0 \
0F85 1A060000 \
E9 [codecave:P2BossAttackToAutoAttack] \
89F9 \
"
    },
    "JumpFromEXAttack": {
      "addr": "RxE80E2",
      "code": "\
660F7EC0 \
39D0 \
0F8D E0010000 \
81FE <Rx1D1A9C> \
75 74 \
8B56 34 \
85D2 \
74 40 \
8B46 38 \
8B72 34 \
85F6 \
74 36 \
8B48 18 \
8B11 \
FF52 24 \
84C0 \
75 2A \
A1 <Rx22BA80> \
0FB688 (<codecave:AutoAttackTable>-1) \
8B56 04 \
0FA3D1 \
73 16 \
8146 08 E8030000 \
8B47 30 \
2B47 28 \
2947 24 \
E9 AC020000 \
CC \
A1 <Rx22B254> \
83F8 04 \
72 0F \
C1F8 1F \
F7D0 \
83E0 03 \
A3 <Rx22B254> \
EB 14 \
85C0 \
"
    }
  }
}