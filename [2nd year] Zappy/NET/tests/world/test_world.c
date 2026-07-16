/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Unit tests for world / AI / team functions
*/

#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>

#include "world.h"
#include "team.h"
#include "ai.h"
#include "map.h"
#include "args.h"
#include "command.h"
#include "parser.h"
#include "cli_opt.h"

/*
** ─── HELPERS ─────────────────────────────────────────────────────────────────
*/

static args_t *make_args(size_t eggs)
{
    args_t *a = calloc(1, sizeof(args_t));

    a->width = 10;
    a->height = 10;
    a->freq = 1;
    a->eggs_per_team = eggs;
    a->team_names = malloc(sizeof(char *) * 3);
    a->team_names[0] = strdup("alpha");
    a->team_names[1] = strdup("beta");
    a->team_names[2] = NULL;
    a->parsing_status = true;
    return a;
}

static void free_args(args_t *a)
{
    free(a->team_names[0]);
    free(a->team_names[1]);
    free(a->team_names);
    free(a);
}

static world_t *make_world_for_teams(void)
{
    args_t *args = make_args(3);
    tile_t *grid = NULL;
    world_t *world = NULL;

    alloc_grid_buffers(args->width * args->height, &grid, &world);
    world->args = args;
    build_grid(world);
    memset(world->resource_counts, 0, sizeof(world->resource_counts));
    world->ais = NULL;
    world->gui = NULL;
    world->teams = NULL;
    world->command_queue = NULL;
    world->old_time_commands = 0;
    world->next_command_time = -1;
    return world;
}

static void free_world_no_ais(world_t *world)
{
    size_t n = world->args->width * world->args->height;

    for (size_t i = 0; i < n; i++) {
        player_t *p = world->grid[i].content->players;
        player_t *next;
        while (p) {
            next = p->next;
            free(p);
            p = next;
        }
        free(world->grid[i].content);
    }
    if (world->teams)
        free_teams(world->teams);
    if (world->ais)
        destroy_ais(world->ais);
    free(world->grid);
    free_args(world->args);
    free(world);
}

static tile_t *make_bare_tile(int x, int y)
{
    tile_t *t = calloc(1, sizeof(tile_t));

    t->x = x;
    t->y = y;
    t->content = calloc(1, sizeof(tile_content_t));
    return t;
}

static ai_t *make_ai(size_t id, size_t level, tile_t *pos)
{
    ai_t *ai = calloc(1, sizeof(ai_t));

    ai->id = id;
    ai->level = level;
    ai->position = pos;
    ai->egg_type = INITIAL;
    ai->is_incanting = false;
    ai->local_commands = NULL;
    return ai;
}

static player_t *make_player_node(ai_t *ai)
{
    player_t *p = calloc(1, sizeof(player_t));

    p->ai = ai;
    return p;
}

/*
** ─── init_teams / free_teams ─────────────────────────────────────────────────
*/

Test(init_teams, creates_two_teams)
{
    world_t *world = make_world_for_teams();
    int count = 0;

    cr_assert_eq(init_teams(world), true);
    for (team_t *t = world->teams; t; t = t->next)
        count++;
    cr_assert_eq(count, 2);
    free_world_no_ais(world);
}

Test(init_teams, teams_have_correct_names)
{
    world_t *world = make_world_for_teams();
    bool found_alpha = false;
    bool found_beta = false;

    init_teams(world);
    for (team_t *t = world->teams; t; t = t->next) {
        if (strcmp(t->name, "alpha") == 0)
            found_alpha = true;
        if (strcmp(t->name, "beta") == 0)
            found_beta = true;
    }
    cr_assert_eq(found_alpha, true);
    cr_assert_eq(found_beta, true);
    free_world_no_ais(world);
}

Test(init_teams, free_slots_equals_eggs_per_team)
{
    world_t *world = make_world_for_teams();

    init_teams(world);
    for (team_t *t = world->teams; t; t = t->next)
        cr_assert_eq(t->free_slots, world->args->eggs_per_team);
    free_world_no_ais(world);
}

Test(free_teams, handles_null)
{
    free_teams(NULL);  /* must not crash */
}

Test(free_teams, frees_single_team)
{
    world_t *world = make_world_for_teams();

    init_teams(world);
    free_teams(world->teams);
    world->teams = NULL;
    free_world_no_ais(world);
}

/*
** ─── is_available_team ───────────────────────────────────────────────────────
*/

Test(is_available_team, returns_true_when_slot_free)
{
    world_t *world = make_world_for_teams();

    init_teams(world);
    cr_assert_eq(is_available_team(world, "alpha"), true);
    free_world_no_ais(world);
}

Test(is_available_team, returns_false_for_unknown_team)
{
    world_t *world = make_world_for_teams();

    init_teams(world);
    cr_assert_eq(is_available_team(world, "unknown"), false);
    free_world_no_ais(world);
}

Test(is_available_team, returns_false_when_no_free_slots)
{
    world_t *world = make_world_for_teams();

    init_teams(world);
    world->teams->free_slots = 0;
    cr_assert_eq(is_available_team(world, world->teams->name), false);
    free_world_no_ais(world);
}

Test(is_available_team, finds_second_team)
{
    world_t *world = make_world_for_teams();

    init_teams(world);
    cr_assert_eq(is_available_team(world, "beta"), true);
    free_world_no_ais(world);
}

/*
** ─── pop_ai_from_list ────────────────────────────────────────────────────────
*/

Test(pop_ai_from_list, returns_null_on_empty_list)
{
    ai_t ai = {0};
    player_t *head = NULL;

    cr_assert_null(pop_ai_from_list(&head, &ai));
}

Test(pop_ai_from_list, pops_only_element)
{
    ai_t ai = {0};
    tile_t *tile = make_bare_tile(0, 0);
    player_t *node = make_player_node(&ai);
    player_t *head = node;
    player_t *result;

    result = pop_ai_from_list(&head, &ai);
    cr_assert_not_null(result);
    cr_assert_eq(result->ai, &ai);
    cr_assert_null(head);
    free(result);
    free(tile->content);
    free(tile);
}

Test(pop_ai_from_list, pops_head_of_two)
{
    ai_t ai1 = {.id = 1};
    ai_t ai2 = {.id = 2};
    player_t *n1 = make_player_node(&ai1);
    player_t *n2 = make_player_node(&ai2);
    player_t *head = n1;
    player_t *result;

    n1->next = n2;
    result = pop_ai_from_list(&head, &ai1);
    cr_assert_not_null(result);
    cr_assert_eq(head, n2);
    cr_assert_null(head->next);
    free(result);
    free(n2);
}

Test(pop_ai_from_list, pops_tail_of_two)
{
    ai_t ai1 = {.id = 1};
    ai_t ai2 = {.id = 2};
    player_t *n1 = make_player_node(&ai1);
    player_t *n2 = make_player_node(&ai2);
    player_t *head = n1;
    player_t *result;

    n1->next = n2;
    result = pop_ai_from_list(&head, &ai2);
    cr_assert_not_null(result);
    cr_assert_eq(head, n1);
    cr_assert_null(n1->next);
    free(result);
    free(n1);
}

Test(pop_ai_from_list, returns_null_when_ai_not_in_list)
{
    ai_t ai1 = {.id = 1};
    ai_t ai2 = {.id = 2};
    player_t *n1 = make_player_node(&ai1);
    player_t *head = n1;
    player_t *result;

    result = pop_ai_from_list(&head, &ai2);
    cr_assert_null(result);
    cr_assert_eq(head, n1);  /* list unchanged */
    free(n1);
}

Test(pop_ai_from_list, pops_middle_of_three)
{
    ai_t a = {.id = 1};
    ai_t b = {.id = 2};
    ai_t c = {.id = 3};
    player_t *na = make_player_node(&a);
    player_t *nb = make_player_node(&b);
    player_t *nc = make_player_node(&c);
    player_t *head = na;
    player_t *result;

    na->next = nb;
    nb->next = nc;
    result = pop_ai_from_list(&head, &b);
    cr_assert_not_null(result);
    cr_assert_eq(head, na);
    cr_assert_eq(na->next, nc);
    cr_assert_null(nc->next);
    free(result);
    free(na);
    free(nc);
}

/*
** ─── bloom_egg ───────────────────────────────────────────────────────────────
*/

Test(bloom_egg, promotes_egg_to_level_1)
{
    world_t *world = make_world_for_teams();
    ai_t *egg;

    init_teams(world);
    spawn_eggs(world);
    egg = bloom_egg(world, "alpha");
    cr_assert_not_null(egg);
    cr_assert_eq(egg->level, 1u);
    free_world_no_ais(world);
}

