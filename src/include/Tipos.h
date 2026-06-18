/**
 * @file Tipos.h
 * @author Prof. Dr. David Buzatto
 * @brief Definição dos tipos utilizados no jogo.
 *
 * @copyright Copyright (c) 2026
 */
#pragma once

#include <stdbool.h>

#include "raylib/raylib.h"

/**
 * @brief Representa o estado do jogador.
 */
typedef enum EstadoJogador {
    ESTADO_JOGADOR_PARADO,
    ESTADO_JOGADOR_ANDANDO,
    ESTADO_JOGADOR_ANDANDO_RAPIDO,
    ESTADO_JOGADOR_CORRENDO,
    ESTADO_JOGADOR_PULANDO,
    ESTADO_JOGADOR_PULANDO_RAPIDO,
    ESTADO_JOGADOR_PULANDO_CORRENDO,
} EstadoJogador;

/**
 * @brief Representa o estado do inimigo do tipo Motobug.
 */
typedef enum EstadoInimigoMotobug {
    ESTADO_INIMIGO_MOTOBUG_ANDANDO,
    ESTADO_INIMIGO_MOTOBUG_MORRENDO,
} EstadoInimigoMotobug;

/**
 * @brief Representa o estado do inimigo do tipo Buzz Bomber.
 */
typedef enum EstadoInimigoBuzzBomber {
    ESTADO_INIMIGO_BUZZ_BOMBER_VOANDO,
    ESTADO_INIMIGO_BUZZ_BOMBER_ATIRANDO,
    ESTADO_INIMIGO_BUZZ_BOMBER_MORRENDO,
} EstadoInimigoBuzzBomber;

/**
 * @brief Representa o estado do inimigo do tipo Crabmeat.
 */
typedef enum EstadoInimigoCrabmeat {
    ESTADO_INIMIGO_CRABMEAT_ANDANDO,
    ESTADO_INIMIGO_CRABMEAT_ATIRANDO,
    ESTADO_INIMIGO_CRABMEAT_MORRENDO,
} EstadoInimigoCrabmeat;

/**
 * @brief Representa o tipo de um inimigo.
 */
typedef enum TipoInimigo {
    TIPO_INIMIGO_MOTOBUG,
    TIPO_INIMIGO_BUZZ_BOMBER,
    TIPO_INIMIGO_BUZZ_BOMBER_PROJETIL,
    TIPO_INIMIGO_CRABMEAT,
    TIPO_INIMIGO_CRABMEAT_PROJETIL,
} TipoInimigo;

/**
 * @brief Representa o estado do item do tipo anel.
 */
typedef enum EstadoItemAnel {
    ESTADO_ITEM_ANEL_PARADO,
    ESTADO_ITEM_ANEL_COLETADO,
} EstadoItemAnel;

typedef enum EstadoItemEscudo {
    ESTADO_ITEM_ESCUDO_PARADO,
    ESTADO_ITEM_ESCUDO_COLETADO,
} EstadoItemEscudo;

typedef enum EstadoItemEstrela {
    ESTADO_ITEM_ESTRELA_PARADO,
    ESTADO_ITEM_ESTRELA_COLETADO,
} EstadoItemEstrela;

/**
 * @brief Representa o tipo de um item.
 */

typedef enum TipoItem {
    TIPO_ITEM_ANEL,
    TIPO_ITEM_ESCUDO,
    TIPO_ITEM_ESTRELA,
    TIPO_ITEM_PLACAFIM
} TipoItem;

/**
 * @brief Representa o tipo de um elemento do mapa
 */
typedef enum TipoElementoMapa {
    TIPO_ELEMENTO_MAPA_OBSTACULO,
    TIPO_ELEMENTO_MAPA_ITEM,
    TIPO_ELEMENTO_MAPA_INIMIGO,
} TipoElementoMapa;

/**
 * @brief Representa um quadro de animação com imagem.
 */
typedef struct QuadroAnimacao {
    Rectangle fonte;
    int duracao;          // milisegundos
    Rectangle retColisao; // deve ser interpretado como posicionamento relativo
} QuadroAnimacao;

/**
 * @brief Representa uma animação.
 */
typedef struct Animacao {
    QuadroAnimacao *quadros;
    int quantidadeQuadros;
    int quadroAtual;
    int contadorTempoQuadro;
    bool pararNoUltimoQuadro;
    bool executarUmaVez;
    bool finalizada;
} Animacao;

/**
 * @brief Representa o jogador controlado pelo usuário.
 */
typedef struct Jogador {

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velAndando;
    float velAndandoRapido;
    float velCorrendo;
    float velPulo;
    float velMaxQueda;

    float aceleracao;
    float desaceleracao;
    float frenagem;

    int quantidadePulos;
    int quantidadeMaxPulos;

    int quantidadeAneis;
    int quantidadeVidas;
    int score;

    bool invulneravel;
    float tempoInvulnerabilidade;
    float contadorTempoInvulnerabilidade;

    bool piscaPisca;
    float tempoPiscaPisca;
    float contadorTempoPiscaPisca;

    bool freando;

    bool temEscudo;
    bool invencivel;
    float tempoInvencibilidade;
    Animacao animacaoEscudo;
    Animacao animacaoSparkles;

    EstadoJogador estado;
    bool olhandoParaDireita;

    Animacao *animacoes[20];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoAndando;
    Animacao animacaoAndandoRapido;
    Animacao animacaoCorrendo;
    Animacao animacaoPulando;
    Animacao animacaoPulandoRapido;
    Animacao animacaoPulandoCorrendo;

} Jogador;

/**
 * @brief Representa um inimigo do tipo Motobug.
 */
