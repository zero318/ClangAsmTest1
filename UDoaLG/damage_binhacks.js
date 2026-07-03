{
    "options": {
        "NazrinC2Damage": {
            // Vanilla: 1
            "type": "i32",
            "val": 1
        },
        "YachieExDamage": {
            // Vanilla: 1
            "type": "i32",
            "val": 1
        },
        "ToutetsuExDamage": {
            // Vanilla: 25
            "type": "i32",
            "val": 25
        },
        "ReimuBombDamage": {
            // Vanilla: 10
            "type": "i32",
            "val": 10
        },
        "MarisaBombDamage": {
            // Vanilla: 20
            "type": "i32",
            "val": 20
        },
        "SanaeBombDamage": {
            // Vanilla: 10
            "type": "i32",
            "val": 10
        },
        "RanBombDamage": {
            // Vanilla: 20
            "type": "i32",
            "val": 20
        },
        "AunBombDamage": {
            // Vanilla: 10
            "type": "i32",
            "val": 10
        },
        "NazrinBombDamage": {
            // Vanilla: 10
            "type": "i32",
            "val": 10
        },
        "SeiranBombDamage": {
            // Vanilla: 10
            "type": "i32",
            "val": 10
        },
        "OrinBombDamage": {
            // Vanilla: 20
            "type": "i32",
            "val": 20
        },
        "TsukasaBombDamage": {
            // Vanilla: 20
            "type": "i32",
            "val": 20
        },
        "MamizouBombDamage": {
            // Vanilla: 20
            "type": "i32",
            "val": 20
        },
        "YachieBombDamage": {
            // Vanilla: 20
            "type": "i32",
            "val": 20
        },
        "SakiBombDamage": {
            // Vanilla: 10
            "type": "i32",
            "val": 10
        },
        "ToutetsuBombDamage": {
            // Vanilla: 10
            "type": "i32",
            "val": 10
        },
        "SuikaBombDamage": {
            // Vanilla: 10
            "type": "i32",
            "val": 10
        },
        "BitenBombDamage": {
            // Vanilla: 10
            "type": "i32",
            "val": 10
        },
        "EnokoBombDamage": {
            // Vanilla: 10
            "type": "i32",
            "val": 10
        },
        "ChiyariBombDamage": {
            // Vanilla: 14
            "type": "i32",
            "val": 14
        },
        "HisamiBombDamage": {
            // Vanilla: 10
            "type": "i32",
            "val": 10
        },
        "ZanmuBombDamage": {
            // Vanilla: 10
            "type": "i32",
            "val": 10
        }
    },
    "binhacks": {
        "NazrinC2Damage": {
            "enable": "<option:NazrinC2Damage>!=1",
            "addr": "Rx1B4DB",
            "code": "\
68 <option:NazrinC2Damage>\
68 E7030000\
C701 00002042\
8D4D FC\
C742 04 28000000\
C702 27000000\
8B86 D0000000\
68 00000000\
"
        },
        "YachieExDamage": {
            "enable": "<option:YachieExDamage>!=1",
            "addr": [ "Rx22547", "Rx22654" ],
            "code": "\
68 <option:YachieExDamage>\
51 \
6A 06\
68 00000000\
"
        },
        "ToutetsuExDamage": {
            "enable": "<option:ToutetsuExDamage>!=25",
            "addr": "Rx257BF",
            "code": "\
68 <option:ToutetsuExDamage>\
6A 01\
68 00000000\
"
        },
        "ReimuBombDamage": {
            "enable": "<option:ReimuBombDamage>!=10",
            "addr": "Rx13C780",
            "code": "\
68 <option:ReimuBombDamage>\
6A 01\
68 00000000\
"
        },
        "MarisaBombDamage": {
            "enable": "<option:MarisaBombDamage>!=20",
            "addr": "Rx13DBFF",
            "code": "\
68 <option:MarisaBombDamage>\
6A 01\
68 00000000\
83EC 04\
"
        },
        "SanaeBombDamage": {
            "enable": "<option:SanaeBombDamage>!=10",
            "addr": "Rx183D5",
            "code": "\
68 <option:SanaeBombDamage>\
6A 01\
68 00000000\
"
        },
        "RanBombDamageA": {
            "enable": "<option:RanBombDamage>!=20",
            "addr": "Rx1925A",
            "code": "\
68 <option:RanBombDamage>\
6A 01\
68 00000000\
83EC 04\
"
        },
        "RanBombDamageB": {
            "enable": "<option:RanBombDamage>!=20",
            "addr": "Rx192EB",
            "code": "\
<nop:14>\
8D4D F8\
8B87 C4000000\
68 <option:RanBombDamage>\
6A 01\
68 DB0FC93F\
83EC 04\
F30F105D E4\
"
        },
        "AunBombDamageA": {
            "enable": "<option:AunBombDamage>!=10",
            "addr": "Rx13FB72",
            "code": "0F8C [Rx13FE25]"
        },
        "AunBombDamageB": {
            "enable": "<option:AunBombDamage>!=10",
            "addr": "",
            "code": "\
C780 98000000 <option:AunBombDamage>\
5F\
5E\
31C0\
5B\
C9\
C3\
"
        },
        "NazrinBombDamage": {
            "enable": "<option:NazrinBombDamage>!=10",
            "addr": "Rx140EB7",
            "code": "\
68 <option:NazrinBombDamage>\
6A 01\
68 00000000\
"
        },
        "SeiranBombDamage": {
            "enable": "<option:SeiranBombDamage>!=10",
            "addr": "Rx141D61",
            "code": "\
68 <option:SeiranBombDamage>\
6A 01\
68 00000000\
"
        },
        "OrinBombDamage": {
            "enable": "<option:OrinBombDamage>!=20",
            "addr": "Rx1C4CC",
            "code": "\
<nop:5>\
F30F105D E4\
8B48 30\
8D47 30\
68 <option:OrinBombDamage>\
6A 01\
FF77 4C\
83EC 04\
"
        },
        "TsukasaBombDamage": {
            "enable": "<option:TsukasaBombDamage>!=20",
            "addr": "Rx1DB36",
            "code": "\
<nop:5>\
8D4D F4\
F30F105D E0\
68 <option:TsukasaBombDamage>\
6A 01\
FF77 4C\
83EC 04\
"
        },
        "MamizouBombDamage": {
            "enable": "<option:MamizouBombDamage>!=20",
            "addr": "Rx1F37F",
            "code": "\
68 <option:MamizouBombDamage>\
6A 01\
68 00000000\
"
        },
        "YachieBombDamage": {
            "enable": "<option:YachieBombDamage>!=20",
            "addr": "Rx211B6",
            "code": "\
8B03\
F30F105D F0\
68 <option:YachieBombDamage>\
83EC 04\
68 06000000\
FFB7 FC000000\
"
        },
        "SakiBombDamageA": {
            "enable": "<option:SakiBombDamage>!=10",
            "addr": "Rx22D8D",
            "code": "0F8C [Rx231E6]"
        },
        "SakiBombDamageB": {
            "enable": "<option:SakiBombDamage>!=10",
            "addr": "",
            "code": "\
C780 98000000 <option:SakiBombDamage>\
5F\
31C0\
5E\
C9\
C3\
"
        },
        "ToutetsuBombDamage": {
            "enable": "<option:ToutetsuBombDamage>!=10",
            "addr": "Rx248E6",
            "code": "\
68 <option:ToutetsuBombDamage>\
6A 01\
68 00000000\
"
        },
        "SuikaBombDamageA": {
            "enable": "<option:SuikaBombDamage>!=10",
            "addr": "Rx25FC4",
            "code": "\
68 <option:SuikaBombDamage>\
6A 01\
8B48 30\
8D47 30\
68 00000000\
"
        },
        "SuikaBombDamageB": {
            "enable": "<option:SuikaBombDamage>!=10",
            "addr": "Rx2605D",
            "code": "\
68 <option:SuikaBombDamage>\
6A 01\
68 00000000\
"
        },
        "BitenBombDamage": {
            "enable": "<option:BitenBombDamage>!=10",
            "addr": "Rx2765E",
            "code": "\
68 <option:BitenBombDamage>\
8B48 30\
8D47 30\
83EC 04\
68 08000000\
FFB7 FC000000\
F30F109F 00010000\
"
        },
        "EnokoBombDamageA": {
            "enable": "<option:EnokoBombDamage>!=10",
            "addr": "Rx29748",
            "code": "\
68 <option:EnokoBombDamage>\
6A 01\
8B48 30\
8D47 30\
68 00000000\
"
        },
        "EnokoBombDamageB": {
            "enable": "<option:EnokoBombDamage>!=10",
            "addr": "Rx297DF",
            "code": "\
68 <option:EnokoBombDamage>\
6A 01\
8B48 30\
8D87 0C010000\
68 00000000\
"
        },
        "EnokoBombDamageC": {
            "enable": "<option:EnokoBombDamage>!=10",
            "addr": "Rx29879",
            "code": "\
68 <option:EnokoBombDamage>\
6A 01\
68 00000000\
"
        },
        "ChiyariBombDamage": {
            "enable": "<option:ChiyariBombDamage>!=14",
            "addr": "Rx2B09F",
            "code": "\
68 <option:ChiyariBombDamage>\
6A 01\
8B48 30\
8D47 30\
68 00000000\
"
        },
        "HisamiBombDamage": {
            "enable": "<option:HisamiBombDamage>!=10",
            "addr": "Rx2C4E5",
            "code": "\
68 <option:HisamiBombDamage>\
6A 01\
8B48 30\
8D47 30\
68 00000000\
"
        },
        "ZanmuBombDamage": {
            "enable": "<option:ZanmuBombDamage>!=10",
            "addr": "Rx2DB38",
            "code": "<option:ZanmuBombDamage>"
        }
    }
}