Test(bloom_egg, decrements_free_slots)
{
    world_t *world = make_world_for_teams();
    team_t *alpha;
    size_t slots_before;

    init_teams(world);
    spawn_eggs(world);
    for (alpha = world->teams; alpha; alpha = alpha->next)
        if (strcmp(alpha->name, "alpha") == 0)
            break;
    slots_before = alpha->free_slots;
    bloom_egg(world, "alpha");
    cr_assert_eq(alpha->free_slots, slots_before - 1);
    free_world_no_ais(world);
}

Test(bloom_egg, returns_null_for_unknown_team)
{
    world_t *world = make_world_for_teams();

    init_teams(world);
    spawn_eggs(world);
    cr_assert_null(bloom_egg(world, "nonexistent"));
    free_world_no_ais(world);
}

Test(bloom_egg, returns_null_when_no_free_egg)
{
    world_t *world = make_world_for_teams();
    team_t *alpha;

    init_teams(world);
    spawn_eggs(world);
    for (alpha = world->teams; alpha; alpha = alpha->next)
        if (strcmp(alpha->name, "alpha") == 0)
            break;
    /* bloom all available eggs */
    while (bloom_egg(world, "alpha") != NULL);
    cr_assert_null(bloom_egg(world, "alpha"));
    free_world_no_ais(world);
}

/*
** ─── AI direction: left turn logic ──────────────────────────────────────────
**  left() / right() are tested by checking the direction field directly
**  via a minimal command_t + ai_t without needing a full server.
*/

static command_t *make_turn_command(ai_t *ai, int fd)
{
    command_t *cmd = calloc(1, sizeof(command_t));

    cmd->player = ai;
    cmd->player_fd = fd;
    return cmd;
}

Test(left_turn, north_becomes_west)
{
    ai_t ai = {.direction = NORTH};
    int fds[2];
    command_t *cmd;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    cmd = make_turn_command(&ai, fds[1]);
    left(NULL, cmd);
    cr_assert_eq(ai.direction, WEST);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
}

Test(left_turn, west_becomes_south)
{
    ai_t ai = {.direction = WEST};
    int fds[2];
    command_t *cmd;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    cmd = make_turn_command(&ai, fds[1]);
    left(NULL, cmd);
    cr_assert_eq(ai.direction, SOUTH);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
}

Test(left_turn, south_becomes_east)
{
    ai_t ai = {.direction = SOUTH};
    int fds[2];
    command_t *cmd;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    cmd = make_turn_command(&ai, fds[1]);
    left(NULL, cmd);
    cr_assert_eq(ai.direction, EAST);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
}

Test(left_turn, east_becomes_north)
{
    ai_t ai = {.direction = EAST};
    int fds[2];
    command_t *cmd;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    cmd = make_turn_command(&ai, fds[1]);
    left(NULL, cmd);
    cr_assert_eq(ai.direction, NORTH);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
}

Test(right_turn, north_becomes_east)
{
    ai_t ai = {.direction = NORTH};
    int fds[2];
    command_t *cmd;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    cmd = make_turn_command(&ai, fds[1]);
    right(NULL, cmd);
    cr_assert_eq(ai.direction, EAST);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
}

Test(right_turn, east_becomes_south)
{
    ai_t ai = {.direction = EAST};
    int fds[2];
    command_t *cmd;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    cmd = make_turn_command(&ai, fds[1]);
    right(NULL, cmd);
    cr_assert_eq(ai.direction, SOUTH);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
}

Test(right_turn, south_becomes_west)
{
    ai_t ai = {.direction = SOUTH};
    int fds[2];
    command_t *cmd;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    cmd = make_turn_command(&ai, fds[1]);
    right(NULL, cmd);
    cr_assert_eq(ai.direction, WEST);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
}

Test(right_turn, west_becomes_north)
{
    ai_t ai = {.direction = WEST};
    int fds[2];
    command_t *cmd;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    cmd = make_turn_command(&ai, fds[1]);
    right(NULL, cmd);
    cr_assert_eq(ai.direction, NORTH);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
}


/*
** ─── SHARED HELPERS ──────────────────────────────────────────────────────────
*/
 
static args_t *make_args_world(size_t eggs)
{
    args_t *a = calloc(1, sizeof(args_t));
 
    a->width = 10;
    a->height = 10;
    a->freq = 100;
    a->eggs_per_team = eggs;
    a->team_names = malloc(sizeof(char *) * 3);
    a->team_names[0] = strdup("red");
    a->team_names[1] = strdup("blue");
    a->team_names[2] = NULL;
    a->parsing_status = true;
    a->verbose = false;
    return a;
}
 
static world_t *make_world(size_t eggs)
{
    args_t *args = make_args_world(eggs);
    tile_t *grid = NULL;
    world_t *world = NULL;
 
    alloc_grid_buffers(args->width * args->height, &grid, &world);
    world->args = args;
    build_grid(world);
    memset(world->resource_counts, 0, sizeof(world->resource_counts));
    world->ais = NULL;
    world->gui = NULL;
    world->teams = NULL;
    world->command_queue = NULL;
    world->old_time_commands = 0;
    world->next_command_time = -1;
    world->next_id = 0;
    world->connections = NULL;
    return world;
}
 
static void free_world_full(world_t *world)
{
    size_t n = world->args->width * world->args->height;
 
    for (size_t i = 0; i < n; i++) {
        player_t *p = world->grid[i].content->players;
        player_t *next;
        while (p) {
            next = p->next;
            free(p);
            p = next;
        }
        free(world->grid[i].content);
    }
    if (world->teams)
        free_teams(world->teams);
    if (world->ais)
        destroy_ais(world->ais);
    free(world->grid);
    free(world->args->team_names[0]);
    free(world->args->team_names[1]);
    free(world->args->team_names);
    free(world->args);
    free(world);
}
 
static int make_socketpair_client(int fds[2])
{
    return socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
}
 
static command_t *make_command(char **args, ai_t *ai, int fd)
{
    command_t *cmd = calloc(1, sizeof(command_t));
 
    cmd->arguments = args;
    cmd->player = ai;
    cmd->player_fd = fd;
    cmd->remaining_time = 100;
    cmd->next = NULL;
    cmd->handler = NULL;
    return cmd;
}
 
/*
** ─── str_to_resource ─────────────────────────────────────────────────────────
*/
 
Test(str_to_resource, food)
{
    cr_assert_eq(str_to_resource("food"), FOOD);
}
 
Test(str_to_resource, linemate)
{
    cr_assert_eq(str_to_resource("linemate"), LINEMATE);
}
 
Test(str_to_resource, deraumere)
{
    cr_assert_eq(str_to_resource("deraumere"), DERAUMERE);
}
 
Test(str_to_resource, sibur)
{
    cr_assert_eq(str_to_resource("sibur"), SIBUR);
}
 
Test(str_to_resource, mendiane)
{
    cr_assert_eq(str_to_resource("mendiane"), MENDIANE);
}
 
Test(str_to_resource, phiras)
{
    cr_assert_eq(str_to_resource("phiras"), PHIRAS);
}
 
Test(str_to_resource, thystame)
{
    cr_assert_eq(str_to_resource("thystame"), THYSTAME);
}
 
Test(str_to_resource, unknown_returns_minus_one)
{
    cr_assert_eq((int)str_to_resource("nope"), -1);
}
 
/*
** ─── take ────────────────────────────────────────────────────────────────────
*/
 
Test(take_cmd, takes_food_from_tile)
{
    int fds[2];
    world_t *world = make_world(1);
    tile_t *tile;
    ai_t ai = {0};
    char *args[] = {"Take", "food", NULL};
    command_t *cmd;
 
    make_socketpair_client(fds);
    init_teams(world);
    spawn_eggs(world);
    tile = get_tile(world, 0, 0);
    tile->content->resources[FOOD] = 5;
    ai.position = tile;
    ai.inventory[FOOD] = 0;
    cmd = make_command(args, &ai, fds[1]);
    take(world, cmd);
    cr_assert_eq(ai.inventory[FOOD], 1);
    cr_assert_eq(tile->content->resources[FOOD], 4);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
    free_world_full(world);
}
 
Test(take_cmd, ko_on_unknown_resource)
{
    int fds[2];
    world_t *world = make_world(1);
    tile_t *tile;
    ai_t ai = {0};
    char *args[] = {"Take", "unobtanium", NULL};
    command_t *cmd;
 
    make_socketpair_client(fds);
    init_teams(world);
    spawn_eggs(world);
    tile = get_tile(world, 0, 0);
    ai.position = tile;
    cmd = make_command(args, &ai, fds[1]);
    take(world, cmd);
    /* no crash and fd still valid */
    free(cmd);
    close(fds[0]);
    close(fds[1]);
    free_world_full(world);
}
 
