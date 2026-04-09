@echo off
setlocal EnableDelayedExpansion

SET "truth_path=F:\Users\zero318\Source\Repos\truth_custom\target\release\"
SET "truth_map_path=F:\Touhou_Stuff_2\_thtk-bin-new\__truth\map\"
SET "truecl=%truth_path%truth-core.exe truecl"
SET "truanm=%truth_path%truth-core.exe truanm"
SET "trustd=%truth_path%truth-core.exe trustd"
SET "trumsg=%truth_path%truth-core.exe trumsg"

SET "out=..\..\out\build\x86-Clang-Release-Debug\"
SET "outD=..\..\out\build\x86-Clang-Debug\"

SET "thecl=F:\Users\zero318\Source\Repos\thtk_current2\out\build\x64-Clang-Release-Debug\thecl\thecl.exe"

for %%g in (*.anm_dump_map) do (
	%truanm% c "%%g" -g 18 -m %truth_map_path%any.anmm -o "%out%%%~ng.anm" -i "."
    %truanm% d "%out%%%~ng.anm" -g 18 -m %truth_map_path%any.anmm -o "%%~ng.anm_redump_map"
    COPY "%out%%%~ng.anm" "%outD%%%~ng.anm"
    REM thanm.exe -x 18 "%%g" "anm_data/%%~ng.anm_temp"
    REM thanm.exe -c 18 "anm_data/%%~ng.anm_temp" "%out%%%~ng.anm"
)
REM for %%g in (*.std_dump_map) do (
    REM %trustd% c "%%g" -g 18 -m %truth_map_path%any.stdm -o "%out%%%~ng.std"
REM )
for %%g in (*.ecl_dump_map) do (
    REM call clang_cpp_comments.bat "%%g" "%%~ng.pre_ecl_dump_map" "-DPRAGMA=#pragma"
	REM %thecl% -c 18 -m "%truth_map_path%th18_thtk.eclm" "%%~ng.pre_ecl_dump_map"  "%out%%%~ng.ecl"
	%thecl% -c 18 -m "%truth_map_path%th18_thtk.eclm" "%%~ng.ecl_dump_map"  "%out%%%~ng.ecl"
	%thecl% -d 18 -m "%truth_map_path%th18_thtk.eclm" "%out%%%~ng.ecl" "%%~ng.ecl_redump_map"
	%thecl% -d 18 -r -m "%truth_map_path%th18_thtk.eclm" "%out%%%~ng.ecl" "%%~ng.ecl_raw_redump_map"
    COPY "%out%%%~ng.ecl" "%outD%%%~ng.ecl"
)