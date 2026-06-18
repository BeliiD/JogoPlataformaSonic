/**
 * @file Inimigo.c
 * @author Prof. Dr. David Buzatto
 * @brief Implementação do Inimigo.
 *
 * @copyright Copyright (c) 2026
 */
#include <stdio.h>
#include <stdlib.h>

#include "raylib/raylib.h"

#include "Inimigo.h"
#include "InimigoMotobug.h"
#include "InimigoBuzzBomber.h"
#include "InimigoCrabmeat.h"
#include "Tipos.h"

/**
 * @brief Cria um novo Inimigo.
 */
Inimigo *criarInimigo( TipoInimigo tipo ) {

    Inimigo *novoInimigo = (Inimigo*) malloc( sizeof( Inimigo ) );
    novoInimigo->objeto = NULL;
    novoInimigo->tipo = tipo;

    return novoInimigo;

}

/**
 * @brief Destroi um inimigo.
 */
void destruirInimigo( Inimigo *inimigo ) {
    if ( inimigo != NULL ) {
        switch ( inimigo->tipo ) {
            case TIPO_INIMIGO_MOTOBUG:
                destruirInimigoMotobug( (InimigoMotobug*) inimigo->objeto );
                break;
            case TIPO_INIMIGO_BUZZ_BOMBER:
                destruirInimigoBuzzBomber( (InimigoBuzzBomber*) inimigo->objeto );
                break;
            case TIPO_INIMIGO_BUZZ_BOMBER_PROJETIL:
                destruirInimigoBuzzBomberProjetil( (InimigoBuzzBomberProjetil*) inimigo->objeto );
                break;
            case TIPO_INIMIGO_CRABMEAT:
                destruirInimigoCrabmeat( (InimigoCrabmeat*) inimigo->objeto );
                break;
            case TIPO_INIMIGO_CRABMEAT_PROJETIL:
                destruirInimigoCrabmeatProjetil( (InimigoCrabmeatProjetil*) inimigo->objeto );
                break;
            default:
                break;
        }
        free( inimigo );
    }
}

/**
 * @brief Atualiza um inimigo.
 */
void atualizarInimigo( Inimigo *inimigo, GameWorld *gw, float delta ) {

    switch ( inimigo->tipo ) {
        case TIPO_INIMIGO_MOTOBUG:
            atualizarInimigoMotobug( (InimigoMotobug*) inimigo->objeto, gw, delta );
            break;
        case TIPO_INIMIGO_BUZZ_BOMBER:
            atualizarInimigoBuzzBomber( (InimigoBuzzBomber*) inimigo->objeto, gw, delta );
            break;
        case TIPO_INIMIGO_BUZZ_BOMBER_PROJETIL:
            atualizarInimigoBuzzBomberProjetil( (InimigoBuzzBomberProjetil*) inimigo->objeto, gw, delta );
            break;
        case TIPO_INIMIGO_CRABMEAT:
            atualizarInimigoCrabmeat( (InimigoCrabmeat*) inimigo->objeto, gw, delta );
            break;
        case TIPO_INIMIGO_CRABMEAT_PROJETIL:
            atualizarInimigoCrabmeatProjetil( (InimigoCrabmeatProjetil*) inimigo->objeto, gw, delta );
            break;
        default:
            return;
    }

}

/**
 * @brief Desenha um inimigo.
 */
void desenharInimigo( Inimigo *inimigo ) {

    switch ( inimigo->tipo ) {
        case TIPO_INIMIGO_MOTOBUG:
            desenharInimigoMotobug( (InimigoMotobug*) inimigo->objeto );
            break;
        case TIPO_INIMIGO_BUZZ_BOMBER:
            desenharInimigoBuzzBomber( (InimigoBuzzBomber*) inimigo->objeto );
            break;
        case TIPO_INIMIGO_BUZZ_BOMBER_PROJETIL:
            desenharInimigoBuzzBomberProjetil( (InimigoBuzzBomberProjetil*) inimigo->objeto );
            break;
        case TIPO_INIMIGO_CRABMEAT:
            desenharInimigoCrabmeat( (InimigoCrabmeat*) inimigo->objeto );
            break;
        case TIPO_INIMIGO_CRABMEAT_PROJETIL:
            desenharInimigoCrabmeatProjetil( (InimigoCrabmeatProjetil*) inimigo->objeto );
            break;
        default:
            return;
    }

}

/**
 * @brief Resolve colisões do inimigo com o mapa no eixo X.
 */