Test(take_cmd, ko_when_resource_absent_from_tile)
{
    int fds[2];
    world_t *world = make_world(1);
    tile_t *tile;
    ai_t ai = {0};
    char *args[] = {"Take", "linemate", NULL};
    command_t *cmd;
 
    make_socketpair_client(fds);
    init_teams(world);
    spawn_eggs(world);
    tile = get_tile(world, 0, 0);
    tile->content->resources[LINEMATE] = 0;
    ai.position = tile;
    cmd = make_command(args, &ai, fds[1]);
    take(world, cmd);
    cr_assert_eq(ai.inventory[LINEMATE], 0);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
    free_world_full(world);
}
 
/*
** ─── set ─────────────────────────────────────────────────────────────────────
*/
 
Test(set_cmd, sets_resource_on_tile)
{
    int fds[2];
    world_t *world = make_world(1);
    tile_t *tile;
    ai_t ai = {0};
    char *args[] = {"Set", "linemate", NULL};
    command_t *cmd;
 
    make_socketpair_client(fds);
    init_teams(world);
    spawn_eggs(world);
    tile = get_tile(world, 0, 0);
    tile->content->resources[LINEMATE] = 0;
    ai.position = tile;
    ai.inventory[LINEMATE] = 3;
    cmd = make_command(args, &ai, fds[1]);
    set(world, cmd);
    cr_assert_eq(ai.inventory[LINEMATE], 2);
    cr_assert_eq(tile->content->resources[LINEMATE], 1);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
    free_world_full(world);
}
 
Test(set_cmd, ko_on_unknown_resource)
{
    int fds[2];
    world_t *world = make_world(1);
    tile_t *tile;
    ai_t ai = {0};
    char *args[] = {"Set", "unobtanium", NULL};
    command_t *cmd;
 
    make_socketpair_client(fds);
    init_teams(world);
    spawn_eggs(world);
    tile = get_tile(world, 0, 0);
    ai.position = tile;
    cmd = make_command(args, &ai, fds[1]);
    set(world, cmd);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
    free_world_full(world);
}
 
Test(set_cmd, ko_when_not_in_inventory)
{
    int fds[2];
    world_t *world = make_world(1);
    tile_t *tile;
    ai_t ai = {0};
    char *args[] = {"Set", "sibur", NULL};
    command_t *cmd;
 
    make_socketpair_client(fds);
    init_teams(world);
    spawn_eggs(world);
    tile = get_tile(world, 0, 0);
    ai.position = tile;
    ai.inventory[SIBUR] = 0;
    cmd = make_command(args, &ai, fds[1]);
    set(world, cmd);
    cr_assert_eq(tile->content->resources[SIBUR], 0);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
    free_world_full(world);
}
 
/*
** ─── inventory ───────────────────────────────────────────────────────────────
*/
 
Test(inventory_cmd, sends_inventory_string)
{
    int fds[2];
    world_t *world = make_world(1);
    ai_t ai = {0};
    char *args[] = {"Inventory", NULL};
    command_t *cmd;
    char buf[256] = {0};
 
    make_socketpair_client(fds);
    init_teams(world);
    spawn_eggs(world);
    ai.inventory[FOOD] = 10;
    ai.inventory[LINEMATE] = 2;
    cmd = make_command(args, &ai, fds[1]);
    inventory(world, cmd);
    read(fds[0], buf, sizeof(buf) - 1);
    cr_assert(strstr(buf, "food 10") != NULL);
    free(cmd);
    close(fds[0]);
    close(fds[1]);
    free_world_full(world);
}
 
/*
** ─── forward ─────────────────────────────────────────────────────────────────
*/
 
Test(forward_cmd, moves_north)
{
    int fds[2];
    world_t *world = make_world(1);
    tile_t *start;
    tile_t *expected_next;
    ai_t ai = {0};
    player_t *node;
    char *args[] = {"Forward", NULL};
    command_t *cmd;
 
    make_socketpair_client(fds);
    init_teams(world);
    spawn_eggs(world);
    start = get_tile(world, 5, 5);
    expected_next = start->north;
    node = calloc(1, sizeof(player_t));
    node->ai = &ai;
    node->next = start->content->players;
    start->content->players = node;
    ai.position = start;
    ai.direction = NORTH;
    cmd = make_command(args, &ai, fds[1]);
    forward(world, cmd);
    cr_assert_eq(ai.position, expected_next);
    /* clean up the player node from the destination tile */
    player_t *p = expected_next->content->players;
    player_t *prev = NULL;
    while (p && p->ai != &ai) { prev = p; p = p->next; }
    if (p) {
        if (prev) prev->next = p->next; else expected_next->content->players = p->next;
        free(p);
    }
    free(cmd);
    close(fds[0]);
    close(fds[1]);
    free_world_full(world);
}
 
/*
** ─── create_command ──────────────────────────────────────────────────────────
*/
 
Test(create_command, basic_creation)
{
    ai_t ai = {.is_incanting = false};
    char **args = malloc(sizeof(char *) * 2);
 
    args[0] = strdup("Forward");
    args[1] = NULL;
 
    command_t *cmd = create_command(args, &ai, 100, 5);
 
    cr_assert_not_null(cmd);
    cr_assert_eq(cmd->player, &ai);
    cr_assert_eq(cmd->remaining_time, 100);
    free(cmd->arguments[0]);
    free(cmd->arguments);
    free(cmd);
}
 
Test(create_command, null_player_non_refill_returns_null)
{
    char **args = malloc(sizeof(char *) * 2);
 
    args[0] = strdup("Forward");
    args[1] = NULL;
 
    command_t *cmd = create_command(args, NULL, 100, 5);
 
    cr_assert_null(cmd);
    free(args[0]);
    free(args);
}
 
Test(create_command, refill_with_null_player_succeeds)
{
    char **args = malloc(sizeof(char *) * 2);
 
    args[0] = strdup("REFILL");
    args[1] = NULL;
 
    command_t *cmd = create_command(args, NULL, 50, -1);
 
    cr_assert_not_null(cmd);
    free(cmd->arguments[0]);
    free(cmd->arguments);
    free(cmd);
}
 
Test(create_command, incanting_player_returns_null)
{
    ai_t ai = {.is_incanting = true};
    char **args = malloc(sizeof(char *) * 2);
 
    args[0] = strdup("Incantation");
    args[1] = NULL;
 
    command_t *cmd = create_command(args, &ai, 300, 5);
 
    cr_assert_null(cmd);
    free(args[0]);
    free(args);
}
 
/*
** ─── add_command / add_global_command ────────────────────────────────────────
*/
 
static world_t *make_bare_world(void)
{
    world_t *world = make_world(1);
 
    init_teams(world);
    spawn_eggs(world);
    return world;
}
 
static command_t *alloc_cmd(const char *name, ai_t *ai, long time, int fd)
{
    char **args = malloc(sizeof(char *) * 2);
 
    args[0] = strdup(name);
    args[1] = NULL;
    return create_command(args, ai, time, fd);
}
 
Test(add_global_command, inserts_single_command)
{
    world_t *world = make_bare_world();
    ai_t ai = {.is_incanting = false, .level = 1};
    int fds[2];
 
    make_socketpair_client(fds);
    ai.position = get_tile(world, 0, 0);
 
    command_t *cmd = alloc_cmd("Forward", &ai, 100, fds[1]);
    add_global_command(world, cmd);
    cr_assert_not_null(world->command_queue);
    cr_assert_eq(world->command_queue->player, &ai);
 
    free_array(world->command_queue->arguments);
    free(world->command_queue);
    world->command_queue = NULL;
    close(fds[0]);
    close(fds[1]);
    free_world_full(world);
}
 
Test(add_global_command, inserts_sorted_by_time)
{
    world_t *world = make_bare_world();
    ai_t ai1 = {.is_incanting = false, .level = 1};
    ai_t ai2 = {.is_incanting = false, .level = 1};
    int fds[2];
 
    make_socketpair_client(fds);
    ai1.position = get_tile(world, 0, 0);
    ai2.position = get_tile(world, 1, 0);
 
    command_t *cmd2 = alloc_cmd("Forward", &ai2, 200, fds[1]);
    command_t *cmd1 = alloc_cmd("Forward", &ai1, 100, fds[1]);
    add_global_command(world, cmd2);
    add_global_command(world, cmd1);
    /* cmd1 (t=100) should be head */
    cr_assert_eq(world->command_queue->player, &ai1);
    cr_assert_eq(world->command_queue->next->player, &ai2);
 
    command_t *c = world->command_queue;
    while (c) {
        command_t *n = c->next;
        free_array(c->arguments);
        free(c);
        c = n;
    }
    world->command_queue = NULL;
    close(fds[0]);
    close(fds[1]);
    free_world_full(world);
}
 
