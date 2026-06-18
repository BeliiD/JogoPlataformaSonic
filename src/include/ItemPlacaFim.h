#pragma once

#include "Tipos.h"

/**
 * @brief Cria um novo item (placa fim).
 */
ItemPlacaFim *criarItemPlacaFim( Rectangle ret, Color cor );

/**
 * @brief Destroi um item (placa fim).
 */
void destruirItemPlacaFim( ItemPlacaFim *item );

/**
 * @brief Atualiza um item (placa fim).
 */
void atualizarItemPlacaFim( ItemPlacaFim *item, float delta );

/**
 * @brief Desenha um item (placa fim).
 */
void desenharItemPlacaFim( ItemPlacaFim *item );
