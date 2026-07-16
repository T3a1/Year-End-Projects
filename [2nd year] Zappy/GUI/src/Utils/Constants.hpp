/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Constants
*/

#pragma once

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_FPS 60
#define WINDOW_TITLE "TGEgYnJhbmxlIGRlcyBzb3VyaXMK"

#define MAP_TILE_GAP_RATIO 0.025f
#define MAP_TILE_HEIGHT_RATIO 0.05f
#define MAP_REF_TILE_SIZE 3.0f
#define MAP_TILE_HEIGHT 0.1f

#define CAM_FOV 45.f
#define CAM_HEIGHT 18.f
#define CAM_Z_OFFSET 32.f
#define CAM_BOUND_FACTOR 1.5f

#define DRONE_MOVE_SPEED 0.2f
#define DRONE_LOOK_SPEED 0.002f
#define DRONE_MIN_Y 0.7f

#define MAX_VERTICAL_ANGLE (PI * 0.49f)
#define FIXED_CAM_ZOOM_SPEED 2.0f
#define FIXED_CAM_MIN_HEIGHT 2.0f
#define FIXED_CAM_MAX_HEIGHT 200.0f

#define KICK_FALL_DELAY 0.9f

#define FOOD_MODEL_PATH "GUI/Assets/Entities/Items/Food.glb"
#define IDLE_MODEL_PATH "GUI/Assets/Entities/Player/Animations/Idle.glb"
#define RUN_MODEL_PATH "GUI/Assets/Entities/Player/Animations/Running.glb"
#define PICK_MODEL_PATH "GUI/Assets/Entities/Player/Animations/Picking.glb"
#define STANDUP_MODEL_PATH "GUI/Assets/Entities/Player/Animations/StandingUp.glb"
#define KICK_MODEL_PATH "GUI/Assets/Entities/Player/Animations/Kicking.glb"
#define EJECTED_MODEL_PATH "GUI/Assets/Entities/Player/Animations/Ejected.glb"
#define LOOK_MODEL_PATH "GUI/Assets/Entities/Player/Animations/Looking.glb"
#define DIE_MODEL_PATH "GUI/Assets/Entities/Player/Animations/Dying.glb"
#define INCANTATION_MODEL_PATH "GUI/Assets/Entities/Player/Animations/Incantation.glb"
#define WIN_MODEL_PATH "GUI/Assets/Entities/Player/Animations/Winning.glb"

#define LEVEL1_TEXTURE_PATH "GUI/Assets/Entities/Player/Textures/Level1.png"
#define LEVEL2_TEXTURE_PATH "GUI/Assets/Entities/Player/Textures/Level2.png"
#define LEVEL3_TEXTURE_PATH "GUI/Assets/Entities/Player/Textures/Level3.png"
#define LEVEL4_TEXTURE_PATH "GUI/Assets/Entities/Player/Textures/Level4.png"
#define LEVEL5_TEXTURE_PATH "GUI/Assets/Entities/Player/Textures/Level5.png"
#define LEVEL6_TEXTURE_PATH "GUI/Assets/Entities/Player/Textures/Level6.png"
#define LEVEL7_TEXTURE_PATH "GUI/Assets/Entities/Player/Textures/Level7.png"
#define LEVEL8_TEXTURE_PATH "GUI/Assets/Entities/Player/Textures/Level8.png"

#define GRASS_TEXTURE_PATH "GUI/Assets/Map/Grass.png"

#define LINEMATE_MODEL_PATH "GUI/Assets/Entities/Items/Linemate.glb"
#define DERAUMERE_MODEL_PATH "GUI/Assets/Entities/Items/Deraumere.glb"
#define SIBUR_MODEL_PATH "GUI/Assets/Entities/Items/Sibur.glb"
#define MENDIANE_MODEL_PATH "GUI/Assets/Entities/Items/Mendiane.glb"
#define PHIRAS_MODEL_PATH "GUI/Assets/Entities/Items/Phiras.glb"
#define THYSTAME_MODEL_PATH "GUI/Assets/Entities/Items/Thystame.glb"

#define ROCK1_MODEL_PATH "GUI/Assets/Entities/Props/Rock1.glb"
#define ROCK2_MODEL_PATH "GUI/Assets/Entities/Props/Rock2.glb"
#define ROCK3_MODEL_PATH "GUI/Assets/Entities/Props/Rock3.glb"

#define TREE1_MODEL_PATH "GUI/Assets/Entities/Props/Tree1.glb"
#define TREE2_MODEL_PATH "GUI/Assets/Entities/Props/Tree2.glb"
#define TREE3_MODEL_PATH "GUI/Assets/Entities/Props/Tree3.glb"
#define TREE4_MODEL_PATH "GUI/Assets/Entities/Props/Tree4.glb"
#define TREE5_MODEL_PATH "GUI/Assets/Entities/Props/Tree5.glb"
#define TREE6_MODEL_PATH "GUI/Assets/Entities/Props/Tree6.glb"

#define BUSH1_MODEL_PATH "GUI/Assets/Entities/Props/Bush1.glb"

#define EGG_MODEL_PATH "GUI/Assets/Entities/Player/Animations/StandingUp.glb"

#define RESOURCE_COUNT 7

#define PLAYER_MOVE_SPEED 1.f
#define PLAYER_BASE_ANIMATION_SPEED 60.0f
#define PLAYER_ARRIVAL_THRESHOLD 0.1f

#define MENU_MUSIC_PATH "GUI/Assets/Menu/RatDancing.mp3"

#define DEFAULT_FONT_PATH "GUI/Assets/Fonts/Medieval.ttf"

