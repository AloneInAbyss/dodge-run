# DODGE RUN
Dodge Run é um jogo desenvolvido para o microcontrolador **ATmega328p** (Arduino). Se baseia em um personagem em um display LCD que deve desviar de obstáculos que vêm em sua direção.

## Desenvolvimento
O código do [Arduino](https://www.arduino.cc/) foi criado usando um IDE chamado [PlatformIO](https://platformio.org/) instalado em cima do editor de texto [Visual Studio Code](https://code.visualstudio.com/). O diagrama de ligações foi criado utilizando o software [Fritzing](https://fritzing.org/home/). Todos esses softwares são gratuitos e open-source.
- [PlatformIO](https://platformio.org/)
- [Visual Studio Code](https://code.visualstudio.com/)
- [Fritzing](https://fritzing.org/home/)
- [Arduino](https://www.arduino.cc/)

### Componentes
O jogo é feito com base no microcontrolador ATmega328p (que está presente no Arduino), e consiste em um display LCD com um personagem (apelidado de Yetti) que se move para cima e para baixo desviando de obstáculos que vêm em sua direção. Inclui um botão para realizar o movimento, um buzzer para emitir efeitos sonoros, o display LCD de 16x2 e um Arduino Nano. **Alternativamente outros modelos de Arduino, por exemplo o Arduino UNO, também podem ser utilizados**.

- Arduino NANO
- Protoboard
- Display LCD 16x2
- Pushbutton
- Buzzer
- Potenciômetros
- Cabos de ligação

![Diagrama de Ligações](https://github.com/AloneInAbyss/dodge-run/blob/master/diagrama-ligacao.png)

## Instalação 
- **Com Arduino IDE**: Para testar nosso projeto utilizando o Arduino IDE basta acessar o arquivo [main.cpp](https://github.com/AloneInAbyss/dodge-run/blob/master/src/main.cpp) (dodge-run/src/main.cpp), copiar seu conteúdo (exceto a primeira linha que contém uma biblioteca de compatibilidade com o PlatformIO) e colar no Arduino IDE. Faça as conexões elétricas de acordo com o [diagrama esquemático](https://github.com/AloneInAbyss/dodge-run/blob/master/diagrama-ligacao.png). Depois disso conecte o Arduino ao computador e faça o upload do programa.

- **Com PlatformIO**: Basta clonar este repositório e abrir o projeto utilizando o PlatformIO. O arquivo principal é o [main.cpp](https://github.com/AloneInAbyss/dodge-run/blob/master/src/main.cpp) (dodge-run/src/main.cpp), verifique se as bibliotecas LiquidCrystal.h e Wire.h estão instaladas. Após fazer todas as conexões de acordo com o [diagrama esquemático](https://github.com/AloneInAbyss/dodge-run/blob/master/diagrama-ligacao.png) basta conectar o Arduino ao computador e fazer upload.
