#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>

#include "sscommon.h"
#include "maze_std.h"
#include "glmaze.h"
#include "maze.h"
#include "slvmaze.h"

#ifdef __cplusplus
extern "C" {
#endif
	extern BOOL gbTurboMode asm("_gbTurboMode");
#ifdef __cplusplus
}
#endif