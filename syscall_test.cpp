struct SYSTEM_SERVICE_TABLE {
	uint32_t* ServiceTable;
	uint32_t* CounterTable;
	uint32_t ServiceLimit;
	uint8_t* ArgumentTable;
};

struct SERVICE_DESCRIPTOR_TABLE {
    SYSTEM_SERVICE_TABLE item1;
    SYSTEM_SERVICE_TABLE item2;
    SYSTEM_SERVICE_TABLE item3;
    SYSTEM_SERVICE_TABLE item4;
};