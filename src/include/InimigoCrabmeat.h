/**
 * @file InimigoCrabmeat.h
 * @author Prof. Dr. David Buzatto / Adaptado
 * @brief Declarações das funções para o inimigo Crabmeat (Ganigani) e seu projétil.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "raylib/raylib.h"
#include "Tipos.h"

// Crabmeat
InimigoCrabmeat *criarInimigoCrabmeat( Rectangle ret, Color cor );
void destruirInimigoCrabmeat( InimigoCrabmeat *inimigo );
void atualizarInimigoCrabmeat( InimigoCrabmeat *inimigo, GameWorld *gw, float delta );
void desenharInimigoCrabmeat( InimigoCrabmeat *inimigo );
QuadroAnimacao *getQuadroAnimacaoAtualInimigoCrabmeat( InimigoCrabmeat *inimigo );

// Projétil (Bolinha de energia)
InimigoCrabmeatProjetil *criarInimigoCrabmeatProjetil( Rectangle ret, Vector2 vel );
void destruirInimigoCrabmeatProjetil( InimigoCrabmeatProjetil *proj );
void atualizarInimigoCrabmeatProjetil( InimigoCrabmeatProjetil *proj, GameWorld *gw, float delta );
void desenharInimigoCrabmeatProjetil( InimigoCrabmeatProjetil *proj );
