#ifndef __MAZE_H__
#define __MAZE_H__

#include "maze_std.h"

//#define MAZE_GRID 100
#define MAZE_GRID_X 10
#define MAZE_GRID_Y 10
//#define MAZE_CELLS (MAZE_GRID*MAZE_GRID)
#define MAZE_CELLS (MAZE_GRID_X*MAZE_GRID_Y)
//#define MAZE_ARRAY (MAZE_GRID+1)
#define MAZE_ARRAY_X (MAZE_GRID_X+1)
#define MAZE_ARRAY_Y (MAZE_GRID_Y+1)
//#define MAZE_ARRAY_CELLS (MAZE_ARRAY*MAZE_ARRAY)
#define MAZE_ARRAY_CELLS (MAZE_ARRAY_X*MAZE_ARRAY_Y)
#define N_MAZE_PTS MAZE_ARRAY_CELLS
#define N_MAZE_WALLS (2*MAZE_ARRAY_CELLS)

#define MAZE_WALL_LEFT          0x0004
#define MAZE_WALL_RIGHT         0x0008
#define MAZE_WALL_UP            0x0010
#define MAZE_WALL_DOWN          0x0020
#define MAZE_CONTENTS           0x0040

// Partial flags must be shifted versions of the standard flags
#define MAZE_WALL_LEFT_PARTIAL  0x0080
#define MAZE_WALL_RIGHT_PARTIAL 0x0100
#define MAZE_WALL_UP_PARTIAL    0x0200
#define MAZE_WALL_DOWN_PARTIAL  0x0400
#define MAZE_PARTIAL_SHIFT      5

#define WIDX_LEFT       0
#define WIDX_RIGHT      1
#define WIDX_UP         2
#define WIDX_DOWN       3

#define MAZE_CELL_SIZE 1
//#define MAZE_SIZE (MAZE_GRID*MAZE_CELL_SIZE)
#define MAZE_SIZE_X (MAZE_GRID_X*MAZE_CELL_SIZE)
#define MAZE_SIZE_Y (MAZE_GRID_Y*MAZE_CELL_SIZE)
#define FMAZE_CELL_SIZE FxVal(MAZE_CELL_SIZE)
//#define FMAZE_SIZE FxVal(MAZE_SIZE)
#define FMAZE_SIZE_X FxVal(MAZE_SIZE_X)
#define FMAZE_SIZE_Y FxVal(MAZE_SIZE_Y)

#define MfxToCell(mfx) FxInt(mfx)
#define CellToMfx(cl) FxVal(cl)

struct _Cell;

#define DRAW_POLYGON    0
#define DRAW_SPECIAL    1

#define SPECIAL_ARG_ICOSAHEDRON         0
#define SPECIAL_ARG_OCTAHEDRON          1
#define SPECIAL_ARG_DODECAHEDRON        2
#define SPECIAL_ARG_TETRAHEDRON         3
#define SPECIAL_ARG_COUNT               4

typedef struct _Object
{
    FxPt2 p;
    FxValue z;
    FaAngle ang;
    FxValue w, h;
    int col;
    int draw_style;
    int draw_arg;
    TEX_ENV *pTexEnv;  // ptr to texture environment
    int user1, user2, user3;
    struct _Object *next;
    struct _Cell *cell;
} Object;

typedef unsigned short WallFlags;

typedef struct _Cell
{
    WallFlags can_see;
    WallFlags unseen;
    Object *contents;
    struct _Wall *walls[4];
} Cell;

//extern Cell maze_cells[MAZE_GRID][MAZE_GRID];
extern Cell maze_cells[MAZE_GRID_Y][MAZE_GRID_X];

extern float maze_height;
extern double view_rot;
extern int maze_walls_list;

extern int gTexEnvMode;

#define MAX_GOALS       1
//#define MAX_GOALS       10
#define MAX_SPECIALS    (MAX_GOALS-1)
#define MAX_LETTERS 0
//#define MAX_LETTERS 3

#define GOAL_END        0
#define GOAL_SPECIALS   1

BOOL InitMaze(IntPt2 *start_cell, struct _MazeGoal *goals, int *ngoals);

void PlaceObject(Object *obj, FxValue x, FxValue y);
void RemoveObject(Object *obj);
void MoveObject(Object *obj, FxValue x, FxValue y);

void DrawMaze(MazeView *vw);
void DrawMazeWalls(void);
void DrawTopView(MazeView *vw);

#endif
