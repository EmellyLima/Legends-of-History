## Nome do Projeto: **Legends of History**

## Descrição Geral:
O Legends of History é um jogo educativo 2D desenvolvido em C com a biblioteca Allegro 5, que combina elementos de ação e aprendizado histórico.
O jogador percorre labirintos, enfrenta inimigos e responde perguntas de História para avançar entre quatro fases de dificuldade crescente.
Cada fase representa um desafio tanto de agilidade quanto de conhecimento.

## Funcionalidades Principais:
Tela de menu inicial com opções de Jogar, Regras, Som e Sair
Escolha de personagem (masculino ou feminino)
Sistema de nome do jogador
Quatro fases com labirintos e portais
Inimigos inteligentes com perseguição e ataques à distância
Sistema de vidas e pontuação
Perguntas históricas interativas
Som e trilha opcional
Tela final de vitória

## Tecnologias Utilizadas
Linguagem: C
Biblioteca gráfica: Allegro 5
Compilador: GCC / Clang
IDE recomendada: Visual Studio / Code
Sistema Operacional compatível: Windows, macOS, Linux

## Pastas do Jogo
Legends-of-History/
│
├── assets/
│   ├── sprites/         # Sprites dos personagens, inimigos e logotipo
│   ├── audio/           # Trilha sonora e efeitos sonoros
│   ├── fonts/           # Fontes utilizadas no jogo
│   └── quizzes/         # Arquivos de perguntas e respostas
│
├── src/
│   ├── main.c
│   ├── game.c
│   ├── intro.c
│   ├── player.c
│   ├── enemy.c
│   ├── maze.c
│   ├── quiz.c
│   └── ...
│
├── include/
│   ├── game.h
│   ├── player.h
│   ├── enemy.h
│   ├── maze.h
│   ├── quiz.h
│   └── ...
│
├── Makefile
└── README.md

## Sistemas

Como Executar:
Instale a Allegro 5

macOS:
brew install allegro

Linux:
sudo apt install liballegro5-dev

Windows:
Baixe o SDK no site oficial da Allegro.

Compile o jogo:
make

Execute:
make run

## Objetivo Educacional
O jogo busca integrar o aprendizado de História com a prática de programação e desenvolvimento de jogos.
Cada fase foi projetada para relacionar desafios de lógica com temas históricos, tornando o aprendizado mais envolvente e interativo.

## Créditos
**Autores:** Emelly Lima, Lucas Parisi, Mariana Kajita
**Curso:** Ciência da Computação  
**Instituição:** Centro universitário Senac  
**Período:** 2º semestre

## Licença
Projeto de uso acadêmico. Distribuição permitida mediante citação dos autores.
