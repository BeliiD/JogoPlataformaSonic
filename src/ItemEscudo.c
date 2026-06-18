/**
 * @file ItemEscudo.c
 * @author Prof. Dr. David Buzatto (Adaptado)
 * @brief Implementação do Item (Escudo).
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Animacao.h"
#include "ItemEscudo.h"
#include "Macros.h"
#include "ResourceManager.h"
#include "Tipos.h"

static void desenharQuadroAnimacaoItemEscudo( ItemEscudo *item, QuadroAnimacao *qa, Color tonalidade );
static Animacao *getAnimacaoAtualItemEscudo( ItemEscudo *item );

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria um novo Item (escudo).
 */
ItemEscudo *criarItemEscudo( Rectangle ret, Color cor ) {

    ItemEscudo *novoItem = (ItemEscudo*) malloc( sizeof( ItemEscudo ) );

    novoItem->ret = ret;
    novoItem->cor = cor;
    novoItem->estado = ESTADO_ITEM_ESCUDO_PARADO;
    novoItem->ativo = true;

    int quantidadeAnimacoes = 0;

    novoItem->animacaoParado.quantidadeQuadros = 6;
    novoItem->animacaoParado.quadroAtual = 0;
    novoItem->animacaoParado.contadorTempoQuadro = 0.0f;
    novoItem->animacaoParado.pararNoUltimoQuadro = false;
    novoItem->animacaoParado.executarUmaVez = false;
    novoItem->animacaoParado.finalizada = false;
    criarQuadrosAnimacao( &novoItem->animacaoParado, 6 );

    // Manual initialization of the 6 frames to implement flickering blue-gray striped shield
    novoItem->animacaoParado.quadros[0] = (QuadroAnimacao){ .fonte = (Rectangle){ 24, 584, 48, 48 }, .duracao = 50, .retColisao = (Rectangle){ 0, 0, 32, 32 } };
    novoItem->animacaoParado.quadros[1] = (QuadroAnimacao){ .fonte = (Rectangle){ 0, 0, 0, 0 },      .duracao = 50, .retColisao = (Rectangle){ 0, 0, 32, 32 } };
    novoItem->animacaoParado.quadros[2] = (QuadroAnimacao){ .fonte = (Rectangle){ 80, 584, 48, 48 }, .duracao = 50, .retColisao = (Rectangle){ 0, 0, 32, 32 } };
    novoItem->animacaoParado.quadros[3] = (QuadroAnimacao){ .fonte = (Rectangle){ 0, 0, 0, 0 },      .duracao = 50, .retColisao = (Rectangle){ 0, 0, 32, 32 } };
    novoItem->animacaoParado.quadros[4] = (QuadroAnimacao){ .fonte = (Rectangle){ 136, 584, 48, 48 },.duracao = 50, .retColisao = (Rectangle){ 0, 0, 32, 32 } };
    novoItem->animacaoParado.quadros[5] = (QuadroAnimacao){ .fonte = (Rectangle){ 0, 0, 0, 0 },      .duracao = 50, .retColisao = (Rectangle){ 0, 0, 32, 32 } };

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

    novoItem->animacoes[ESTADO_ITEM_ESCUDO_PARADO] = &novoItem->animacaoParado; quantidadeAnimacoes++;
    novoItem->animacoes[ESTADO_ITEM_ESCUDO_COLETADO] = &novoItem->animacaoColetando; quantidadeAnimacoes++;
    novoItem->quantidadeAnimacoes = quantidadeAnimacoes;

    return novoItem;

}

/**
 * @brief Destroi um item (escudo).
 */
void destruirItemEscudo( ItemEscudo *item ) {
    if ( item != NULL ) {
        for ( int i = 0; i < item->quantidadeAnimacoes; i++ ) {
            destruirQuadrosAnimacao( item->animacoes[i] );
        }
        free( item );
    }
}

/**
 * @brief Atualiza um item (escudo).
 */
void atualizarItemEscudo( ItemEscudo *item, float delta ) {
    if ( item->ativo ) {
        Animacao *animacaoAtual = getAnimacaoAtualItemEscudo( item );
        atualizarAnimacao( animacaoAtual, delta );
        if ( item->estado == ESTADO_ITEM_ESCUDO_COLETADO && animacaoAtual->finalizada ) {
            item->ativo = false;
        }
    }
}

/**
 * @brief Desenha um item (escudo).
 */
void desenharItemEscudo( ItemEscudo *item ) {
    if ( item->ativo ) {
        QuadroAnimacao *qa = getQuadroAnimacaoAtualItemEscudo( item );
        desenharQuadroAnimacaoItemEscudo( item, qa, WHITE );
        if ( MOSTRAR_RETANGULOS ) {
            DrawRectangleRec( item->ret, Fade( item->cor, 0.5f ) );
            DrawRectangleLines( item->ret.x, item->ret.y, item->ret.width, item->ret.height, BLACK );
        }
    }
}

/**
 * @brief Obtém o quadro de animação atual de um item (escudo).
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemEscudo( ItemEscudo *item ) {
    return getQuadroAtualAnimacao( getAnimacaoAtualItemEscudo( item ) );
}

static void desenharQuadroAnimacaoItemEscudo( ItemEscudo *item, QuadroAnimacao *qa, Color tonalidade ) {

    if ( qa != NULL ) {
        // Se for a animação de parado do escudo, não desenhar se for um quadro vazio (fonte.width == 0)
        if ( item->estado != ESTADO_ITEM_ESCUDO_COLETADO && (qa->fonte.width <= 0 || qa->fonte.height <= 0) ) {
            return;
        }

        DrawTexturePro(
            item->estado == ESTADO_ITEM_ESCUDO_COLETADO ? rm.texturaItens : rm.texturaShields,
            qa->fonte,
            item->ret,
            (Vector2) { 0 },
            0.0f,
            tonalidade
        );

        if ( MOSTRAR_RETANGULOS ) {
            float xDesenho = item->ret.x + qa->retColisao.x;
            float yDesenho = item->ret.y + qa->retColisao.y;
            DrawRectangle( xDesenho, yDesenho, qa->retColisao.width, qa->retColisao.height, Fade( GREEN, 0.5f ) );
        }

    }

}

static Animacao *getAnimacaoAtualItemEscudo( ItemEscudo *item ) {
    return item->animacoes[item->estado];
}