Test(add_command, second_command_goes_to_local_queue)
{
    world_t *world = make_bare_world();
    ai_t ai = {.is_incanting = false, .level = 1, .local_commands = NULL};
    int fds[2];
 
    make_socketpair_client(fds);
    ai.position = get_tile(world, 0, 0);
 
    command_t *cmd1 = alloc_cmd("Forward", &ai, 100, fds[1]);
    command_t *cmd2 = alloc_cmd("Left", &ai, 100, fds[1]);
    add_command(world, cmd1);
    add_command(world, cmd2);
    cr_assert_not_null(ai.local_commands);
    cr_assert_str_eq(ai.local_commands->arguments[0], "Left");
 
    free_array(world->command_queue->arguments);
    free(world->command_queue);
    world->command_queue = NULL;
    free_array(ai.local_commands->arguments);
    free(ai.local_commands);
    ai.local_commands = NULL;
    close(fds[0]);
    close(fds[1]);
    free_world_full(world);
}
 
/*
** ─── destroy_ai ──────────────────────────────────────────────────────────────
*/
 
Test(destroy_ai_test, destroys_live_ai)
{
    world_t *world = make_bare_world();
    ai_t *egg;
    ai_t *ai;
    team_t *team;
    player_t *node;
    tile_t *tile;
 
    /* bloom one egg into a real ai */
    egg = bloom_egg(world, "red");
    cr_assert_not_null(egg);
    egg->level = 1;
    ai = egg;
 
    /* place on tile */
    tile = get_tile(world, 3, 3);
    node = calloc(1, sizeof(player_t));
    node->ai = ai;
    node->next = tile->content->players;
    tile->content->players = node;
    ai->position = tile;
 
    /* link to team->players */
    for (team = world->teams; team; team = team->next)
        if (strcmp(team->name, "red") == 0)
            break;
    player_t *tnode = calloc(1, sizeof(player_t));
    tnode->ai = ai;
    tnode->next = team->players;
    team->players = tnode;
    ai->team = team;
 
    destroy_ai(world, &ai);
    cr_assert_null(ai);
    free_world_full(world);
}
 
Test(destroy_ai_test, handles_null_ai)
{
    world_t *world = make_bare_world();
    ai_t *ai = NULL;
 
    destroy_ai(world, &ai);   /* must not crash */
    free_world_full(world);
}
 
Test(destroy_ais_test, handles_null)
{
    destroy_ais(NULL);  /* must not crash */
}
 
Test(destroy_ais_test, frees_chain_with_local_commands)
{
    ai_t *a = calloc(1, sizeof(ai_t));
    ai_t *b = calloc(1, sizeof(ai_t));
    command_t *lc = calloc(1, sizeof(command_t));
    char **args = malloc(sizeof(char *) * 2);
 
    args[0] = strdup("Forward");
    args[1] = NULL;
    lc->arguments = args;
    lc->next = NULL;
    a->local_commands = lc;
    b->local_commands = NULL;
    a->next = b;
    destroy_ais(a);
    /* no crash = pass */
}
 
/*
** ─── guis_init_destroy ───────────────────────────────────────────────────────
*/
 
Test(destroy_gui, handles_null)
{
    destroy_gui(NULL);  /* must not crash */
}
 
Test(destroy_gui, frees_single)
{
    gui_t *g = calloc(1, sizeof(gui_t));
 
    g->fd = 5;
    g->next = NULL;
    destroy_gui(g);
}
 
Test(destroy_gui, frees_chain)
{
    gui_t *a = calloc(1, sizeof(gui_t));
    gui_t *b = calloc(1, sizeof(gui_t));
 
    a->fd = 1;
    b->fd = 2;
    a->next = b;
    destroy_gui(a);
}
 
Test(remove_gui, removes_head)
{
    gui_t *a = calloc(1, sizeof(gui_t));
    gui_t *b = calloc(1, sizeof(gui_t));
    gui_t *head = a;
 
    a->fd = 1; b->fd = 2;
    a->next = b;
    remove_gui(&head, a);
    cr_assert_eq(head, b);
    free(b);
}
 
Test(remove_gui, removes_tail)
{
    gui_t *a = calloc(1, sizeof(gui_t));
    gui_t *b = calloc(1, sizeof(gui_t));
    gui_t *head = a;
 
    a->fd = 1; b->fd = 2;
    a->next = b;
    remove_gui(&head, b);
    cr_assert_eq(head, a);
    cr_assert_null(a->next);
    free(a);
}
 
Test(new_gui, allocates_and_links)
{
    world_t *world = make_bare_world();
    connection_t conn = {.fd = 42};
 
    new_gui(world, &conn);
    cr_assert_not_null(world->gui);
    cr_assert_eq(world->gui->fd, 42);
    cr_assert_eq(conn.cli.gui, world->gui);
    destroy_gui(world->gui);
    world->gui = NULL;
    free_world_full(world);
}
 
/*
** ─── parse_teams ─────────────────────────────────────────────────────────────
*/
 
Test(parse_team_names, valid_two_teams)
{
    args_t *args = init_args();
    int optind = 3;
    char *argv[] = {"prog", "-n", "alpha", "beta", NULL};
    char **teams = parse_team_names(args, argv, 4, &optind);
 
    cr_assert_not_null(teams);
    cr_assert_str_eq(teams[0], "alpha");
    cr_assert_str_eq(teams[1], "beta");
    cr_assert_null(teams[2]);
    free_array(teams);
    destroy_args(args);
}
 
Test(parse_team_names, empty_name_fails)
{
    args_t *args = init_args();
    int optind = 2;
    char *argv[] = {"prog", "-n", "", NULL};
    char **teams = parse_team_names(args, argv, 3, &optind);

    cr_assert_null(teams);
    cr_assert_eq(args->parsing_status, false);
    destroy_args(args);
}
 
Test(parse_team_names, zero_teams_fails)
{
    args_t *args = init_args();
    int optind = 2;
    char *argv[] = {"prog", "-p", "4242", NULL};
    char **teams = parse_team_names(args, argv, 3, &optind);

    cr_assert_null(teams);
    cr_assert_eq(args->parsing_status, false);
    destroy_args(args);
}

Test(parse_team_names, duplicate_team_fails)
{
    args_t *args = init_args();
    int optind = 2;
    char *argv[] = {"prog", "-n", "alpha", "alpha", NULL};
    char **teams = parse_team_names(args, argv, 4, &optind);
 
    cr_assert_null(teams);
    cr_assert_eq(args->parsing_status, false);
    destroy_args(args);
}
 
Test(parse_team_names, single_team_valid)
{
    args_t *args = init_args();
    int optind = 3;
    char *argv[] = {"prog", "-n", "solo", NULL};
    char **teams = parse_team_names(args, argv, 3, &optind);
 
    cr_assert_not_null(teams);
    cr_assert_str_eq(teams[0], "solo");
    free_array(teams);
    destroy_args(args);
}
 
Test(parse_team_names, stops_at_next_flag)
{
    args_t *args = init_args();
    int optind = 3;
    char *argv[] = {"prog", "-n", "t1", "t2", "-p", "4242", NULL};
    char **teams = parse_team_names(args, argv, 6, &optind);
 
    cr_assert_not_null(teams);
    cr_assert_str_eq(teams[0], "t1");
    cr_assert_str_eq(teams[1], "t2");
    cr_assert_null(teams[2]);
    /* optind now points at "-p" */
    cr_assert_eq(optind, 4);
    free_array(teams);
    destroy_args(args);
}

Test(parse_team_names, next_flag_immediately_gives_no_teams)
{
    args_t *args = init_args();
    int optind = 3;
    char *argv[] = {"prog", "-n", "-p", NULL};
    char **teams = parse_team_names(args, argv, 3, &optind);

    cr_assert_null(teams);
    cr_assert_eq(args->parsing_status, false);
    destroy_args(args);
}
 
/*
** ─── ai_handler ──────────────────────────────────────────────────────────────
*/
 
Test(ai_handler, empty_command_returns_false)
{
    world_t *world = make_bare_world();
    ai_t ai = {.is_incanting = false, .level = 1};
    client_data_t data = {.ai = &ai};
 
    cr_assert_eq(ai_handler(world, data, "", 5), false);
    free_world_full(world);
}
 
Test(ai_handler, unknown_command_returns_false)
{
    world_t *world = make_bare_world();
    ai_t ai = {.is_incanting = false, .level = 1};
    client_data_t data = {.ai = &ai};
 
    cr_assert_eq(ai_handler(world, data, "FluxCapacitor", 5), false);
    free_world_full(world);
}
 
Test(ai_handler, null_ai_returns_false)
{
    world_t *world = make_bare_world();
    client_data_t data = {.ai = NULL};
 
    cr_assert_eq(ai_handler(world, data, "Forward", 5), false);
    free_world_full(world);
}
 
Test(ai_handler, valid_forward_enqueues)
{
    world_t *world = make_bare_world();
    ai_t ai = {.is_incanting = false, .level = 1};
    client_data_t data = {.ai = &ai};
    int fds[2];
 
    make_socketpair_client(fds);
    ai.position = get_tile(world, 0, 0);
    cr_assert_eq(ai_handler(world, data, "Forward", fds[1]), true);
    cr_assert_not_null(world->command_queue);
 
    command_t *c = world->command_queue;
    while (c) {
        command_t *n = c->next;
        free_array(c->arguments);
        free(c);
        c = n;
    }
    world->command_queue = NULL;
    close(fds[0]);
    close(fds[1]);
    free_world_full(world);
}
 
