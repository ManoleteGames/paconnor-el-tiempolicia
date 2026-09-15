cmake_minimum_required(VERSION 3.7)
project(dos-app)

set(CMAKE_GENERATOR Ninja)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
add_compile_options(-Wall -std=gnu99 -save-temps)

file(GLOB sources CONFIGURE_DEPENDS "src/*.c" "src/*.cpp" "src/*.h" "src/**/*.c" "src/**/*.cpp" "src/**/*.h" "src/**/**/*.c" "src/**/**/*.cpp" "src/**/**/*.h" )

add_executable(paconnor ${sources})

add_custom_target(extender
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/utils/CWSDPMI.EXE
    ${CMAKE_BINARY_DIR}/   
)

add_custom_target(binaries
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/binaries/BINARIES.DAT
    ${CMAKE_BINARY_DIR}/   
)

add_custom_target(fonts
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/fonts/FONTS.DAT
    ${CMAKE_BINARY_DIR}/
)

add_custom_target(images
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/logo/ILOGO.DAT
    ${CMAKE_BINARY_DIR}/
     COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/intro/INTRO.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/menu/IMENU.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/scn1/ISCN1.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/scn1/OSCN1.DAT
    ${CMAKE_BINARY_DIR}/  
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/scn2/ISCN2.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/scn2/OSCN2.DAT
    ${CMAKE_BINARY_DIR}/  
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/scn3/ISCN3.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/scn3/OSCN3.DAT
    ${CMAKE_BINARY_DIR}/  
     COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/scn4/ISCN4.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/scn4/OSCN4.DAT
    ${CMAKE_BINARY_DIR}/  
     COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/scn5/ISCN5.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/scn5/OSCN5.DAT
    ${CMAKE_BINARY_DIR}/  
     COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/scn6/ISCN6.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/scn6/OSCN6.DAT
    ${CMAKE_BINARY_DIR}/  
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/images/end/END.DAT
    ${CMAKE_BINARY_DIR}/  
)

add_custom_target(maps
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/maps/scn1_1/MAPSCN11.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/maps/scn1_2/MAPSCN12.DAT
    ${CMAKE_BINARY_DIR}/  
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/maps/scn2_1/MAPSCN21.DAT
    ${CMAKE_BINARY_DIR}/      
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/maps/scn3_1/MAPSCN31.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/maps/scn3_2/MAPSCN32.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/maps/scn4_1/MAPSCN41.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/maps/scn4_2/MAPSCN42.DAT
    ${CMAKE_BINARY_DIR}/    
     COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/maps/scn5_1/MAPSCN51.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/maps/scn5_2/MAPSCN52.DAT
    ${CMAKE_BINARY_DIR}/   
     COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/maps/scn6_1/MAPSCN61.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/maps/scn6_2/MAPSCN62.DAT
    ${CMAKE_BINARY_DIR}/   
)

add_custom_target(sounds  
     COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sounds/MSCENE1.DAT
    ${CMAKE_BINARY_DIR}/
     COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sounds/MSCENE2.DAT
    ${CMAKE_BINARY_DIR}/
      COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sounds/MUSICSPK.DAT
    ${CMAKE_BINARY_DIR}/
)

add_custom_target(tilesets
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/tilesets/scn1_1/TSCN11.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/tilesets/scn1_2/TSCN12.DAT
    ${CMAKE_BINARY_DIR}/ 
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/tilesets/scn2_1/TSCN21.DAT
    ${CMAKE_BINARY_DIR}/ 
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/tilesets/scn3_1/TSCN31.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/tilesets/scn3_2/TSCN32.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/tilesets/scn4_1/TSCN41.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/tilesets/scn4_2/TSCN42.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/tilesets/scn5_1/TSCN51.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/tilesets/scn5_2/TSCN52.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/tilesets/scn6_1/TSCN61.DAT
    ${CMAKE_BINARY_DIR}/    
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/tilesets/scn6_2/TSCN62.DAT
    ${CMAKE_BINARY_DIR}/    
)

add_custom_target(sprites
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/intro/ISPR.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/actor/ACTOR.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/cursor/CURSOR.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/objects/object1/OBJECT1.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/objects/object2/OBJECT2.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/objects/object3/OBJECT3.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/objects/object4/OBJECT4.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/objects/object5/OBJECT5.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/misc/MISC.DAT
    ${CMAKE_BINARY_DIR}/
     COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/effects/EFFECTS.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/bullets/BULLETS.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/panel/PANEL.DAT    
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/items/ITEMS.DAT    
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/enemies/enemy1/ENEMY1.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/enemies/enemy2/ENEMY2.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/enemies/enemy3/ENEMY3.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/enemies/enemy4/ENEMY4.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/enemies/enemy5/ENEMY5.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/enemies/enemy6/ENEMY6.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/enemies/enemy7/ENEMY7.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/enemies/enemy8/ENEMY8.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/enemies/enemy9/ENEMY9.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/enemies/enemy10/ENEMY10.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/enemies/enemy11/ENEMY11.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/enemies/enemy12/ENEMY12.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/boss/boss1/BOSS1.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/boss/boss2/BOSS2.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/boss/boss3/BOSS3.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/boss/boss4/BOSS4.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/npcs/npc1/NPC1.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/npcs/npc2/NPC2.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/npcs/npc3/NPC3.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/npcs/npc4/NPC4.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/npcs/npc5/NPC5.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/npcs/npc6/NPC6.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/npcs/npc7/NPC7.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/npcs/npc8/NPC8.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/menu/SMENU.DAT
    ${CMAKE_BINARY_DIR}/
)

add_custom_target(palettes
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/palettes/PALETTES.DAT
    ${CMAKE_BINARY_DIR}/
)

add_custom_target(dialogs
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/text/ES/DIALOGES.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/text/EN/DIALOGEN.DAT
    ${CMAKE_BINARY_DIR}/
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/text/FR/DIALOGFR.DAT
    ${CMAKE_BINARY_DIR}/
)

add_custom_target(settings
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/settings/SETTINGS.CFG
    ${CMAKE_BINARY_DIR}/
)

#add_dependencies(main assets)
add_dependencies(paconnor extender binaries images fonts maps tilesets sprites palettes dialogs sounds settings)