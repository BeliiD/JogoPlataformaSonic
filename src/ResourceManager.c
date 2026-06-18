/**
 * @file ResourceManager.c
 * @author Prof. Dr. David Buzatto
 * @brief ResourceManager implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "ResourceManager.h"
#include "Utils.h"

ResourceManager rm = { 0 };

void loadResourcesResourceManager( void ) {

    rm.texturaJogador = carregarTexturaAlterandoCores( 
        "resources/imagens/sprites/sonic.png",
        (Color[]) {
            { 37, 102, 26, 255 },
            { 13, 72, 7, 255 },
        },
        (Color[]) {
            BLANK,
            BLANK,
        },
        2
    );

    rm.texturaBadniks = carregarTexturaAlterandoCores( 
        "resources/imagens/sprites/badniks.png",
        (Color[]) {
            { 13, 72, 7, 255 },
        },
        (Color[]) {
            BLANK,
        },
        1
    );

    rm.texturaBadniksBase = carregarTexturaAlterandoCores( 
        "resources/imagens/sprites/badniks-base.png",
        (Color[]) {
            { 13, 72, 7, 255 },
            { 37, 102, 26, 255 },
        },
        (Color[]) {
            BLANK,
            BLANK,
        },
        2
    );

    rm.texturaItens = carregarTexturaAlterandoCores( 
        "resources/imagens/itens/itens.png",
        (Color[]) {
            { 16, 112, 132, 255 },
        },
        (Color[]) {
            BLANK,
        },
        1
    );

    rm.texturaShields = carregarTexturaAlterandoCores( 
        "resources/imagens/sprites/shields-base.png",
        (Color[]) {
            { 13, 72, 7, 255 },
            { 37, 102, 26, 255 },
        },
        (Color[]) {
            BLANK,
            BLANK,
        },
        2
    );

    rm.texturaObjetosBaseS1 = carregarTexturaAlterandoCores( 
        "resources/imagens/itens/objetos-base-s1.png",
        (Color[]) {
            { 13, 72, 7, 255 },
            { 37, 102, 26, 255 },
        },
        (Color[]) {
            BLANK,
            BLANK,
        },
        2
    );

    rm.texturaTerreno = LoadTexture( "resources/imagens/tiles/terreno.png" );
    rm.texturasFundo[0] = LoadTexture( "resources/imagens/fundo/fundo01.png" );
    rm.texturasFundo[1] = LoadTexture( "resources/imagens/fundo/fundo02.png" ); 

    rm.texturaHud = carregarTexturaAlterandoCores(

        "resources/imagens/hud/hud.png",
        ( Color[] ) { 16, 112, 132, 255 },
        ( Color[] ) { BLANK },
        1
    );

    // Filtro de textura nearest-neighbor (ponto) para todas as texturas do jogo.
    // Evita interpolação bilinear nas bordas dos tiles e sprites, que causaria
    // bleeding de cor entre pixels adjacentes — efeito indesejável em pixel art.
    SetTextureFilter( rm.texturaJogador, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaBadniks, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaBadniksBase, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaItens, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaShields, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaObjetosBaseS1, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaTerreno, TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturasFundo[0], TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturasFundo[1], TEXTURE_FILTER_POINT );
    SetTextureFilter( rm.texturaHud, TEXTURE_FILTER_POINT );


    rm.somAnel = LoadSound( "resources/sons/efeitos/anel.wav" );
    rm.somFrenagem = LoadSound( "resources/sons/efeitos/frenagem.wav" );
    rm.somHitComAnel = LoadSound( "resources/sons/efeitos/hit-com-anel.wav" );
    rm.somHitInimigo = LoadSound( "resources/sons/efeitos/hit-inimigo.wav" );
    rm.somMorte = LoadSound( "resources/sons/efeitos/morte.wav" );
    rm.somPulo = LoadSound( "resources/sons/efeitos/pulo.wav" );

    rm.musicaFase01 = LoadMusicStream( "resources/sons/musicas/green-hill-zone.mp3" );
    rm.musicaFase02 = LoadMusicStream( "resources/sons/musicas/marble-zone.mp3" );
    rm.musicaInvencibilidade = LoadMusicStream( "resources/sons/musicas/musica_imortal.mp3" );

}

void unloadResourcesResourceManager( void ) {

    UnloadTexture( rm.texturaJogador );
    UnloadTexture( rm.texturaBadniks );
    UnloadTexture( rm.texturaBadniksBase );
    UnloadTexture( rm.texturaItens );
    UnloadTexture( rm.texturaShields );
    UnloadTexture( rm.texturaObjetosBaseS1 );
    UnloadTexture( rm.texturaTerreno );
    UnloadTexture( rm.texturasFundo[0] );
    UnloadTexture( rm.texturasFundo[1] );
    UnloadTexture( rm.texturaHud );

    UnloadSound( rm.somAnel );
    UnloadSound( rm.somFrenagem );
    UnloadSound( rm.somHitComAnel );
    UnloadSound( rm.somHitInimigo );
    UnloadSound( rm.somMorte );
    UnloadSound( rm.somPulo );

    UnloadMusicStream( rm.musicaFase01 );
    UnloadMusicStream( rm.musicaFase02 );
    UnloadMusicStream( rm.musicaInvencibilidade );

}