/*
** ─── send_to_guis ────────────────────────────────────────────────────────────
*/
 
Test(send_to_guis_test, null_gui_no_crash)
{
    send_to_guis(NULL, "hello %d", 42);
}
 
Test(send_to_guis_test, sends_to_one_gui)
{
    int fds[2];
    gui_t g = {0};
    char buf[64] = {0};
 
    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    g.fd = fds[1];
    g.next = NULL;
    send_to_guis(&g, "pin #%d %d %d", 1, 2, 3);
    read(fds[0], buf, sizeof(buf) - 1);
    cr_assert(strstr(buf, "pin #1 2 3") != NULL);
    close(fds[0]);
    close(fds[1]);
}
 
Test(send_to_guis_test, sends_to_two_guis)
{
    int fds1[2], fds2[2];
    gui_t g1 = {0}, g2 = {0};
    char b1[64] = {0}, b2[64] = {0};
 
    socketpair(AF_UNIX, SOCK_STREAM, 0, fds1);
    socketpair(AF_UNIX, SOCK_STREAM, 0, fds2);
    g1.fd = fds1[1]; g1.next = &g2;
    g2.fd = fds2[1]; g2.next = NULL;
    send_to_guis(&g1, "hello");
    read(fds1[0], b1, sizeof(b1) - 1);
    read(fds2[0], b2, sizeof(b2) - 1);
    cr_assert(strstr(b1, "hello") != NULL);
    cr_assert(strstr(b2, "hello") != NULL);
    close(fds1[0]); close(fds1[1]);
    close(fds2[0]); close(fds2[1]);
}
 
/*
** ─── refresh_ais ─────────────────────────────────────────────────────────────
*/
 
Test(refresh_ais_test, empty_world_no_crash)
{
    world_t *world = make_bare_world();
 
    world->ais = NULL;
    refresh_ais(world, NULL);
    free_world_full(world);
}
 
Test(refresh_ais_test, egg_level_0_not_processed)
{
    world_t *world = make_bare_world();
    ai_t egg = {0};
 
    egg.level = 0;
    egg.next = NULL;
    world->ais = &egg;
    refresh_ais(world, NULL);
    world->ais = NULL;   /* avoid double free */
    free_world_full(world);
}
 
Test(refresh_ais_test, ai_with_enough_food_survives)
{
    world_t *world = make_world(1);
    ai_t ai = {0};
    tile_t *tile;

    init_teams(world);
    spawn_eggs(world);
    tile = get_tile(world, 0, 0);
    ai.level = 1;
    ai.position = tile;
    ai.inventory[FOOD] = 999;
    ai.last_check = -1;
    ai.next = NULL;
    world->ais = &ai;
    refresh_ais(world, NULL);
    cr_assert(ai.inventory[FOOD] < 999);
    world->ais = NULL;
    free_world_full(world);
}

/*
** ─── destroy_ai: with global command in queue ────────────────────────────────
*/

Test(destroy_ai_test, removes_command_from_middle_of_queue)
{
    world_t *world = make_world(1);
    ai_t *ai1 = calloc(1, sizeof(ai_t));
    ai_t *ai2 = calloc(1, sizeof(ai_t));

    init_teams(world);
    spawn_eggs(world);
    ai1->level = 1; ai1->team = world->teams;
    ai2->level = 1; ai2->team = world->teams;
    ai1->position = get_tile(world, 0, 0);
    ai2->position = get_tile(world, 1, 0);
    ai1->next = ai2; world->ais = ai1;

    /* put ai1's command before ai2's in the queue */
    command_t *c1 = calloc(1, sizeof(command_t));
    command_t *c2 = calloc(1, sizeof(command_t));
    char **a1 = malloc(2 * sizeof(char *));
    char **a2 = malloc(2 * sizeof(char *));
    a1[0] = strdup("Forward"); a1[1] = NULL;
    a2[0] = strdup("Left");    a2[1] = NULL;
    c1->arguments = a1; c1->player = ai1; c1->next = c2;
    c2->arguments = a2; c2->player = ai2; c2->next = NULL;
    world->command_queue = c1;

    destroy_ai(world, &ai1);
    /* c1 must be gone, c2 must remain */
    cr_assert_not_null(world->command_queue);
    cr_assert_eq(world->command_queue->player, ai2);

    free_array(world->command_queue->arguments);
    free(world->command_queue);
    world->command_queue = NULL;
    world->ais = NULL;
    free(ai2);
    free_world_full(world);
}

Test(destroy_ai_test, ai_not_on_tile_no_crash)
{
    world_t *world = make_world(1);
    ai_t *ai = calloc(1, sizeof(ai_t));

    init_teams(world);
    spawn_eggs(world);
    ai->level = 1;
    ai->team = world->teams;
    ai->position = NULL;   /* no tile */
    ai->next = NULL;
    world->ais = ai;

    destroy_ai(world, &ai);
    cr_assert_null(ai);
    world->ais = NULL;
    free_world_full(world);
}

/*
** ─── refresh_ais: ai dies (no connection found) ──────────────────────────────
*/

Test(refresh_ais_test, ai_starves_no_connection)
{
    world_t *world = make_world(1);
    ai_t ai = {0};
    tile_t *tile;
    team_t *team;

    init_teams(world);
    spawn_eggs(world);
    tile = get_tile(world, 0, 0);
    for (team = world->teams; team; team = team->next)
        if (strcmp(team->name, "red") == 0) break;

    /* add ai to tile and team so destroy_ai doesn't segfault */
    player_t *tnode = calloc(1, sizeof(player_t));
    player_t *pnode = calloc(1, sizeof(player_t));
    tnode->ai = &ai; tnode->next = team->players; team->players = tnode;
    pnode->ai = &ai; pnode->next = tile->content->players;
    tile->content->players = pnode;

    ai.level = 1;
    ai.position = tile;
    ai.team = team;
    ai.inventory[FOOD] = 0;  /* will starve immediately */
    ai.last_check = -1;
    ai.next = NULL;
    world->ais = &ai;

    /* no connection list → handle_dead_ai logs and returns true (not found) */
    refresh_ais(world, NULL);

    world->ais = NULL;
    free_world_full(world);
}

Test(refresh_ais_test, ai_food_check_not_due)
{
    world_t *world = make_world(1);
    ai_t ai = {0};
    tile_t *tile;

    init_teams(world);
    spawn_eggs(world);
    tile = get_tile(world, 0, 0);
    ai.level = 1;
    ai.position = tile;
    ai.inventory[FOOD] = 50;
    ai.last_check = get_time_ms();  /* just checked → due = 0 */
    ai.next = NULL;
    world->ais = &ai;
    refresh_ais(world, NULL);
    /* food unchanged because period not elapsed */
    cr_assert_eq(ai.inventory[FOOD], 50);
    world->ais = NULL;
    free_world_full(world);
}

/*
** ─── zfork ───────────────────────────────────────────────────────────────────
*/

Test(zfork_cmd, spawns_egg_and_sends_ok)
{
    int fds[2];
    world_t *world = make_world(1);
    char buf[64] = {0};
    char *args[] = {"Fork", NULL};

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);

    ai_t *parent = bloom_egg(world, "red");
    cr_assert_not_null(parent);
    parent->level = 1;
    parent->position = get_tile(world, 2, 2);

    /* place parent on tile */
    player_t *pnode = calloc(1, sizeof(player_t));
    pnode->ai = parent;
    pnode->next = parent->position->content->players;
    parent->position->content->players = pnode;

    command_t *cmd = make_command(args, parent, fds[1]);
    zfork(world, cmd);
    read(fds[0], buf, sizeof(buf) - 1);
    cr_assert(strstr(buf, "ok") != NULL);
    free(cmd);
    close(fds[0]); close(fds[1]);
    free_world_full(world);
}

/*
** ─── look ────────────────────────────────────────────────────────────────────
*/

Test(look_cmd, level1_returns_bracket_string)
{
    int fds[2];
    world_t *world = make_world(1);
    char buf[256] = {0};
    char *args[] = {"Look", NULL};

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    cr_assert_not_null(ai);
    ai->level = 1;
    ai->direction = NORTH;
    ai->position = get_tile(world, 5, 5);

    player_t *pnode = calloc(1, sizeof(player_t));
    pnode->ai = ai;
    pnode->next = ai->position->content->players;
    ai->position->content->players = pnode;

    command_t *cmd = make_command(args, ai, fds[1]);
    look(world, cmd);
    read(fds[0], buf, sizeof(buf) - 1);
    cr_assert(buf[0] == '[');
    cr_assert(strchr(buf, ']') != NULL);
    free(cmd);
    close(fds[0]); close(fds[1]);

    /* cleanup pnode */
    player_t *p = ai->position->content->players;
    player_t *prev = NULL;
    while (p && p->ai != ai) { prev = p; p = p->next; }
    if (p) {
        if (prev) prev->next = p->next;
        else ai->position->content->players = p->next;
        free(p);
    }
    free_world_full(world);
}

