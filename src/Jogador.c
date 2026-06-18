/**
 * @file Jogador.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Jogador.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "Inimigo.h"
#include "InimigoMotobug.h"
#include "InimigoBuzzBomber.h"
#include "InimigoCrabmeat.h"
#include "Item.h"
#include "ItemAnel.h"
#include "ItemEscudo.h"
#include "ItemEstrela.h"
#include "ItemPlacaFim.h"
#include "Macros.h"
#include "Jogador.h"
#include "Mapa.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoJogador( Jogador *j, QuadroAnimacao *qa, Color tonalidade );
static QuadroAnimacao *getQuadroAnimacaoAtualJogador( Jogador *j );
static Animacao *getAnimacaoAtualJogador( Jogador *j );

static void resolverColisaoJogadorObstaculosMapaX( Jogador *j, Mapa *mapa );
static void resolverColisaoJogadorObstaculosMapaY( Jogador *j, Mapa *mapa );

static void resolverColisaoJogadorItensMapa( Jogador *j, Mapa *mapa );
static void resolverColisaoJogadorInimigosMapa( Jogador *j, Mapa *mapa );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria uma instância alocada dinamicamente da struct Jogador.
 */
Jogador *criarJogador( float x, float y, float w, float h ) {

    Jogador *novoJogador = (Jogador*) malloc( sizeof( Jogador ) );

    novoJogador->ret.x = x;
    novoJogador->ret.y = y;
    novoJogador->ret.width = w;
    novoJogador->ret.height = h;
    novoJogador->vel = (Vector2) { 0 };

    novoJogador->cor = BLUE;

    novoJogador->velAndando = 200;
    novoJogador->velAndandoRapido = 400;
    novoJogador->velCorrendo = 800;
    novoJogador->velPulo = -550;
    novoJogador->velMaxQueda = 600;

    novoJogador->aceleracao = 200;
    novoJogador->desaceleracao = 400;
    novoJogador->frenagem = 1800;

    novoJogador->quantidadePulos = 0;
    novoJogador->quantidadeMaxPulos = 1;

    novoJogador->quantidadeAneis = 0;
    novoJogador->quantidadeVidas = 3;
    novoJogador-> score = 0;

    novoJogador->invulneravel = false;
    novoJogador->tempoInvulnerabilidade = 3.0f;
    novoJogador->contadorTempoInvulnerabilidade = 0.0f;

    novoJogador->piscaPisca = false;
    novoJogador->tempoPiscaPisca = 0.05f;
    novoJogador->contadorTempoPiscaPisca = 0.0f;

    novoJogador->freando = false;

    novoJogador->estado = ESTADO_JOGADOR_PARADO;
    novoJogador->olhandoParaDireita = true;

    int quantidadeAnimacoes = 0;

    novoJogador->animacaoParado.quantidadeQuadros = 1;
    novoJogador->animacaoParado.quadroAtual = 0;
    novoJogador->animacaoParado.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoParado.pararNoUltimoQuadro = false;
    novoJogador->animacaoParado.executarUmaVez = false;
    novoJogador->animacaoParado.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoParado, novoJogador->animacaoParado.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoParado.quadros,
        novoJogador->animacaoParado.quantidadeQuadros,
        1000,            // duração padrão para todos os quadros
        24, 251,         // início
        48, 48,          // dimensões
        4,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            32, 20, 42, 76
            //18, 20, 54, 76
        }
    );

    novoJogador->animacaoAndando.quantidadeQuadros = 8;
    novoJogador->animacaoAndando.quadroAtual = 0;
    novoJogador->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoAndando.pararNoUltimoQuadro = false;
    novoJogador->animacaoAndando.executarUmaVez = false;
    novoJogador->animacaoAndando.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoAndando, novoJogador->animacaoAndando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoAndando.quadros,
        novoJogador->animacaoAndando.quantidadeQuadros,
        80,              // duração padrão para cada quadro
        664, 324,        // início
        48, 48,          // dimensões
        4,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            32, 20, 42, 76
            //18, 20, 54, 76
        }
    );

    novoJogador->animacaoAndandoRapido.quantidadeQuadros = 8;
    novoJogador->animacaoAndandoRapido.quadroAtual = 0;
    novoJogador->animacaoAndandoRapido.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoAndandoRapido.pararNoUltimoQuadro = false;
    novoJogador->animacaoAndandoRapido.executarUmaVez = false;
    novoJogador->animacaoAndandoRapido.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoAndandoRapido, novoJogador->animacaoAndandoRapido.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoAndandoRapido.quadros,
        novoJogador->animacaoAndandoRapido.quantidadeQuadros,
        40,              // duração padrão para cada quadro
        664, 324,        // início
        48, 48,          // dimensões
        4,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            32, 20, 42, 76
            //18, 20, 54, 76
        }
    );

    novoJogador->animacaoCorrendo.quantidadeQuadros = 4;
    novoJogador->animacaoCorrendo.quadroAtual = 0;
    novoJogador->animacaoCorrendo.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoCorrendo.pararNoUltimoQuadro = false;
    novoJogador->animacaoCorrendo.executarUmaVez = false;
    novoJogador->animacaoCorrendo.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoCorrendo, novoJogador->animacaoCorrendo.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoCorrendo.quadros,
        novoJogador->animacaoCorrendo.quantidadeQuadros,
        20,              // duração padrão para cada quadro
        24, 397,         // início
        48, 48,          // dimensões
        4,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            32, 20, 42, 76
            //18, 20, 54, 76
        }
    );

    novoJogador->animacaoPulando.quantidadeQuadros = 4;
    novoJogador->animacaoPulando.quadroAtual = 0;
    novoJogador->animacaoPulando.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulando.pararNoUltimoQuadro = false;
    novoJogador->animacaoPulando.executarUmaVez = false;
    novoJogador->animacaoPulando.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoPulando, novoJogador->animacaoPulando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoJogador->animacaoPulando.quadros,
        novoJogador->animacaoPulando.quantidadeQuadros,
        40,              // duração padrão para cada quadro
        248, 397,        // início
        48, 48,          // dimensões
        4,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            32, 46, 42, 50
            //18, 36, 60, 60
        }
    );

    novoJogador->animacaoPulandoRapido.quantidadeQuadros = 4;
    novoJogador->animacaoPulandoRapido.quadroAtual = 0;
    novoJogador->animacaoPulandoRapido.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulandoRapido.pararNoUltimoQuadro = false;
    novoJogador->animacaoPulandoRapido.executarUmaVez = false;
    novoJogador->animacaoPulandoRapido.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoPulandoRapido, novoJogador->animacaoPulandoRapido.quantidadeQuadros );
    inicializarQuadrosAnimacao(
        novoJogador->animacaoPulandoRapido.quadros,
        novoJogador->animacaoPulandoRapido.quantidadeQuadros,
        25,              // duração padrão para cada quadro
        248, 397,        // início
        48, 48,          // dimensões
        4,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            32, 46, 42, 50
            //18, 36, 60, 60
        }
    );

    novoJogador->animacaoPulandoCorrendo.quantidadeQuadros = 4;
    novoJogador->animacaoPulandoCorrendo.quadroAtual = 0;
    novoJogador->animacaoPulandoCorrendo.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoPulandoCorrendo.pararNoUltimoQuadro = false;
    novoJogador->animacaoPulandoCorrendo.executarUmaVez = false;
    novoJogador->animacaoPulandoCorrendo.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoPulandoCorrendo, novoJogador->animacaoPulandoCorrendo.quantidadeQuadros );
    inicializarQuadrosAnimacao(
        novoJogador->animacaoPulandoCorrendo.quadros,
        novoJogador->animacaoPulandoCorrendo.quantidadeQuadros,
        15,              // duração padrão para cada quadro
        248, 397,        // início
        48, 48,          // dimensões
        4,               // separação
        false,           // de trás para frente
        (Rectangle) {    // retângulo de colisão padrão para cada quadro
            32, 46, 42, 50
            //18, 36, 60, 60
        }
    );

    novoJogador->temEscudo = false;
    novoJogador->invencivel = false;
    novoJogador->tempoInvencibilidade = 0.0f;

    novoJogador->animacaoEscudo.quantidadeQuadros = 6;
    novoJogador->animacaoEscudo.quadroAtual = 0;
    novoJogador->animacaoEscudo.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoEscudo.pararNoUltimoQuadro = false;
    novoJogador->animacaoEscudo.executarUmaVez = false;
    novoJogador->animacaoEscudo.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoEscudo, 6 );

    // Manual initialization of the 6 frames to implement flickering blue-gray striped shield
    novoJogador->animacaoEscudo.quadros[0] = (QuadroAnimacao){ .fonte = (Rectangle){ 24, 584, 48, 48 }, .duracao = 50, .retColisao = (Rectangle){ 0 } };
    novoJogador->animacaoEscudo.quadros[1] = (QuadroAnimacao){ .fonte = (Rectangle){ 0, 0, 0, 0 },      .duracao = 50, .retColisao = (Rectangle){ 0 } };
    novoJogador->animacaoEscudo.quadros[2] = (QuadroAnimacao){ .fonte = (Rectangle){ 80, 584, 48, 48 }, .duracao = 50, .retColisao = (Rectangle){ 0 } };
    novoJogador->animacaoEscudo.quadros[3] = (QuadroAnimacao){ .fonte = (Rectangle){ 0, 0, 0, 0 },      .duracao = 50, .retColisao = (Rectangle){ 0 } };
    novoJogador->animacaoEscudo.quadros[4] = (QuadroAnimacao){ .fonte = (Rectangle){ 136, 584, 48, 48 },.duracao = 50, .retColisao = (Rectangle){ 0 } };
    novoJogador->animacaoEscudo.quadros[5] = (QuadroAnimacao){ .fonte = (Rectangle){ 0, 0, 0, 0 },      .duracao = 50, .retColisao = (Rectangle){ 0 } };

    novoJogador->animacaoSparkles.quantidadeQuadros = 4;
    novoJogador->animacaoSparkles.quadroAtual = 0;
    novoJogador->animacaoSparkles.contadorTempoQuadro = 0.0f;
    novoJogador->animacaoSparkles.pararNoUltimoQuadro = false;
    novoJogador->animacaoSparkles.executarUmaVez = false;
    novoJogador->animacaoSparkles.finalizada = false;
    criarQuadrosAnimacao( &novoJogador->animacaoSparkles, 4 );
    inicializarQuadrosAnimacao(
        novoJogador->animacaoSparkles.quadros,
        4,
        80,              // duração
        24, 712,         // início X, Y em shields-base.png
        48, 48,          // dimensões
        0,               // separação (contíguos)
        false,
        (Rectangle) { 0 }
    );

    novoJogador->animacoes[ESTADO_JOGADOR_PARADO] = &novoJogador->animacaoParado; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_ANDANDO] = &novoJogador->animacaoAndando; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_ANDANDO_RAPIDO] = &novoJogador->animacaoAndandoRapido; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_CORRENDO] = &novoJogador->animacaoCorrendo; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO] = &novoJogador->animacaoPulando; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO_RAPIDO] = &novoJogador->animacaoPulandoRapido; quantidadeAnimacoes++;
    novoJogador->animacoes[ESTADO_JOGADOR_PULANDO_CORRENDO] = &novoJogador->animacaoPulandoCorrendo; quantidadeAnimacoes++;
    novoJogador->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoJogador;

}

