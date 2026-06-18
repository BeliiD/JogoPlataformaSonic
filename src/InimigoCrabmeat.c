/**
 * @file InimigoCrabmeat.c
 * @author Prof. Dr. David Buzatto / Adaptado
 * @brief Implementação do Inimigo Crabmeat e seu Projétil.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "Inimigo.h"
#include "InimigoCrabmeat.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoInimigoCrabmeat( InimigoCrabmeat *inimigo, QuadroAnimacao *qa, Color tonalidade );
static void desenharQuadroAnimacaoInimigoCrabmeatMorrendo( InimigoCrabmeat *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade );
static Animacao *getAnimacaoAtualInimigoCrabmeat( InimigoCrabmeat *inimigo );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Inimigo (Crabmeat).
 */
InimigoCrabmeat *criarInimigoCrabmeat( Rectangle ret, Color cor ) {

    InimigoCrabmeat *novoInimigo = (InimigoCrabmeat*) malloc( sizeof( InimigoCrabmeat ) );

    novoInimigo->ret = ret;
    novoInimigo->vel = (Vector2) { 0 };
    novoInimigo->cor = cor;

    novoInimigo->velAndando = 60;
    novoInimigo->velMaxQueda = 600;

    novoInimigo->estado = ESTADO_INIMIGO_CRABMEAT_ANDANDO;
    novoInimigo->ativo = true;
    novoInimigo->olhandoParaDireita = false;

    novoInimigo->walkTimer = 2.5f;
    novoInimigo->shootCooldownTimer = 0.0f;
    novoInimigo->shootPreparationTimer = 0.0f;
    novoInimigo->hasFired = false;

    novoInimigo->quantidadeAnimacoes = 0;

    // Animação Andando: ciclo Walk 1 -> Walk 2 -> Walk 3 -> Walk 2 (4 quadros)
    // Coordenadas uniformizadas: w=48, h=38, y=199
    novoInimigo->animacaoAndando.quantidadeQuadros = 4;
    novoInimigo->animacaoAndando.quadroAtual = 0;
    novoInimigo->animacaoAndando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoAndando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoAndando.executarUmaVez = false;
    novoInimigo->animacaoAndando.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoAndando, 4 );
    
    // Walk 1 (X=80)
    novoInimigo->animacaoAndando.quadros[0] = (QuadroAnimacao) {
        .fonte = { 80, 199, 48, 38 },
        .duracao = 150,
        .retColisao = { 8, 8, 80, 68 }
    };
    // Walk 2 (X=137)
    novoInimigo->animacaoAndando.quadros[1] = (QuadroAnimacao) {
        .fonte = { 137, 199, 48, 38 },
        .duracao = 150,
        .retColisao = { 8, 8, 80, 68 }
    };
    // Walk 3 (X=192)
    novoInimigo->animacaoAndando.quadros[2] = (QuadroAnimacao) {
        .fonte = { 192, 199, 48, 38 },
        .duracao = 150,
        .retColisao = { 8, 8, 80, 68 }
    };
    // Walk 2 (X=137)
    novoInimigo->animacaoAndando.quadros[3] = (QuadroAnimacao) {
        .fonte = { 137, 199, 48, 38 },
        .duracao = 150,
        .retColisao = { 8, 8, 80, 68 }
    };

    // Animação Atirando: usa o frame de disparo (1 quadro)
    // Firing (X=247)
    novoInimigo->animacaoAtirando.quantidadeQuadros = 1;
    novoInimigo->animacaoAtirando.quadroAtual = 0;
    novoInimigo->animacaoAtirando.contadorTempoQuadro = 0.0f;
    novoInimigo->animacaoAtirando.pararNoUltimoQuadro = false;
    novoInimigo->animacaoAtirando.executarUmaVez = false;
    novoInimigo->animacaoAtirando.finalizada = false;
    criarQuadrosAnimacao( &novoInimigo->animacaoAtirando, 1 );
    novoInimigo->animacaoAtirando.quadros[0] = (QuadroAnimacao) {
        .fonte = { 247, 199, 48, 38 },
        .duracao = 1000,
        .retColisao = { 8, 8, 80, 68 }
    };

    // Animação Morrendo: explosão padrão do Sonic (4 quadros)
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

    novoInimigo->animacoes[ESTADO_INIMIGO_CRABMEAT_ANDANDO] = &novoInimigo->animacaoAndando; novoInimigo->quantidadeAnimacoes++;
    novoInimigo->animacoes[ESTADO_INIMIGO_CRABMEAT_ATIRANDO] = &novoInimigo->animacaoAtirando; novoInimigo->quantidadeAnimacoes++;

    return novoInimigo;

}

/**
 * @brief Destroi um Crabmeat.
 */
void destruirInimigoCrabmeat( InimigoCrabmeat *inimigo ) {
    if ( inimigo != NULL ) {
        for ( int i = 0; i < inimigo->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( inimigo->animacoes[i] );
        }
        destruirQuadrosAnimacao( &inimigo->animacaoMorrendo );
        free( inimigo );
    }
}

/**
 * @brief Atualiza um Crabmeat.
 */
