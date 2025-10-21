{\rtf1\ansi\ansicpg1252\cocoartf2822
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\paperw11900\paperh16840\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx720\tx1440\tx2160\tx2880\tx3600\tx4320\tx5040\tx5760\tx6480\tx7200\tx7920\tx8640\pardirnatural\partightenfactor0

\f0\fs24 \cf0 # Legends of History\
\
## \uc0\u55357 \u56534  Sobre o Jogo\
\
**Legends of History** \'e9 um jogo educacional em formato de labirinto, desenvolvido em C com a biblioteca Allegro 5. O jogador escolhe entre dois avatares (feminino e masculino) e explora caminhos hist\'f3ricos, interagindo com figuras marcantes como Cle\'f3patra, Zumbi dos Palmares, Dom Pedro I e Napole\'e3o Bonaparte.\
\
O objetivo \'e9 chegar ao centro do labirinto, desbloqueando uma linha do tempo interativa a cada portal atravessado e pergunta respondida.\
\
**P\'fablico-Alvo:** Estudantes do Ensino M\'e9dio, professores e amantes de Hist\'f3ria que buscam uma forma interativa e divertida de aprender.\
\
### Mec\'e2nicas Principais\
* Explora\'e7\'e3o de labirintos que representam per\'edodos hist\'f3ricos.\
* Portais com perguntas de m\'faltipla escolha sobre figuras hist\'f3ricas.\
* Desafio computacional com combate em arena, no estilo _top-down_.\
* Escolha de avatar e progress\'e3o na linha do tempo.\
\
---\
\
## \uc0\u55357 \u57056 \u65039  Requisitos e Compila\'e7\'e3o\
\
Este projeto foi desenvolvido e configurado para o ambiente **macOS (Apple Silicon)**.\
\
### Pr\'e9-requisitos\
Para compilar e executar o projeto, \'e9 necess\'e1rio ter o **Homebrew** e as seguintes bibliotecas instaladas:\
\
1.  Abra o Terminal.\
2.  Execute o comando abaixo para instalar o Allegro 5 e suas depend\'eancias:\
    ```bash\
    brew install allegro pkg-config\
    ```\
\
### Como Compilar e Executar\
\
Com as depend\'eancias instaladas, utilize os comandos `make` na pasta raiz do projeto:\
\
* **Para compilar o jogo:**\
    ```bash\
    make\
    ```\
    Este comando criar\'e1 o execut\'e1vel na pasta `./bin/`.\
\
* **Para executar o jogo:**\
    ```bash\
    ./bin/LegendsOfHistory\
    ```\
\
* **Para limpar os arquivos de compila\'e7\'e3o:**\
    ```bash\
    make clean\
    ```\
\
---\
\
## \uc0\u55357 \u56960  Status do Projeto\
\
### Marco 1 (Entregue em 14/10/2025)\
\
* **Estrutura de pastas completa e profissional (`src`, `include`, `data`, etc.).**\
* **`Makefile` configurado para compila\'e7\'e3o em macOS com Allegro 5.**\
* **Esqueleto do jogo funcional, demonstrando a transi\'e7\'e3o entre todas as telas principais, conforme solicitado:**\
    1.  Tela de Menu\
    2.  Tela de Labirinto (placeholder)\
    3.  Tela de Quiz (placeholder)\
    4.  Tela de Luta (placeholder)\
    5.  Tela de Fim de Jogo\
\
### Marco 2 (Pr\'f3ximos Passos)\
\
* Implementa\'e7\'e3o da renderiza\'e7\'e3o gr\'e1fica com Allegro 5.\
* Desenvolvimento da l\'f3gica de leitura dos mapas de labirinto a partir da pasta `/data`.\
* Cria\'e7\'e3o do sistema de perguntas e respostas, lendo os quizzes da pasta `/data`.\
* Implementa\'e7\'e3o do sistema de combate em tempo real.\
* Integra\'e7\'e3o da movimenta\'e7\'e3o do jogador e da l\'f3gica de intera\'e7\'e3o com os portais.}