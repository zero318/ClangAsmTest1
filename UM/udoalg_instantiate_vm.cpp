gnu_used gnu_noinline AnmID& thiscall instantiate_vm_19_impl(AnmID& out, int, int32_t script_index, Float3* position, float z_rotation, int32_t layer, int flags, AnmVM** raw_out) asm("instantiate_vm_19_impl") {
	use_var(script_index);
	use_var(position);
	use_var(z_rotation);
	use_var(layer);
	use_var(flags);
	use_var(raw_out);
	return this->instantiate_vm(out, script_index, position, z_rotation, layer, bitcast<AnmVMCreationFlags>(flags), raw_out);
}

gnu_used forceinline AnmID& thiscall instantiate_vm_19(AnmID& out, int32_t script_index, Float3* position, float z_rotation, int32_t layer, int flags, AnmVM** raw_out) {
	return this->instantiate_vm_19_impl(out, UNUSED_DWORD, script_index, position, z_rotation, layer, flags, raw_out);
}