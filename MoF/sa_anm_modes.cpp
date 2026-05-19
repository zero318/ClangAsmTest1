// 0x44FE30
void AnmVM::__get_rounded_vertex_positions(
    Float3* vert0, // Arg1
    Float3* vert1, // EBP
    Float3* vert2, // EBX
    Float3* vert3 // EDI
) {
    float width = this->data.scale.x * this->data.sprite_size.x;
    float height = this->data.scale.y * this->data.sprite_size.y;
    float half_width = width * 0.5;
    float half_height = height * 0.5;
    float left, top, right, bottom;
    switch (this->data.x_anchor_mode) {
        case 0:
            left = floor((this->data.position.x + this->data.__position_3.x + this->data.__position_2.x) - half_width);
            vert0->x = vert2->x = left;
            right = left + width;
            vert1->x = vert3->x = right;
            break;
        case 1:
            left = (this->data.position.x + this->data.__position_3.x + this->data.__position_2.x);
            vert0->x = vert2->x = left;
            right = (this->data.position.x + this->data.__position_3.x + this->data.__position_2.x) + width;
            vert1->x = vert3->x = right;
            break;
        case 2:
            left = (this->data.position.x + this->data.__position_3.x + this->data.__position_2.x) - width;
            vert0->x = vert2->x = left;
            right = (this->data.position.x + this->data.__position_3.x + this->data.__position_2.x);
            vert1->x = vert3->x = right;
            break;
    }
    switch (this->data.y_anchor_mode) {
        case 0:
            top = floor((this->data.position.y + this->data.__position_3.y + this->data.__position_2.y) - half_height);
            vert0->y = vert1->y = top;
            bottom = top + height;
            vert2->y = vert3->y = bottom;
            break;
        case 1:
            left = (this->data.position.y + this->data.__position_3.y + this->data.__position_2.y);
            vert0->y = vert1->y = top;
            right = (this->data.position.y + this->data.__position_3.y + this->data.__position_2.y) + height;
            vert2->y = vert3->y = bottom;
            break;
        case 2:
            left = (this->data.position.y + this->data.__position_3.y + this->data.__position_2.y) - height;
            vert0->y = vert1->y = top;
            right = (this->data.position.y + this->data.__position_3.y + this->data.__position_2.y);
            vert2->y = vert3->y = bottom;
            break;
    }
    vert0->z = vert1->z = vert2->z = vert3->z = this->data.position.z + this->data.__position_3.z + this->data.__position_2.z;
}

// 0x4500F0
void AnmVM::__get_vertex_positions(
    Float3* vert0, // EBX
    Float3* vert1, // EDI
    Float3* vert2, // ESI
    Float3* vert3 // EDX
) {
    float width = this->data.scale.x * this->data.sprite_size.x;
    float height = this->data.scale.y * this->data.sprite_size.y;
    float half_width = width * 0.5;
    float half_height = height * 0.5;
    float left, top, right, bottom;
    switch (this->data.x_anchor_mode) {
        case 0:
            left = (this->data.position.x + this->data.__position_3.x + this->data.__position_2.x) - half_width;
            vert0->x = vert2->x = left;
            right = left + width;
            vert1->x = vert3->x = right;
            break;
        case 1:
            left = (this->data.position.x + this->data.__position_3.x + this->data.__position_2.x);
            vert0->x = vert2->x = left;
            right = (this->data.position.x + this->data.__position_3.x + this->data.__position_2.x) + width;
            vert1->x = vert3->x = right;
            break;
        case 2:
            left = (this->data.position.x + this->data.__position_3.x + this->data.__position_2.x) - width;
            vert0->x = vert2->x = left;
            right = (this->data.position.x + this->data.__position_3.x + this->data.__position_2.x);
            vert1->x = vert3->x = right;
            break;
    }
    switch (this->data.y_anchor_mode) {
        case 0:
            top = (this->data.position.y + this->data.__position_3.y + this->data.__position_2.y) - half_height;
            vert0->y = vert1->y = top;
            bottom = top + height;
            vert2->y = vert3->y = bottom;
            break;
        case 1:
            left = (this->data.position.y + this->data.__position_3.y + this->data.__position_2.y);
            vert0->y = vert1->y = top;
            right = (this->data.position.y + this->data.__position_3.y + this->data.__position_2.y) + height;
            vert2->y = vert3->y = bottom;
            break;
        case 2:
            left = (this->data.position.y + this->data.__position_3.y + this->data.__position_2.y) - height;
            vert0->y = vert1->y = top;
            right = (this->data.position.y + this->data.__position_3.y + this->data.__position_2.y);
            vert2->y = vert3->y = bottom;
            break;
    }
    vert0->z = vert1->z = vert2->z = vert3->z = this->data.position.z + this->data.__position_3.z + this->data.__position_2.z;
}

