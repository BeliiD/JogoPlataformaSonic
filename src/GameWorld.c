/**
 * @file GameWorld.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do GameWorld.
 *
 * @copyright Copyright (c) 2026
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "GameWorld.h"
#include "Jogador.h"
#include "Macros.h"
#include "Mapa.h"
#include "Obstaculo.h"
#include "Tipos.h"
#include "ResourceManager.h"

#include "raylib/raylib.h"


// declarações
static void desenharFundo( GameWorld *gw );
static void atualizarCamera( GameWorld *gw );

static void inicializar( GameWorld *gw, int nivel );
static void reiniciar( GameWorld *gw );


/**
 * @brief Cria uma instância alocada dinamicamente da struct GameWorld.
 */
GameWorld *createGameWorld( int nivelAtual ) {
    GameWorld *gw = (GameWorld*) malloc( sizeof( GameWorld ) );
    gw->nivelAtual = nivelAtual; 
    inicializar( gw, nivelAtual );
    return gw;
}

/**
 * @brief Destrói um objeto GameWorld e suas dependências.
 */
void destroyGameWorld( GameWorld *gw ) {
    if ( gw != NULL ) {
        StopMusicStream( rm.musicaFase01 );
        StopMusicStream( rm.musicaFase02 );
        StopMusicStream( rm.musicaInvencibilidade );
        destruirMapa( gw->mapa );
        destruirJogador( gw->jogador );
        free( gw );
    }
}

/**
 * @brief Lê a entrada do usuário e atualiza o estado do jogo.
 */
void updateGameWorld( GameWorld *gw, float delta ) {

    if ( gw->jogador != NULL && gw->jogador->invencivel ) {
        // Pausa as músicas normais
        if ( gw->nivelAtual == 1 || gw->nivelAtual == 3 ) {
            PauseMusicStream( rm.musicaFase01 );
        } else if ( gw->nivelAtual == 2 ) {
            PauseMusicStream( rm.musicaFase02 );
        }

        // Toca a música da invencibilidade
        if ( !IsMusicStreamPlaying( rm.musicaInvencibilidade ) ) {
            PlayMusicStream( rm.musicaInvencibilidade );
        } else {
            UpdateMusicStream( rm.musicaInvencibilidade );
        }
    } else {
        // Para a música da invencibilidade caso ela estivesse tocando
        if ( IsMusicStreamPlaying( rm.musicaInvencibilidade ) ) {
            StopMusicStream( rm.musicaInvencibilidade );
        }

        // Retoma a música normal da fase
        if ( gw->nivelAtual == 1 || gw->nivelAtual == 3 ) {
            ResumeMusicStream( rm.musicaFase01 );
            if ( !IsMusicStreamPlaying( rm.musicaFase01 ) ) {
                PlayMusicStream( rm.musicaFase01 );
            }
            UpdateMusicStream( rm.musicaFase01 );
        } else if ( gw->nivelAtual == 2 ) {
            ResumeMusicStream( rm.musicaFase02 );
            if ( !IsMusicStreamPlaying( rm.musicaFase02 ) ) {
                PlayMusicStream( rm.musicaFase02 );
            }
            UpdateMusicStream( rm.musicaFase02 );
        }
    }
    

    if ( IsKeyPressed( KEY_R ) ) {
        reiniciar( gw );
        return;
    }

    Jogador *j = gw->jogador;
    atualizarMapa( gw->mapa, gw, delta );
    entradaJogador( j, delta );
    atualizarJogador( j, gw, delta );
    atualizarCamera( gw );

    gw-> tempoDeJogo += delta;

}

//Função base
static void desenharHudRec( Rectangle src, float x, float y, Color cor ) {

    Rectangle dest = { x, y, src.width * 2.0f, src.height * 2.0f };     //Recorte do sprite
    DrawTexturePro( rm.texturaHud, src, dest, (Vector2){ 0, 0 }, 0.0f, cor );   //Desenhar a textura do HUD

}

