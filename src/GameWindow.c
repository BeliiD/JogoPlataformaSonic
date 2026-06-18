/**
 * @file GameWindow.c
 * @author Prof. Dr. David Buzatto
 * @brief GameWindow implementation.
 * 
 * @copyright Copyright (c) 2026
 */
#include <stdlib.h>
#include <stdbool.h>

#include "GameWindow.h"
#include "GameWorld.h"
#include "ResourceManager.h"
#include "raylib/raylib.h"

/**
 * @brief Creates a dinamically allocated GameWindow struct instance.
 */
GameWindow *createGameWindow( 
        int width, 
        int height, 
        const char *title, 
        int targetFPS,
        bool antialiasing, 
        bool resizable, 
        bool fullScreen,
        bool undecorated, 
        bool alwaysOnTop, 
        bool invisibleBackground, 
        bool alwaysRun, 
        bool loadResources, 
        bool initAudio ) {

    GameWindow *gameWindow = (GameWindow*) malloc( sizeof( GameWindow ) );

    gameWindow->width = width;
    gameWindow->height = height;
    gameWindow->title = title;
    gameWindow->targetFPS = targetFPS;
    gameWindow->antialiasing = antialiasing;
    gameWindow->resizable = resizable;
    gameWindow->fullScreen = fullScreen;
    gameWindow->undecorated = undecorated;
    gameWindow->alwaysOnTop = alwaysOnTop;
    gameWindow->invisibleBackground = invisibleBackground;
    gameWindow->alwaysRun = alwaysRun;
    gameWindow->loadResources = loadResources;
    gameWindow->initAudio = initAudio;
    gameWindow->gw = NULL;
    gameWindow->initialized = false;

    return gameWindow;

}

/**
 * @brief Initializes the Window, starts the game loop and, when it
 * finishes, the window will be finished and destroyed too.
 */
void initGameWindow( GameWindow *gameWindow ) {

    if ( !gameWindow->initialized ) {

        gameWindow->initialized = true;

        if ( gameWindow->antialiasing ) {
            SetConfigFlags( FLAG_MSAA_4X_HINT );
        }

        if ( gameWindow->resizable ) {
            SetConfigFlags( FLAG_WINDOW_RESIZABLE );
        }

        if ( gameWindow->fullScreen ) {
            SetConfigFlags( FLAG_FULLSCREEN_MODE );
        }

        if ( gameWindow->undecorated ) {
            SetConfigFlags( FLAG_WINDOW_UNDECORATED );
        }

        if ( gameWindow->alwaysOnTop ) {
            SetConfigFlags( FLAG_WINDOW_TOPMOST );
        }

        if ( gameWindow->invisibleBackground ) {
            SetConfigFlags( FLAG_WINDOW_TRANSPARENT );
        }

        if ( gameWindow->alwaysRun ) {
            SetConfigFlags( FLAG_WINDOW_ALWAYS_RUN );
        }

        InitWindow( gameWindow->width, gameWindow->height, gameWindow->title );
        SetExitKey( 999 );

        if ( gameWindow->initAudio ) {
            InitAudioDevice();
        }

        SetTargetFPS( gameWindow->targetFPS );    

        if ( gameWindow->loadResources ) {
            loadResourcesResourceManager();
        }

        gameWindow->estadoAtual = ESTADO_SELECAO_NIVEL;
        gameWindow->nivelSelecionado = 1;
        gameWindow->totalNiveis = 3;
        gameWindow->gw = NULL;       // Começa nulo, só criamos quando o jogo iniciar

        bool rodando = true;

        // game loop
        while ( rodando && !WindowShouldClose() ) {

            // O delta time é limitado a 1/30s para evitar que frames muito
            // longos (ex.: lentidão na inicialização) causem deslocamentos
            // grandes demais, fazendo personagens atravessarem obstáculos
            // (tunneling).
            float delta = GetFrameTime();
            if ( delta > 1.0f / 30.0f ) {
                delta = 1.0f / 30.0f;
            }

            // CONTROLE DE ESTADOS
            switch ( gameWindow->estadoAtual ) {

                case ESTADO_SELECAO_NIVEL:
                    // Lógica de navegação do menu
                    if ( IsKeyPressed( KEY_RIGHT ) ) gameWindow->nivelSelecionado++;
                    if ( IsKeyPressed( KEY_LEFT ) ) gameWindow->nivelSelecionado--;
                    if ( IsKeyPressed( KEY_ESCAPE ) ) {
                        rodando = false;
                        break;
                    }

                    // Loop do menu
                    if ( gameWindow->nivelSelecionado > gameWindow->totalNiveis ) gameWindow->nivelSelecionado = 1;
                    if ( gameWindow->nivelSelecionado < 1 ) gameWindow->nivelSelecionado = gameWindow->totalNiveis;

                    // Confirmação do nível
                    if ( IsKeyPressed( KEY_ENTER ) ) {
                        // Se já existia um jogo anterior rodando, destrói para evitar memory leak
                        if ( gameWindow->gw != NULL ) {
                            destroyGameWorld( gameWindow->gw );
                        }
                        gameWindow->gw = createGameWorld( gameWindow->nivelSelecionado );
                        gameWindow->estadoAtual = ESTADO_JOGANDO;
                    }

                    // Desenho da tela de seleção
                    BeginDrawing();
                        ClearBackground( BLACK );
                        DrawText( "SELECIONE A FASE", gameWindow->width / 2 - 150, 150, 30, WHITE );
                        DrawText( TextFormat( "< FASE %02d >", gameWindow->nivelSelecionado ), gameWindow->width / 2 - 100, 220, 40, YELLOW );
                        DrawText( "Pressione ENTER para jogar", gameWindow->width / 2 - 150, 350, 20, LIGHTGRAY );
                        DrawText( "Pressione ESC para sair", 0, 0, 20, LIGHTGRAY );
                    EndDrawing();
                    break;

                case ESTADO_JOGANDO:
                    // Atualiza e desenha o jogo normalmente
                    updateGameWorld( gameWindow->gw, delta );
                    drawGameWorld( gameWindow->gw );

                    // Verifica se a fase foi concluída
                    if ( verificarFaseConcluida( gameWindow->gw ) ) {
                        int proximoNivel = gameWindow->gw->nivelAtual + 1;
                        destroyGameWorld( gameWindow->gw );
                        gameWindow->gw = NULL;
                        if ( proximoNivel <= gameWindow->totalNiveis ) {
                            gameWindow->nivelSelecionado = proximoNivel;
                            gameWindow->gw = createGameWorld( proximoNivel );
                        } else {
                            gameWindow->estadoAtual = ESTADO_SELECAO_NIVEL;
                        }
                    }

                    // Botão para voltar ao menu
                    if ( IsKeyPressed( KEY_Q ) ) {
                        if ( gameWindow->gw != NULL ) {
                            destroyGameWorld( gameWindow->gw );
                            gameWindow->gw = NULL;
                        }
                        gameWindow->estadoAtual = ESTADO_SELECAO_NIVEL;
                    }
                    break;
            }
        }

        if ( gameWindow->loadResources ) {
            unloadResourcesResourceManager();
        }

        bool initAudio = gameWindow->initAudio;

        destroyGameWindow( gameWindow );

        if ( initAudio ) {
            CloseAudioDevice();
        }

        CloseWindow();

    }

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWindow( GameWindow *gameWindow ) {
    if ( gameWindow != NULL ) {
        destroyGameWorld( gameWindow->gw );
        free( gameWindow );
    }
}