/**
 * @brief Destrói um objeto Jogador e libera seus recursos.
 */
void destruirJogador( Jogador *j ) {
    if ( j != NULL ) {
        for ( int i = 0; i < j->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( j->animacoes[i] );
        }
        destruirQuadrosAnimacao( &j->animacaoEscudo );
        destruirQuadrosAnimacao( &j->animacaoSparkles );
        free( j );
    }
}

/**
 * @brief Lê a entrada do usuário e atualiza as velocidades do jogador.
 */
void entradaJogador( Jogador *j, float delta ) {

    EstadoJogador estadoAnterior = j->estado;

    bool direitaDown  = IsKeyDown( KEY_RIGHT )     || ( IsGamepadAvailable( 0 ) && IsGamepadButtonDown( 0, GAMEPAD_BUTTON_LEFT_FACE_RIGHT ) );
    bool esquerdaDown = IsKeyDown( KEY_LEFT )      || ( IsGamepadAvailable( 0 ) && IsGamepadButtonDown( 0, GAMEPAD_BUTTON_LEFT_FACE_LEFT ) );
    bool puloPressed  = IsKeyPressed( KEY_SPACE )  || ( IsGamepadAvailable( 0 ) && IsGamepadButtonDown( 0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN ) );

    if ( direitaDown ) {
        if ( j->vel.x < 0 ) {
            j->vel.x += j->frenagem * delta;
            if ( !j->freando && j->estado == ESTADO_JOGADOR_CORRENDO ) {
                PlaySound( rm.somFrenagem );
                j->freando = true;
            }
            if ( j->vel.x > 0 ) {
                j->vel.x = 0;
                j->freando = false;
            }
        } else {
            j->vel.x += j->aceleracao * delta;
            if ( j->vel.x > j->velCorrendo ) {
                j->vel.x = j->velCorrendo;
            }
        }
        j->olhandoParaDireita = true;
    } else if ( esquerdaDown ) {
        if ( j->vel.x > 0 ) {
            j->vel.x -= j->frenagem * delta;
            if ( !j->freando && j->estado == ESTADO_JOGADOR_CORRENDO ) {
                PlaySound( rm.somFrenagem );
                j->freando = true;
            }
            if ( j->vel.x < 0 ) {
                j->vel.x = 0;
                j->freando = false;
            }
        } else {
            j->vel.x -= j->aceleracao * delta;
            if ( j->vel.x < -j->velCorrendo ) {
                j->vel.x = -j->velCorrendo;
            }
        }
        j->olhandoParaDireita = false;
    } else {
        if ( j->vel.x > 0 ) {
            j->vel.x -= j->desaceleracao * delta;
            if ( j->vel.x < 0 ) {
                j->vel.x = 0;
            }
        } else if ( j->vel.x < 0 ) {
            j->vel.x += j->desaceleracao * delta;
            if ( j->vel.x > 0 ) {
                j->vel.x = 0;
            }
        }
    }

    float absVelX = fabsf( j->vel.x );
    if ( j->quantidadePulos > 0 ) {
        if ( absVelX <= j->velAndando ) {
            j->estado = ESTADO_JOGADOR_PULANDO;
        } else if ( absVelX <= j->velAndandoRapido ) {
            j->estado = ESTADO_JOGADOR_PULANDO_RAPIDO;
        } else {
            j->estado = ESTADO_JOGADOR_PULANDO_CORRENDO;
        }
    } else if ( absVelX < 1.0f ) {
        j->estado = ESTADO_JOGADOR_PARADO;
    } else if ( absVelX <= j->velAndando ) {
        j->estado = ESTADO_JOGADOR_ANDANDO;
    } else if ( absVelX <= j->velAndandoRapido ) {
        j->estado = ESTADO_JOGADOR_ANDANDO_RAPIDO;
    } else {
        j->estado = ESTADO_JOGADOR_CORRENDO;
    }

    if ( puloPressed && j->quantidadePulos < j->quantidadeMaxPulos ) {
        j->vel.y = j->velPulo;
        j->quantidadePulos++;
        PlaySound( rm.somPulo );
    }

    // sincronização de animações andando e andando rápido
    if ( estadoAnterior == ESTADO_JOGADOR_ANDANDO && j->estado == ESTADO_JOGADOR_ANDANDO_RAPIDO ) {
        sincronizarAnimacao( &j->animacaoAndandoRapido, &j->animacaoAndando );
    } else if ( estadoAnterior == ESTADO_JOGADOR_ANDANDO_RAPIDO && j->estado == ESTADO_JOGADOR_ANDANDO ) {
        sincronizarAnimacao( &j->animacaoAndando, &j->animacaoAndandoRapido );
    }

}

