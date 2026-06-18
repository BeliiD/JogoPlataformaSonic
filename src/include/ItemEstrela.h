#pragma once

#include "Tipos.h"
#include "Animacao.h"

/**
 * @brief Cria um novo item (estrela).
 */
ItemEstrela *criarItemEstrela( Rectangle ret, Color cor );

/**
 * @brief Destroi um item (estrela).
 */
void destruirItemEstrela( ItemEstrela *item );

/**
 * @brief Atualiza um item (estrela).
 */
void atualizarItemEstrela( ItemEstrela *item, float delta );

/**
 * @brief Desenha um item (estrela).
 */
void desenharItemEstrela( ItemEstrela *item );

/**
 * @brief Obtém o quadro de animação atual de um item (estrela).
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemEstrela( ItemEstrela *item );