Test(look_cmd, tile_with_resources_shows_them)
{
    int fds[2];
    world_t *world = make_world(1);
    char buf[512] = {0};
    char *args[] = {"Look", NULL};

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    cr_assert_not_null(ai);
    ai->level = 1;
    ai->direction = NORTH;
    tile_t *tile = get_tile(world, 5, 5);
    ai->position = tile;
    tile->content->resources[FOOD] = 2;
    tile->content->resources[LINEMATE] = 1;

    player_t *pnode = calloc(1, sizeof(player_t));
    pnode->ai = ai;
    pnode->next = tile->content->players;
    tile->content->players = pnode;

    command_t *cmd = make_command(args, ai, fds[1]);
    look(world, cmd);
    read(fds[0], buf, sizeof(buf) - 1);
    cr_assert(strstr(buf, "food") != NULL);
    cr_assert(strstr(buf, "linemate") != NULL);
    free(cmd);
    close(fds[0]); close(fds[1]);

    player_t *p = tile->content->players;
    if (p && p->ai == ai) {
        tile->content->players = p->next;
        free(p);
    }
    tile->content->resources[FOOD] = 0;
    tile->content->resources[LINEMATE] = 0;
    free_world_full(world);
}

/*
** ─── verify_incantation ──────────────────────────────────────────────────────
*/

static command_t *make_incan_cmd(ai_t *ai, int fd)
{
    command_t *cmd = calloc(1, sizeof(command_t));
    char **args = malloc(2 * sizeof(char *));

    args[0] = strdup("Incantation");
    args[1] = NULL;
    cmd->arguments = args;
    cmd->player = ai;
    cmd->player_fd = fd;
    return cmd;
}

Test(verify_incantation_test, level_0_returns_false)
{
    world_t *world = make_world(1);

    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    ai->level = 0;
    ai->position = get_tile(world, 0, 0);

    command_t *cmd = make_incan_cmd(ai, -1);
    cr_assert_eq(verify_incantation(world, cmd), false);
    free_array(cmd->arguments);
    free(cmd);
    free_world_full(world);
}

Test(verify_incantation_test, not_enough_resources_returns_false)
{
    int fds[2];
    world_t *world = make_world(1);

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    ai->level = 1;
    tile_t *tile = get_tile(world, 0, 0);
    ai->position = tile;
    /* level 1 needs 1 linemate — leave tile empty */
    memset(tile->content->resources, 0, sizeof(tile->content->resources));

    player_t *pnode = calloc(1, sizeof(player_t));
    pnode->ai = ai; pnode->next = tile->content->players;
    tile->content->players = pnode;

    command_t *cmd = make_incan_cmd(ai, fds[1]);
    cr_assert_eq(verify_incantation(world, cmd), false);
    free_array(cmd->arguments);
    free(cmd);

    player_t *p = tile->content->players;
    if (p && p->ai == ai) { tile->content->players = p->next; free(p); }
    close(fds[0]); close(fds[1]);
    free_world_full(world);
}

Test(verify_incantation_test, level_8_returns_false)
{
    world_t *world = make_world(1);

    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    ai->level = 8;
    ai->position = get_tile(world, 0, 0);

    command_t *cmd = make_incan_cmd(ai, -1);
    cr_assert_eq(verify_incantation(world, cmd), false);
    free_array(cmd->arguments);
    free(cmd);
    free_world_full(world);
}

/*
** ─── add_global_command: Incantation already incanting branch ────────────────
*/

Test(add_global_command, incantation_already_incanting_freed)
{
    world_t *world = make_world(1);
    int fds[2];

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    ai->level = 1;
    tile_t *tile = get_tile(world, 0, 0);
    ai->position = tile;
    ai->is_incanting = true;
    ai->incant_tile = tile;  /* already incanting HERE → pre_incantation_check returns false */

    char **args = malloc(2 * sizeof(char *));
    args[0] = strdup("Incantation");
    args[1] = NULL;
    command_t *cmd = create_command(args, ai, 300, fds[1]);
    /* create_command returns NULL when is_incanting=true, so set it manually */
    if (!cmd) {
        ai->is_incanting = false;
        cmd = create_command(args, ai, 300, fds[1]);
        ai->is_incanting = true;
        ai->incant_tile = tile;
    }
    cr_assert_not_null(cmd);
    add_global_command(world, cmd);
    /* cmd was freed inside because already_incanting_here = true */
    cr_assert_null(world->command_queue);
    close(fds[0]); close(fds[1]);
    free_world_full(world);
}

/*
** ─── spawn_resources ─────────────────────────────────────────────────────────
*/

Test(spawn_resources, populates_at_least_one_resource)
{
    world_t *world = make_world(1);

    init_teams(world);
    memset(world->resource_counts, 0, sizeof(world->resource_counts));
    cr_assert_eq(spawn_resources(world), true);
    size_t total = 0;
    for (int i = 0; i < RESOURCES_COUNT; i++)
        total += world->resource_counts[i];
    cr_assert_gt(total, 0u);
    free_world_full(world);
}

Test(spawn_resources, does_not_exceed_density_target)
{
    world_t *world = make_world(1);

    init_teams(world);
    memset(world->resource_counts, 0, sizeof(world->resource_counts));
    spawn_resources(world);
    size_t tiles = world->args->width * world->args->height;
    /* food density is 0.5, so count should be around tiles/2 */
    cr_assert_leq(world->resource_counts[FOOD],
        (size_t)(tiles * 0.5) + 1);
    free_world_full(world);
}

Test(spawn_resources, skips_spawn_when_already_at_target)
{
    world_t *world = make_world(1);
    size_t tiles = world->args->width * world->args->height;

    init_teams(world);
    /* pre-fill food to target so spawn_one_resource does nothing for food */
    world->resource_counts[FOOD] = (size_t)(tiles * 0.5) + 1;
    size_t before = world->resource_counts[FOOD];
    spawn_resources(world);
    cr_assert_eq(world->resource_counts[FOOD], before);
    free_world_full(world);
}

Test(spawn_resources, returns_true_on_valid_world)
{
    world_t *world = make_world(1);

    init_teams(world);
    cr_assert_eq(spawn_resources(world), true);
    free_world_full(world);
}

/*
** ─── connect_nbr ─────────────────────────────────────────────────────────────
*/

Test(connect_nbr_cmd, reports_free_slots)
{
    int fds[2];
    world_t *world = make_world(1);
    char buf[32] = {0};

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    cr_assert_not_null(ai);
    ai->level = 1;
    ai->team->free_slots = 3;

    char *args[] = {"Connect_nbr", NULL};
    command_t *cmd = make_command(args, ai, fds[1]);
    connect_nbr(world, cmd);
    read(fds[0], buf, sizeof(buf) - 1);
    cr_assert(strstr(buf, "3") != NULL);
    free(cmd);
    close(fds[0]); close(fds[1]);
    free_world_full(world);
}

/*
** ─── get_dir_from_vector: all 8 directions facing all 4 ways ─────────────────
*/

Test(get_dir_from_vector, facing_east_source_north_is_7)
{
    int_pair_t v = {0, 1};
    cr_assert_eq(get_dir_from_vector(&v, EAST), 7);
}

Test(get_dir_from_vector, facing_south_source_east_is_7)
{
    int_pair_t v = {1, 0};
    cr_assert_eq(get_dir_from_vector(&v, SOUTH), 7);
}

Test(get_dir_from_vector, facing_west_source_south_is_7)
{
    int_pair_t v = {0, -1};
    cr_assert_eq(get_dir_from_vector(&v, WEST), 7);
}

Test(get_dir_from_vector, diagonal_northeast_facing_north)
{
    int_pair_t v = {1, 1};
    cr_assert_eq(get_dir_from_vector(&v, NORTH), 2);
}

Test(get_dir_from_vector, diagonal_northwest_facing_north)
{
    int_pair_t v = {-1, 1};
    cr_assert_eq(get_dir_from_vector(&v, NORTH), 8);
}

Test(get_dir_from_vector, diagonal_southeast_facing_north)
{
    int_pair_t v = {1, -1};
    cr_assert_eq(get_dir_from_vector(&v, NORTH), 4);
}

Test(get_dir_from_vector, diagonal_southwest_facing_north)
{
    int_pair_t v = {-1, -1};
    cr_assert_eq(get_dir_from_vector(&v, NORTH), 6);
}

/*
** ─── logger_incantation ──────────────────────────────────────────────────────
*/