/**
 * @brief Aplica física e resolve colisões do jogador com o mundo.
 */
void atualizarJogador( Jogador *j, GameWorld *gw, float delta ) {

    if ( j->invulneravel ) {

        j->contadorTempoPiscaPisca += delta;
        if ( j->contadorTempoPiscaPisca >= j->tempoPiscaPisca ) {
            j->contadorTempoPiscaPisca = 0.0f;
            j->piscaPisca = !j->piscaPisca;
        }

        j->contadorTempoInvulnerabilidade += delta;
        if ( j->contadorTempoInvulnerabilidade >= j->tempoInvulnerabilidade ) {
            j->contadorTempoInvulnerabilidade = 0.0f;
            j->invulneravel = false;
            j->contadorTempoPiscaPisca = 0.0f;
            j->piscaPisca = false;
        }

    }

    if ( j->temEscudo ) {
        atualizarAnimacao( &j->animacaoEscudo, delta );
    }

    if ( j->invencivel ) {
        atualizarAnimacao( &j->animacaoSparkles, delta );
        j->tempoInvencibilidade -= delta;
        if ( j->tempoInvencibilidade <= 0.0f ) {
            j->invencivel = false;
        }
    }

    Animacao *animacaoAtual = getAnimacaoAtualJogador( j );
    atualizarAnimacao( animacaoAtual, delta );

    // fase X: move horizontalmente e resolve colisões laterais
    j->ret.x += j->vel.x * delta;
    resolverColisaoJogadorObstaculosMapaX( j, gw->mapa );

    // fase Y: aplica gravidade, move verticalmente e resolve colisões verticais
    j->vel.y += gw->gravidade * delta;
    if ( j->vel.y > j->velMaxQueda ) {
        j->vel.y = j->velMaxQueda;
    }
    j->ret.y += j->vel.y * delta;
    resolverColisaoJogadorObstaculosMapaY( j, gw->mapa );

    resolverColisaoJogadorItensMapa( j, gw->mapa );
    resolverColisaoJogadorInimigosMapa( j, gw->mapa );

    float limiteQueda = calcularAlturaMapa( gw->mapa ); 

    if ( j->ret.y > limiteQueda ) {

        j->quantidadeVidas--;
        PlaySound( rm.somMorte );
        
        // Respawnar jogador na posição inicial
        j->ret.x = GetScreenWidth() / 2.0f + 144.0f;
        j->ret.y = limiteQueda - 196.0f;
        j->vel = (Vector2){ 0, 0 };
        j->invulneravel = true;
        j->contadorTempoInvulnerabilidade = 0.0f;
        j->piscaPisca = false;
        j->contadorTempoPiscaPisca = 0.0f;
        j->estado = ESTADO_JOGADOR_PARADO;

    }

}

