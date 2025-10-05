struct CloneRef {
    SQObjectValue original;
    SQObjectValue clone;
    
    inline bool is_clone_of(const HSQOBJECT& obj) const {
        return !memcmp(&this->original, &obj._unVal, sizeof(SQObjectValue));
    }
    inline void make_clone_handle(HSQOBJECT& obj) const {
        obj._unVal = this->clone;
    }
};

// Stack top must be a table-like value with fields
static void squirrel_deep_copy(HSQUIRRELVM v, std::vector<CloneRef>& clone_vec) {
    HSQOBJECT obj;
    sq_getstackobj(v, -1, &obj);
    
    for (const auto& cloned : clone_vec) {
        if (cloned.is_clone_of(obj)) {
            sq_pop(v, 1);
            cloned.make_clone_handle(obj);
            sq_pushobject(v, obj);
            return;
        }
    }
    sq_clone(v, -1);
    sq_remove(v, -2);
    SQObjectValue original_value = obj._unVal;
    sq_getstackobj(v, -1, &obj);
    
    clone_vec.emplace_back(original_value, obj._unVal);
    
    SQInteger write_offset = -4;
    if (_RAW_TYPE(obj._type) == _RT_INSTANCE) {
        write_offset = -5;
        sq_getclass(v, -1);
    }
    
    sq_pushnull(v);
    while (SQ_SUCCEEDED(sq_next(v, -2))) {
        // -Instance:
        // Instance|Class|Iter|Key|Value
        // -5      |-4   |-3  |-2 |-1
        //
        // -Everything else:
        // Object|Iter|Key|Value
        // -4    |-3  |-2 |-1
        switch (uint32_t val_type = _RAW_TYPE(sq_gettype(v, -1))) {
            default:
                sq_pop(v, 2);
                break;
            case _RT_INSTANCE:
            case _RT_TABLE: case _RT_ARRAY:
                squirrel_deep_copy(v, clone_vec);
                sq_rawset(v, write_offset);
        }
    }
    sq_pop(v, 1 + (_RAW_TYPE(obj._type) == _RT_INSTANCE));
}

SQInteger sq_deepcopy(HSQUIRRELVM v) {
    if (sq_gettop(v) != 2) {
        return sq_throwerror(v, _SC("Invalid arguments... expected: <object>.\n"));
    }
    sq_push(v, -1);
    switch (uint32_t val_type = _RAW_TYPE(sq_gettype(v, -1))) {
        case _RT_INSTANCE:
        case _RT_TABLE: case _RT_ARRAY: {
            std::vector<CloneRef> clone_vec;
            squirrel_deep_copy(v, clone_vec);
        }
    }
    return 1;
}