Test(logger_incantation_test, sends_pic_to_gui)
{
    int fds[2];
    gui_t gui = {0};
    char buf[128] = {0};
    tile_t *tile;
    world_t *world = make_world(1);

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);
    tile = get_tile(world, 3, 4);

    ai_t *ai = bloom_egg(world, "red");
    ai->level = 1;
    player_t *pnode = calloc(1, sizeof(player_t));
    pnode->ai = ai; pnode->next = tile->content->players;
    tile->content->players = pnode;

    gui.fd = fds[1]; gui.next = NULL;
    logger_incantation(&gui, tile, 1);
    read(fds[0], buf, sizeof(buf) - 1);
    cr_assert(strstr(buf, "pic") != NULL);

    tile->content->players = pnode->next;
    free(pnode);
    close(fds[0]); close(fds[1]);
    free_world_full(world);
}

Test(logger_incantation_test, null_gui_no_crash)
{
    world_t *world = make_world(1);
    init_teams(world);
    spawn_eggs(world);
    tile_t *tile = get_tile(world, 0, 0);
    logger_incantation(NULL, tile, 1);   /* must not crash */
    free_world_full(world);
}

/*
** ─── logger_client: write to closed fd covers the res < 0 branch ─────────────
*/

Test(logger_client_test, closed_fd_no_crash)
{
    int fds[2];
    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    close(fds[0]);
    close(fds[1]);
    /* writing to a closed fd triggers res < 0 branch */
    logger_client(fds[1], "hello %d", 42);
}

/*
** ─── incantation: successful level-up path ───────────────────────────────────
*/

Test(incantation_cmd, successful_levelup)
{
    int fds[2];
    world_t *world = make_world(1);
    connection_t conn = {0};
    connection_t *conn_ptr = &conn;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    cr_assert_not_null(ai);
    ai->level = 1;
    tile_t *tile = get_tile(world, 0, 0);
    ai->position = tile;

    /* place on tile */
    player_t *pnode = calloc(1, sizeof(player_t));
    pnode->ai = ai; pnode->next = tile->content->players;
    tile->content->players = pnode;

    /* set resources: level 1 needs 1 linemate */
    tile->content->resources[LINEMATE] = 1;

    /* wire connection so level_up_players can find the fd */
    conn.fd = fds[1]; conn.type = AI; conn.cli.ai = ai; conn.next = NULL;
    world->connections = &conn_ptr;

    command_t *cmd = make_incan_cmd(ai, fds[1]);
    incantation(world, cmd);
    cr_assert_eq(ai->level, 2u);
    free_array(cmd->arguments);
    free(cmd);

    /* cleanup pnode */
    player_t *p = tile->content->players;
    player_t *prev = NULL;
    while (p && p->ai != ai) { prev = p; p = p->next; }
    if (p) {
        if (prev) prev->next = p->next;
        else tile->content->players = p->next;
        free(p);
    }
    close(fds[0]); close(fds[1]);
    world->connections = NULL;
    free_world_full(world);
}

Test(incantation_cmd, fails_when_not_enough_players)
{
    int fds[2];
    world_t *world = make_world(1);

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    cr_assert_not_null(ai);
    connection_t conn = {0};
    connection_t *conn_ptr = &conn;
    conn.fd = fds[1]; conn.type = AI; conn.cli.ai = ai; conn.next = NULL;
    world->connections = &conn_ptr;
    ai->level = 2;  /* level 2 needs 2 players */
    tile_t *tile = get_tile(world, 0, 0);
    ai->position = tile;

    player_t *pnode = calloc(1, sizeof(player_t));
    pnode->ai = ai; pnode->next = tile->content->players;
    tile->content->players = pnode;
    /* resources for level 2: linemate=1, deraumere=1, sibur=1 */
    tile->content->resources[LINEMATE] = 1;
    tile->content->resources[DERAUMERE] = 1;
    tile->content->resources[SIBUR] = 1;
    /* only 1 player of level 2, need 2 → verify_incantation fails */

    command_t *cmd = make_incan_cmd(ai, fds[1]);
    incantation(world, cmd);
    cr_assert_eq(ai->level, 2u);  /* level unchanged */
    free_array(cmd->arguments);
    free(cmd);

    player_t *p = tile->content->players;
    if (p && p->ai == ai) { tile->content->players = p->next; free(p); }
    close(fds[0]); close(fds[1]);
    free_world_full(world);
}

Test(incantation_cmd, broadcasts_pie_to_guis_on_success)
{
    int fds[2];
    connection_t conn = {0};
    connection_t *conn_ptr = &conn;
    world_t *world = make_world(1);
    gui_t *gui = calloc(1, sizeof(gui_t));

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    cr_assert_not_null(ai);
    ai->level = 1;
    tile_t *tile = get_tile(world, 0, 0);
    ai->position = tile;

    player_t *pnode = calloc(1, sizeof(player_t));
    pnode->ai = ai; pnode->next = tile->content->players;
    tile->content->players = pnode;
    tile->content->resources[LINEMATE] = 1;

    conn.fd = fds[1]; conn.type = AI; conn.cli.ai = ai; conn.next = NULL;
    world->connections = &conn_ptr;
    gui->fd = fds[1];
    gui->next = NULL;
    world->gui = gui;

    command_t *cmd = make_incan_cmd(ai, fds[1]);
    incantation(world, cmd);
    cr_assert_eq(ai->level, 2u);
    free_array(cmd->arguments);
    free(cmd);

    player_t *p = tile->content->players;
    if (p && p->ai == ai) { tile->content->players = p->next; free(p); }
    destroy_gui(gui);
    world->gui = NULL;
    world->connections = NULL;
    close(fds[0]); close(fds[1]);
    free_world_full(world);
}

Test(incantation_cmd, broadcasts_pie_to_guis_on_failure)
{
    int fds[2];
    connection_t conn = {0};
    connection_t *conn_ptr = &conn;
    world_t *world = make_world(1);
    gui_t *gui = calloc(1, sizeof(gui_t));

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    cr_assert_not_null(ai);
    ai->level = 2;
    tile_t *tile = get_tile(world, 0, 0);
    ai->position = tile;

    player_t *pnode = calloc(1, sizeof(player_t));
    pnode->ai = ai; pnode->next = tile->content->players;
    tile->content->players = pnode;
    /* only 1 player of level 2, need 2 -> verify_incantation fails */
    tile->content->resources[LINEMATE] = 1;
    tile->content->resources[DERAUMERE] = 1;
    tile->content->resources[SIBUR] = 1;

    conn.fd = fds[1]; conn.type = AI; conn.cli.ai = ai; conn.next = NULL;
    world->connections = &conn_ptr;
    gui->fd = fds[1];
    gui->next = NULL;
    world->gui = gui;

    command_t *cmd = make_incan_cmd(ai, fds[1]);
    incantation(world, cmd);
    cr_assert_eq(ai->level, 2u);
    free_array(cmd->arguments);
    free(cmd);

    player_t *p = tile->content->players;
    if (p && p->ai == ai) { tile->content->players = p->next; free(p); }
    destroy_gui(gui);
    world->gui = NULL;
    world->connections = NULL;
    close(fds[0]); close(fds[1]);
    free_world_full(world);
}

/*
** ─── eject ───────────────────────────────────────────────────────────────────
*/

Test(eject_cmd, ejects_other_ai_to_front_tile)
{
    int fds_ejector[2], fds_victim[2];
    world_t *world = make_world(1);
    connection_t c_victim = {0};
    connection_t *conn_ptr = &c_victim;

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds_ejector);
    socketpair(AF_UNIX, SOCK_STREAM, 0, fds_victim);
    init_teams(world);
    spawn_eggs(world);

    ai_t *ejector = bloom_egg(world, "red");
    ai_t *victim  = bloom_egg(world, "blue");
    cr_assert_not_null(ejector); cr_assert_not_null(victim);
    ejector->level = 1; victim->level = 1;

    tile_t *src = get_tile(world, 5, 5);
    ejector->position = src;
    victim->position  = src;
    ejector->direction = NORTH;

    /* place both on src */
    player_t *pe = calloc(1, sizeof(player_t));
    player_t *pv = calloc(1, sizeof(player_t));
    pe->ai = ejector; pe->next = src->content->players; src->content->players = pe;
    pv->ai = victim;  pv->next = src->content->players; src->content->players = pv;

    /* wire victim's connection so communicate_eject finds the fd */
    c_victim.fd = fds_victim[1]; c_victim.type = AI;
    c_victim.cli.ai = victim; c_victim.next = NULL;
    world->connections = &conn_ptr;

    char *args[] = {"Eject", NULL};
    command_t *cmd = make_command(args, ejector, fds_ejector[1]);
    eject(world, cmd);

    cr_assert_neq(victim->position, src);  /* victim was moved */
    free(cmd);
    close(fds_ejector[0]); close(fds_ejector[1]);
    close(fds_victim[0]);  close(fds_victim[1]);
    world->connections = NULL;

    /* cleanup remaining nodes on tiles */
    player_t *p = src->content->players;
    while (p) { player_t *n = p->next; free(p); p = n; }
    src->content->players = NULL;
    tile_t *dst = src->north;
    p = dst->content->players;
    while (p) { player_t *n = p->next; free(p); p = n; }
    dst->content->players = NULL;
    free_world_full(world);
}

