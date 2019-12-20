/* Para testar na IDE padrão do Arduino, remova este #include e copie todo o código restante. */
#include <Arduino.h>

/*
 *    HEISENBERG - Dodge Run
 *    Terceira Versão
 *    Desenvolvedores: Thiago Assi & Michell Christofferson
*/

// Display LCD
#include <LiquidCrystal.h>
int rs = 12, e = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, e, d4, d5, d6, d7);

// Pinos
const int buttonPin = 8;
const int buzzerPin = A0;

// Execução do jogo
bool debugMode;
bool exibirTelaInicio = true;
bool exibirGameOver = false;
bool iniciarJogo = false;
int score = 0;

// Botão
int buttonState;
int reading;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

// Tempo do loop
unsigned long lastDelayRodada = 0;
unsigned long delayRodada;

// Objetos
int obstaculo_x[5];
int obstaculo_y[5];
int obstaculos = 0;
int maxObst;
int espera = 0;
int py, px;

// Funções
void exibirTelaInicial();
void exibirTelaGameOver();
void reiniciarVariaveis();
void exibirObjetos();
void moverPersonagem();
bool verificarColisao();
void moverObstaculos();

// Sprites
byte yetti[] = {
  B01110,
  B11111,
  B10101,
  B10101,
  B01110,
  B11111,
  B11111,
  B11011
};

byte obstaculo[] = {
  B00000,
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00100
};

byte trofeu[] = {
  B10101,
  B01110,
  B11111,
  B10101,
  B11111,
  B01110,
  B11011,
  B10001
};

byte trofeu_balao[] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B01000,
  B10000
};

// Início da Execução
void setup() {
  // Modo Debug
  debugMode = false;
  if (debugMode) {
    Serial.begin(9600);
  }
  
  // Pinos
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  
  // Número de Colunas e Linhas no Display
  lcd.begin(16, 2);

  // Cria caracteres
  lcd.createChar(0, yetti);
  lcd.createChar(1, obstaculo);
  lcd.createChar(2, trofeu_balao);
  lcd.createChar(3, trofeu);
}

// Laço da Execução
void loop() {
  // Tela Inicial
  if (exibirTelaInicio) {
    exibirTelaInicial();
    exibirTelaInicio = false;
  }
  
  // Tela de GAME OVER
  if (exibirGameOver) {
    Serial.print("GAME OVER! Score: ");
    Serial.println(score);
    exibirTelaGameOver();
    exibirGameOver = false;
  } 

  while (!iniciarJogo) {
    // Leitura do botão
    int reading = digitalRead(buttonPin);
    if (reading != lastButtonState) {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
      if (reading != buttonState) {
        buttonState = reading;
        if (buttonState == LOW) {
          // Ação a ser tomada ao pressionar o botão
          iniciarJogo = true;
        }
      }
    }
    lastButtonState = reading;
  }

  if (iniciarJogo) {
    Serial.println("Início do Jogo");
    Serial.println("");
    Serial.println("-------------------------------------");
    Serial.println("");
    delay(250);

    // Resetar o valor das variáveis
    reiniciarVariaveis();

    // Mostrar os objetos na tela
    exibirObjetos();

    // Loop principal do jogo
    while (!exibirGameOver) {
      // Leitura do botão
      int reading = digitalRead(buttonPin);
      if (reading != lastButtonState) {
        lastDebounceTime = millis();
      }
      if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading != buttonState) {
          buttonState = reading;
          if (buttonState == LOW) {
            // Ação a ser tomada ao pressionar o botão
            moverPersonagem();
          }
        }
      }
      lastButtonState = reading;

      // Espera "delayRodada", em milissegundos & verifica se já colidiu
      if ((millis() - lastDelayRodada) > delayRodada && !exibirGameOver) {
        // Movimenta os obstáculos
        moverObstaculos();
          
        // Exibir obstáculos e personagem
        exibirObjetos();

        // Reseta o tempo
        lastDelayRodada = millis();

        Serial.println("-------------------------------------");
        Serial.println("");
      }
    }
    // Fim do WHILE
  }
  // Fim do IF
}
// Fim do LOOP


/*  ---------------------------------------------------------------------- */
/*  ---------------------             FUNÇÕES        --------------------- */
/*  ---------------------------------------------------------------------- */

// Função que exibe a tela de início
void exibirTelaInicial() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DODGE RUN");
  lcd.setCursor(11, 0);
  lcd.write((uint8_t)0);
  lcd.setCursor(0, 1);
  lcd.print("Aperte o botao");
  delay(250);
}