static void desenharNumeroHUD( int valor, int xRight, int y ) {

    if ( valor == 0 ) {

        Rectangle src = { 73 + 0 * 16, 433, 7, 12 };
        desenharHudRec( src, xRight - 16, y, WHITE );
        return;

    }
    
    int temp = valor;
    int curX = xRight;
    while ( temp > 0 ) {

        int dig = temp % 10;
        curX -= 16;
        Rectangle src = { 73 + dig * 16, 433, 8, 12 };
        desenharHudRec( src, curX, y, WHITE );
        temp /= 10;

    }

}

static void desenharTempoHUD( float tempo, int xRight, int y ) {

    int minutos = (int)(tempo / 60);
    int segundos = (int)(tempo) % 60;
    if ( minutos > 9 ) minutos = 9;
    
    int curX = xRight;
    
    // Dois pontos
    curX -= 10;
    Rectangle srcColon = { 90, 226, 5, 9 };
    
    // Segundos
    int digit1 = segundos % 10;
    int digit2 = segundos / 10;
    
    curX -= 16;
    Rectangle srcSec1 = { 73 + digit1 * 16, 433, 7, 12 };
    desenharHudRec( srcSec1, curX, y, WHITE );
    
    curX -= 16;
    Rectangle srcSec2 = { 73 + digit2 * 16, 433, 7, 12 };
    desenharHudRec( srcSec2, curX, y, WHITE );
    
    // Dois pontos
    curX -= 10;
    desenharHudRec( srcColon, curX, y + 2, WHITE );
    
    // Minutos
    curX -= 16;
    Rectangle srcMin = { 73 + minutos * 16, 433, 7, 12 };
    desenharHudRec( srcMin, curX, y, WHITE );
}

static void desenharVidasHUD( int vidas, int x, int y ) {
    Rectangle srcFace = { 40, 400, 16, 16 };
    desenharHudRec( srcFace, x, y, WHITE );
    
    Rectangle srcSonic = { 57, 400, 30, 8 };
    desenharHudRec( srcSonic, x + 36, y, WHITE );
    
    Rectangle srcCross = { 61, 409, 8, 6 };
    desenharHudRec( srcCross, x + 44, y + 22, WHITE );
    
    if ( vidas < 0 ) {

        DrawRectangle( x + 66, y + 16, 8, 4, BLACK );
        DrawRectangle( x + 64, y + 14, 8, 4, WHITE );
        
        int count = -vidas;
        if ( count > 9 ) count = 9; 
        Rectangle srcNum = { 73 + count * 16, 433, 7, 12 };
        desenharHudRec( srcNum, x + 75, y + 18, WHITE );

    } else {

        int count = (vidas > 9) ? 9 : vidas;
        Rectangle srcNum = { 73 + count * 16, 433, 7, 12 };
        desenharHudRec( srcNum, x + 75, y + 18, WHITE );

    }

}