void atualizarInimigoCrabmeat( InimigoCrabmeat *inimigo, GameWorld *gw, float delta ) {

    if ( !inimigo->ativo ) {
        return;
    }

    if ( inimigo->estado == ESTADO_INIMIGO_CRABMEAT_ANDANDO ) {

        Animacao *animacaoAtual = getAnimacaoAtualInimigoCrabmeat( inimigo );
        atualizarAnimacao( animacaoAtual, delta );

        if ( inimigo->olhandoParaDireita ) {
            inimigo->vel.x = inimigo->velAndando;
        } else {
            inimigo->vel.x = -inimigo->velAndando;
        }

        Inimigo ini = {
            .objeto = inimigo,
            .tipo = TIPO_INIMIGO_CRABMEAT
        };

        // Movimento horizontal e colisão X
        inimigo->ret.x += inimigo->vel.x * delta;
        resolverColisaoInimigoObstaculosMapaX( &ini, gw->mapa );

        // Gravidade e colisão Y
        inimigo->vel.y += gw->gravidade * delta;
        if ( inimigo->vel.y > inimigo->velMaxQueda ) {
            inimigo->vel.y = inimigo->velMaxQueda;
        }
        inimigo->ret.y += inimigo->vel.y * delta;
        resolverColisaoInimigoObstaculosMapaY( &ini, gw->mapa );

        // Timer para parar e atirar
        inimigo->walkTimer -= delta;
        if ( inimigo->walkTimer <= 0.0f ) {
            inimigo->estado = ESTADO_INIMIGO_CRABMEAT_ATIRANDO;
            inimigo->shootPreparationTimer = 0.5f; // 0.5s preparando o tiro
            inimigo->hasFired = false;
            inimigo->vel.x = 0;
            reiniciarAnimacao( &inimigo->animacaoAtirando );
        }

    } else if ( inimigo->estado == ESTADO_INIMIGO_CRABMEAT_ATIRANDO ) {

        atualizarAnimacao( &inimigo->animacaoAtirando, delta );

        Inimigo ini = {
            .objeto = inimigo,
            .tipo = TIPO_INIMIGO_CRABMEAT
        };

        // Aplica gravidade e resolve Y mesmo parado
        inimigo->vel.y += gw->gravidade * delta;
        if ( inimigo->vel.y > inimigo->velMaxQueda ) {
            inimigo->vel.y = inimigo->velMaxQueda;
        }
        inimigo->ret.y += inimigo->vel.y * delta;
        resolverColisaoInimigoObstaculosMapaY( &ini, gw->mapa );

        inimigo->shootPreparationTimer -= delta;
        if ( inimigo->shootPreparationTimer <= 0.0f && !inimigo->hasFired ) {

            // Disparar duas bolinhas (uma de cada pinça)
            float spawnX1 = inimigo->ret.x + 10;
            float spawnY1 = inimigo->ret.y + 16;
            
            float spawnX2 = inimigo->ret.x + inimigo->ret.width - 32;
            float spawnY2 = inimigo->ret.y + 16;

            Rectangle rect1 = { spawnX1, spawnY1, 22, 22 }; // 11x11 escalado 2x
            Rectangle rect2 = { spawnX2, spawnY2, 22, 22 };
            
            // Lançadas para cima e diagonalmente
            Vector2 vel1 = { -120.0f, -380.0f };
            Vector2 vel2 = { 120.0f, -380.0f };

            // Instancia o projétil da esquerda
            Inimigo *proj1 = criarInimigo( TIPO_INIMIGO_CRABMEAT_PROJETIL );
            proj1->objeto = criarInimigoCrabmeatProjetil( rect1, vel1 );
            ElementoMapa *el1 = (ElementoMapa*) malloc( sizeof( ElementoMapa ) );
            el1->objeto = proj1;
            el1->tipo = TIPO_ELEMENTO_MAPA_INIMIGO;
            el1->proximo = gw->mapa->inimigos;
            gw->mapa->inimigos = el1;
            gw->mapa->quantidadeInimigos++;

            // Instancia o projétil da direita
            Inimigo *proj2 = criarInimigo( TIPO_INIMIGO_CRABMEAT_PROJETIL );
            proj2->objeto = criarInimigoCrabmeatProjetil( rect2, vel2 );
            ElementoMapa *el2 = (ElementoMapa*) malloc( sizeof( ElementoMapa ) );
            el2->objeto = proj2;
            el2->tipo = TIPO_ELEMENTO_MAPA_INIMIGO;
            el2->proximo = gw->mapa->inimigos;
            gw->mapa->inimigos = el2;
            gw->mapa->quantidadeInimigos++;

            // Tocar som de tiro
            PlaySound( rm.somPulo );

            inimigo->hasFired = true;

        }

        // 0.5s preparando + 0.5s esperando após o tiro
        if ( inimigo->shootPreparationTimer <= -0.5f ) {
            inimigo->estado = ESTADO_INIMIGO_CRABMEAT_ANDANDO;
            inimigo->walkTimer = 2.5f;
            // Muda de direção depois de atirar
            inimigo->olhandoParaDireita = !inimigo->olhandoParaDireita;
            reiniciarAnimacao( &inimigo->animacaoAndando );
        }

    } else if ( inimigo->estado == ESTADO_INIMIGO_CRABMEAT_MORRENDO ) {

        atualizarAnimacao( &inimigo->animacaoMorrendo, delta );

        if ( inimigo->animacaoMorrendo.finalizada ) {
            inimigo->ativo = false;
        }

    }

}

