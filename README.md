# 🕹️ Legends of History

**Legends of History** é um jogo educativo interativo desenvolvido em C com a biblioteca **Allegro 5**. O jogador explora um labirinto, responde perguntas históricas e descobre curiosidades enquanto enfrenta inimigos e desafios.

---

## 📁 Estrutura do Projeto
LegendsofHistory/
├── src/             # Código-fonte (.c)
├── include/         # Arquivos de cabeçalho (.h)
├── data/            # Perguntas e curiosidades em .txt
├── docs/            # Documentação adicional
├── Makefile         # Script de compilação
├── README.md        # Este arquivo

---

## 🛠️ Requisitos

- Compilador C compatível com C99 (`gcc`, `clang`, etc.)
- Biblioteca **Allegro 5**
- Sistema operacional compatível (Linux, macOS, Windows)

---

## 📦 Instalação das dependências

Se estiver usando [vcpkg](https://github.com/microsoft/vcpkg):

```bash
./vcpkg install allegro5