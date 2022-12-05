#include "ESPAsyncWebServer.h"
#include "WiFi.h"
//definição da rede e senha
const char *ssid = "Mufasa";
const char *password = "1234567890";

// pinos
#define buz 7
#define ledG 8
#define ledR 9
#define ledY 10
#define ledB 13

// vetores auxiliares
const int LEDS[4] = {ledG, ledR, ledY, ledB}; // vetor com os pinos dos leds
const int tones[4] = {270, 294, 310, 350};    // vetor com os tons

// variáveis auxiliares
int luzes[16]; // vetor com as luzes que serão acesas
int posicao = 0;
int quant = 0; // posição do vetor luzes que uma cor será inserida ou comparada
bool acertou = true; // impedimento para caso ele erre ou acerte a cor
bool terminou = true;
bool comecou = false;

// objeto servidor
AsyncWebServer webServer(80);

// declaração das funções
String head();
String body();
String html1();
String html2(int target);
void desligaTudo();
void ligaTudo();
void pisca(int led);
void Genius(int target);
//função que reseta vetores e variáveis;
void reset() {
  delay(10);
  for (int i = 0; i < 16; i++) {
    luzes[i] = 0;
  }
  delay(60);
  ligaTudo();
  quant = 0;
  posicao = 0;
  acertou = true;
  terminou = true;
  comecou = false;
}

// setup de ambiente
void setup() {
  Serial.begin(115200);

  // pinModes da vida
  pinMode(buz, OUTPUT);
  for (int i = 0; i < 4; i++) {
    pinMode(LEDS[i], OUTPUT);
    digitalWrite(LEDS[i], HIGH);
  }

  // conectando-se ao WiFi
  Serial.println("Conectando ao WiFi...");
  while (WiFi.status() !=
         WL_CONNECTED) {        // enquanto não estiver conectado a uma rede
    WiFi.begin(ssid, password); // ele fica tentando se conectar
    delay(741);                 // um delay sincero pra poder esperar a conexão
    Serial.print(".");          // resposta no serial
    // só uma medida de espera no próprio bloco com os leds piscando
    desligaTudo();
    ligaTudo();
  }

  Serial.println(WiFi.localIP()); // printa no serial o ip do servidor web

  // quando estiver na porta padrão, envia o html padrão com apenas os botões
  webServer.on("/", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "text/html; charset=UTF-8", html1());
  });

  // funções dos botões, a cor que os representa é
  // passada no argumento da função que retorna o html da página novamente
  webServer.on("/G", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "text/html; charset=UTF-8", html2(1)); // verde
  });

  webServer.on("/R", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "text/html; charset=UTF-8", html2(2)); // vermelho
  });

  webServer.on("/Y", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "text/html; charset=UTF-8", html2(3)); // amarelo
  });

  webServer.on("/B", HTTP_GET, [](AsyncWebServerRequest *req) {
    req->send(200, "text/html; charset=UTF-8", html2(4)); // azul
  });

  // inicia o servidor
  webServer.begin();
}

// função que retorna o head com os styles
String head() {
  String head = "";

  head.concat("<html>");
  head.concat("<head><meta content =\"width=device-width, initial-scale=1\">");
  head.concat("<style>");
  head.concat("html {margin: 0px auto; text-align: center;}");

  head.concat(".botao {");
  head.concat("color: white;");
  head.concat("padding: 15px 40px;");
  head.concat("border-radius: 25px; }");

  head.concat(".verde { background-color: #00FF00; }");
  head.concat(".vermelho { background-color: #FF0000; }");
  head.concat(".amarelo { background-color: #FFFF00; }");
  head.concat(".azul { background-color: #4169E1; }");
  head.concat("</style></head>");
  return head;
}

// função que retorna o body com os botões
String body() {
  String html = "";
  html.concat(head());

  html.concat("<body><h1> GENIUS COM ESP32</h1>");

  html.concat("<span> Pontuação: ");
  html.concat(quant - 1);
  html.concat("</span>");

  html.concat("<br/>");

  html.concat("<p><a href=\"/G\">");
  html.concat("<button class=\"botao verde\">VERDE!</button>");
  html.concat("</a></p>");

  html.concat("<p><a href=\"/R\">");
  html.concat("<button class=\"botao vermelho\">VERMELHO!</button>");
  html.concat("</a></p>");

  html.concat("<p><a href=\"/Y\">");
  html.concat("<button class=\"botao amarelo\">AMARELO!</button>");
  html.concat("</a></p>");

  html.concat("<p><a href=\"/B\">");
  html.concat("<button class=\"botao azul\">AZUL!</button>");
  html.concat("</a></p>");

  html.concat("</body></html>");
  return html;
}
//retorna a página com os botões
String html1() {
  String html = "";
  html.concat(body());
  return html;
}
//retorna a página com as portas selecionadas após apertas os botões 
String html2(int target) {
  String html = "";
  html.concat(body());
  //função de andamento do jogo
  Genius(target);

  return html;
}
//função que apaga os leds
void desligaTudo() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDS[i], LOW);
    delay(65);
  }
}
//função que liga os leds
void ligaTudo() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDS[i], HIGH);
    tone(buz, tones[i], 65);
    delay(65);
  }
}
//função que faz os leds piscarem
void pisca(int led) {
  digitalWrite(LEDS[led], HIGH);
  tone(buz, tones[led], 500);
  delay(500);
  digitalWrite(LEDS[led], LOW);
  delay(200);
}

void Genius(int target) {
  if (comecou == false) { // no começo do jogo ele entra nessa condição para desligar os leds
    comecou = true;
    desligaTudo();
  }

  if (terminou == false) { // depois que os leds ascendem aleatoriamente, ele entra nessa condição para o usuário apertar os botões
    if (target == luzes[posicao]) { //se target com a porta de requisição for igual a luz ascendida na lista de luzes 
      posicao += 1; //passa para a próxima fase
      pisca((target - 1)); // repete o valor ascendido após clicar no botão
      if (posicao == quant) { // se o usuário termina de apertar os botões na ordem correta
        terminou = true;
      }
    } else { // se o usuário errar reseta o jogo
      ligaTudo();
      desligaTudo();
      reset();
    }
  }
}

void loop() {
  // ambientação de fase
  if ((comecou == true) && (terminou == true)) {
    delay(750);
    Serial.println("Bora Bill!");

    luzes[quant] = random(1, 5); // adiciona na lista de luzes valores aleatórios entre 1 e 4 para cada led para mudar os valores que inicialmente eram 0
    quant += 1;              // passa para a próxima posição da lista    

    // acender as luzes na ordem
    for (int i = 0; i < quant;
         i++) {            // Loop para acender a sequência de luzes uma a uma
      if (luzes[i] == 1) { // Verifica se é esta a luz que deve ascender
        pisca(0);
      } // A mesma lógica é implementada para as demais luzes
      if (luzes[i] == 2) {
        pisca(1);
      }
      if (luzes[i] == 3) {
        pisca(2);
      }
      if (luzes[i] == 4) {
        pisca(3);
      }
    }

    terminou = false; // variável terminou fica falsa e possibilita o início do aperto de botões
    posicao = 0; // a posição na lista depois da sequência aleatória aparecer para começar a sequência de apertos dos botões do usuário
  }
}