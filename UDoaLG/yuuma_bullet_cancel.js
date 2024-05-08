{
    "codecaves": {
        "ToutetsuEraseBulletsCaveA": {
            "access": "re",
            "code": "\
0F28D8 \
F30F591D {f32:2.0} \
F30F5905 {f32:0.5} \
F30F115D F8 \
F30F1145 FC \
6A 01 \
6A 02 \
6A 00 \
6A 01 \
57 \
FFB6 CC000000 \
E8 [Rx10AA10] \
E9 [Rx21E15] \
"
        },
        "ToutetsuEraseBulletsCaveB": {
            "access": "re",
            "code": "\
F30F581D {f32:10.0} \
E9 [Rx21E5E] \
"
        }
    },
    "binhacks": {
        "ToutetsuEraseBulletsA": {
            "addr": "Rx21E10",
            "code": "\
E9 [codecave:ToutetsuEraseBulletsCaveA] \
8B8E D0000000 \
FF75 FC \
8B5F 04 \
57 \
"
        },
        "ToutetsuEraseBulletsB": {
            "addr": "Rx21E46",
            "code": "F30F105D F8"
        },
        "ToutetsuEraseBulletsC": {
            "addr": "Rx21E52",
            "code": "\
6A 00 \
57 \
83C7 04 \
E9 [codecave:ToutetsuEraseBulletsCaveB] \
CC \
"
        },
        "ToutetsuEraseBulletsD": {
            "addr": "Rx21E69",
            "code": "\
31C9 \
A9 00000F00 \
50 \
0F95C1 \
6BC9 3C \
891F \
"
        }
    }
}