#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "zero/util.h"
#include "zero/custom_intrin.h"

enum BsonType : int8_t {
	BsonEnd = 0,
	BsonDouble = 1,
	BsonUTF8 = 2,
	BsonObject = 3,
	BsonArray = 4,
	BsonBlob = 5,
	BsonUndefined = 6,
	BsonObjectID = 7,
	BsonBool = 8,
	BsonTime = 9,
	BsonRegex = 10,
	BsonDBPointer = 11,
	BsonJS = 12,
	BsonSymbol = 13,
	BsonJSScoped = 14,
	BsonInt32 = 15,
	BsonTimestamp = 16,
	BsonInt64 = 17,
	BsonDecimal128 = 18,

	BsonMinKey = -1,
	BsonMaxKey = INT8_MAX
};
enum BsonBlobType : int8_t {
	BsonBlobBinary = 0,
	BsonBlobFunc = 1,
	BsonBlobBinaryOld = 2,
	BsonBlobUUIDOld = 3,
	BsonBlobUUID = 4,
	BsonBlobMD5 = 5,
	BsonBlobEncrypted = 6,
	BsonBlobCompressed = 7,
	BsonBlobUser = INT8_MIN,
	BsonBlobUserEnd = -1
};

struct BsonPString {
	uint32_t buffer_size;
	char8_t data[];
};
struct BsonCString {
	char8_t data[];
};
struct BsonBlob {
	uint32_t buffer_size;
	BsonBlobType type;
	uint8_t data[];
};

struct BsonValue {
	BsonType type;
	char8_t name[];
};

struct BsonFile {
	uint32_t file_size;
};