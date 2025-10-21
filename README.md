# Legends of History

## Sobre o Jogo

**Legends of History** é um jogo educativo e de aventura desenvolvido em **C** utilizando a biblioteca **Allegro 5**.

O jogador deve percorrer labirintos, atravessar portais e responder perguntas para avançar de fase. O projeto foi desenvolvido como trabalho acadêmico no curso de **Ciência da Computação**, com foco em lógica de programação e desenvolvimento de jogos 2D.

---

## Estrutura do Projeto

```
Legends-of-History/
├── assets/
│   ├── sprites/
│   │   ├── player_male.png
│   │   ├── player_female.png
│   │   ├── portal_1.png
│   │   ├── portal_2.png
│   │   ├── portal_3.png
│   │   ├── portal_4.png
│   │   ├── wall.png
│   │   ├── floor.png
│   │   └── logo.png
│   ├── audio/
│   │   ├── menu_music.ogg
│   │   └── game_music.ogg
│   └── fonts/
│       └── font.ttf
│
├── include/
│   ├── config.h
│   ├── game.h
│   ├── maze.h
│   ├── player.h
│   └── portal.h
│
├── src/
│   ├── main.c
│   ├── game.c
│   ├── intro.c
│   ├── menu.c
│   ├── maze.c
│   ├── player.c
│   ├── portal.c
│   ├── maze1.c
│   ├── maze2.c
│   ├── maze3.c
│   └── maze4.c
│
└── Makefile
```

---

## Controles do Jogo

| Ação | Tecla |
|------|--------|
| Mover para cima | ↑ |
| Mover para baixo | ↓ |
| Mover para esquerda | ← |
| Mover para direita | → |
| Confirmar / Iniciar | Enter |
| Sair do jogo | Esc |

---

## Requisitos

- macOS, Linux ou Windows
- Biblioteca **Allegro 5** instalada

Para instalar o Allegro no macOS:

```bash
brew install allegro
```

---

## Como Compilar e Executar

### Compilar o jogo
```bash
make
```

### Limpar arquivos objeto
```bash
make clean
```

### Executar o jogo
```bash
./bin/LegendsOfHistory
```

---

## Créditos

**Autora:** Emelly Lima, Lucas Parisi, Mariana Kajita
**Curso:** Ciência da Computação  
**Instituição:** Centro universitário Senac  
**Período:** 2º semestre

---

## Licença

Projeto de uso acadêmico. Distribuição permitida mediante citação da autora.