/**
 * @brief Desenha o jogador.
 */
void desenharJogador( Jogador *j ) {

    if ( !j->piscaPisca ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );
        desenharQuadroAnimacaoJogador( j, qa, WHITE );
    }

    if ( j->temEscudo ) {
        QuadroAnimacao *qa = getQuadroAtualAnimacao( &j->animacaoEscudo );
        if ( qa != NULL && qa->fonte.width > 0 && qa->fonte.height > 0 ) {
            DrawTexturePro(
                rm.texturaShields,
                qa->fonte,
                j->ret,
                (Vector2) { 0 },
                0.0f,
                WHITE
            );
        }
    }

    if ( j->invencivel ) {
        QuadroAnimacao *qa = getQuadroAtualAnimacao( &j->animacaoSparkles );
        if ( qa != NULL ) {
            Rectangle ret64 = { j->ret.x + 16, j->ret.y + 16, 64, 64 };
            DrawTexturePro(
                rm.texturaShields,
                qa->fonte,
                ret64,
                (Vector2) { 0 },
                0.0f,
                WHITE
            );
            DrawTexturePro(
                rm.texturaShields,
                qa->fonte,
                j->ret,
                (Vector2) { 0 },
                0.0f,
                Fade( WHITE, 0.6f )
            );
        }
    }

    if ( MOSTRAR_RETANGULOS ) {
        DrawRectangleRec( j->ret, Fade( j->cor, 0.5f ) );
        DrawRectangleLines( j->ret.x, j->ret.y, j->ret.width, j->ret.height, BLACK );
    }

}

