/**
 * @file InimigoBuzzBomber.c
 * @author Prof. Dr. David Buzatto / Adaptado
 * @brief Implementação do Inimigo Buzz Bomber e seu Projétil.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "Inimigo.h"
#include "InimigoBuzzBomber.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoInimigoBuzzBomber( InimigoBuzzBomber *inimigo, QuadroAnimacao *qa, Color tonalidade );
static void desenharQuadroAnimacaoInimigoBuzzBomberMorrendo( InimigoBuzzBomber *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade );
static Animacao *getAnimacaoAtualInimigoBuzzBomber( InimigoBuzzBomber *inimigo );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Inimigo (Buzz Bomber).
 */
InimigoBuzzBomber *criarInimigoBuzzBomber( Rectangle ret, Color cor ) {

    InimigoBuzzBomber *novoInimigo = (InimigoBuzzBomber*) malloc( sizeof( InimigoBuzzBomber ) );

    novoInimigo->ret = ret;
    novoInimigo->vel = (Vector2) { 0 };
    novoInimigo->cor = cor;

    novoInimigo->velVoo = 120;

    novoInimigo->estado = ESTADO_INIMIGO_BUZZ_BOMBER_VOANDO;
    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = false;

    novoInimigo->cooldownTiro = 0.0f;
    novoInimigo->tempoPreparacaoTiro = 0.0f;

    novoInimigo->quantidadeAnimacoes = 0;

    // Animação Voando (2 quadros - abelha azul Buzz Bomber com asas voando)
    novoInimigo->animacaoVoando.quantidadeQuadros = 2;
    novoInimigo->animacaoVoando.quadroAtual = 0;
    novoInimigo->animacaoVoando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoVoando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoVoando.executarUmaVez = false;
    novoInimigo->animacaoVoando.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoVoando, 2 );
    novoInimigo->animacaoVoando.quadros[0] = (QuadroAnimacao) {
        .fonte = { 24, 358, 45, 19 },
        .duracao = 150,
        .retColisao = { 6, 4, 78, 30 }
    };
    novoInimigo->animacaoVoando.quadros[1] = (QuadroAnimacao) {
        .fonte = { 80, 358, 45, 19 },
        .duracao = 150,
        .retColisao = { 6, 4, 78, 30 }
    };

    // Animação Atirando (2 quadros - abelha azul Buzz Bomber apontando a cauda para baixo para atirar)
    novoInimigo->animacaoAtirando.quantidadeQuadros = 2;
    novoInimigo->animacaoAtirando.quadroAtual = 0;
    novoInimigo->animacaoAtirando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoAtirando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoAtirando.executarUmaVez = false;
    novoInimigo->animacaoAtirando.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoAtirando, 2 );
    novoInimigo->animacaoAtirando.quadros[0] = (QuadroAnimacao) {
        .fonte = { 24, 390, 45, 19 },
        .duracao = 200,
        .retColisao = { 6, 4, 78, 30 }
    };
    novoInimigo->animacaoAtirando.quadros[1] = (QuadroAnimacao) {
        .fonte = { 80, 390, 45, 19 },
        .duracao = 200,
        .retColisao = { 6, 4, 78, 30 }
    };

    // Animação Morrendo (4 quadros, explosão idêntica ao Motobug)
    novoInimigo->animacaoMorrendo.quantidadeQuadros = 4;
    novoInimigo->animacaoMorrendo.quadroAtual = 0;
    novoInimigo->animacaoMorrendo.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoMorrendo.pararNoUltimoQuadro = false;
    novoInimigo->animacaoMorrendo.executarUmaVez = true;
    novoInimigo->animacaoMorrendo.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoMorrendo, 4 );
    inicializarQuadrosAnimacao( 
        novoInimigo->animacaoMorrendo.quadros,
        novoInimigo->animacaoMorrendo.quantidadeQuadros,
        100,              // duração padrão para todos os quadros
        169, 1,           // início
        32, 32,           // dimensões
        1,                // separação
        false,            // de trás para frente
        (Rectangle) { 0 } // retângulo de colisão padrão para cada quadro
    );

    novoInimigo->animacoes[ESTADO_INIMIGO_BUZZ_BOMBER_VOANDO] = &novoInimigo->animacaoVoando; novoInimigo->quantidadeAnimacoes++;
    novoInimigo->animacoes[ESTADO_INIMIGO_BUZZ_BOMBER_ATIRANDO] = &novoInimigo->animacaoAtirando; novoInimigo->quantidadeAnimacoes++;
    
    return novoInimigo;

}

/**
 * @brief Destroi um Buzz Bomber.
 */
void destruirInimigoBuzzBomber( InimigoBuzzBomber *buzz ) {
    if ( buzz != NULL ) {
        for ( int i = 0; i < buzz->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( buzz->animacoes[i] );
        }
        destruirQuadrosAnimacao( &buzz->animacaoMorrendo );
        free( buzz );
    }
}

