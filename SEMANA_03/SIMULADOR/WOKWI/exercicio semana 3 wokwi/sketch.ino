//definições das variáveis das portas e de ajuste dos valores para binários;
int portaLDR = 7;
int portaLED1 = 2;
int portaLED2 = 3;
int portaLED3 = 4;
int portaLED4 = 5;
int portaA0 = 12;
int valorLDR;
int valorLED;
int ajuste;
int bin;
int buz = 21;
int button1 = 9;
int button2 = 8;
void setup() {
    // setup de pinModes e inicialização do Serial;
  Serial.begin(115200);
  Serial.println("Hello, ESP32-S3!");
  pinMode(portaLDR, INPUT);
  pinMode(portaLED1, OUTPUT);
  pinMode(portaLED2, OUTPUT);
  pinMode(portaLED3, OUTPUT);
  pinMode(portaLED4, OUTPUT);
  pinMode(buz, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
}

int converte(){ //função que ajusta os valores lidos pelo LDR e retorna o valor ajustado;
  ajuste = (15*(valorLDR-64)/(8126));

  return ajuste;
}

int binario(int decimal){
  int bin = 0;    //função que transforma em binário os valores decimais ajustados na função converte() e retorna esses binários;
  int remainder;
  int product = 1;

  while(decimal !=0){
    remainder = decimal % 2;
    bin = bin +(remainder*product);
    decimal = decimal/2;
    product *=10;
  }
  return bin;
}

void ledX(int binary){  //funçao que ascende os leds de acordo com os valores binários;
  const int divSplit1 = binary/1000 % 10;  //essas constantes recebem "splits" dos valores binários;
  const int divSplit2 = binary/100 % 10;
  const int divSplit3 = binary/10 % 10;
  const int divSplit4 = binary % 10;
  if(divSplit1 == 1){ //fiz várias condições com essas constantes caso os valores assumam 0 ou 1 determinado led ascende ou não;
    digitalWrite(portaLED1, HIGH);
  }
  else if(divSplit1 == 0){
    digitalWrite(portaLED1, LOW);
  }
  if(divSplit2 == 1){
    digitalWrite(portaLED2, HIGH);
  }
  else if(divSplit2 == 0){
    digitalWrite(portaLED2, LOW);    
  }
  if(divSplit3 == 1){
    digitalWrite(portaLED3, HIGH);
  }
  else if(divSplit3 == 0){
    digitalWrite(portaLED3, LOW);
  }
  if(divSplit4 == 1){
    digitalWrite(portaLED4, HIGH);
  }
  else if(divSplit4 == 0){
    digitalWrite(portaLED4, LOW);
  }
}

void buzX(int binariodnv){  //função que reproduz diferentes frequências de sons no buzzer de acordo com o valor binário;
  if(binariodnv == 0){  //eu fiz 16 condições de 0000 à 1111 mesmo, pois fiquei com preguiça de pensar em um for ou outra maneira para reprodzuir os sons;
    tone(buz,262,200);
  }
  else if(binariodnv == 1){
    tone(buz,294,300);
  }
  else if(binariodnv == 10){
    tone(buz,330,300);
  }
  else if(binariodnv == 11){
    tone(buz,349,300);
  }
  else if(binariodnv == 100){
    tone(buz,392,300);
  }
  else if(binariodnv == 101){
    tone(buz, 440, 300);
  }
  else if(binariodnv == 110){
    tone(buz, 494, 300);
  }
  else if(binariodnv == 111){
    tone(buz, 523, 300);
  }
  else if(binariodnv == 1000){
    tone(buz, 587, 300);
  }
  else if(binariodnv == 1001){
    tone(buz, 659, 300);
  }
  else if(binariodnv == 1010){
    tone(buz, 698, 300);
  }
  else if(binariodnv == 1011){
    tone(buz, 784, 300);
  }
  else if(binariodnv == 1100){
    tone(buz, 880, 300);
  }
  else if(binariodnv == 1101){
    tone(buz, 988, 300);
  }
  else if(binariodnv == 1110){
    tone(buz, 1047, 300);
  }
  else if(binariodnv == 1111){
    tone(buz, 1175, 300);
  }
}
void BinNaLista(int binario, int posicao, int *lista1){  // funçao que adiciona os numeros binários retornados em uma lista e printa no Serial;
  lista1[posicao] = binario;
  Serial.print("Valor na Posição: ");
  Serial.println(lista1[posicao]);
  Serial.print("Posição: ");
  Serial.println(posicao);

}
int posicao = 0;  //posição da lista;
int lista1[100];  //tamanho da lista;

void Genius(int *lista2){  //função que reproduz as luzes do Led e os sons do buzzer de acordo com os binários na lista;
  int valorBotao2 = digitalRead(button2);
  if(valorBotao2 == 0){  //se o botão for apertado;
    for(int i; i < posicao; i ++){  //passa pelos valores da lista que contém os binários;
      
    ledX(lista2[i]);
    delay(500);

    buzX(lista2[i]);
    delay(500);

    }
    posicao = 0;
  }
}

void loop() {
  int valorBotao1 = digitalRead(button1);
  if(valorBotao1 == 0){  //Se o primeiro botão for apertado;
    valorLDR = analogRead(portaA0);

    converte();  //converte os valores lidos do LDR;

    ledX(binario(ajuste));  //o parâmetro é a função que já retorna o binário para a função de ascender os leds;
    delay(500);

    buzX(binario(ajuste));  // o parâmetro é a função que já retorna o binário para a função de tocar o buzzer;
    delay(500);

    BinNaLista(binario(ajuste), posicao, lista1);  //função adicionando os valores na lista;  
    posicao +=1;

  }

  digitalWrite(portaLED1, LOW);  //desligando os leds enquanto o botão não está apertado
  digitalWrite(portaLED2, LOW);
  digitalWrite(portaLED3, LOW);
  digitalWrite(portaLED4, LOW);
  Genius(lista1);   //chamando a função para reproduzir os leds e o buzzer baseado nos valores binários;
}
