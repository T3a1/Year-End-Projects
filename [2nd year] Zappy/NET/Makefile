##
## EPITECH PROJECT, 2024
## Zappy Server
## File description:
## Makefile
##

SRC	=	Src/main.c												\
		Src/Logger/display_help.c 								\
		Src/Logger/display_config.c 							\
		Src/Logger/logger_server.c								\
		Src/Logger/logger_client.c								\
		Src/Logger/logger_incantation.c							\
		Src/Logger/display_usage.c								\
		Src/Server/server_init_destroy.c						\
		Src/Server/run_server.c									\
		Src/Server/Args/args_init_destroy.c						\
		Src/Server/Network/network_init_destroy.c				\
		Src/Server/Network/new_connection.c						\
		Src/Server/Network/get_request.c						\
		Src/Server/Network/disconnect_connection.c				\
		Src/Server/Network/Blacklist/blacklist_init_destroy.c	\
		Src/Server/Network/Blacklist/is_banned.c				\
		Src/Server/Network/Connection/connection_init_destroy.c	\
		Src/Server/Network/Connection/get_connection.c			\
		Src/Parser/parse_args.c									\
		Src/Parser/parse_integer.c								\
		Src/Parser/parse_teams.c								\
		Src/Parser/parse_player_number.c						\
		Src/Parser/split_arguments.c							\
		Src/Parser/get_split_commands.c							\
		Src/Utils/free_array.c									\
		Src/Utils/int_to_str.c									\
		Src/Utils/get_integer.c									\
		Src/Utils/send_to_guis.c								\
		Src/Utils/get_time_ms.c									\
		Src/World/world_init_destroy.c							\
		Src/World/Ais/init.c									\
		Src/World/Ais/destroy.c									\
		Src/World/Ais/bloom_egg.c								\
		Src/World/Ais/is_available_team.c						\
		Src/World/Guis/guis_init_destroy.c						\
		Src/World/Map/map_init_destroy.c						\
		Src/World/Map/get_tiles.c								\
		Src/World/Resources/spawn_resources.c					\
		Src/World/Teams/teams_init_destroy.c					\
		Src/Execution/command_handling/identify_and_exec.c		\
		Src/Execution/command_handling/handle_ai_commands.c		\
		Src/Execution/command_handling/handle_gui_commands.c	\
		Src/Execution/command_handling/handle_unknown.c			\
		Src/Execution/command_handling/add_command.c			\
		Src/Execution/command_handling/create_command.c			\
		Src/Execution/command_handling/refresh_commands.c		\
		Src/Execution/command_handling/execute_command.c		\
		Src/Execution/command_handling/count_args.c				\
		Src/Execution/gui/input/bct.c							\
		Src/Execution/gui/input/mct.c							\
		Src/Execution/gui/input/msz.c							\
		Src/Execution/gui/input/pin.c							\
		Src/Execution/gui/input/plv.c							\
		Src/Execution/gui/input/ppo.c							\
		Src/Execution/gui/input/sgt.c							\
		Src/Execution/gui/input/sst.c							\
		Src/Execution/gui/input/tna.c							\
		Src/Execution/gui/output/enw.c							\
		Src/Execution/gui/output/pnw.c							\
		Src/Execution/gui/output/pex.c							\
		Src/Execution/gui/output/pbc.c							\
		Src/Execution/gui/output/pic.c							\
 		Src/Execution/gui/output/pie.c							\
		Src/Execution/gui/output/pdr.c							\
		Src/Execution/gui/output/pgt.c							\
		Src/Execution/gui/output/pdi.c							\
		Src/Execution/gui/output/pfk.c							\
		Src/Execution/gui/output/ebo.c							\
		Src/Execution/gui/output/edi.c							\
		Src/Execution/gui/graphic.c								\
		Src/Execution/ai/look.c									\
		Src/Execution/utils/move_from_directions.c				\
		Src/Execution/ai/connect_nbr.c							\
		Src/Execution/ai/forward.c								\
		Src/Execution/ai/right.c								\
		Src/Execution/ai/left.c									\
		Src/Execution/ai/inventory.c							\
		Src/Execution/ai/incantation.c							\
		Src/World/Map/get_torus_shortest_path.c					\
		Src/World/Map/interpret_sound_direction.c				\
		Src/Execution/ai/broadcast.c							\
		Src/Execution/ai/eject.c								\
		Src/World/Ais/refresh_ais.c								\
		Src/World/Ais/win.c										\
		Src/Utils/get_fd_from_connections.c						\
		Src/Execution/ai/set.c									\
		Src/Execution/ai/take.c									\
		Src/Execution/ai/zfork.c								\

INCLUDE	=	-IInclude			\
			-IInclude/Logger	\
			-IInclude/Parser	\
			-IInclude/Server	\
			-IInclude/Utils		\
			-IInclude/World		\
			-IInclude/Execution	\

CC	=	epiclang

OBJ	=	$(SRC:.c=.o)

NAME	=	zappy_server

CFLAGS	=	$(INCLUDE) -Wall -Wextra

DOCUMENTATION_NAME	=	TECHNICAL_SERVER_DOCUMENTATION.pdf

OUTDIR	= ..

GREEN	=	\033[0;32m
YELLOW	=	\033[0;33m
MAGENTA	=	\033[0;35m
BLUE	=	\033[0;36m
RESET	=	\033[0m

TOTAL	=	$(words $(SRC))
CURRENT	=	0

all:	$(NAME)

$(NAME):	$(OBJ)
	@$(CC) $(CFLAGS) -o $(OUTDIR)/$(NAME) $(OBJ)
	@printf "$(GREEN)[OK]$(RESET) Binary $(BLUE)'$(NAME)'$(RESET) compiled successfully.\n"

%.o: %.c
	$(eval CURRENT := $(shell echo $$(($(CURRENT) + 1))))
	@printf "$(BLUE)[%3d%%]$(RESET) Compiling $(BLUE)$<$(RESET)\n" \
		$$(($(CURRENT) * 100 / $(TOTAL)))
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)
	@rm -f $(DOCUMENTATION_NAME)
	@printf "$(YELLOW)[INFO]$(RESET) Object files removed.\n"

fclean: clean
	@rm -f $(OUTDIR)/$(NAME)
	@printf "$(YELLOW)[INFO]$(RESET) Binary $(BLUE)'$(NAME)'$(RESET) removed.\n"

re: fclean all
	@printf "$(BLUE)[INFO]$(RESET) Project rebuilt successfully.\n"

documentation:
	@doxygen Doxfile
	@$(MAKE) -C ./latex
	@mv ./latex/refman.pdf ./$(DOCUMENTATION_NAME)
	@rm -rf ./latex
	@printf "$(GREEN)[OK]$(RESET) Documentation generated successfully.\n"

debug: CFLAGS += -g
debug: fclean $(NAME)
	@printf "$(MAGENTA)[OK]$(RESET) Debug Binary $(BLUE)'$(NAME)'$(RESET) compiled successfully.\n"