typedef struct InimigoMotobug {

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velAndando;
    float velMaxQueda;

    EstadoInimigoMotobug estado;
    bool ativo;
    bool olhandoParaDireita;     // *cuidado! a reflexão dos inimigos é ao contrário
                                 // do jogador! eles começam olhando para a esquerda
                                 // e as sprites são orientadas para a esquerda inicialmente
    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoAndando;
    Animacao animacaoMorrendo;

} InimigoMotobug;

/**
 * @brief Representa um inimigo do tipo Buzz Bomber.
 */
typedef struct InimigoBuzzBomber {

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velVoo;

    EstadoInimigoBuzzBomber estado;
    bool ativo;
    bool olhandoParaDireita;

    Animacao *animacoes[3];
    int quantidadeAnimacoes;

    Animacao animacaoVoando;
    Animacao animacaoAtirando;
    Animacao animacaoMorrendo;

    float cooldownTiro;
    float tempoPreparacaoTiro;

} InimigoBuzzBomber;

/**
 * @brief Representa o projétil (bomba) disparado pelo Buzz Bomber.
 */
typedef struct InimigoBuzzBomberProjetil {

    Rectangle ret;
    Vector2 vel;
    Color cor;
    bool ativo;
    Rectangle fonte;
    float vidaUtil;

} InimigoBuzzBomberProjetil;

/**
 * @brief Representa um inimigo do tipo Crabmeat.
 */
typedef struct InimigoCrabmeat {

    Rectangle ret;
    Vector2 vel;
    Color cor;

    float velAndando;
    float velMaxQueda;

    EstadoInimigoCrabmeat estado;
    bool ativo;
    bool olhandoParaDireita;

    Animacao *animacoes[3];
    int quantidadeAnimacoes;

    Animacao animacaoAndando;
    Animacao animacaoAtirando;
    Animacao animacaoMorrendo;

    float walkTimer;
    float shootCooldownTimer;
    float shootPreparationTimer;
    bool hasFired;

} InimigoCrabmeat;

/**
 * @brief Representa o projétil disparado pelo Crabmeat.
 */
typedef struct InimigoCrabmeatProjetil {

    Rectangle ret;
    Vector2 vel;
    Color cor;
    bool ativo;
    Rectangle fonte;
    float vidaUtil;
    float tempoAnimacao;
    int quadroAtual;

} InimigoCrabmeatProjetil;

/**
 * @brief Representa um inimigo.
 * O inimigo de fato é endereçado via membro "objeto".
 */
typedef struct Inimigo {
    void *objeto;
    TipoInimigo tipo;
} Inimigo;

/**
 * @brief Representa um item do tipo anel.
 */
typedef struct ItemAnel {

    Rectangle ret;
    Color cor;

    EstadoItemAnel estado;
    bool ativo;

    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoColetando;

} ItemAnel;

typedef struct ItemEscudo {
    Rectangle ret;
    Color cor;

    EstadoItemEscudo estado;
    bool ativo;

    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoColetando;
    
} ItemEscudo;

typedef struct ItemEstrela {
    Rectangle ret;
    Color cor;

    EstadoItemEstrela estado;
    bool ativo;

    Animacao *animacoes[2];
    int quantidadeAnimacoes;

    Animacao animacaoParado;
    Animacao animacaoColetando;
} ItemEstrela;

typedef enum EstadoItemPlacaFim {
    ESTADO_ITEM_PLACAFIM_ROBOTNIK,
    ESTADO_ITEM_PLACAFIM_GIRANDO,
    ESTADO_ITEM_PLACAFIM_SONIC
} EstadoItemPlacaFim;

typedef struct ItemPlacaFim {
    Rectangle ret;
    Color cor;
    EstadoItemPlacaFim estado;
    bool ativo;
    float tempoQuadro;
    float contadorTempoQuadro;
    int frameAtual;
    int passosRestantes;
    bool ladoSonic;
    float tempoExibicaoSonic;
} ItemPlacaFim;

/**
 * @brief Representa um item estático do mapa.
 * O item de fato é endereçado via membro "objeto".
 */
typedef struct Item {
    void *objeto;
    TipoItem tipo;
} Item;

/**
 * @brief Representa um obstáculo estático do mapa.
 */
typedef struct Obstaculo {
    Rectangle ret;
    Color cor;
    Rectangle fonte;
    Texture2D *textura;
} Obstaculo;

/**
 * @brief Representa um elemento do mapa.
 * O elemento de fato é endereçado via membro "objeto".
 */
typedef struct ElementoMapa ElementoMapa;
struct ElementoMapa {
    void *objeto;
    TipoElementoMapa tipo;
    ElementoMapa *proximo;
};

/**
 * @brief Representa um mapa de fase do jogo.
 */
typedef struct Mapa {

    // listas ligadas de elementos do mapa
    ElementoMapa *obstaculos; // marca o fim da lista
    int quantidadeObstaculos;

    ElementoMapa *itens;      // marca o fim da lista
    int quantidadeItens;

    ElementoMapa *inimigos;   // marca o fim da lista
    int quantidadeInimigos;

    float dimensaoPadraoElementos;
    int linhas;
    int colunas;

} Mapa;

/**
 * @brief Representa a HUD do jogo (aneis, vida, pontuação e tempo)
 */
typedef struct  Hud {
   
    int pontuacao;
    int tempo;
    float acumularTempo;

} Hud;



/**
 * @brief Representa o mundo do jogo e seus elementos.
 */
typedef struct GameWorld {

    Mapa *mapa;
    Jogador *jogador;


    Camera2D camera;

    float gravidade;
    float tempoDeJogo;

    int nivelAtual;

} GameWorld;