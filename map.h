/* The map dimensions */
#define MAP_WIDTH	50
#define MAP_HEIGHT	50
#define MAP_SIZE	MAP_WIDTH * MAP_HEIGHT

/* Tile types */
#define TILE_EMPTY	-1
#define TILE_FLOOR	1
#define TILE_WALL	2

/* The viewport dimensions */
#define VP_X		1
#define VP_Y		3
#define VP_WIDTH	38
#define VP_HEIGHT	19

typedef char tile_t;

typedef struct {
	int width, height;
	tile_t *tiles;
	TCOD_map_t fov;
} map_t;

/* Function (fungahhh) prototypes */
map_t *map_new(int, int);
map_t *map_new_static(const char *);
void map_build_fov(map_t *);
void map_dig(map_t*, TCOD_map_t);
tile_t tile_at(map_t*, int, int);