static void desenharQuadroAnimacaoJogador( Jogador *j, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {

        DrawTexturePro(
            rm.texturaJogador,
            (Rectangle) {
                qa->fonte.x,
                qa->fonte.y,
                j->olhandoParaDireita ? qa->fonte.width : -qa->fonte.width,
                qa->fonte.height
            },
            j->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = j->olhandoParaDireita
                ? j->ret.x + qa->retColisao.x
                : j->ret.x + j->ret.width - qa->retColisao.x - qa->retColisao.width;
            float yDesenho = j->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static QuadroAnimacao *getQuadroAnimacaoAtualJogador( Jogador *j ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualJogador( j ) );
}

static Animacao *getAnimacaoAtualJogador( Jogador *j ) {
    return j->animacoes[j->estado];
}

/**
 * @brief Resolve colisões do jogador com o mapa no eixo X.
 */
static void resolverColisaoJogadorObstaculosMapaX( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float deslocamentoX = j->olhandoParaDireita
            ? qa->retColisao.x
            : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Obstaculo *o = (Obstaculo*) el->objeto;

        if ( CheckCollisionRecs( retColCalculado, o->ret ) ) {
            if ( retColCalculado.x + retColCalculado.width / 2 < o->ret.x + o->ret.width / 2 ) {
                j->ret.x = o->ret.x - qa->retColisao.width - deslocamentoX;
            } else {
                j->ret.x = o->ret.x + o->ret.width - deslocamentoX;
            }
            j->vel.x = 0;
        }

        el = el->proximo;

    }

    // Colisão horizontal com monitores/itens sólidos
    el = mapa->itens;
    while ( el != NULL ) {
        Item *item = (Item*) el->objeto;
        if ( item->tipo == TIPO_ITEM_ESTRELA ) {
            ItemEstrela *itemEstrela = (ItemEstrela*) item->objeto;
            if ( itemEstrela->ativo && itemEstrela->estado == ESTADO_ITEM_ESTRELA_PARADO ) {
                QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );
                float deslocamentoX = j->olhandoParaDireita
                    ? qa->retColisao.x
                    : j->ret.width - qa->retColisao.x - qa->retColisao.width;
                float deslocamentoY = qa->retColisao.y;

                Rectangle retColCalculado = {
                    j->ret.x + deslocamentoX,
                    j->ret.y + deslocamentoY,
                    qa->retColisao.width,
                    qa->retColisao.height
                };

                QuadroAnimacao *qaItem = getQuadroAnimacaoAtualItemEstrela( itemEstrela );
                Rectangle retColItemCalculado = {
                    itemEstrela->ret.x + qaItem->retColisao.x,
                    itemEstrela->ret.y + qaItem->retColisao.y,
                    qaItem->retColisao.width,
                    qaItem->retColisao.height
                };

                if ( CheckCollisionRecs( retColCalculado, retColItemCalculado ) ) {
                    if ( retColCalculado.x + retColCalculado.width / 2 < retColItemCalculado.x + retColItemCalculado.width / 2 ) {
                        j->ret.x = retColItemCalculado.x - qa->retColisao.width - deslocamentoX;
                    } else {
                        j->ret.x = retColItemCalculado.x + retColItemCalculado.width - deslocamentoX;
                    }
                    j->vel.x = 0;
                }
            }
        }
        el = el->proximo;
    }

}

