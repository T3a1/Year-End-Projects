##
## EPITECH PROJECT, 2026
## Zappy
## File description:
## Makefile for Server tests (multi-domain)
##

TCC		=	gcc

# ─── INCLUDE ──────────────────────────────────────────────────────────────────

INCLUDE	=	-I../Include		\
			-I../Include/Logger	\
			-I../Include/Parser	\
			-I../Include/Server	\
			-I../Include/Utils	\
			-I../Include/World	\
			-I../Include/Execution	\

# ─── TEST SOURCES ─────────────────────────────────────────────────────────────

# Original tests (network / parsing / connection)
TEST_NETWORK	:= network/test_network_parsing.c

# New domain tests
TEST_UTILS	:= utils/test_utils.c
TEST_PARSER	:= parser/test_parser.c
TEST_MAP	:= map/test_map.c
TEST_BLACKLIST	:= blacklist/test_blacklist.c
TEST_WORLD	:= world/test_world.c

ALL_TESTS	:= $(TEST_NETWORK) \
			   $(TEST_UTILS)   \
			   $(TEST_PARSER)  \
			   $(TEST_MAP)     \
			   $(TEST_BLACKLIST) \
			   $(TEST_WORLD)

# ─── SOURCES UNDER TEST ───────────────────────────────────────────────────────

# Shared (always needed: logging, free helpers)
SRC_COMMON	:=				\
	../Src/Logger/logger_server.c		\
	../Src/Logger/logger_client.c		\
	../Src/Logger/logger_incantation.c	\
	../Src/Utils/free_array.c		\
	../Src/Utils/get_integer.c		\
	../Src/Utils/int_to_str.c		\
	../Src/Utils/get_fd_from_connections.c	\
	../Src/Utils/send_to_guis.c		\
	../Src/Utils/get_time_ms.c		\

# Network / connection
SRC_NETWORK	:=						\
	../Src/Server/Network/Connection/get_connection.c	\
	../Src/Server/Network/Connection/connection_init_destroy.c		\
	../Src/Server/Network/disconnect_connection.c		\
	../Src/Parser/get_split_commands.c			\
	../Src/Parser/split_arguments.c				\

# Parser
SRC_PARSER	:=					\
	../Src/Parser/parse_integer.c			\
	../Src/Parser/parse_player_number.c		\
	../Src/Parser/parse_teams.c			\
	../Src/Server/Args/args_init_destroy.c		\

# Map / world geometry
SRC_MAP		:=						\
	../Src/World/Map/map_init_destroy.c				\
	../Src/World/Map/get_tiles.c				\
	../Src/World/Map/get_torus_shortest_path.c		\
	../Src/World/Map/interpret_sound_direction.c		\
	../Src/Execution/utils/move_from_directions.c		\

# World / teams / ais
SRC_WORLD	:=						\
	../Src/World/Teams/teams_init_destroy.c			\
	../Src/World/Ais/init.c					\
	../Src/World/Ais/bloom_egg.c				\
	../Src/World/Ais/is_available_team.c			\
	../Src/World/Ais/destroy.c				\
	../Src/World/Ais/refresh_ais.c				\
	../Src/World/Guis/guis_init_destroy.c			\
	../Src/World/Resources/spawn_resources.c		\
	../Src/Execution/ai/forward.c				\
	../Src/Execution/ai/left.c				\
	../Src/Execution/ai/right.c				\
	../Src/Execution/ai/look.c				\
	../Src/Execution/ai/connect_nbr.c			\
	../Src/Execution/ai/inventory.c			\
	../Src/Execution/ai/take.c				\
	../Src/Execution/ai/set.c				\
	../Src/Execution/ai/zfork.c				\
	../Src/Execution/ai/incantation.c			\
	../Src/Execution/ai/broadcast.c			\
	../Src/Execution/ai/eject.c				\
	../Src/Execution/gui/output/pbc.c			\
	../Src/Execution/command_handling/count_args.c		\
	../Src/Execution/command_handling/handle_ai_commands.c	\
	../Src/Execution/command_handling/add_command.c		\
	../Src/Execution/command_handling/create_command.c	\

# Blacklist
SRC_BLACKLIST	:=					\
	../Src/Server/Network/Blacklist/blacklist_init_destroy.c	\
	../Src/Server/Network/Blacklist/is_banned.c	\

SRC_UNDER_TEST	:= $(SRC_COMMON) $(SRC_NETWORK) $(SRC_PARSER) \
				   $(SRC_MAP) $(SRC_WORLD) $(SRC_BLACKLIST)

# ─── BUILD ────────────────────────────────────────────────────────────────────

TEST_BIN	:= unit_tests

CRITERION	:= -lcriterion
GCOV_FLAGS	:= --coverage -fprofile-arcs -ftest-coverage
GCOV_DIR	:= coverage_html

# ─── PER-DOMAIN BINARIES ─────────────────────────────────────────────────────

# At top of Makefile
GCOV_NETWORK_DIR := coverage_network
GCOV_WORLD_DIR := coverage_world

$(BIN_NETWORK):
	@mkdir -p $(GCOV_NETWORK_DIR)
	@$(TCC) ... GCOV_OUTPUT_DIR=$(GCOV_NETWORK_DIR) ...
BIN_NETWORK	:= unit_tests_network
BIN_UTILS	:= unit_tests_utils
BIN_PARSER	:= unit_tests_parser
BIN_MAP		:= unit_tests_map
BIN_BLACKLIST	:= unit_tests_blacklist
BIN_WORLD	:= unit_tests_world

