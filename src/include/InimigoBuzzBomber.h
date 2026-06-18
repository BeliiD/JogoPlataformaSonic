/**
 * @file InimigoBuzzBomber.h
 * @author Prof. Dr. David Buzatto / Adaptado
 * @brief Declarações das funções para o inimigo Buzz Bomber e seu projétil.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include "raylib/raylib.h"
#include "Tipos.h"

// Buzz Bomber
InimigoBuzzBomber *criarInimigoBuzzBomber( Rectangle ret, Color cor );
void destruirInimigoBuzzBomber( InimigoBuzzBomber *buzz );
void atualizarInimigoBuzzBomber( InimigoBuzzBomber *buzz, GameWorld *gw, float delta );
void desenharInimigoBuzzBomber( InimigoBuzzBomber *buzz );
QuadroAnimacao *getQuadroAnimacaoAtualInimigoBuzzBomber( InimigoBuzzBomber *buzz );

// Projétil (Bomba)
InimigoBuzzBomberProjetil *criarInimigoBuzzBomberProjetil( Rectangle ret, Vector2 vel );
void destruirInimigoBuzzBomberProjetil( InimigoBuzzBomberProjetil *proj );
void atualizarInimigoBuzzBomberProjetil( InimigoBuzzBomberProjetil *proj, GameWorld *gw, float delta );
void desenharInimigoBuzzBomberProjetil( InimigoBuzzBomberProjetil *proj );