/**
 * @brief Resolve colisões do jogador com o mapa no eixo Y.
 */
static void resolverColisaoJogadorObstaculosMapaY( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float deslocamentoX = j->olhandoParaDireita
            ? qa->retColisao.x
            : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Obstaculo *o = (Obstaculo*) el->objeto;

        if ( CheckCollisionRecs( retColCalculado, o->ret ) ) {
            if ( retColCalculado.y + retColCalculado.height / 2 < o->ret.y + o->ret.height / 2 ) {
                j->ret.y = o->ret.y - qa->retColisao.height - deslocamentoY;
                j->quantidadePulos = 0;
            } else {
                j->ret.y = o->ret.y + o->ret.height - deslocamentoY;
            }
            j->vel.y = 0;
        }

        el = el->proximo;

    }

    // Colisão vertical com monitores/itens sólidos
    el = mapa->itens;
    while ( el != NULL ) {
        Item *item = (Item*) el->objeto;
        if ( item->tipo == TIPO_ITEM_ESTRELA ) {
            ItemEstrela *itemEstrela = (ItemEstrela*) item->objeto;
            if ( itemEstrela->ativo && itemEstrela->estado == ESTADO_ITEM_ESTRELA_PARADO ) {
                QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );
                float deslocamentoX = j->olhandoParaDireita
                    ? qa->retColisao.x
                    : j->ret.width - qa->retColisao.x - qa->retColisao.width;
                float deslocamentoY = qa->retColisao.y;

                Rectangle retColCalculado = {
                    j->ret.x + deslocamentoX,
                    j->ret.y + deslocamentoY,
                    qa->retColisao.width,
                    qa->retColisao.height
                };

                QuadroAnimacao *qaItem = getQuadroAnimacaoAtualItemEstrela( itemEstrela );
                Rectangle retColItemCalculado = {
                    itemEstrela->ret.x + qaItem->retColisao.x,
                    itemEstrela->ret.y + qaItem->retColisao.y,
                    qaItem->retColisao.width,
                    qaItem->retColisao.height
                };

                if ( CheckCollisionRecs( retColCalculado, retColItemCalculado ) ) {
                    // Se o Sonic está vindo de cima (caindo)
                    if ( retColCalculado.y + retColCalculado.height / 2 < retColItemCalculado.y + retColItemCalculado.height / 2 ) {
                        // Quebra o monitor (coleta o item) e pula
                        itemEstrela->estado = ESTADO_ITEM_ESTRELA_COLETADO;
                        j->invencivel = true;
                        j->tempoInvencibilidade = 10.0f;
                        j->vel.y = j->velPulo;
                        PlaySound( rm.somAnel );
                    } else {
                        // Vindo de baixo, bate a cabeça e é empurrado para baixo
                        j->ret.y = retColItemCalculado.y + retColItemCalculado.height - deslocamentoY;
                        j->vel.y = 0;
                    }
                }
            }
        }
        el = el->proximo;
    }

}