#define MENU_BUTTON_TEXTURE_PATH "GUI/Assets/Menu/Menu_Button.png"
#define MENU_BACKGROUND_TEXTURE_PATH "GUI/Assets/Menu/Menu_Background.jpg"
#define MENU_LOCK_TEXTURE_PATH "GUI/Assets/Menu/Lock.png"
#define MENU_RAT_TEXTURE_PATH "GUI/Assets/Menu/Rat.png"

#define PANEL_TEXTURE_PATH "GUI/Assets/Ui/Panels/panelBackground.png"

#define INFO_FOOD_TEXTURE_PATH "GUI/Assets/Ui/Resources/food.png"
#define INFO_LINEMATE_TEXTURE_PATH "GUI/Assets/Ui/Resources/linemate.png"
#define INFO_DERAUMERE_TEXTURE_PATH "GUI/Assets/Ui/Resources/deraumere.png"
#define INFO_SIBUR_TEXTURE_PATH "GUI/Assets/Ui/Resources/sibur.png"
#define INFO_MENDIANE_TEXTURE_PATH "GUI/Assets/Ui/Resources/mendiane.png"
#define INFO_PHIRAS_TEXTURE_PATH "GUI/Assets/Ui/Resources/phiras.png"
#define INFO_THYSTAME_TEXTURE_PATH "GUI/Assets/Ui/Resources/thystame.png"
#define INFO_TILE_TEXTURE_PATH "GUI/Assets/Map/Tile.png"
#define INFO_LEFT_ARROW_TEXTURE_PATH "GUI/Assets/Ui/Buttons/leftArrow.png"
#define INFO_RIGHT_ARROW_TEXTURE_PATH "GUI/Assets/Ui/Buttons/rightArrow.png"

#define INFO_PANEL_TEXTURE_PATH "GUI/Assets/Ui/Panels/infoPanel.png"

#define PLAYERS_BUTTON_TEXTURE_PATH "GUI/Assets/Ui/Buttons/PlayersButton.png"
#define CLOSE_BUTTON_TEXTURE_PATH "GUI/Assets/Ui/Buttons/ExitButton.png"
#define INFO_BUTTON_TEXTURE_PATH "GUI/Assets/Ui/Buttons/InfosButton.png"
#define CHAT_BUTTON_TEXTURE_PATH "GUI/Assets/Ui/Buttons/ChatButton.png"
#define GRID_BUTTON_TEXTURE_PATH "GUI/Assets/Ui/Buttons/GridButton.png"
#define CLOSE_CROSS_BUTTON_TEXTURE_PATH "GUI/Assets/Ui/Buttons/closeCrossButton.png"

#define BOOK_COVER_TEXTURE_PATH "GUI/Assets/Ui/Info/bookCover.png"
#define BOOK_PAGE_TEXTURE_PATH "GUI/Assets/Ui/Info/bookPage.png"
#define BOOK_PAGES_TEXTURE_PATH "GUI/Assets/Ui/Info/bookPages.png"
#define ABOUT_BOOKMARK_TEXTURE_PATH "GUI/Assets/Ui/Info/aboutBookmark.png"
#define CREDITS_BOOKMARK_TEXTURE_PATH "GUI/Assets/Ui/Info/creditsBookmark.png"
#define INPUTS_BOOKMARK_TEXTURE_PATH "GUI/Assets/Ui/Info/inputsBookmark.png"
#define SETTINGS_BOOKMARK_TEXTURE_PATH "GUI/Assets/Ui/Info/settingsBookmark.png"

#define SETTINGS_TICKBOX_TEXTURE_PATH "GUI/Assets/Ui/Settings/TickBox.png"
#define SETTINGS_FILLED_TICKBOX_TEXTURE_PATH "GUI/Assets/Ui/Settings/FilledTickBox.png"
#define SETTINGS_KEY_BUTTON "GUI/Assets/Ui/Settings/KeyButton.png"
#define SETTINGS_WINDOW_BUTTON_TEXTURE_PATH "GUI/Assets/Ui/Settings/WindowButton.png"
#define PLUS_ICON_TEXTURE_PATH "GUI/Assets/Ui/Settings/PlusIcon.png"
#define MINUS_ICON_TEXTURE_PATH "GUI/Assets/Ui/Settings/MinusIcon.png"


#define BARBIE_MUSIC_PATH "GUI/Assets/Sounds/Barbie.ogg"
#define MACARENA_MUSIC_PATH "GUI/Assets/Sounds/Macarena.ogg"
#define BLUE_MUSIC_PATH "GUI/Assets/Sounds/Blue.ogg"
#define MII_MUSIC_PATH "GUI/Assets/Sounds/Mii.ogg"
#define CHEVAUX_MUSIC_PATH "GUI/Assets/Sounds/Chevaux.ogg"
#define SOURIS_MUSIC_PATH "GUI/Assets/Sounds/Souris.ogg"

#define PAGE_FLIP_SOUND_PATH "GUI/Assets/Sounds/PageFlip.mp3"

#define LEVEL_1_SKIN_PATH "GUI/Assets/Ui/Models/level_1.png"
#define LEVEL_2_SKIN_PATH "GUI/Assets/Ui/Models/level_2.png"
#define LEVEL_3_SKIN_PATH "GUI/Assets/Ui/Models/level_3.png"
#define LEVEL_4_SKIN_PATH "GUI/Assets/Ui/Models/level_4.png"
#define LEVEL_5_SKIN_PATH "GUI/Assets/Ui/Models/level_5.png"
#define LEVEL_6_SKIN_PATH "GUI/Assets/Ui/Models/level_6.png"
#define LEVEL_7_SKIN_PATH "GUI/Assets/Ui/Models/level_7.png"
#define LEVEL_8_SKIN_PATH "GUI/Assets/Ui/Models/level_8.png"

#define MAX_VISIBILITY_DISTANCE 400.0f
#define MAX_BROADCASTS_IN_PANEL 10
