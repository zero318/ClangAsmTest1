enum MenuItemStyle : size_t {
					// Menu Title
	PrintText,		// -Label
	Checkbox,		// *Label: [X]
	HSpinnerNumber,	// *Label: <  1>
	HSpinnerText,	// *Label: <Yeet>
	WriteValue,		// *Quit
	InvalidStyle,
};

// Tells thcrap to execute the original
// bytes overwritten by the breakpoint
// call after returning
#define BREAKPOINT_EXEC_CAVE 1

// These would be defined as per-game
// options of some sort in the codecave
#define INPUT_ADDR 0xDEADBEEF
#define ASCII_MANAGER_PTR_ADDR 0xDEADBEEF

dllexport int cdecl menu_on_tick(x86_reg_t* regs, json_t* bp_info) {
	MenuState* menu_state = (MenuState*)json_object_get_eval_addr_default(bp_info, "state_buffer", 0, JEVAL_DEFAULT);
	if (!menu_state) {
		return BREAKPOINT_EXEC_CAVE;
	}
	json_t* menu_items = json_object_get(bp_info, "items");
	if (!json_is_array(menu_items)) {
		return BREAKPOINT_EXEC_CAVE;
	}
	Input input = *(Input*)INPUT_ADDR;

	size_t array_max = json_array_size(menu_items) - 1;
	size_t starting_index = menu_state->selection;
	size_t selected_index = starting_index;

	MenuItemStyle style;
	json_t* menu_item;
	if (input.up && !input.down) {
		do {
			if (--selected_index == SIZE_MAX) {
				selected_index = array_max;
			}
			menu_item = json_array_get(menu_items, selected_index);
		} while (
			selected_index != starting_index &&
			(json_object_get_eval_bool_default(menu_item, "hidden", false, JEVAL_DEFAULT) ||
			 PrintText == (style = (MenuItemStyle)json_object_get_eval_int_default(menu_item, "style", PrintText, JEVAL_DEFAULT)))
		);
	} else if (input.down) {
		do {
			if (++selected_index > array_max) {
				selected_index = 0;
			}
		} while (
			selected_index != starting_index &&
			(json_object_get_eval_bool_default(menu_item, "hidden", false, JEVAL_DEFAULT) ||
			 PrintText == (style = (MenuItemStyle)json_object_get_eval_int_default(menu_item, "style", PrintText, JEVAL_DEFAULT)))
		);
	}
	menu_state->selection = selected_index;

	switch (style) {
		case WriteValue:
			if (input.select) {
				size_t* val_ptr = (size_t*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				*val_ptr = json_object_get_eval_addr_default(menu_item, "val", 0, JEVAL_DEFAULT);
			}
			break;
		case Checkbox: 
			if (input.select || (input.left != input.right)) {
				bool* val_ptr = (bool*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				*val_ptr ^= true;
			}
			break;
		case HSpinnerNumber:
			if (input.left != input.right) {
				int32_t change = input.left ? 1 : -1;
				const char* type_str = json_object_get_string(menu_item, "type");
				if (!type_str) goto is_bad;
				patch_value_type_t value_type = patch_parse_type(type_str);
				switch (value_type) {
					case PVT_BYTE ... PVT_SDWORD:
						break;
					default:
						goto is_bad;
				}
				void* val_ptr = (void*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				size_t value;
				switch (value_type--) {
					default:
						unreachable;
					case PVT_BYTE: case PVT_SBYTE:
						*(int8_t*)val_ptr += change;
						break;
					case PVT_WORD: case PVT_SWORD:
						*(int16_t*)val_ptr += change;
						break;
					case PVT_DWORD: case PVT_SDWORD:
						*(int32_t*)val_ptr += change;
						break;
				}
			}
			break;
		case HSpinnerText:
			if (input.left != input.right) {
				json_t* text_array = json_object_get(menu_item, "text");
				if (!json_is_array(text_array)) goto is_bad;
				size_t text_array_max = json_array_size(text_array) - 1;
				size_t* val_ptr = (size_t*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				if (input.left) {
					if (++*val_ptr > text_array_max) {
						*val_ptr = 0;
					}
				} else {
					if (--*val_ptr == SIZE_MAX) {
						*val_ptr = text_array_max;
					}
				}
			}
			break;
		default:
			break;
		is_bad:
			json_object_set_new_nocheck(menu_item, "hidden", json_true());
			break;
	}
	return BREAKPOINT_EXEC_CAVE;
}

dllexport int cdecl menu_on_draw(x86_reg_t* regs, json_t* bp_info) {
	MenuState* menu_state = (MenuState*)json_object_get_eval_addr_default(bp_info, "state_buffer", NULL, JEVAL_DEFAULT);
	if (!menu_state) {
		return BREAKPOINT_EXEC_CAVE;
	}
	AsciiManager19* ascii_manager = *(AsciiManager19**)ASCII_MANAGER_PTR_ADDR;
	float position[4];
	position[0] = json_object_get_eval_real_default(bp_info, "x_pos", 0.0, JEVAL_DEFAULT);
	position[1] = json_object_get_eval_real_default(bp_info, "y_pos", 0.0, JEVAL_DEFAULT);
	position[2] = 0.0f;
	position[3] = json_object_get_eval_real_default(bp_info, "y_spacing", 1.0, JEVAL_DEFAULT);

	auto draw_text = [ascii_manager, &position]<typename ... Args>(const char* format, Args... args) gnu_always_inline {
		ascii_draw_text(ascii_manager, position, format, args...);
		position[1] += position[3];
	};

	if (const char* title_str = json_object_get_string(bp_info, "title")) {
		draw_text("%s", title_str);
	}
	json_t* menu_items = json_object_get(bp_info, "items");
	if (!json_is_array(menu_items)) {
		return BREAKPOINT_EXEC_CAVE;
	}

	json_t* menu_item;
	json_array_foreach_scoped(size_t, i, menu_items, menu_item) {
		if (json_object_get_eval_bool_default(menu_item, "hidden", false, JEVAL_DEFAULT)) {
			continue;
		}
		const char* label_str = json_object_get_string(menu_item, "label");
		if (!label_str) {
			label_str = "";
		}
		MenuItemStyle action = (MenuItemStyle)json_object_get_eval_int_default(menu_item, "style", PrintText, JEVAL_DEFAULT);

		uint32_t selection_characters = menu_state->selection == i ? PackInt('*', '<', '>') : PackInt(' ', ' ', ' ');
		switch (action) {
			case PrintText: {
				draw_text("-%s", label_str);
				break;
			}
			case WriteValue: {
				draw_text("%c%s", selection_characters, label_str);
				break;
			}
			case Checkbox: {
				bool* val_ptr = (bool*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				draw_text("%c%s: [%c]", selection_characters, label_str, *val_ptr ? 'X' : ' ');
				break;
			}
			case HSpinnerNumber: {
				const char* type_str = json_object_get_string(menu_item, "type");
				if (!type_str) goto is_bad;
				patch_value_type_t value_type = patch_parse_type(type_str);
				switch (value_type) {
					case PVT_BYTE ... PVT_SDWORD:
						break;
					default:
						goto is_bad;
				}
				void* val_ptr = (void*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				size_t value;
				switch (value_type--) {
					default:
						unreachable;
					case PVT_BYTE: case PVT_SBYTE:
						value = *(uint8_t*)val_ptr;
						break;
					case PVT_WORD: case PVT_SWORD:
						value = *(uint16_t*)val_ptr;
						break;
					case PVT_DWORD: case PVT_SDWORD:
						value = *(uint32_t*)val_ptr;
						break;
				}
				static constexpr const char* format_table[] = {
					"%c%s: %c%3" PRIu8 "%c", "%c%s: %c%4" PRIi8 "%c",
					"%c%s: %c%5" PRIu16 "%c", "%c%s: %c%6" PRIi16 "%c",
					"%c%s: %c%10" PRIu32 "%c", "%c%s: %c%11" PRIi32 "%c"
				};
				draw_text(format_table[value_type], selection_characters, label_str, selection_characters >> 8, value, selection_characters >> 16);
				break;
			}
			case HSpinnerText: {
				json_t* text_array = json_object_get(menu_item, "text");
				if (!json_is_array(text_array)) goto is_bad;
				size_t* val_ptr = (size_t*)json_object_get_eval_addr_default(menu_item, "val_addr", NULL, JEVAL_DEFAULT);
				if (!val_ptr) goto is_bad;
				json_t* text_value = json_array_get(text_array, *val_ptr);
				if (!json_is_string(text_value)) goto is_bad;
				draw_text("%c%s: %c%s%c", selection_characters, label_str, selection_characters >> 8, json_string_value(text_value), selection_characters >> 16);
				break;
			}
			default:
				unreachable;
			is_bad:
				json_object_set_new_nocheck(menu_item, "hidden", json_true());
				break;
		}
	}
	return BREAKPOINT_EXEC_CAVE;
}