static void resolverColisaoJogadorItensMapa( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->itens;

    while ( el != NULL ) {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float deslocamentoX = j->olhandoParaDireita
            ? qa->retColisao.x
            : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Item *item = (Item*) el->objeto;

        if ( item->tipo == TIPO_ITEM_ANEL ) {

            ItemAnel *itemAnel = (ItemAnel*) item->objeto;

            if ( !itemAnel->ativo || itemAnel->estado == ESTADO_ITEM_ANEL_COLETADO ) {
                el = el->proximo;
                continue;
            }

            QuadroAnimacao *qaItem = getQuadroAnimacaoAtualItemAnel( itemAnel );
            
            Rectangle retColItemCalculado = {
                itemAnel->ret.x + qaItem->retColisao.x,
                itemAnel->ret.y + qaItem->retColisao.y,
                qaItem->retColisao.width,
                qaItem->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColItemCalculado ) ) {
                itemAnel->estado = ESTADO_ITEM_ANEL_COLETADO;
                j->quantidadeAneis++;
                PlaySound( rm.somAnel );
            }

        } else if ( item->tipo == TIPO_ITEM_ESCUDO ) {

            ItemEscudo *itemEscudo = (ItemEscudo*) item->objeto;

            if ( !itemEscudo->ativo || itemEscudo->estado == ESTADO_ITEM_ESCUDO_COLETADO ) {
                el = el->proximo;
                continue;
            }

            QuadroAnimacao *qaItem = getQuadroAnimacaoAtualItemEscudo( itemEscudo );
            
            Rectangle retColItemCalculado = {
                itemEscudo->ret.x + qaItem->retColisao.x,
                itemEscudo->ret.y + qaItem->retColisao.y,
                qaItem->retColisao.width,
                qaItem->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColItemCalculado ) ) {
                itemEscudo->estado = ESTADO_ITEM_ESCUDO_COLETADO;
                j->temEscudo = true;
                PlaySound( rm.somAnel );
            }

        } else if ( item->tipo == TIPO_ITEM_PLACAFIM ) {

            ItemPlacaFim *itemPlaca = (ItemPlacaFim*) item->objeto;

            if ( !itemPlaca->ativo || itemPlaca->estado != ESTADO_ITEM_PLACAFIM_ROBOTNIK ) {
                el = el->proximo;
                continue;
            }

            if ( CheckCollisionRecs( retColCalculado, itemPlaca->ret ) ) {
                itemPlaca->estado = ESTADO_ITEM_PLACAFIM_GIRANDO;
                itemPlaca->ladoSonic = false;
                itemPlaca->passosRestantes = 8 * 4;
                itemPlaca->frameAtual = 0;
                itemPlaca->contadorTempoQuadro = 0.0f;
                PlaySound( rm.somAnel );
            }

        }

        el = el->proximo;

    }

}

