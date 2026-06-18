#pragma once

#include "Tipos.h"
#include "Animacao.h"

/**
 * @brief Cria um novo item (escudo).
 */
ItemEscudo *criarItemEscudo( Rectangle ret, Color cor );

/**
 * @brief Destroi um item (escudo).
 */
void destruirItemEscudo( ItemEscudo *item );

/**
 * @brief Atualiza um item (escudo).
 */
void atualizarItemEscudo( ItemEscudo *item, float delta );

/**
 * @brief Desenha um item (escudo).
 */
void desenharItemEscudo( ItemEscudo *item );

/**
 * @brief Obtém o quadro de animação atual de um item (escudo).
 */
QuadroAnimacao *getQuadroAnimacaoAtualItemEscudo( ItemEscudo *item );