# ─── TARGETS ─────────────────────────────────────────────────────────────────

# Build and run all tests in one binary
tests_run: $(TEST_BIN)
	@./$(TEST_BIN) --verbose ; true

# In the $(TEST_BIN) target, REMOVE GCOV_FLAGS for now to stop collisions
$(TEST_BIN): $(ALL_TESTS) $(SRC_UNDER_TEST)
	@$(TCC) $(INCLUDE) \
		$(ALL_TESTS) $(SRC_UNDER_TEST) \
		$(CRITERION) -o $(TEST_BIN)
	@printf "[OK] Test binary compiled (no coverage to avoid collisions): $(TEST_BIN)\n"

# If you MUST have coverage for the full run, you need a more complex setup 
# with separate output directories per source file, which is likely overkill for now.

# Per-domain convenience targets
network: $(BIN_NETWORK)
	@./$(BIN_NETWORK) --verbose ; true

$(BIN_NETWORK): $(TEST_NETWORK) $(SRC_COMMON) $(SRC_NETWORK)
	@$(TCC) $(INCLUDE) $(GCOV_FLAGS) \
		$(TEST_NETWORK) $(SRC_COMMON) $(SRC_NETWORK) \
		$(CRITERION) -o $(BIN_NETWORK)

utils: $(BIN_UTILS)
	@./$(BIN_UTILS) --verbose ; true

$(BIN_UTILS): $(TEST_UTILS) $(SRC_COMMON) $(SRC_NETWORK)
	@$(TCC) $(INCLUDE) $(GCOV_FLAGS) \
		$(TEST_UTILS) $(SRC_COMMON) $(SRC_NETWORK) \
		$(CRITERION) -o $(BIN_UTILS)

parser: $(BIN_PARSER)
	@./$(BIN_PARSER) --verbose ; true

$(BIN_PARSER): $(TEST_PARSER) $(SRC_COMMON) $(SRC_PARSER)
	@$(TCC) $(INCLUDE) $(GCOV_FLAGS) \
		$(TEST_PARSER) $(SRC_COMMON) $(SRC_PARSER) \
		$(CRITERION) -o $(BIN_PARSER)

map: $(BIN_MAP)
	@./$(BIN_MAP) --verbose ; true

$(BIN_MAP): $(TEST_MAP) $(SRC_COMMON) $(SRC_MAP)
	@$(TCC) $(INCLUDE) $(GCOV_FLAGS) \
		$(TEST_MAP) $(SRC_COMMON) $(SRC_MAP) \
		$(CRITERION) -o $(BIN_MAP)

blacklist: $(BIN_BLACKLIST)
	@./$(BIN_BLACKLIST) --verbose ; true

$(BIN_BLACKLIST): $(TEST_BLACKLIST) $(SRC_COMMON) $(SRC_BLACKLIST)
	@$(TCC) $(INCLUDE) $(GCOV_FLAGS) \
		$(TEST_BLACKLIST) $(SRC_COMMON) $(SRC_BLACKLIST) \
		$(CRITERION) -o $(BIN_BLACKLIST)

world: $(BIN_WORLD)
	@./$(BIN_WORLD) --verbose ; true

$(BIN_WORLD): $(TEST_WORLD) $(SRC_COMMON) $(SRC_MAP) $(SRC_WORLD) \
			  $(SRC_PARSER) $(SRC_BLACKLIST)
	@$(TCC) $(INCLUDE) $(GCOV_FLAGS) \
		$(TEST_WORLD) $(SRC_COMMON) $(SRC_MAP) $(SRC_WORLD) \
		$(SRC_PARSER) $(SRC_BLACKLIST) \
		$(CRITERION) -o $(BIN_WORLD)

# ─── COVERAGE ────────────────────────────────────────────────────────────────

GCOV_DATA_DIR   := gcov_data
TEST_BIN_COV    := unit_tests_cov

$(TEST_BIN_COV): $(ALL_TESTS) $(SRC_UNDER_TEST)
	@$(TCC) $(INCLUDE) $(GCOV_FLAGS) \
		$(ALL_TESTS) $(SRC_UNDER_TEST) \
		$(CRITERION) -o $(TEST_BIN_COV)
	@printf "[OK] Coverage binary compiled: $(TEST_BIN_COV)\n"

coverage: $(TEST_BIN_COV)
	@./$(TEST_BIN_COV) --verbose; true
	@mkdir -p $(GCOV_DIR)
	@gcovr --root .. \
		--filter "\.\.\/Src\/" \
		--html --html-details \
		-o $(GCOV_DIR)/index.html
	@printf "[OK] Coverage report: $(GCOV_DIR)/index.html\n"

coverage_clean:
	@$(RM) -r $(GCOV_DIR) $(GCOV_DATA_DIR) $(TEST_BIN_COV)
	@find . -name "*.gcda" -delete
	@find . -name "*.gcno" -delete
	@printf "[INFO] Coverage artefacts removed.\n"

tests_clean: coverage_clean
	@$(RM) $(TEST_BIN) $(BIN_NETWORK) $(BIN_UTILS) $(BIN_PARSER) \
		   $(BIN_MAP) $(BIN_BLACKLIST) $(BIN_WORLD)
	@printf "[INFO] Test binaries removed.\n"

.PHONY: tests_run network utils parser map blacklist world \
		coverage coverage_clean tests_clean
