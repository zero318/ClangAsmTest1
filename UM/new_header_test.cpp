#include "..\zero\th_common.h"


__declspec(dllexport) void pingas(ZUNListRoot<ZUNInterp<Float3>>& list_head) {
	list_head.for_each([](auto* interp) {
		interp->step();
	});
}