static void resolverColisaoJogadorInimigosMapa( Jogador *j, Mapa *mapa ) {

    ElementoMapa *el = mapa->inimigos;

    while ( el != NULL ) {

        QuadroAnimacao *qa = getQuadroAnimacaoAtualJogador( j );

        float deslocamentoX = j->olhandoParaDireita
            ? qa->retColisao.x
            : j->ret.width - qa->retColisao.x - qa->retColisao.width;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            j->ret.x + deslocamentoX,
            j->ret.y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Inimigo *inimigo = (Inimigo*) el->objeto;

        QuadroAnimacao *qaInimigo = NULL;
        bool *olhandoParaDireita = NULL;
        Rectangle *ret = NULL;

        if ( inimigo->tipo == TIPO_INIMIGO_MOTOBUG ) {

            InimigoMotobug *motobug = (InimigoMotobug*) inimigo->objeto;

            if ( !motobug->ativo || motobug->estado == ESTADO_INIMIGO_MOTOBUG_MORRENDO ) {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoMotobug( motobug );
            olhandoParaDireita = &motobug->olhandoParaDireita;
            ret = &motobug->ret;

            float deslocamentoX = *olhandoParaDireita
                ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                : qaInimigo->retColisao.x;
            float deslocamentoY = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocamentoX,
                ret->y + deslocamentoY,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColInimigoCalculado ) ) {

                if ( j->invencivel || (j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO) ) {
                    if ( j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO ) {
                        j->vel.y = j->velPulo;
                    }
                    motobug->estado = ESTADO_INIMIGO_MOTOBUG_MORRENDO;
                    j->score += 100;
                    PlaySound( rm.somHitInimigo );
                } else if ( !j->invulneravel ) {
                    if ( j->temEscudo ) {
                        j->temEscudo = false;
                        PlaySound( rm.somHitComAnel );
                    } else if ( j->quantidadeAneis > 0 ) {
                        j->quantidadeAneis = 0;
                        PlaySound( rm.somHitComAnel );
                    } else {
                        j->quantidadeVidas--;
                        PlaySound( rm.somMorte );
                    }
                    j->invulneravel = true;
                }

                return; // um inimigo de cada vez!

            }

        } else if ( inimigo->tipo == TIPO_INIMIGO_BUZZ_BOMBER ) {

            InimigoBuzzBomber *buzz = (InimigoBuzzBomber*) inimigo->objeto;

            if ( !buzz->ativo || buzz->estado == ESTADO_INIMIGO_BUZZ_BOMBER_MORRENDO ) {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoBuzzBomber( buzz );
            olhandoParaDireita = &buzz->olhandoParaDireita;
            ret = &buzz->ret;

            float deslocamentoX = *olhandoParaDireita
                ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                : qaInimigo->retColisao.x;
            float deslocamentoY = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocamentoX,
                ret->y + deslocamentoY,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColInimigoCalculado ) ) {

                if ( j->invencivel || (j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO) ) {
                    if ( j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO ) {
                        j->vel.y = j->velPulo;
                    }
                    buzz->estado = ESTADO_INIMIGO_BUZZ_BOMBER_MORRENDO;
                    buzz->vel.x = 0;
                    j->score += 100;
                    PlaySound( rm.somHitInimigo );
                } else if ( !j->invulneravel ) {
                    if ( j->temEscudo ) {
                        j->temEscudo = false;
                        PlaySound( rm.somHitComAnel );
                    } else if ( j->quantidadeAneis > 0 ) {
                        j->quantidadeAneis = 0;
                        PlaySound( rm.somHitComAnel );
                    } else {
                        j->quantidadeVidas--;
                        PlaySound( rm.somMorte );
                    }
                    j->invulneravel = true;
                }

                return; // um inimigo de cada vez!

            }

        } else if ( inimigo->tipo == TIPO_INIMIGO_BUZZ_BOMBER_PROJETIL ) {

            InimigoBuzzBomberProjetil *proj = (InimigoBuzzBomberProjetil*) inimigo->objeto;

            if ( !proj->ativo ) {
                el = el->proximo;
                continue;
            }

            if ( CheckCollisionRecs( retColCalculado, proj->ret ) ) {
                
                proj->ativo = false; // Desativa o projétil ao atingir o jogador

                if ( !j->invencivel && !j->invulneravel ) {
                    if ( j->temEscudo ) {
                        j->temEscudo = false;
                        PlaySound( rm.somHitComAnel );
                    } else if ( j->quantidadeAneis > 0 ) {
                        j->quantidadeAneis = 0;
                        PlaySound( rm.somHitComAnel );
                    } else {
                        j->quantidadeVidas--;
                        PlaySound( rm.somMorte );
                    }
                    j->invulneravel = true;
                }

                return;

            }

        } else if ( inimigo->tipo == TIPO_INIMIGO_CRABMEAT ) {

            InimigoCrabmeat *crab = (InimigoCrabmeat*) inimigo->objeto;

            if ( !crab->ativo || crab->estado == ESTADO_INIMIGO_CRABMEAT_MORRENDO ) {
                el = el->proximo;
                continue;
            }

            qaInimigo = getQuadroAnimacaoAtualInimigoCrabmeat( crab );
            olhandoParaDireita = &crab->olhandoParaDireita;
            ret = &crab->ret;

            float deslocamentoX = *olhandoParaDireita
                ? ret->width - qaInimigo->retColisao.x - qaInimigo->retColisao.width
                : qaInimigo->retColisao.x;
            float deslocamentoY = qaInimigo->retColisao.y;

            Rectangle retColInimigoCalculado = {
                ret->x + deslocamentoX,
                ret->y + deslocamentoY,
                qaInimigo->retColisao.width,
                qaInimigo->retColisao.height
            };

            if ( CheckCollisionRecs( retColCalculado, retColInimigoCalculado ) ) {

                if ( j->invencivel || (j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO) ) {
                    if ( j->estado >= ESTADO_JOGADOR_PULANDO && j->estado <= ESTADO_JOGADOR_PULANDO_CORRENDO ) {
                        j->vel.y = j->velPulo;
                    }
                    crab->estado = ESTADO_INIMIGO_CRABMEAT_MORRENDO;
                    crab->vel.x = 0;
                    j->score += 100;
                    PlaySound( rm.somHitInimigo );
                } else if ( !j->invulneravel ) {
                    if ( j->temEscudo ) {
                        j->temEscudo = false;
                        PlaySound( rm.somHitComAnel );
                    } else if ( j->quantidadeAneis > 0 ) {
                        j->quantidadeAneis = 0;
                        PlaySound( rm.somHitComAnel );
                    } else {
                        j->quantidadeVidas--;
                        PlaySound( rm.somMorte );
                    }
                    j->invulneravel = true;
                }

                return; // um inimigo de cada vez!

            }

        } else if ( inimigo->tipo == TIPO_INIMIGO_CRABMEAT_PROJETIL ) {

            InimigoCrabmeatProjetil *proj = (InimigoCrabmeatProjetil*) inimigo->objeto;

            if ( !proj->ativo ) {
                el = el->proximo;
                continue;
            }

            if ( CheckCollisionRecs( retColCalculado, proj->ret ) ) {
                
                proj->ativo = false; // Desativa o projétil ao atingir o jogador

                if ( !j->invencivel && !j->invulneravel ) {
                    if ( j->temEscudo ) {
                        j->temEscudo = false;
                        PlaySound( rm.somHitComAnel );
                    } else if ( j->quantidadeAneis > 0 ) {
                        j->quantidadeAneis = 0;
                        PlaySound( rm.somHitComAnel );
                    } else {
                        j->quantidadeVidas--;
                        PlaySound( rm.somMorte );
                    }
                    j->invulneravel = true;
                }

                return;

            }

        }

        el = el->proximo;

    }

}