void resolverColisaoInimigoObstaculosMapaX( Inimigo *inimigo, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

        QuadroAnimacao *qa = NULL;

        bool *olhandoParaDireita = NULL;
        Rectangle *ret = NULL;

        if ( inimigo->tipo == TIPO_INIMIGO_MOTOBUG ) {
            InimigoMotobug *motobug = (InimigoMotobug*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoMotobug( motobug );
            olhandoParaDireita = &motobug->olhandoParaDireita;
            ret = &motobug->ret;
        } else if ( inimigo->tipo == TIPO_INIMIGO_BUZZ_BOMBER ) {
            InimigoBuzzBomber *buzz = (InimigoBuzzBomber*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoBuzzBomber( buzz );
            olhandoParaDireita = &buzz->olhandoParaDireita;
            ret = &buzz->ret;
        } else if ( inimigo->tipo == TIPO_INIMIGO_CRABMEAT ) {
            InimigoCrabmeat *crab = (InimigoCrabmeat*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoCrabmeat( crab );
            olhandoParaDireita = &crab->olhandoParaDireita;
            ret = &crab->ret;
        } else {
            el = el->proximo;
            continue;
        }

        float deslocamentoX = *olhandoParaDireita
            ? ret->width - qa->retColisao.x - qa->retColisao.width
            : qa->retColisao.x;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            ret->x + deslocamentoX,
            ret->y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        Obstaculo *o = (Obstaculo*) el->objeto;

        if ( CheckCollisionRecs( retColCalculado, o->ret ) ) {
            if ( retColCalculado.x + retColCalculado.width / 2 < o->ret.x + o->ret.width / 2 ) {
                ret->x = o->ret.x - qa->retColisao.width - deslocamentoX;
                *olhandoParaDireita = !*olhandoParaDireita;
            } else {
                ret->x = o->ret.x + o->ret.width - deslocamentoX;
                *olhandoParaDireita = !*olhandoParaDireita;
            }
        }

        el = el->proximo;

    }

}

/**
 * @brief Resolve colisões do inimigo com o mapa no eixo Y.
 */
void resolverColisaoInimigoObstaculosMapaY( Inimigo *inimigo, Mapa *mapa ) {

    ElementoMapa *el = mapa->obstaculos;

    while ( el != NULL ) {

        Obstaculo *o = (Obstaculo*) el->objeto;
        QuadroAnimacao *qa = NULL;

        bool *olhandoParaDireita = NULL;
        Rectangle *ret = NULL;
        Vector2 *vel = NULL;

        if ( inimigo->tipo == TIPO_INIMIGO_MOTOBUG ) {
            InimigoMotobug *motobug = (InimigoMotobug*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoMotobug( motobug );
            olhandoParaDireita = &motobug->olhandoParaDireita;
            ret = &motobug->ret;
            vel = &motobug->vel;
        } else if ( inimigo->tipo == TIPO_INIMIGO_BUZZ_BOMBER ) {
            InimigoBuzzBomber *buzz = (InimigoBuzzBomber*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoBuzzBomber( buzz );
            olhandoParaDireita = &buzz->olhandoParaDireita;
            ret = &buzz->ret;
            vel = &buzz->vel;
        } else if ( inimigo->tipo == TIPO_INIMIGO_CRABMEAT ) {
            InimigoCrabmeat *crab = (InimigoCrabmeat*) inimigo->objeto;
            qa = getQuadroAnimacaoAtualInimigoCrabmeat( crab );
            olhandoParaDireita = &crab->olhandoParaDireita;
            ret = &crab->ret;
            vel = &crab->vel;
        } else {
            el = el->proximo;
            continue;
        }

        float deslocamentoX = *olhandoParaDireita
            ? ret->width - qa->retColisao.x - qa->retColisao.width
            : qa->retColisao.x;
        float deslocamentoY = qa->retColisao.y;

        Rectangle retColCalculado = {
            ret->x + deslocamentoX,
            ret->y + deslocamentoY,
            qa->retColisao.width,
            qa->retColisao.height
        };

        if ( CheckCollisionRecs( retColCalculado, o->ret ) ) {
            if ( retColCalculado.y + retColCalculado.height / 2 < o->ret.y + o->ret.height / 2 ) {
                ret->y = o->ret.y - qa->retColisao.height - deslocamentoY;
            } else {
                ret->y = o->ret.y + o->ret.height - deslocamentoY;
            }
            vel->y = 0;
        }

        el = el->proximo;

    }

}