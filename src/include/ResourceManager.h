/**
 * @file ResourceManager.h
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager struct and function declarations.
 * 
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "raylib/raylib.h"

typedef struct ResourceManager {

    Texture2D texturaJogador;
    Texture2D texturaBadniks;
    Texture2D texturaBadniksBase;
    Texture2D texturaItens;
    Texture2D texturaShields;
    Texture2D texturaObjetosBaseS1;

    Texture2D texturaTerreno;
    Texture2D texturasFundo[2];
    Texture2D texturaHud;

    Sound somAnel;
    Sound somFrenagem;
    Sound somHitComAnel;
    Sound somHitInimigo;
    Sound somMorte;
    Sound somPulo;

    Music musicaFase01;
    Music musicaFase02;
    Music musicaInvencibilidade;

} ResourceManager;

/**
 * @brief Global ResourceManager instance.
 */
extern ResourceManager rm;

/**
 * @brief Load global game resources, linking them in the global instance of
 * ResourceManager called rm.
 */
void loadResourcesResourceManager( void );

/**
 * @brief Unload global game resources.
 */
void unloadResourcesResourceManager( void );