// Função que exibe a tela de "GAME OVER"
void exibirTelaGameOver() {
  // Emite som no buzzer
  tone(buzzerPin, 4000, 100);
  delay(200);
  tone(buzzerPin, 4000, 100);
  delay(200);
  tone(buzzerPin, 4000, 100);
  delay(200);
  noTone(buzzerPin);
  
  delay(500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GAME OVER");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.setCursor(7, 1);
  lcd.print(score);

  // Exibe um troféu
  if (score >= 100) {
    lcd.setCursor(11, 0);
    lcd.write((uint8_t)2);
  }
  if (score >= 200) {
    lcd.setCursor(12, 0);
    lcd.write((uint8_t)3);
  }
  delay(250);
}

// Função que reseta as variáveis
void reiniciarVariaveis() {
  // Variável do Score
  score = 0;

  // Posição do Jogador
  px = 0;
  py = random(0, 2);

  // Obstáculos
  obstaculos = 0;
  for (int indexObst = 0; indexObst < maxObst; indexObst++) {
    obstaculo_x[indexObst] = 15;
    obstaculo_y[indexObst] = random(0, 2);
  }
  espera = 0;
  
  // Tempo da Rodada
  lastDelayRodada = millis();
}

// Função que exibe os objetos na tela
void exibirObjetos() {
  lcd.clear();

  // Exibe o personagem
  lcd.setCursor(px, py);
  lcd.write((uint8_t)0);
  
  // Exibe obstáculos
  for (int exibeObst=0; exibeObst < obstaculos; exibeObst++) {
    Serial.print("Obstáculo: ");
    Serial.println(exibeObst+1);
    Serial.print("Coluna: ");
    Serial.println(obstaculo_x[exibeObst]+1);
    Serial.print("Linha: ");
    Serial.println(obstaculo_y[exibeObst]+1);
    Serial.println("");
    
    lcd.setCursor(obstaculo_x[exibeObst], obstaculo_y[exibeObst]);
    lcd.write((uint8_t)1);
  }
}

// Função da movimentação do personagem
void moverPersonagem() {
  Serial.println("Personagem moveu!");
  Serial.println("");
  if (py == 0) {
    py = 1;
  } else {
    py = 0;
  }

  // Verificar se o jogador colidiu
  if(!verificarColisao()) {
    // Tocar som
    tone(buzzerPin, 1000, 100);
  }
}

// Função para verificar uma colisão
bool verificarColisao() {
  for (int colisao = 0; colisao < obstaculos; colisao++) {
    if (px == obstaculo_x[colisao] && py == obstaculo_y[colisao]) {
      exibirGameOver = true;
      iniciarJogo = false;
      return true;
    }  
  }
  return false;
}

void moverObstaculos() {
  // Define a quantidade de obstáculos
  if (!debugMode) {
    if (score < 52) {
      maxObst = 2;
      delayRodada = 150;
    } else if (score < 107) {
      maxObst = 3;
      delayRodada = 130;
    } else if (score < 182) {
      maxObst = 4;
      delayRodada = 110;
    } else if (score < 250) {
      maxObst = 5;
      delayRodada = 90;
    } else {
      maxObst = 5;
      delayRodada = 70;
    } 
  } else {
    maxObst = 3;
    delayRodada = 500;
  }
  
  // Cria novos obstáculos
  if (obstaculos < maxObst && espera == 0) {
    obstaculos++;
    espera = 2;
  } else if (obstaculos < maxObst && espera > 0) {
    espera--; 
  }

  // Movimenta os obstáculos
  for (int mov = 0; mov < obstaculos; mov++) {
    if (obstaculo_x[mov] <= 0) {
      obstaculo_x[mov] = 15;
      obstaculo_y[mov] = random(0, 2);
    } else {
      obstaculo_x[mov] -= 1;
    }
  }

  // Verifica a colisão e dá pontos
  if (!verificarColisao()) {
    score++;
  }
}

/*
 *  IDEIAS PARA ADICIONAR
 *  - Aprimorar a dificuldade progressiva
 *  
 *  - Menu, loja e modos de jogo (campanha/infinito/insano)
 *  
 *  - Remover redundância de limpar a tela e exibir tudo de novo a cada rodada,
 *  basta apagar a antiga posição e exibir na nova
 *  
 *  - 'Troféus' a cada 100 de pontuação
 *  
 *  - Pontuação limite que muda o cenário do jogo (fim do jogo)
 *  
 *  - Adicionar tempo após Game Over para voltar a tela inicial
 *  
 *  - Melhorar função Random()
 */

 /*
  * CHANGELOG
  *   Arquivos gerados com "PlatformIO"
  */