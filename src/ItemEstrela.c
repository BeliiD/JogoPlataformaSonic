/**
 * @file ItemEstrela.c
 * @author Prof. Dr. David Buzatto (Adaptado)
 * @brief Implementação do Item (Estrela).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "ItemEstrela.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoItemEstrela( ItemEstrela *item, QuadroAnimacao *qa, Color tonalidade );
static Animacao *getAnimacaoAtualItemEstrela( ItemEstrela *item );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Item (estrela).
 */
ItemEstrela *criarItemEstrela( Rectangle ret, Color cor ) {

    ItemEstrela *novoItem = (ItemEstrela*) malloc( sizeof( ItemEstrela ) );

    novoItem->ret = ret;
    novoItem->cor = cor;
    novoItem->estado = ESTADO_ITEM_ESTRELA_PARADO;
    novoItem->ativo = true;

    int quantidadeAnimacoes = 0;

    novoItem->animacaoParado.quantidadeQuadros = 4;
    novoItem->animacaoParado.quadroAtual = 0;
    novoItem->animacaoParado.contadorTempoQuadro = 0.0f;
    novoItem->animacaoParado.pararNoUltimoQuadro = false;
    novoItem->animacaoParado.executarUmaVez = false;
    novoItem->animacaoParado.finalizada = false;
    criarQuadrosAnimacao( &novoItem->animacaoParado, 4 );

    // Manual initialization of the 4 frames of the monitor screen (cycling between star icon and static noise)
    novoItem->animacaoParado.quadros[0] = (QuadroAnimacao){ .fonte = (Rectangle){ 72, 564, 16, 16 },  .duracao = 400, .retColisao = (Rectangle){ 0, 0, 30, 30 } };
    novoItem->animacaoParado.quadros[1] = (QuadroAnimacao){ .fonte = (Rectangle){ 24, 540, 16, 16 },  .duracao = 80,  .retColisao = (Rectangle){ 0, 0, 30, 30 } };
    novoItem->animacaoParado.quadros[2] = (QuadroAnimacao){ .fonte = (Rectangle){ 48, 540, 16, 16 },  .duracao = 80,  .retColisao = (Rectangle){ 0, 0, 30, 30 } };
    novoItem->animacaoParado.quadros[3] = (QuadroAnimacao){ .fonte = (Rectangle){ 72, 540, 16, 16 },  .duracao = 80,  .retColisao = (Rectangle){ 0, 0, 30, 30 } };

    novoItem->animacaoColetando.quantidadeQuadros = 4;
    novoItem->animacaoColetando.quadroAtual = 0;
    novoItem->animacaoColetando.contadorTempoQuadro = 0.0f;
    novoItem->animacaoColetando.pararNoUltimoQuadro = false;
    novoItem->animacaoColetando.executarUmaVez = true;
    novoItem->animacaoColetando.finalizada = false;
    criarQuadrosAnimacao( &novoItem->animacaoColetando, novoItem->animacaoColetando.quantidadeQuadros );
    inicializarQuadrosAnimacao( 
        novoItem->animacaoColetando.quadros,
        novoItem->animacaoColetando.quantidadeQuadros,
        80,               // duração padrão para todos os quadros
        1, 18,            // início (x, y) faíscas de anel em itens.png
        16, 16,           // dimensões
        1,                // separação
        false,            // de trás para frente
        (Rectangle) { 0 } // retângulo de colisão padrão para cada quadro
    );

    novoItem->animacoes[ESTADO_ITEM_ESTRELA_PARADO] = &novoItem->animacaoParado; quantidadeAnimacoes++;
    novoItem->animacoes[ESTADO_ITEM_ESTRELA_COLETADO] = &novoItem->animacaoColetando; quantidadeAnimacoes++;
    novoItem->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoItem;

}

/**
 * @brief Destroi um item (estrela).
 */
void destruirItemEstrela( ItemEstrela *item ) {
    if ( item != NULL ) {
        for ( int i = 0; i < item->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( item->animacoes[i] );
        }
        free( item );
    }
}

/**
 * @brief Atualiza um item (estrela).
 */
void atualizarItemEstrela( ItemEstrela *item, float delta ) {
    if ( item->ativo ) {
        Animacao *animacaoAtual = getAnimacaoAtualItemEstrela( item );
        atualizarAnimacao( animacaoAtual, delta );
        if ( item->estado == ESTADO_ITEM_ESTRELA_COLETADO && animacaoAtual->finalizada ) {
            item->ativo = false;
        }
    }
}

/**
 * @brief Desenha um item (estrela).
 */
void desenharItemEstrela( ItemEstrela *item ) {
    if ( item->ativo ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualItemEstrela( item );
        desenharQuadroAnimacaoItemEstrela( item, qa, WHITE );
        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( item->ret, Fade( item->cor, 0.5f ) );
            DrawRectangleLines( item->ret.x, item->ret.y, item->ret.width, item->ret.height, BLACK );
        }
    }
}

/**
 * @brief Obtém o quadro de animação atual de um item (estrela).
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemEstrela( ItemEstrela *item ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualItemEstrela( item ) );
}

static void desenharQuadroAnimacaoItemEstrela( ItemEstrela *item, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {

        if ( item->estado == ESTADO_ITEM_ESTRELA_PARADO ) {
            // 1. Draw the monitor body (30x30 in objetos-base-s1.png)
            DrawTexturePro(
                rm.texturaObjetosBaseS1,
                (Rectangle){ 25, 487, 30, 30 },
                item->ret,
                (Vector2) { 0 },
                0.0f,
                tonalidade
            );

            // 2. Draw the screen icon (qa->fonte) centered inside the screen area (X_offset=7, Y_offset=5, size=16x16)
            Rectangle retTela = {
                item->ret.x + (7.0f / 30.0f) * item->ret.width,
                item->ret.y + (5.0f / 30.0f) * item->ret.height,
                (16.0f / 30.0f) * item->ret.width,
                (16.0f / 30.0f) * item->ret.height
            };
            DrawTexturePro(
                rm.texturaObjetosBaseS1,
                qa->fonte,
                retTela,
                (Vector2) { 0 },
                0.0f,
                tonalidade
            );
        } else {
            // When collected, draw sparkles from rm.texturaItens
            DrawTexturePro(
                rm.texturaItens,
                qa->fonte,
                item->ret,
                (Vector2) { 0 },
                0.0f,
                tonalidade
            );
        }

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = item->ret.x + qa->retColisao.x;
            float yDesenho = item->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static Animacao *getAnimacaoAtualItemEstrela( ItemEstrela *item ) {
    return item->animacoes[item->estado];
}