// 0x4500F0
void AnmVM::__get_rotated_vertex_positions(
    Float3* vert0, // Arg1
    Float3* vert1, // EBX
    Float3* vert2, // EDI
    Float3* vert3 // ESI
) {
    float angle = this->data.rotation.z;
    float unit_x, unit_y;
    __asm {
        FLD angle
        FSINCOS
        FSTP unit_x
        FSTP unit_y
    }
    float X = this->data.position.x + this->data.__position_3.x + this->data.__position_2.x;
    float Y = this->data.position.y + this->data.__position_3.y + this->data.__position_2.y;
    float width = this->data.scale.x * this->data.sprite_size.x;
    float height = this->data.scale.y * this->data.sprite_size.y;
    
    float x0, x1, x2, x3;
    switch (this->data.x_anchor_mode) {
        case 0:
            x0 = x2 = -width * 0.5;
            x1 = x3 = width * 0.5;
            break;
        case 1:
            x0 = x2 = 0.0;
            x1 = x3 = width;
            break;
        case 2:
            x0 = x2 = -width;
            x1 = x3 = 0.0;
            break;
    }
    float y0, y1, y2, y3;
    switch (this->data.y_anchor_mode) {
        case 0:
            y0 = y2 = -height * 0.5;
            y1 = y3 = height * 0.5;
            break;
        case 1:
            y0 = y1 = 0.0f;
            y2 = y3 = height;
            break;
        case 2:
            y0 = y1 = -height;
            y2 = y3 = 0.0f;
            break;
    }
    
    vert0->x = X + ((unit_x * x0) - (unit_y * y0));
    vert0->y = Y + ((unit_x * y0) + (unit_y * x0));
    vert1->x = X + ((unit_x * x1) - (unit_y * y1));
    vert1->y = Y + ((unit_x * y1) + (unit_y * x1));
    vert2->x = X + ((unit_x * x2) - (unit_y * y2));
    vert2->y = Y + ((unit_x * y2) + (unit_y * x2));
    vert3->x = X + ((unit_x * x3) - (unit_y * y3));
    vert3->y = Y + ((unit_x * y3) + (unit_y * x3));
    
    vert0->z = vert1->z = vert2->z = vert3->z = this->data.position.z + this->data.__position_3.z + this->data.__position_2.z;
}

// 0x4500B0
void AnmManager::__draw_vm_type_0(AnmVM* vm) {
    vm->__get_rounded_vertex_positions(&SPRITE_VERTEX_BUFFER_A[0].position, &SPRITE_VERTEX_BUFFER_A[1].position, &SPRITE_VERTEX_BUFFER_A[2].position, &SPRITE_VERTEX_BUFFER_A[3].position);
    this->__render_vertices(vm, RENDER_VERTICES_ROUND_INPUTS);
}

// 0x450610
void AnmManager::__draw_vm_type_1(AnmVM* vm) {
    if (vm->data.rotation.z == 0.0f) {
        vm->__get_rounded_vertex_positions(&SPRITE_VERTEX_BUFFER_A[0].position, &SPRITE_VERTEX_BUFFER_A[1].position, &SPRITE_VERTEX_BUFFER_A[2].position, &SPRITE_VERTEX_BUFFER_A[3].position);
        this->__render_vertices(vm, RENDER_VERTICES_ROUND_INPUTS);
    } else {
        vm->__get_rotated_vertex_positions(&SPRITE_VERTEX_BUFFER_A[0].position, &SPRITE_VERTEX_BUFFER_A[1].position, &SPRITE_VERTEX_BUFFER_A[2].position, &SPRITE_VERTEX_BUFFER_A[3].position);
        this->__render_vertices(vm, RENDER_VERTICES_DEFAULT);
    }
}

// 0x450310
void AnmManager::__draw_vm_type_2(AnmVM* vm) {
    vm->__get_vertex_positions(&SPRITE_VERTEX_BUFFER_A[0].position, &SPRITE_VERTEX_BUFFER_A[1].position, &SPRITE_VERTEX_BUFFER_A[2].position, &SPRITE_VERTEX_BUFFER_A[3].position);
    this->__render_vertices(vm, RENDER_VERTICES_DEFAULT);
}

// 0x450690
void AnmManager::__draw_vm_type_3(AnmVM* vm) {
    if (vm->data.rotation.z == 0.0f) {
        vm->__get_vertex_positions(&SPRITE_VERTEX_BUFFER_A[0].position, &SPRITE_VERTEX_BUFFER_A[1].position, &SPRITE_VERTEX_BUFFER_A[2].position, &SPRITE_VERTEX_BUFFER_A[3].position);
        this->__render_vertices(vm, RENDER_VERTICES_DEFAULT);
    } else {
        vm->__get_rotated_vertex_positions(&SPRITE_VERTEX_BUFFER_A[0].position, &SPRITE_VERTEX_BUFFER_A[1].position, &SPRITE_VERTEX_BUFFER_A[2].position, &SPRITE_VERTEX_BUFFER_A[3].position);
        this->__render_vertices(vm, RENDER_VERTICES_DEFAULT);
    }
}

// 0x452010
void AnmVM::__get_vertex_quad(Float3* out) {
    switch (this->data.render_mode) {
        case Mode2DSprite: // 0
            this->__get_rounded_vertex_positions(&out[0], &out[1], &out[2], &out[3]);
            break;
        case Mode2DSpriteRotated: // 1
            this->__get_rotated_vertex_positions(&out[0], &out[1], &out[2], &out[3]);
            break;
        case Mode2DSpriteSmooth: // 2
        case Mode2DSpriteRotatedB: // 3
            this->__get_vertex_positions(&out[0], &out[1], &out[2], &out[3]);
            break;
    }
}