/**
 * @brief Atualiza um Buzz Bomber.
 */
void atualizarInimigoBuzzBomber( InimigoBuzzBomber *buzz, GameWorld *gw, float delta ) {

    if ( !buzz->ativo ) {
        return;
    }

    if ( buzz->estado == ESTADO_INIMIGO_BUZZ_BOMBER_VOANDO ) {

        Animacao *animacaoAtual = getAnimacaoAtualInimigoBuzzBomber( buzz );
        atualizarAnimacao( animacaoAtual, delta );

        if ( buzz->olhandoParaDireita ) {
            buzz->vel.x = buzz->velVoo;
        } else {
            buzz->vel.x = -buzz->velVoo;
        }

        Inimigo ini = {
            .objeto = buzz,
            .tipo = TIPO_INIMIGO_BUZZ_BOMBER
        };

        // Movimento horizontal e colisão com o cenário
        buzz->ret.x += buzz->vel.x * delta;
        resolverColisaoInimigoObstaculosMapaX( &ini, gw->mapa );

        // Atualização da recarga de tiro
        if ( buzz->cooldownTiro > 0.0f ) {
            buzz->cooldownTiro -= delta;
        }

        // Lógica de Detecção do Sonic para atirar
        Jogador *j = gw->jogador;
        float distX = j->ret.x - buzz->ret.x;
        float distY = j->ret.y - buzz->ret.y;

        // Se o Sonic estiver na distância de detecção (ex: 220px horizontais e 150px verticais abaixo)
        if ( buzz->cooldownTiro <= 0.0f && fabsf( distX ) < 220.0f && distY > 0.0f && distY < 150.0f ) {
            buzz->estado = ESTADO_INIMIGO_BUZZ_BOMBER_ATIRANDO;
            buzz->tempoPreparacaoTiro = 0.5f; // 0.5s preparando o tiro
            buzz->vel.x = 0;
            buzz->olhandoParaDireita = ( distX > 0 );
            reiniciarAnimacao( &buzz->animacaoAtirando );
        }

    } else if ( buzz->estado == ESTADO_INIMIGO_BUZZ_BOMBER_ATIRANDO ) {

        atualizarAnimacao( &buzz->animacaoAtirando, delta );

        buzz->tempoPreparacaoTiro -= delta;
        if ( buzz->tempoPreparacaoTiro <= 0.0f ) {
            
            // Disparar bomba diagonalmente
            float spawnX = buzz->ret.x + ( buzz->olhandoParaDireita ? buzz->ret.width - 20 : 0 );
            float spawnY = buzz->ret.y + buzz->ret.height - 12;
            
            Rectangle rect = { spawnX, spawnY, 22, 22 }; // projétil de 11x11 escalado por 2x
            Vector2 vel = { buzz->olhandoParaDireita ? 180.0f : -180.0f, 180.0f };

            // Instancia o projétil
            Inimigo *inimigoProj = criarInimigo( TIPO_INIMIGO_BUZZ_BOMBER_PROJETIL );
            inimigoProj->objeto = criarInimigoBuzzBomberProjetil( rect, vel );

            // Insere dinamicamente na lista encadeada do mapa
            ElementoMapa *el = (ElementoMapa*) malloc( sizeof( ElementoMapa ) );
            el->objeto = inimigoProj;
            el->tipo = TIPO_ELEMENTO_MAPA_INIMIGO;
            el->proximo = gw->mapa->inimigos;
            gw->mapa->inimigos = el;
            gw->mapa->quantidadeInimigos++;

            // Som de tiro
            PlaySound( rm.somPulo );

            // Retorna ao estado normal de voo
            buzz->estado = ESTADO_INIMIGO_BUZZ_BOMBER_VOANDO;
            buzz->cooldownTiro = 2.5f; // 2.5 segundos de cooldown
            reiniciarAnimacao( &buzz->animacaoVoando );

        }

    } else if ( buzz->estado == ESTADO_INIMIGO_BUZZ_BOMBER_MORRENDO ) {

        atualizarAnimacao( &buzz->animacaoMorrendo, delta );

        if ( buzz->animacaoMorrendo.finalizada ) {
            buzz->ativo = false;
        }

    }

}

/**
 * @brief Desenha um Buzz Bomber.
 */
void desenharInimigoBuzzBomber( InimigoBuzzBomber *buzz ) {

    if ( !buzz->ativo ) {
        return;
    }

    if ( buzz->estado == ESTADO_INIMIGO_BUZZ_BOMBER_VOANDO ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualInimigoBuzzBomber( buzz );
        desenharQuadroAnimacaoInimigoBuzzBomber( buzz, qa, WHITE );
    } else if ( buzz->estado == ESTADO_INIMIGO_BUZZ_BOMBER_ATIRANDO ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualInimigoBuzzBomber( buzz );
        desenharQuadroAnimacaoInimigoBuzzBomber( buzz, qa, WHITE );
    } else if ( buzz->estado == ESTADO_INIMIGO_BUZZ_BOMBER_MORRENDO ) {
        desenharQuadroAnimacaoInimigoBuzzBomberMorrendo( buzz, getQuadroAtualAnimacao( &buzz->animacaoMorrendo ), 2.0f, WHITE );
    }

    if ( MOSTRAR_RETANGULOS ) {
        DrawRectangleRec( buzz->ret, Fade( buzz->cor, 0.5f ) );
        DrawRectangleLines( buzz->ret.x, buzz->ret.y, buzz->ret.width, buzz->ret.height, BLACK );
    }

}