/**
 * @brief Desenha um Crabmeat.
 */
void desenharInimigoCrabmeat( InimigoCrabmeat *inimigo ) {

    if ( !inimigo->ativo ) {
        return;
    }

    if ( inimigo->estado == ESTADO_INIMIGO_CRABMEAT_ANDANDO || inimigo->estado == ESTADO_INIMIGO_CRABMEAT_ATIRANDO ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualInimigoCrabmeat( inimigo );
        desenharQuadroAnimacaoInimigoCrabmeat( inimigo, qa, WHITE );
    } else if ( inimigo->estado == ESTADO_INIMIGO_CRABMEAT_MORRENDO ) {
        desenharQuadroAnimacaoInimigoCrabmeatMorrendo( inimigo, getQuadroAtualAnimacao( &inimigo->animacaoMorrendo ), 2.0f, WHITE );
    }

    if ( MOSTRAR_RETANGULOS ) {
        DrawRectangleRec( inimigo->ret, Fade( inimigo->cor, 0.5f ) );
        DrawRectangleLines( inimigo->ret.x, inimigo->ret.y, inimigo->ret.width, inimigo->ret.height, BLACK );
    }

}

/**
 * @brief Obtém o quadro de animação atual do Crabmeat.
 */
QuadroAnimacao *getQuadroAnimacaoAtualInimigoCrabmeat( InimigoCrabmeat *inimigo ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualInimigoCrabmeat( inimigo ) );
}

static void desenharQuadroAnimacaoInimigoCrabmeat( InimigoCrabmeat *inimigo, QuadroAnimacao *qa, Color tonalidade ) {

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

static void desenharQuadroAnimacaoInimigoCrabmeatMorrendo( InimigoCrabmeat *inimigo, QuadroAnimacao *qa, float escala, Color tonalidade ) {

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

static Animacao *getAnimacaoAtualInimigoCrabmeat( InimigoCrabmeat *inimigo ) {
    return inimigo->animacoes[inimigo->estado];
}


// =============================================================================
// PROJÉTIL (BOLINHA DE ENERGIA) IMPLEMENTATION
// =============================================================================

/**
 * @brief Cria um novo projétil do Crabmeat.
 */
InimigoCrabmeatProjetil *criarInimigoCrabmeatProjetil( Rectangle ret, Vector2 vel ) {

    InimigoCrabmeatProjetil *proj = (InimigoCrabmeatProjetil*) malloc( sizeof( InimigoCrabmeatProjetil ) );
    
    proj->ret = ret;
    proj->vel = vel;
    proj->cor = WHITE;
    proj->ativo = true;
    proj->fonte = (Rectangle){ 306, 200, 11, 11 }; // Começa na bolinha vermelha
    proj->vidaUtil = 3.0f; // 3 segundos de vida útil
    proj->tempoAnimacao = 0.0f;
    proj->quadroAtual = 0;

    return proj;

}

/**
 * @brief Destroi o projétil.
 */
void destruirInimigoCrabmeatProjetil( InimigoCrabmeatProjetil *proj ) {
    if ( proj != NULL ) {
        free( proj );
    }
}

/**
 * @brief Atualiza o projétil.
 */
void atualizarInimigoCrabmeatProjetil( InimigoCrabmeatProjetil *proj, GameWorld *gw, float delta ) {

    if ( !proj->ativo ) {
        return;
    }

    // Movimentação balística: aplica gravidade no eixo Y
    proj->vel.y += gw->gravidade * delta;
    
    proj->ret.x += proj->vel.x * delta;
    proj->ret.y += proj->vel.y * delta;

    // Ciclo de vida útil
    proj->vidaUtil -= delta;
    if ( proj->vidaUtil <= 0.0f ) {
        proj->ativo = false;
        return;
    }

    // Alternar frames para efeito piscante
    proj->tempoAnimacao += delta;
    if ( proj->tempoAnimacao >= 0.1f ) {
        proj->tempoAnimacao = 0.0f;
        proj->quadroAtual = !proj->quadroAtual;
        if ( proj->quadroAtual == 0 ) {
            proj->fonte = (Rectangle){ 306, 200, 11, 11 }; // vermelha
        } else {
            proj->fonte = (Rectangle){ 306, 224, 11, 11 }; // branca/vermelha
        }
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
void desenharInimigoCrabmeatProjetil( InimigoCrabmeatProjetil *proj ) {

    if ( !proj->ativo ) {
        return;
    }

    DrawTexturePro(
        rm.texturaBadniksBase,
        proj->fonte,
        proj->ret,
        (Vector2) { 0 },
        0.0f,
        WHITE
    );

    if ( MOSTRAR_RETANGULOS ) {
        DrawRectangleRec( proj->ret, Fade( RED, 0.5f ) );
    }

}
