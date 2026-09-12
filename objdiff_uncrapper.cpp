#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <map>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <utility>
#include <string_view>

#include "Windows.h"

#include "jansson.h"

#define json_array_foreach_scoped(ind_type, ind, arr, val) \
	for(ind_type ind = 0, ind ## _max = json_array_size(arr); \
		ind < ind ## _max ? (val = json_array_get(arr, ind)), 1 : 0; \
		ind++)

struct LeftSymbolData {
	std::string_view name;
	json_t* instructions;
};
struct RightSymbolData {
	std::string_view name;
	json_t* instructions;
	size_t offset;
};

struct ObjectFileData {
	json_t* raw;
	//std::map<std::string_view, json_t*> left_symbols;
	//std::map<std::string_view, json_t*> right_symbols;

	std::vector<LeftSymbolData> left_symbols;
	std::vector<RightSymbolData> right_symbols;

	ObjectFileData(json_t* raw) : raw(raw) {}
};

struct GlobalData {
	std::string_view name;
	uintptr_t address;
	size_t size;
};

struct FloatData {
	uintptr_t address;
	size_t size;
};

struct StringData {
	std::string_view str;
	uintptr_t address;
};

static std::unordered_set<std::string_view> jump_opcodes = {
	"jmp",
	"jo",
	"jno",
	"jb", "jnae", "jc",
	"jnb", "jae", "jnc",
	"je", "jz",
	"jne", "jnz",
	"jbe", "jna",
	"ja", "jnbe",
	"js",
	"jns",
	"jp", "jpe",
	"jnp", "jpo",
	"jl", "jnge",
	"jge", "jnl",
	"jle", "jng",
	"jg", "jnle"
};

// Used to strip extra underscores off symbols
static std::unordered_set<std::string_view> standard_library_names = {
	"_sprintf",
	"_vsprintf",
	"_fprintf",
	"_sin",
	"_cos",
	"_tan",
	"_atan2",
	"_sqrt",
	"_fmod",
	"_fopen",
	"_ftell",
	"_fread",
	"_fclose",
	"_fwrite",
	"__mkdir",
	"__chdir"
};

