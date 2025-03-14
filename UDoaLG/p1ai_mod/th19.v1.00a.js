{
  "binhacks": {
    "NoAutoAttacks": {
      "addr": "RxA510",
      "code": "C2 0400 CC"
    },
    "JumpFromBossAttack": {
      "addr": "RxD7E3F",
      "code": "\
31C0 \
8943 64 \
3943 18 \
0F8E 88090000 \
8BB3 CC000000 \
8B7E 2C \
F30F7E47 2C \
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
660FD647 2C \
8B93 94000000 \
3B57 30 \
0F8C BB040000 \
8B46 38 \
8B48 18 \
8B11 \
FF52 24 \
84C0 \
0F85 03050000 \
E9 [codecave:P2BossAttackToAutoAttack] \
89F9 \
"
    },
    "JumpFromEXAttack": {
      "addr": "RxD8356",
      "code": "\
660F7EC0 \
39D0 \
0F8D E8010000 \
81FE <Rx1AE4AC> \
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
A1 <Rx2082C0> \
0FB688 (<codecave:AutoAttackTable>-1) \
8B56 04 \
0FA3D1 \
73 16 \
8146 08 E8030000 \
8B47 2C \
2B47 24 \
2947 20 \
E9 B6020000 \
CC \
A1 <Rx207A94> \
83F8 04 \
72 0F \
C1F8 1F \
F7D0 \
83E0 03 \
A3 <Rx207A94> \
EB 14 \
85C0 \
"
    },

    "P2StoryMeterRate": {
      "addr": [ "RxEADA9", "RxEADBD", "RxEAE36", "RxEAFDA" ],
      "code": "A0000000"
    },

    // P2 Meter Rate Uncap
    "P2StoryMeterRateUnCap1": {
      "ignore": true,
      "addr": "RxEB00B",
      "code": "6690"
    },
    "P2StoryMeterRateUnCap2": {
      "addr": "Rx10EA53",
      "code": "8BC2 90"
    },

    "StoryP1Input": {
      "addr": "Rx10F890",
      "code": "00000000"
    },
    
    // Toggle P1 AI
    "StoryP2Input": {
      "enable": "<option:P1AI>",
      "addr": "Rx10F89A",
      "code": "02000000"
    },
    "StoryP1AI": {
      "enable": "<option:P1AI>",
      "addr": "Rx141532",
      "code": "01000000"
    },
    
    // Toggle P1 No AI
    "StoryNoAIInput": {
      "ignore": "<option:P1AI>",
      "addr": "RxE998F",
      "code": "<nop:6>"
    },
    "StoryAllowPlayerInput": {
      "ignore": "<option:P1AI>",
      "addr": "RxEAC42",
      "code": "<nop:10>"
    },

    // Vs AI Swap
    "VsP1AI": {
      "addr": "Rx1414FC",
      "code": "01"
    },
    "VsP2Input": {
      "addr": "Rx141506",
      "code": "00"
    }
  },

  "codecaves": {
    "P2BossAttackToAutoAttack": {
      "access": "re",
      "code": "\
81FE <Rx1AE4AC> \
0F85 [RxD7EB3] \
8B4E 34 \
85C9 \
0F84 [RxD7EB3] \
8B51 34 \
85D2 \
0F84 [RxD7EB3] \
A1 <Rx2082C0> \
0FB688 (<codecave:AutoAttackTable>-1) \
8B42 04 \
0FA3C1 \
0F82 [RxD7EB3] \
8142 08 E8030000 \
8B57 30 \
2B57 24 \
2957 20 \
89F0 \
E9 [RxD866C] \
"
    }
  }
}