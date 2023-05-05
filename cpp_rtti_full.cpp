// ClassName::`RTTI Base Class Descriptor at (mpdisp,pdisp,vdisp,attributes)'
// ClassName::`RTTI Base Class Descriptor at (0,-1,0,0)'
// ClassName::`RTTI Base Class Array'
// ClassName::`RTTI Class Hierarchy Descriptor'
// ClassName `RTTI Type Descriptor'
// ClassName::`RTTI Complete Object Locator'
// ClassName::`vftable'

// ClassName::`scalar deleting destructor'
// ClassName::`vector deleting destructor'

// ClassName::`vftable'
// size: Variable
template<class T>
struct Vtable {
	void* funcs[];
};

// ClassName `RTTI Type Descriptor'
// size: Variable
template<class T, class U = type_info>
struct RttiTypeDescriptor {
	// Apparently this is always a pointer to the type_info
	// vtable, but then why is it a member at all?
	Vtable<U>* pVFTable; // 0x0
	// Unused field?
	void* spare; // 0x4
	// Mangled class name
	char name[]; // 0x8
};

// ClassName::`RTTI Base Class Array'
// size: Variable
struct RttiBaseClassArray {
	// There's always an extra NULL terminating the array
	void* arrayOfBaseClassDescriptors[]; // 0x0
};

enum CHDAttributeFlags : uint32_t {
	CHD_MULTINH = 0x1, // Hierarchy contains multiple inheritance.
	CHD_VIRTINH = 0x2, // Hierarchy contains at least one virtual base.
	CHD_AMBIGUOUS = 0x4, // The current type contains an ambiguous base class.
};

// ClassName::`RTTI Class Hierarchy Descriptor'
// size: 0x10
template<class T>
struct RttiClassHierarchyDescriptor {
	// This field is apparently always 0?
	uint32_t signature; // 0x0
	
	// Bitfield describing the inheritance type
	CHDAttributeFlags attributes; // 0x4
	
	// Size of base class array, including self
	uint32_t numBaseClasses; // 0x8
	
	RttiBaseClassArray<T>* pBaseClassArray // 0xC
};

enum BCDAttributeFlags : uint32_t {
	BCD_NOTVISIBLE = 0x01, // set when the current base class is not inherited publicly.
	BCD_AMBIGUOUS = 0x02, // current base class is ambiguous in the class hierarchy.
	BCD_PRIVORPROTINCOMPOBJ = 0x04, // current base class is inherited privately.
	BCD_PRIVORPROTBASE = 0x08, // part of a privately inherited base class hierarchy.
	BCD_VBOFCONTOBJ = 0x10, // current base class is virtually inherited.
	BCD_NONPOLYMORPHIC = 0x20, // ???
	BCD_HASPCHD = 0x40, // The ClassHierarchyDescriptor field is valid
};

// Pointer to member data
// size: 0xC
struct PMD {
	// Offset of this type's fields relative to complete object.
	int32_t mdisp; // 0x0
	// Offset to virtual base table.
	// Field is -1 when inheritance is not virtual.
	int32_t pdisp; // 0x4
	// Offset within virtual base table.
	// Field is 0 when inheritance is not virtual.
	int32_t vdisp; // 0x8
};

// This struct can be present multiple times for the same
// class depending on how other classes inherit it
// ClassName::`RTTI Base Class Descriptor at (mpdisp,pdisp,vdisp,attributes)'
// size: 0x14 or 0x18
template<class T>
struct RTTIBaseClassDescriptor {
	// Type descriptor of this type
	RttiTypeDescriptor<T>* pTypeDescriptor; // 0x0
	// Number of base classes
	uint32_t numContainedBases; // 0x4
	// These fields are used in the name of the struct
	PMD where; // 0x8
	// The attributes are the final field used in the struct name
	BCDAttributeFlags attributes; // 0x14
	// This field can apparently be completely
	// omitted when BCD_HASPCHD isn't set?
	RttiClassHierarchyDescriptor<T>* pClassDescriptor; // 0x18
};

// ClassName::`RTTI Complete Object Locator'
// size: 0x14
template<class T>
struct RttiCompleteObjectLocator {
	// This value is 1 on x64 to indicate the pointers
	// are 32 offsets from the module base instead of
	// full addresses. On x86-32 it's just 0.
	uint32_t signature; // 0x0
	
	// Offset of vftable in the complete class
	uint32_t offset; // 0x4
	
	// Constructor displacement offset
	uint32_t cdOffset; // 0x8
	
	RttiTypeDescriptor<T>* pTypeDescriptor; // 0xC
	RttiClassHierarchyDescriptor<T>* pClassDescriptor; // 0x10
	
	// x64 only
	// RttiCompleteObjectLocator<T>* pSelf; 
};

// size: Variable
template<class T>
struct ClassData {
	RttiCompleteObjectLocator<T>* meta; // 0x0
	Vtable<T> vtable; // 0x4
};