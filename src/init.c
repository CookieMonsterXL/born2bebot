#include "born2bebot.h"

t_tile*	game_get(t_vars *v, t_gamestate *g, int q, int r, int s)
{
	t_tile* t;

	for (int i = 0; i < v->gameinput.amount_of_tiles; i += 1)
	{
		t = &g->tile[i];
		if (t->q == q && t->r == r && t->s == s)
			return t;
	}
	return NULL;
}

static void init_neigh(t_vars *v, t_gamestate *g)
{
	for (int i = 0; i < v->gameinput.amount_of_tiles; i += 1)
	{
		t_tile* t = &g->tile[i];
		t->neigh[0] = game_get(v, g, t->q, t->r - 1, t->s + 1);
		t->neigh[1] = game_get(v, g, t->q + 1, t->r - 1, t->s);
		t->neigh[2] = game_get(v, g, t->q + 1, t->r, t->s - 1);
		t->neigh[3] = game_get(v, g, t->q, t->r + 1, t->s - 1);
		t->neigh[4] = game_get(v, g, t->q - 1, t->r + 1, t->s);
		t->neigh[5] = game_get(v, g, t->q - 1, t->r, t->s + 1);
	}
}

static void coord_convert(float *x, float *y, int q, int r, int s)
{
	(void) s;
	*x = q * sqrt(3) / 2;
	*y = (int) (r + (q - (q & 1)) / 2) + (q & 1) * 0.5f;
}

static void	init_tiles(t_vars *v, t_gamestate *g)
{
	g->tile = calloc(v->gameinput.amount_of_tiles, sizeof(t_tile));
	int	i = 0;

	for (int q = -v->gameinput.grid_size + 1; q < v->gameinput.grid_size; q += 1) 
	{
		for (int r = -v->gameinput.grid_size + 1; r < v->gameinput.grid_size; r += 1)
		{
			for (int s = -v->gameinput.grid_size + 1; s < v->gameinput.grid_size; s += 1)
			{
				if (q + r + s == 0)
				{
					t_tile* t = &g->tile[i];
					t->q = q;
					t->r = r;
					t->s = s;
					t->wall = false;
					coord_convert(&t->x, &t->y, q, r, s);
					t->chip.value = -1;
					t->chip.tile_index = -1;
					t->chip.placed = false;
					t->chip.x = t->x;
					t->chip.y = t->y;
					i += 1;
				}
			}
		}
	}
}

static void	copy_tiles(t_vars *v, t_gamestate *g, t_gamestate *src)
{
	g->tile = calloc(v->gameinput.amount_of_tiles, sizeof(t_tile));
	int	i = 0;

	for (int q = -v->gameinput.grid_size + 1; q < v->gameinput.grid_size; q += 1) 
	{
		for (int r = -v->gameinput.grid_size + 1; r < v->gameinput.grid_size; r += 1)
		{
			for (int s = -v->gameinput.grid_size + 1; s < v->gameinput.grid_size; s += 1)
			{
				if (q + r + s == 0)
				{
					t_tile* t = &g->tile[i];
					t_tile *s = &src->tile[i];
					t->q = s->q;
					t->r = s->r;
					t->s = s->s;
					t->wall = s->wall;
					t->x = s->x;
					t->y = s->y;
					t->chip.value = s->chip.value;
					t->chip.tile_index = s->chip.tile_index;
					t->chip.placed = s->chip.placed;
					t->chip.x = s->chip.x;
					t->chip.y = s->chip.y;
					i += 1;
				}
			}
		}
	}
}

void	init_gamestate(t_vars *v, t_gamestate *g)
{
	init_tiles(v, g);
	init_neigh(v, g);
	v->current.gravity = bottom;
}
void	copy_gamestate(t_vars *v, t_gamestate *dst, t_gamestate *src)
{
	copy_tiles(v, dst, src);
	init_neigh(v, dst);
	dst->gravity = src->gravity;
}

static void	get_gameinput(t_gameinput *g)
{
	scanf("init %d %d %d %d %f %d",	&g->total_number_of_colors, \
									&g->chips_per_color, \
									&g->grid_size, \
									&g->win_length, \
									&g->timeout, \
									&g->player_id);
}

void	init(t_vars *v)
{
	bzero(v, sizeof(t_vars));
	setbuf(stdout, NULL);
	setbuf(stdin, NULL);
	setbuf(stderr, NULL);
	get_gameinput(&v->gameinput);
	v->chips_data.opp.remaining_chips = 2 * v->gameinput.chips_per_color;
	v->gameinput.win_length = 4;
	v->gameinput.max_column = v->gameinput.grid_size - 1;
	v->gameinput.min_column = -v->gameinput.max_column;
	v->gameinput.amount_of_tiles = 3 * pow(v->gameinput.grid_size, 2) - 3 * v->gameinput.grid_size + 1;
	v->gameinput.amount_colors_per_player = v->gameinput.total_number_of_colors / 2;
	init_gamestate(v, &v->current);
}
