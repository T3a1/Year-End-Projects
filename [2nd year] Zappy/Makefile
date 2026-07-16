##
## EPITECH PROJECT, 2024
## Zappy
## File description:
## Root Makefile
##

BUILD_DIR = build

.PHONY: all zappy_server zappy_gui zappy_ai \
		clean fclean re tests_run coverage tests_clean

all: zappy_server zappy_gui zappy_ai
	@echo "All components compiled"

zappy_server:
	@make -C NET

zappy_gui:
	@mkdir -p $(BUILD_DIR)
	@cmake -B $(BUILD_DIR) -S GUI
	@cmake --build $(BUILD_DIR)

zappy_ai:
	@echo "Compiling zappy_ai..."
	@cp AI/main.py zappy_ai
	@chmod +x zappy_ai

zappy_ai_bonus:
	@echo "Compiling zappy_ai..."
	@cp AI/Bonus.py zappy_ai
	@chmod +x zappy_ai

clean:
	@make -C NET clean
	@rm -rf $(BUILD_DIR)
	@find . -type f -name "vgcore*" -delete
	@echo "Cleaning done"

fclean: clean
	@make -C NET fclean
# 	@make -C zappy_ai fclean
	@make -C NET/tests tests_clean
	@rm -f zappy_gui
	@rm -f zappy_ai

re: fclean all

make tests_run:
	@make -C NET/tests tests_run

make coverage:
	@make -C NET/tests coverage