/**
 * @brief Obtém o quadro atual da animação do Buzz Bomber.
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoBuzzBomber( InimigoBuzzBomber *buzz ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualInimigoBuzzBomber( buzz ) );
}

static void desenharQuadroAnimacaoInimigoBuzzBomber( InimigoBuzzBomber *inimigo, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {
        
        DrawTexturePro(
            rm.texturaBadniksBase,
            (Rectangle) {
                qa->fonte.x,
                qa->fonte.y,
                inimigo->olhandoParaDireita ? -qa->fonte.width : qa->fonte.width,
                qa->fonte.height
            },
            inimigo->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = inimigo->olhandoParaDireita
                ? inimigo->ret.x + inimigo->ret.width - qa->retColisao.x - qa->retColisao.width
                : inimigo->ret.x + qa->retColisao.x;
            float yDesenho = inimigo->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static void desenharQuadroAnimacaoInimigoBuzzBomberMorrendo( InimigoBuzzBomber *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade ) {

    if ( qa != NULL ) {
        
        DrawTexturePro(
            rm.texturaBadniks,
            qa->fonte,
            (Rectangle) {
                inimigo->ret.x + ( inimigo->ret.width - qa->fonte.width * escala ) / 2.0f,
                inimigo->ret.y + ( inimigo->ret.height - qa->fonte.height * escala ) / 2.0f,
                qa->fonte.width * escala,
                qa->fonte.height * escala
            },
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

    }

}

static Animacao *getAnimacaoAtualInimigoBuzzBomber( InimigoBuzzBomber *buzz ) {
    return buzz->animacoes[buzz->estado];
}


// =============================================================================
// PROJÉTIL (BOMBA) IMPLEMENTATION
// =============================================================================

/**
 * @brief Cria um novo projétil.
 */
InimigoBuzzBomberProjetil *criarInimigoBuzzBomberProjetil( Rectangle ret, Vector2 vel ) {

    InimigoBuzzBomberProjetil *proj = (InimigoBuzzBomberProjetil*) malloc( sizeof( InimigoBuzzBomberProjetil ) );
    
    proj->ret = ret;
    proj->vel = vel;
    proj->cor = WHITE;
    proj->ativo = true;
    proj->fonte = (Rectangle){ 138, 350, 11, 11 };
    proj->vidaUtil = 3.0f; // 3 segundos de vida útil

    return proj;

}

/**
 * @brief Destroi o projétil.
 */
void destruirInimigoBuzzBomberProjetil( InimigoBuzzBomberProjetil *proj ) {
    if ( proj != NULL ) {
        free( proj );
    }
}

/**
 * @brief Atualiza o projétil.
 */
void atualizarInimigoBuzzBomberProjetil( InimigoBuzzBomberProjetil *proj, GameWorld *gw, float delta ) {

    if ( !proj->ativo ) {
        return;
    }

    // Movimentação
    proj->ret.x += proj->vel.x * delta;
    proj->ret.y += proj->vel.y * delta;

    // Ciclo de vida útil
    proj->vidaUtil -= delta;
    if ( proj->vidaUtil <= 0.0f ) {
        proj->ativo = false;
        return;
    }

    // Colisão com obstáculos do cenário (desativa se colidir)
    ElementoMapa *el = gw->mapa->obstaculos;
    while ( el != NULL ) {
        Obstaculo *o = (Obstaculo*) el->objeto;
        if ( CheckCollisionRecs( proj->ret, o->ret ) ) {
            proj->ativo = false;
            break;
        }
        el = el->proximo;
    }

}

/**
 * @brief Desenha o projétil.
 */
void desenharInimigoBuzzBomberProjetil( InimigoBuzzBomberProjetil *proj ) {

    if ( !proj->ativo ) {
        return;
    }

    DrawTexturePro(
        rm.texturaBadniksBase,
        (Rectangle) {
            proj->fonte.x,
            proj->fonte.y,
            proj->vel.x > 0 ? -proj->fonte.width : proj->fonte.width,
            proj->fonte.height
        },
        proj->ret,
        (Vector2) { 0 },
        0.0f,
        WHITE
    );

    if ( MOSTRAR_RETANGULOS ) {
        DrawRectangleRec( proj->ret, Fade( RED, 0.5f ) );
    }

}
