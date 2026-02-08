# DogRun

DogRun é um jogo 2D inspirado no famoso jogo do dinossauro do Google Chrome (Chrome Dino), no qual o personagem corre continuamente e deve desviar de obstáculos para sobreviver o maior tempo possível.

O jogador controla um cachorro que corre automaticamente pelo cenário e pode pular para evitar obstáculos que surgem ao longo do percurso. A pontuação aumenta com o tempo de sobrevivência e a dificuldade cresce progressivamente.

---

## Objetivo do Projeto

Este projeto está sendo desenvolvido como parte da **2ª Verificação de Aprendizagem** da disciplina de **Computação Gráfica**, ministrada pelo **Professor Dr. Ícaro Lins Leitão da Cunha**.

O objetivo é aplicar na prática os conceitos fundamentais de computação gráfica utilizando OpenGL, além de demonstrar organização de projeto, estrutura de código e uso de estruturas de dados.

---

## Ideia do Jogo

O jogo segue a mecânica clássica de *endless runner*:

- O personagem corre automaticamente
- Obstáculos surgem periodicamente
- O jogador pode pular para desviar
- A velocidade aumenta com o tempo
- O jogo termina ao colidir com um obstáculo


## Padrão de Projeto

Este projeto segue rigorosamente o padrão técnico definido para a disciplina:

| Item | Definição |
|-----|-----------|
| **Linguagem** | C++ |
| **API gráfica** | OpenGL |
| **Versão do OpenGL** | 3.3 Core Profile |
| **Criação de janela** | GLFW |
| **Carregamento de funções OpenGL** | GLAD |
| **Build system** | CMake |
| **Sistema operacional alvo** | Windows (compatível com Linux) |