int main(int argc, char* argv[]) {
	int ret = EXIT_FAILURE;
	if (argc >= 8) {
		if (FILE* globals_file = fopen(argv[2], "r")) {

			fseek(globals_file, 0, SEEK_END);
			long globals_length = ftell(globals_file);
			rewind(globals_file);
			if (char* globals = (char*)malloc(globals_length + 1)) {
				globals[globals_length] = '\0';
				fread(globals, globals_length, 1, globals_file);

				std::vector<GlobalData> global_vars = {
					// Security cookie didn't seem to show up in labels without this
					{"___security_cookie", strtoul(argv[7], NULL, 16), sizeof(intptr_t)}
				};

				for (
					char* globals_read = globals, *global_name_end;
					globals_read && (global_name_end = strchr(globals_read, ','));

				) {
					char* addr_start = strchr(global_name_end, '0');
					*global_name_end = '\0';
					uintptr_t addr = strtoul(addr_start, NULL, 16);
					// offset by 1 to skip leading _
					global_vars.emplace_back(std::string_view(globals_read + 1, global_name_end - globals_read - 1), addr, 0);
					globals_read = strchr(addr_start, '_');
				}

				if (FILE* floats_file = fopen(argv[5], "r")) {
					fseek(floats_file, 0, SEEK_END);
					long floats_length = ftell(floats_file);
					rewind(floats_file);

					if (char* floats_str = (char*)malloc(floats_length)) {
						floats_str[floats_length] = '\0';
						fread(floats_str, floats_length, 1, floats_file);

						std::vector<FloatData> floats;

						char* floats_read = floats_str;

						do {
							uintptr_t addr = strtoul(floats_read, &floats_read, 16);
							size_t size = strtoul(floats_read + 1, &floats_read, 10);
							floats.emplace_back(addr, size);
						} while (*floats_read);

						free(floats_str);

						if (FILE* strings_file = fopen(argv[6], "r")) {
							fseek(strings_file, 0, SEEK_END);
							long strings_length = ftell(strings_file);
							rewind(strings_file);

							if (char* strings_str = (char*)malloc(strings_length + 1)) {
								strings_str[strings_length] = '\0';
								fread(strings_str, strings_length, 1, strings_file);

								std::vector<StringData> strings;

								char* strings_read = strings_str;

								do {
									uintptr_t addr = strtoul(strings_read, &strings_read, 16);
									std::string_view str;
									char* string_end = strchr(strings_read + 1, '\1');
									if (string_end) {
										str = std::string_view(strings_read + 2, (string_end++ - 2) - (strings_read + 1));
									} else {
										str = strings_read + 1;
									}
									strings.emplace_back(str, addr);
									strings_read = string_end;
								} while (strings_read);

								/*
								for (const StringData& string : strings) {
									printf("String: \"%.*s\"\n", (int)string.str.size(), string.str.data());
								}
								*/

								// The exe file is loaded just to get the raw values of floats
								if (HMODULE exe_raw = LoadLibraryExA(argv[3], NULL, DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE)) {
									uintptr_t base_addr = strtoul(argv[4], NULL, 16);
									const uint8_t* exe = (const uint8_t*)((uintptr_t)exe_raw & -(uintptr_t)3);

									std::map<std::string, ObjectFileData> object_files;

									WIN32_FIND_DATAA find_data;
									HANDLE find = FindFirstFileA(argv[1], &find_data);
									if (find != INVALID_HANDLE_VALUE) {
										do {
											if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
											} else if (uint64_t file_size = find_data.nFileSizeLow | (uint64_t)find_data.nFileSizeHigh << 32) {
												char* find_extension = strchr(find_data.cFileName, L'.');
												if (find_extension && !strcmp(find_extension + 1, "json")) {
													if (json_t* object = json_load_file(find_data.cFileName, 0, NULL)) {
														object_files.emplace(find_data.cFileName, object);
													}
												}
											}
										} while (FindNextFileA(find, &find_data));
										FindClose(find);
									}

									std::map<std::string_view, std::string_view> function_demangle_map;
									//std::map<std::string_view, std::string_view> data_demangle_map;

									// Iterate the entire set of object files once to
									// collect all of the symbol names for pretty printing.
									for (auto& [filename_str, obj] : object_files) {
										// Left input has all of the mangled symbols
										if (json_t* left = json_object_get(obj.raw, "left")) {
											if (json_t* left_symbols_json = json_object_get(left, "symbols")) {
												json_t* symbol;
												json_array_foreach_scoped(size_t, i, left_symbols_json, symbol) {
													if (json_t* kind_json = json_object_get(symbol, "kind")) {
														const char* kind = json_string_value(kind_json);
														if (!strcmp(kind, "SYMBOL_FUNCTION")) {
															if (json_t* mangled_name = json_object_get(symbol, "name")) {
																const char* mangled = json_string_value(mangled_name);
																size_t mangled_length = json_string_length(mangled_name);

																// Demangled field is present if "name" is a mangled name,
																// otherwise "name" is a normal non-mangled name.
																if (json_t* demangled_name = json_object_get(symbol, "demangled_name")) {
																	const char* demangled = json_string_value(demangled_name);

																	// Find the start of the arguments and then go backwards
																	// to get the whole symbol name.
																	if (const char* args_start = strchr(demangled, '(')) {
																		for (
																			const char* symbol_start = args_start;
																			symbol_start > demangled;
																			--symbol_start
																		) {
																			if (symbol_start[-1] == ' ') {
																				std::string_view demangled_view(symbol_start, args_start - symbol_start);
																				function_demangle_map.emplace(std::string_view(mangled, mangled_length), demangled_view);
																				if (json_t* instructions = json_object_get(symbol, "instructions")) {
																					obj.left_symbols.emplace_back(demangled_view, instructions);
																				}
																				break;
																			}
																		}
																	}
																} else {
																	// Unmangled symbol with a calling convention decorator.
																	// Go backwards until the _ part of the decorator and keep the middle.
																	if (const char* sigil_start = strchr(mangled, '@')) {
																		for (
																			const char* symbol_start = sigil_start;
																			symbol_start > mangled;
																			--symbol_start
																		) {
																			if (symbol_start[-1] == '_') {
																				std::string_view demangled_view(symbol_start, sigil_start - symbol_start);
																				function_demangle_map.emplace(std::string_view(mangled, mangled_length), demangled_view);
																				if (json_t* instructions = json_object_get(symbol, "instructions")) {
																					obj.left_symbols.emplace_back(demangled_view, instructions);
																				}
																				break;
																			}
																		}
																	} else {
																		const char* symbol_start = mangled;
																		size_t symbol_length = mangled_length;
																		if (mangled[0] == '_') { // Skip cdecl decorator
																			++symbol_start;
																			--symbol_length;
																		}
																		std::string_view demangled_view(symbol_start, symbol_length);
																		function_demangle_map.emplace(std::string_view(mangled, mangled_length), demangled_view);
																		if (json_t* instructions = json_object_get(symbol, "instructions")) {
																			obj.left_symbols.emplace_back(demangled_view, instructions);
																		}
																	}
																}
															}
														}
														else if (!strcmp(kind, "SYMBOL_OBJECT")) {
															if (json_t* mangled_name = json_object_get(symbol, "name")) {
																const char* mangled = json_string_value(mangled_name);
																size_t mangled_length = json_string_length(mangled_name);

																// We don't really care about non-mangled data refs, those
																// seem to print right anyway.
																if (json_t* demangled_name = json_object_get(symbol, "demangled_name")) {
																	const char* demangled = json_string_value(demangled_name);

																	std::string_view global_symbol;
																	// Ignore namespaces if present, only keep the final part of the name.
																	if (const char* namespace_end = strrchr(demangled, ':')) {
																		global_symbol = namespace_end + 1;
																	check_global_data:
																		//data_demangle_map.emplace(std::string_view(mangled, mangled_length), global_symbol);
																		if (json_t* size_json = json_object_get(symbol, "size")) {
																			size_t size = strtoul(json_string_value(size_json), NULL, 10);
																			//printf("Setting size of %.*s to 0x%zX\n", (int)global_symbol.size(), global_symbol.data(), size);
																			for (GlobalData& global : global_vars) {
																				if (global.name == global_symbol) {
																					global.size = size;
																					break;
																				}
																			}
																		}
																	}
																	// No namespace, so just keep the entire ending string
																	else if (const char* var_start = strrchr(demangled, ' ')) {
																		global_symbol = var_start + 1;
																		goto check_global_data;
																	}
																}
															}
														}
													}
												}
											}
										}
										// Right input tends to have raw addresses
										if (json_t* right = json_object_get(obj.raw, "right")) {
											if (json_t* right_symbols_json = json_object_get(right, "symbols")) {
												json_t* symbol;
												json_array_foreach_scoped(size_t, i, right_symbols_json, symbol) {
													if (json_t* kind_json = json_object_get(symbol, "kind")) {
														if (!strcmp(json_string_value(kind_json), "SYMBOL_FUNCTION")) {
															if (json_t* name = json_object_get(symbol, "name")) {
																const char* symbol_name = json_string_value(name);
																if (!strncmp(symbol_name, "DAT_", 4)) {
																	// I don't remember what I was going to put here,
																	// but it works without it. :LeSanae:
																}
																else if (json_t* instructions = json_object_get(symbol, "instructions")) {
																	size_t offset = 0;
																	// If the function has an address explicitly listed use that,
																	// otherwise default to 0.
																	if (json_t* address = json_object_get(symbol, "address")) {
																		offset = strtoul(json_string_value(address), NULL, 10);
																	}
																	//printf("Right symbol %zu %s\n", offset, symbol_name);
																	obj.right_symbols.emplace_back(std::string_view(symbol_name, json_string_length(name)), instructions, offset);
																}
															}
														}
													}
												}
											}
										}

										/*
										printf(
											"File: %s\n"
											"Left symbol count:  %zu\n"
											"Right symbol count: %zu\n"
											, filename_str.c_str()
											, obj.left_symbols.size(), obj.right_symbols.size()
										);
										*/
									}

									/*
									for (const GlobalData& global : global_vars) {
										printf(
											"Global: %.*s at 0x%zX (0x%zX bytes)\n"
											, (int)global.name.size(), global.name.data(), global.address, global.size
										);
									}
									*/

									// Iterate the entire set of object files *again*
									// to actually generate the outputs.
									for (const auto& [filename_str, obj] : object_files) {
										const char* filename = filename_str.c_str();
										if (const char* extension = strchr(filename, '.')) {
											size_t filename_length = extension - filename;
											if (char* paths = (char*)malloc(filename_length * 2 + sizeof(".decomp") + sizeof(".original"))) {
												memcpy(paths, filename, filename_length);
												memcpy(paths + filename_length, ".decomp", sizeof(".decomp"));
												memcpy(paths + filename_length + sizeof(".decomp"), filename, filename_length);
												memcpy(paths + filename_length * 2 + sizeof(".decomp"), ".original", sizeof(".original"));
												const char* left_file = paths;
												const char* right_file = paths + filename_length + sizeof(".decomp");

												if (FILE* left_output = fopen(left_file, "w")) {
													if (FILE* right_output = fopen(right_file, "w")) {

														/*
														std::map<std::string_view, std::pair<json_t*, json_t*>> diff_symbols;

														for (const auto& [key, value] : obj.left_symbols) {
															printf(
																"Matching symbol %s\n"
																, key.data()
															);
															auto iter = obj.right_symbols.find(key);
															if (iter != obj.right_symbols.end()) {
																diff_symbols.emplace(key, std::make_pair(value, iter->second));
															}
														}
														*/

														// Only print symbols that are present in both left and right
														std::vector<std::pair<const LeftSymbolData*, const RightSymbolData*>> diff_symbols;

														for (const LeftSymbolData& left : obj.left_symbols) {
															//printf("Matching symbol %s\n", left.name.data());
															for (const RightSymbolData& right : obj.right_symbols) {
																if (left.name == right.name) {
																	diff_symbols.emplace_back(&left, &right);
																	break;
																}
															}
														}

														for (const auto& [left_ptr, right_ptr] : diff_symbols) {
															const LeftSymbolData& left = *left_ptr;
															const RightSymbolData& right = *right_ptr;

															// Deal with left first

															_fwrite_nolock(left.name.data(), left.name.size(), 1, left_output);
															_fwrite_nolock(":\n\t", 3, 1, left_output);
															json_t* instruction_json;
															json_array_foreach_scoped(size_t, i, left.instructions, instruction_json) {
																if (json_t* instruction = json_object_get(instruction_json, "instruction")) {
																	if (json_t* formatted = json_object_get(instruction, "formatted")) {
																		const char* formatted_instr = json_string_value(formatted);
																		if (formatted_instr[0] == '.') {
																			// This is a directive like a switch case table.
																			// These aren't present in right inputs, so ignore.
																			continue;
																		}

																		size_t formatted_instr_length = json_string_length(formatted);

																		if (!strncmp(formatted_instr, "call", 4)) {
																			// Mangled call symbol, look up in the mangled list or fallthrough
																			// to normal symbol handling if that fails.
																			// TODO: Why is normal symbol handling involved? I don't remember.
																			// TODO: make formatted_instr[5] == '?' work better than memchr
																			if (memchr(formatted_instr, '?', formatted_instr_length)) {
																				auto iter = function_demangle_map.find(&formatted_instr[5]);
																				if (iter != function_demangle_map.end()) {
																					_fwrite_nolock("call ", 5, 1, left_output);
																					_fwrite_nolock(iter->second.data(), iter->second.size(), 1, left_output);
																					goto printed_symbolL;
																				}
																			}
																			else {
																				// Indirect call, assume this is __imp__Name@0 formatting
																				// and cut that mess out to only print Name.
																				if (!strncmp(&formatted_instr[5], "dword", 5)) {
																					if (const char* arg_count = strchr(&formatted_instr[23], '@')) {
																						_fwrite_nolock("call dword ptr [", 16, 1, left_output);
																						_fwrite_nolock(&formatted_instr[23], arg_count - &formatted_instr[23], 1, left_output);
																						_fputc_nolock(']', left_output);
																						goto printed_symbolL;
																					}
																				}
																				// nasty hack to remove two underscores
																				if (!strncmp(&formatted_instr[5], "__strdate", 9)) {
																					_fwrite_nolock("call strdate", 12, 1, left_output);
																					goto printed_symbolL;
																				}
																				// Ignore calling convention sigils
																				if (formatted_instr[5] == '_' || formatted_instr[5] == '@') {
																					if (const char* decorated_end = strchr(&formatted_instr[6], '@')) {
																						_fwrite_nolock("call ", 5, 1, left_output);
																						_fwrite_nolock(&formatted_instr[6], decorated_end - &formatted_instr[6], 1, left_output);
																						goto printed_symbolL;
																					}
																					// Right inputs don't print some standard library functions with a
																					// leading _ even though that would make sense. It's easier to strip
																					// the _ from left inputs than add it to the right ones.
																					else if (formatted_instr[5] == '_') {
																						if (standard_library_names.find(&formatted_instr[5]) != standard_library_names.end()) {
																							_fwrite_nolock("call ", 5, 1, left_output);
																							fputs(&formatted_instr[6], left_output);
																							goto printed_symbolL;
																						}
																					}
																				}
																			}
																		}
																		// Check if the opcode has arguments
																		else if (const char* opcode_end = strchr(formatted_instr, ' ')) {
																			size_t opcode_length = opcode_end - formatted_instr + 1;
																			// Check if this is a jump instruction to convert to relative format.
																			// Left inputs never output raw numbers on call ops, so no need to
																			// check those.
																			if (jump_opcodes.find(std::string_view(formatted_instr, opcode_length - 1)) != jump_opcodes.end()) {
																				const char* formatted_after_arg = formatted_instr + opcode_length;
																				if (!strncmp(formatted_after_arg, "short", 5)) {
																					opcode_length += 6;
																					formatted_after_arg += 6;
																				}
																				if (!strncmp(formatted_after_arg, "0x", 2)) {
																					size_t target = strtoul(formatted_after_arg, NULL, 16);
																					//printf("Finding jumpL target %zu\n", target);
																					size_t offset = 0;
																					if (json_t* size = json_object_get(instruction, "size")) {
																						offset = json_integer_value(size);
																					}
																					// Address isn't listed on the first instruction,
																					// so only size applies then
																					if (json_t* address = json_object_get(instruction, "address")) {
																						offset += strtoul(json_string_value(address), NULL, 10);
																					}
																					// Convert to relative jump target to avoid diff spam
																					offset -= target;
																					_fwrite_nolock(formatted_instr, opcode_length, 1, left_output);
																					if ((intptr_t)offset >= 0) {
																						fprintf(left_output, opcode_length <= 5 ? "0x%08zX" : "0x%02zX", offset);
																					} else {
																						fprintf(left_output, opcode_length <= 5 ? "-0x%08zX" : "-0x%02zX", -offset);
																					}
																					goto printed_symbolL;
																				}
																			}
																		}
																		// Check for mangled symbols
																		if (const char* mangled_start = strchr(formatted_instr, '?')) {
																			// Symbols with two ?? are strings, just ignore these
																			if (mangled_start[1] == '?') {
																				if (const char* str_end = strrchr(mangled_start, '@')) {
																					_fwrite_nolock(formatted_instr, mangled_start - formatted_instr, 1, left_output);
																					_fwrite_nolock("STRING", 6, 1, left_output);
																					fputs(str_end + 1, left_output);
																					goto printed_symbolL;
																				}
																			}
																			// look up in the mangled list
																			auto iter = function_demangle_map.find(mangled_start);
																			if (iter != function_demangle_map.end()) {
																				_fwrite_nolock(formatted_instr, mangled_start - formatted_instr, 1, left_output);
																				_fwrite_nolock(iter->second.data(), iter->second.size(), 1, left_output);
																				goto printed_symbolL;
																			}
																			// If the lookup fails this may be a global variable.
																			if (const char* mangled_end = strchr(mangled_start, '@')) {
																				std::string_view global_var(mangled_start + 1, mangled_end - (mangled_start + 1));
																				//printf("Looking for global:%.*s\n", (int)global_var.size(), global_var.data());
																				for (const GlobalData& global : global_vars) {
																					if (global.name == global_var) {
																						const char* symbol_end;
																						// funny logic to preserve any offsets that were applied
																						(symbol_end = strchr(mangled_end, '+')) ||
																						(symbol_end = strchr(mangled_end, '-')) ||
																						(symbol_end = strchr(mangled_end, ']')) ||
																						(symbol_end = strchr(mangled_end, '\0'));

																						_fwrite_nolock(formatted_instr, mangled_start - formatted_instr, 1, left_output);
																						_fwrite_nolock(global.name.data(), global.name.size(), 1, left_output);
																						fputs(symbol_end, left_output);
																						goto printed_symbolL;
																					}
																				}
																			}
																		}
																		// Right inputs print fs:[0x0] and it's easier to replace __except_list
																		// with 0x0 than it is the other way around.
																		else if (const char* except_list = strstr(formatted_instr, "__except_list")) {
																			_fwrite_nolock(formatted_instr, except_list - formatted_instr, 1, left_output);
																			_fwrite_nolock("0x0", 3, 1, left_output);
																			fputs(except_list + 13, left_output);
																			goto printed_symbolL;
																		}

																		// nothing else matched, just print the line
																		_fwrite_nolock(formatted_instr, formatted_instr_length, 1, left_output);

																	printed_symbolL:
																		_fwrite_nolock("\n\t", 2, 1, left_output);
																	}
																}
															}
															_fputc_nolock('\n', left_output);

															// The right side sucks worse

															_fwrite_nolock(right.name.data(), right.name.size(), 1, right_output);
															_fwrite_nolock(":\n\t", 3, 1, right_output);
															json_array_foreach_scoped(size_t, i, right.instructions, instruction_json) {
																if (json_t* instruction = json_object_get(instruction_json, "instruction")) {
																	if (json_t* formatted = json_object_get(instruction, "formatted")) {
																		const char* formatted_instr = json_string_value(formatted);
																		size_t formatted_instr_length = json_string_length(formatted);

																		// Some push opcodes decompile as push s_whatever
																		// instead of using the string table. Ignore these.
																		if (!strncmp(formatted_instr, "push", 4)) {
																			if (formatted_instr[5] == 's' && formatted_instr[6] == '_') {
																				_fwrite_nolock("push STRING", 11, 1, right_output);
																				goto printed_symbolR;
																			}
																		}
																		const char* formatted_after_arg;
																		size_t opcode_length;
																		if (!strncmp(formatted_instr, "call", 4)) {
																			// Mangled call symbol, look up in the mangled list or fallthrough
																			// to normal symbol handling if that fails.
																			// TODO: Why is normal symbol handling involved? I don't remember.
																			// TODO: make formatted_instr[5] == '?' work better than memchr
																			if (memchr(formatted_instr, '?', formatted_instr_length)) {
																				auto iter = function_demangle_map.find(&formatted_instr[5]);
																				if (iter != function_demangle_map.end()) {
																					_fwrite_nolock("call ", 5, 1, right_output);
																					_fwrite_nolock(iter->second.data(), iter->second.size(), 1, right_output);
																					goto printed_symbolR;
																				}
																			}
																			// Right inputs print new/delete in stupid ways,
																			// just sync them with how left inputs do it.
																			else if (!strncmp(&formatted_instr[5], "newcall", 7)) {
																				_fwrite_nolock("call new", 8, 1, right_output);
																				goto printed_symbolR;
																			}
																			else if (!strncmp(&formatted_instr[5], "operator_delete", 15)) {
																				_fwrite_nolock("call delete", 11, 1, right_output);
																				goto printed_symbolR;
																			}
																			// Indirect call, assume this is PTR_Name_Addr formatting
																			// and cut that mess out to only print Name.
																			else if (!strncmp(&formatted_instr[5], "dword", 5)) {
																				if (const char* symbol_end = strchr(&formatted_instr[20], '_')) {
																					_fwrite_nolock("call dword ptr [", 16, 1, right_output);
																					_fwrite_nolock(&formatted_instr[20], symbol_end - &formatted_instr[20], 1, right_output);
																					_fputc_nolock(']', right_output);
																					goto printed_symbolR;
																				}
																			}
																			else {
																				opcode_length = 5;
																				formatted_after_arg = formatted_instr + opcode_length;
																			jump_symbolR:
																				// If the target isn't a number use normal symbol handling.
																				if (!strncmp(formatted_after_arg, "0x", 2)) {
																					size_t target = strtoul(formatted_after_arg, NULL, 16);
																					//printf("Finding jumpR target %zu\n", target);
																				
																					// For some reason right input calls can print numeric
																					// values instead of symbols. Try to look those up.
																					for (const RightSymbolData& right_symbol : obj.right_symbols) {
																						if (right_symbol.offset == target) {
																							_fwrite_nolock(formatted_instr, opcode_length, 1, right_output);
																							_fwrite_nolock(right_symbol.name.data(), right_symbol.name.size(), 1, right_output);
																							goto printed_symbolR;
																						}
																					}
																					// Not a symbol, so just convert to relative format
																					// to prevent diff spam.
																					size_t offset = 0;
																					if (json_t* size = json_object_get(instruction, "size")) {
																						offset = json_integer_value(size);
																					}
																					// Address isn't listed on the first instruction,
																					// so only size applies then
																					if (json_t* address = json_object_get(instruction, "address")) {
																						offset += strtoul(json_string_value(address), NULL, 10);
																					}
																					offset -= target;
																					_fwrite_nolock(formatted_instr, opcode_length, 1, right_output);
																					if ((intptr_t)offset >= 0) {
																						fprintf(right_output, opcode_length <= 5 ? "0x%08zX" : "0x%02zX", offset);
																					} else {
																						fprintf(right_output, opcode_length <= 5 ? "-0x%08zX" : "-0x%02zX", -offset);
																					}
																					goto printed_symbolR;
																				}
																			}
																		}
																		// Check if the opcode has arguments
																		else if (const char* opcode_end = strchr(formatted_instr, ' ')) {
																			opcode_length = opcode_end - formatted_instr + 1;
																			// Check if this is a jump instruction to convert to relative format.
																			if (jump_opcodes.find(std::string_view(formatted_instr, opcode_length - 1)) != jump_opcodes.end()) {
																				formatted_after_arg = formatted_instr + opcode_length;
																				if (!strncmp(formatted_after_arg, "short", 5)) {
																					opcode_length += 6;
																					formatted_after_arg += 6;
																				}
																				// normally I would put the logic here, but opcode_end
																				// is declared and C++ rules about goto suck
																				goto jump_symbolR;
																			}
																		}
																		// Right inputs just don't output names for data section
																		// things in most cases, so a whole bunch of lookups instead.
																		if (const char* dat_str = strstr(formatted_instr, "DAT_")) {
																			char* end_str;
																			size_t addr = strtoul(dat_str + 4, &end_str, 16);
																			//printf("Looking for global:0x%zX\n", addr);

																			// For some reason the global ranges can overlap,
																			// so iterate the entire list and pick the one with the smallest
																			// offset value.
																			const GlobalData* matched_global = NULL;
																			size_t match_difference = SIZE_MAX;
																			for (const GlobalData& global : global_vars) {
																				if (global.address <= addr && addr < global.address + global.size) {
																					size_t new_diff = abs((ptrdiff_t)(addr - global.address));
																					if (new_diff < match_difference) {
																						matched_global = &global;
																						match_difference = new_diff;
																						if (!match_difference) break; // can't be smaller than 0
																					}
																				}
																			}
																			if (matched_global) {
																				_fwrite_nolock(formatted_instr, dat_str - formatted_instr, 1, right_output);
																				_fwrite_nolock(matched_global->name.data(), matched_global->name.size(), 1, right_output);
																				// Check if there's an offset from the original symbol
																				// and merge that with the offset from the global.
																				switch (end_str[0]) {
																					case '+':
																						addr += strtoul(&end_str[1], &end_str, 16);
																						break;
																					case '-':
																						addr -= strtoul(&end_str[1], &end_str, 16);
																						break;
																				}
																				if (matched_global->address != addr) {
																					_fputc_nolock('+', right_output);
																					fprintf(right_output, "0x%zx", addr - matched_global->address);
																				}
																				fputs(end_str, right_output);
																				goto printed_symbolR;
																			}

																			// Check for matches in the list of floats
																			for (const FloatData& float_var : floats) {
																				if (float_var.address == addr) {
																					_fwrite_nolock(formatted_instr, dat_str - formatted_instr, 1, right_output);
																					_fwrite_nolock("__real@", 7, 1, right_output);
																					size_t offset = addr - base_addr;
																					// Left inputs print the raw float bits regardless
																					// of what the address is, so just mimic that format
																					// by pulling the float value from the original exe.
																					switch (float_var.size) {
																						case sizeof(float) :
																							fprintf(right_output, "%08x", *(uint32_t*)&exe[offset]);
																							break;
																						case sizeof(double):
																							fprintf(right_output, "%016llx", *(uint64_t*)&exe[offset]);
																							break;
																					}
																					fputs(end_str, right_output);
																					goto printed_symbolR;
																				}
																			}

																			// Check for matches in the list of strings
																			for (const StringData& string_var : strings) {
																				if (string_var.address <= addr && addr < string_var.address + string_var.str.size() + 1) {
																					// Printing the string contents would be cool, but is way
																					// too unreliable. Just keep ignoring string contents.
																					_fwrite_nolock(formatted_instr, dat_str - formatted_instr, 1, right_output);
																					//_fputc_nolock('"', right_output);
																					//fflush(right_output);
																					//_fwrite_nolock(string_var.str.data(), string_var.str.size(), 1, right_output);
																					//fflush(right_output);
																					//_fputc_nolock('"', right_output);
																					_fwrite_nolock("STRING", 6, 1, right_output);
																					fputs(end_str, right_output);
																					goto printed_symbolR;
																				}
																			}
																		}

																		// nothing else matched, just print the line
																		_fwrite_nolock(formatted_instr, formatted_instr_length, 1, right_output);
																		
																	printed_symbolR:
																		_fwrite_nolock("\n\t", 2, 1, right_output);
																	}
																}
															}
															_fputc_nolock('\n', right_output);
														}

														// oh boy closing bracket time
														// I have a wide monitor if you hadn't guessed :P
														ret = EXIT_SUCCESS;

														fclose(right_output);
													}
													fclose(left_output);
												}

												free(paths);
											}
										}
									}
									FreeLibrary(exe_raw);
								}
								free(strings_str);
							}
							fclose(strings_file);
						}
					}
					fclose(floats_file);
				}
				free(globals);
			}
			fclose(globals_file);
		}
	} else {
		// TODO: print a help message
	}
	return ret;
}