Test(eject_cmd, empty_tile_sends_ok)
{
    int fds[2];
    world_t *world = make_world(1);

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    cr_assert_not_null(ai);
    ai->level = 1;
    tile_t *src = get_tile(world, 5, 5);
    ai->position = src;
    ai->direction = NORTH;

    player_t *pe = calloc(1, sizeof(player_t));
    pe->ai = ai; pe->next = src->content->players; src->content->players = pe;

    char *args[] = {"Eject", NULL};
    command_t *cmd = make_command(args, ai, fds[1]);
    char buf[32] = {0};
    eject(world, cmd);
    read(fds[0], buf, sizeof(buf) - 1);
    cr_assert(strstr(buf, "ok") != NULL);
    free(cmd);

    player_t *p = src->content->players;
    while (p) { player_t *n = p->next; free(p); p = n; }
    src->content->players = NULL;
    close(fds[0]); close(fds[1]);
    free_world_full(world);
}

Test(verify_incantation_test, already_incanting_wrong_tile_returns_false)
{
    world_t *world = make_world(1);

    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    ai->level = 1;
    ai->position = get_tile(world, 0, 0);
    ai->is_incanting = true;
    ai->incant_tile = get_tile(world, 1, 1);

    command_t *cmd = make_incan_cmd(ai, -1);
    cr_assert_eq(verify_incantation(world, cmd), false);
    free_array(cmd->arguments);
    free(cmd);
    free_world_full(world);
}

Test(verify_incantation_test, already_incanting_same_tile_skips_set_state)
{
    int fds[2];
    world_t *world = make_world(1);

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds);
    init_teams(world);
    spawn_eggs(world);

    ai_t *ai = bloom_egg(world, "red");
    ai->level = 1;
    tile_t *tile = get_tile(world, 0, 0);
    ai->position = tile;
    ai->is_incanting = true;
    ai->incant_tile = tile;
    tile->content->resources[LINEMATE] = 1;

    player_t *pnode = calloc(1, sizeof(player_t));
    pnode->ai = ai; pnode->next = tile->content->players;
    tile->content->players = pnode;

    command_t *cmd = make_incan_cmd(ai, fds[1]);
    cr_assert_eq(verify_incantation(world, cmd), true);
    cr_assert_eq(ai->is_incanting, true);
    free_array(cmd->arguments);
    free(cmd);

    player_t *p = tile->content->players;
    if (p && p->ai == ai) { tile->content->players = p->next; free(p); }
    close(fds[0]); close(fds[1]);
    free_world_full(world);
}

/*
** ─── check_broadcast_command ──────────────────────────────────────────────────
*/

Test(check_broadcast_command, one_arg_returns_true)
{
    char *args[] = {"hello", NULL};

    cr_assert_eq(check_broadcast_command(args), true);
}

Test(check_broadcast_command, zero_args_returns_false)
{
    char *args[] = {NULL};

    cr_assert_eq(check_broadcast_command(args), false);
}

Test(check_broadcast_command, two_args_returns_false)
{
    char *args[] = {"hello", "world", NULL};

    cr_assert_eq(check_broadcast_command(args), false);
}

/*
** ─── broadcast ────────────────────────────────────────────────────────────────
*/

Test(broadcast_cmd, sends_message_to_other_ai)
{
    int fds_sender[2], fds_other[2];
    connection_t conn_sender = {0}, conn_other = {0};
    connection_t *conn_ptr = &conn_sender;
    world_t *world = make_world(2);
    char buf[64] = {0};

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds_sender);
    socketpair(AF_UNIX, SOCK_STREAM, 0, fds_other);
    init_teams(world);
    spawn_eggs(world);

    ai_t *sender = bloom_egg(world, "red");
    ai_t *other = bloom_egg(world, "red");
    cr_assert_not_null(sender);
    cr_assert_not_null(other);
    sender->level = 1;
    other->level = 1;
    other->direction = NORTH;
    sender->position = get_tile(world, 0, 0);
    other->position = get_tile(world, 1, 1);

    conn_sender.fd = fds_sender[1]; conn_sender.type = AI;
    conn_sender.cli.ai = sender; conn_sender.next = &conn_other;
    conn_other.fd = fds_other[1]; conn_other.type = AI;
    conn_other.cli.ai = other; conn_other.next = NULL;
    world->connections = &conn_ptr;

    char *args[] = {"Broadcast", "hi", NULL};
    command_t *cmd = make_command(args, sender, fds_sender[1]);
    broadcast(world, cmd);
    read(fds_other[0], buf, sizeof(buf) - 1);
    cr_assert(strstr(buf, "message") != NULL);
    memset(buf, 0, sizeof(buf));
    read(fds_sender[0], buf, sizeof(buf) - 1);
    cr_assert(strstr(buf, "ok") != NULL);

    free(cmd);
    world->connections = NULL;
    close(fds_sender[0]); close(fds_sender[1]);
    close(fds_other[0]); close(fds_other[1]);
    free_world_full(world);
}

Test(broadcast_cmd, skips_self_and_dead_and_gui_for_ai_loop)
{
    int fds_sender[2], fds_dead[2];
    connection_t conn_sender = {0}, conn_self = {0};
    connection_t conn_dead = {0}, conn_gui = {0};
    connection_t *conn_ptr = &conn_sender;
    world_t *world = make_world(2);
    gui_t gui = {0};

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds_sender);
    socketpair(AF_UNIX, SOCK_STREAM, 0, fds_dead);
    init_teams(world);
    spawn_eggs(world);

    ai_t *sender = bloom_egg(world, "red");
    ai_t *dead = bloom_egg(world, "red");
    cr_assert_not_null(sender);
    cr_assert_not_null(dead);
    sender->level = 1;
    dead->level = 0;
    sender->position = get_tile(world, 0, 0);
    dead->position = get_tile(world, 0, 0);

    gui.fd = fds_dead[1];
    gui.next = NULL;

    conn_sender.fd = fds_sender[1]; conn_sender.type = AI;
    conn_sender.cli.ai = sender; conn_sender.next = &conn_self;
    conn_self.fd = fds_sender[1]; conn_self.type = AI;
    conn_self.cli.ai = sender; conn_self.next = &conn_dead;
    conn_dead.fd = fds_dead[1]; conn_dead.type = AI;
    conn_dead.cli.ai = dead; conn_dead.next = &conn_gui;
    conn_gui.fd = fds_dead[1]; conn_gui.type = GUI;
    conn_gui.cli.gui = &gui; conn_gui.next = NULL;
    world->connections = &conn_ptr;

    char *args[] = {"Broadcast", "hi", NULL};
    command_t *cmd = make_command(args, sender, fds_sender[1]);
    broadcast(world, cmd);

    free(cmd);
    world->connections = NULL;
    close(fds_sender[0]); close(fds_sender[1]);
    close(fds_dead[0]); close(fds_dead[1]);
    free_world_full(world);
}

Test(broadcast_cmd, sends_pbc_to_gui_connection)
{
    int fds_sender[2], fds_gui[2];
    connection_t conn_sender = {0}, conn_gui = {0};
    connection_t *conn_ptr = &conn_sender;
    world_t *world = make_world(1);
    char buf[64] = {0};

    socketpair(AF_UNIX, SOCK_STREAM, 0, fds_sender);
    socketpair(AF_UNIX, SOCK_STREAM, 0, fds_gui);
    init_teams(world);
    spawn_eggs(world);

    ai_t *sender = bloom_egg(world, "red");
    cr_assert_not_null(sender);
    sender->level = 1;
    sender->position = get_tile(world, 0, 0);

    conn_sender.fd = fds_sender[1]; conn_sender.type = AI;
    conn_sender.cli.ai = sender; conn_sender.next = &conn_gui;
    conn_gui.fd = fds_gui[1]; conn_gui.type = GUI;
    conn_gui.cli.gui = NULL; conn_gui.next = NULL;
    world->connections = &conn_ptr;

    char *args[] = {"Broadcast", "hi", NULL};
    command_t *cmd = make_command(args, sender, fds_sender[1]);
    broadcast(world, cmd);
    read(fds_gui[0], buf, sizeof(buf) - 1);
    cr_assert(strstr(buf, "pbc") != NULL);

    free(cmd);
    world->connections = NULL;
    close(fds_sender[0]); close(fds_sender[1]);
    close(fds_gui[0]); close(fds_gui[1]);
    free_world_full(world);
}
