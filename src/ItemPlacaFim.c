/**
 * @file ItemPlacaFim.c
 * @brief Implementação do Item (Placa de Fim de Fase / Goal Post).
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"
#include "ItemPlacaFim.h"
#include "ResourceManager.h"
#include "Tipos.h"

static const bool MOSTRAR_RETANGULOS = false;

/**
 * @brief Cria uma nova placa de fim de fase.
 */
ItemPlacaFim *criarItemPlacaFim( Rectangle ret, Color cor ) {

    ItemPlacaFim *novoItem = (ItemPlacaFim*) malloc( sizeof( ItemPlacaFim ) );

    novoItem->ret = ret;
    novoItem->cor = cor;
    novoItem->estado = ESTADO_ITEM_PLACAFIM_ROBOTNIK;
    novoItem->ativo = true;
    novoItem->tempoQuadro = 0.05f; // 50ms por quadro de animação
    novoItem->contadorTempoQuadro = 0.0f;
    novoItem->frameAtual = 0;
    novoItem->passosRestantes = 0;
    novoItem->ladoSonic = false;
    novoItem->tempoExibicaoSonic = 0.0f;

    return novoItem;

}

/**
 * @brief Destroi a placa.
 */
void destruirItemPlacaFim( ItemPlacaFim *item ) {
    if ( item != NULL ) {
        free( item );
    }
}

/**
 * @brief Atualiza o estado da placa de fim de fase.
 */
void atualizarItemPlacaFim( ItemPlacaFim *item, float delta ) {
    if ( item != NULL && item->ativo ) {
        if ( item->estado == ESTADO_ITEM_PLACAFIM_GIRANDO ) {
            item->contadorTempoQuadro += delta;
            if ( item->contadorTempoQuadro >= item->tempoQuadro ) {
                item->contadorTempoQuadro = 0.0f;
                item->frameAtual++;
                item->passosRestantes--;

                if ( item->passosRestantes <= 0 ) {
                    if ( !item->ladoSonic ) {
                        // Concluiu os giros do Robotnik, inicia os giros do Sonic
                        item->ladoSonic = true;
                        item->passosRestantes = 8 * 4; // 8 rotações * 4 quadros
                    } else {
                        // Concluiu os giros do Sonic, para exibindo a foto do Sonic
                        item->estado = ESTADO_ITEM_PLACAFIM_SONIC;
                    }
                }
            }
        } else if ( item->estado == ESTADO_ITEM_PLACAFIM_SONIC ) {
            item->tempoExibicaoSonic += delta;
        }
    }
}

/**
 * @brief Desenha a placa de fim de fase.
 */
void desenharItemPlacaFim( ItemPlacaFim *item ) {

    if ( item == NULL || !item->ativo ) {
        return;
    }

    Rectangle src;

    if ( item->estado == ESTADO_ITEM_PLACAFIM_ROBOTNIK ) {
        src = (Rectangle){ 244, 678, 48, 48 };
    } else if ( item->estado == ESTADO_ITEM_PLACAFIM_SONIC ) {
        src = (Rectangle){ 244, 734, 48, 48 };
    } else { // ESTADO_ITEM_PLACAFIM_GIRANDO
        int passo = item->frameAtual % 4;
        if ( passo == 0 ) {
            if ( !item->ladoSonic ) {
                src = (Rectangle){ 244, 678, 48, 48 };
            } else {
                src = (Rectangle){ 244, 734, 48, 48 };
            }
        } else if ( passo == 1 ) {
            src = (Rectangle){ 300, 706, 32, 48 };
        } else if ( passo == 2 ) {
            src = (Rectangle){ 340, 706, 8, 48 };
        } else { // passo == 3
            src = (Rectangle){ 356, 706, 32, 48 };
        }
    }

    // Calcula escala baseada na altura da placa (48px no sprite sheet original)
    float escala = item->ret.height / 48.0f;
    float draw_w = src.width * escala;
    float draw_h = src.height * escala;

    // Centraliza o desenho em relação ao eixo horizontal da placa
    float cx = item->ret.x + item->ret.width / 2.0f;
    float draw_x = cx - draw_w / 2.0f;
    float draw_y = item->ret.y;

    DrawTexturePro(
        rm.texturaObjetosBaseS1,
        src,
        (Rectangle){ draw_x, draw_y, draw_w, draw_h },
        (Vector2){ 0.0f, 0.0f },
        0.0f,
        WHITE
    );

    if ( MOSTRAR_RETANGULOS ) {
        DrawRectangleRec( item->ret, Fade( item->cor, 0.5f ) );
        DrawRectangleLines( item->ret.x, item->ret.y, item->ret.width, item->ret.height, BLACK );
    }

}