/**
 * @brief Desenha o estado do jogo.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( (Color) { 36, 0, 180, 255 } );

    BeginMode2D( gw->camera );
    desenharFundo( gw );
    desenharMapa( gw->mapa );
    desenharJogador( gw->jogador );
    EndMode2D();

    // Desenhar Rótulos do HUD (SCORE, TIME, RINGS)

    desenharHudRec( (Rectangle){ 25, 433, 39, 11 }, 16, 16, WHITE );    //SCORE

    desenharHudRec( (Rectangle){ 25, 457, 31, 11 }, 16, 40, WHITE );    //TIME

    bool ringsFlash = (gw->jogador->quantidadeAneis == 0) && (((int)(GetTime() * 4) % 2) == 0);
    Rectangle srcRings = ringsFlash ? (Rectangle){ 73, 481, 39, 11 } : (Rectangle){ 25, 481, 39, 11 };  
    desenharHudRec( srcRings, 16, 64, WHITE );  //RING

    // Desenhar Valores do HUD
    desenharNumeroHUD( gw->jogador->score, 165, 16 );
    desenharTempoHUD( gw->tempoDeJogo, 160, 40 );
    desenharNumeroHUD( gw->jogador->quantidadeAneis, 150, 64 );

    // Desenhar vidasJ
    desenharVidasHUD( gw->jogador->quantidadeVidas, 16, GetScreenHeight() - 48 );

    EndDrawing();

}

static void desenharFundo( GameWorld *gw ) {

    Texture2D fundoAtual;
    // Obtém a textura correspondente ao nível atual
    if(gw->nivelAtual == 1 || gw->nivelAtual == 3){
        fundoAtual = rm.texturasFundo[0];
    } else {
        fundoAtual = rm.texturasFundo[1];
    }
    

    int larguraFundo = fundoAtual.width;
    int larguraMapa = calcularLarguraMapa( gw->mapa );
    int repeticoes = larguraMapa / larguraFundo;

    int deslocamentoParallax = (int) ( ( gw->camera.target.x / (float) larguraMapa ) * 200 );
    int yFundo = (int) ( gw->camera.target.y - GetScreenHeight() / 2.0f );

    for ( int i = 0; i <= repeticoes; i++ ) {
        DrawTexture( fundoAtual, larguraFundo * i - deslocamentoParallax, yFundo, WHITE );
    }

}

static void atualizarCamera( GameWorld *gw ) {

    Jogador *j = gw->jogador;
    Camera2D *c = &gw->camera;

    c->offset.x = GetScreenWidth() / 2;
    c->offset.y = GetScreenHeight() / 2;

    // O target é arredondado para o inteiro mais próximo para garantir que a
    // translação da câmera ocorra sempre em posições inteiras de pixel. Sem esse
    // arredondamento, o valor float contínuo de ret.x faz os tiles serem
    // renderizados em posições subpixel, causando frestas visíveis entre eles.
    c->target.x = roundf( j->ret.x + j->ret.width / 2.0f );
    c->target.y = roundf( j->ret.y + j->ret.height / 2.0f );

    int minX = GetScreenWidth() / 2;
    int maxX = calcularLarguraMapa( gw->mapa ) - GetScreenWidth() / 2;
    int maxY = calcularAlturaMapa( gw->mapa ) - GetScreenHeight() / 2;

    if ( c->target.x < minX ) {
        c->target.x = minX;
    } else if ( c->target.x > maxX ) {
        c->target.x = maxX;
    }

    if ( c->target.y > maxY ) {
        c->target.y = maxY;
    }

}

static void inicializar( GameWorld *gw, int nivel ) {

    //gw->mapa = carregarMapa( "resources/mapas/mapaTeste.txt" );
    char caminhoMapa[100];
    sprintf( caminhoMapa, "resources/mapas/mapa%02d.txt", nivel );
    gw->mapa = carregarMapa( caminhoMapa );
    gw->jogador = criarJogador( 64, calcularAlturaMapa( gw->mapa ) - 196, 96, 96 );

    gw->camera = (Camera2D) {
        .offset = { 0 },    // deslocamento relativo da câmera em relação ao alvo
        .target = { 0 },    // o alvo da câmera, ou seja, a coordenada em que ela está centralizada
        .rotation = 0.0f,   // rotação da câmera em graus. o pivô é o alvo.
        .zoom = 1.0f        // zoom da câmera. 1.0f significa sem escala
    };

    gw->gravidade = 900;
    gw->tempoDeJogo = 0.0f;

}

static void reiniciar( GameWorld *gw ) {

    destruirMapa( gw->mapa );
    destruirJogador( gw->jogador );

    StopMusicStream( rm.musicaFase01 );
    StopMusicStream( rm.musicaFase02 );
    StopMusicStream( rm.musicaInvencibilidade );

    inicializar( gw, gw->nivelAtual);

}

bool verificarFaseConcluida( GameWorld *gw ) {
    if ( gw == NULL || gw->mapa == NULL ) {
        return false;
    }
    ElementoMapa *el = gw->mapa->itens;
    while ( el != NULL ) {
        Item *item = (Item*) el->objeto;
        if ( item != NULL && item->tipo == TIPO_ITEM_PLACAFIM ) {
            ItemPlacaFim *placa = (ItemPlacaFim*) item->objeto;
            if ( placa != NULL && placa->estado == ESTADO_ITEM_PLACAFIM_SONIC && placa->tempoExibicaoSonic >= 1.5f ) {
                return true;
            }
        }
        el = el->proximo;
    }
    return false;
}