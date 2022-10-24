#include <iostream>
#include <string>
using namespace std;

// 1 -  Faça uma função que recebe uma certa medida e ajusta ela percentualmente 
// entre dois valores mínimo e máximo e retorna esse valor
float converteSensor(float min, float max, float numero){
	float resp = (100*(numero-min)/(max-min)); //calculo de ajustamento do valor;		
	return resp;
}
// }/*  */
// 2 - Faça uma função que simule a leitura de um sensor lendo o 
// valor do teclado ao final a função retorna este valor
int captaSensor(string direcao){
	int valor_teclado;		//cria variavel;
	// printf("digite um valor para o sensor %s :", direcao);
	cout << "digite um valor para o sensor " << direcao << " :";
	cin >> valor_teclado;		//input armazenado
	return valor_teclado;
}
// 3 - Faça uma função que armazena uma medida inteira qualquer 
// em um vetor fornecido. Note que como C não possui vetores 
// nativos da linguagem, lembre-se que você precisa passar o 
// valor máximo do vetor assim como a última posição preenchida
// Evite também que, por acidente, um valor seja escrito em 
// uma área de memória fora do vetor
int InsereNaLista(int valorNovo, int posicao, int *array, int maxPosicao){
	if (posicao < maxPosicao){	//verificação da possibilidade de fazer a alteraçao na lista;
		array[posicao] = valorNovo; 	//coloca o valor na posição;
		return posicao+1;		//retorna a posicao seguinte;

	}
	return posicao; //posicao nao alterada
}



// 4 - Faça uma função que recebe um vetor com 4 posições que contém 
// o valor da distância de um pequeno robô até cada um dos seus 4 lados.
// A função deve retornar duas informações: A primeira é a direção 
// de maior distância ("Direita", "Esquerda", "Frente", "Tras") e a 
// segunda é esta maior distância.
char *direcaoMenorCaminho(int *direcao, int *distanciaMax){
	char *sentidos[] = {"Direita","Esquerda", "Frente", "Tras"};

	int index = 0;		//indice dos sentidos
	int maior = direcao[0]; //maior valor do vetor com as distancias

	for (int i = 0; i<4; i++){
		if (direcao[i]>maior){		//identifica se o valor é o maior
			maior = direcao[i];		//se o valor é maior, o valor passa ser o maior numero
			index = i;		//index passa a ser o indice do maior valor
		}
	}
	*distanciaMax = maior;		//ponteiro apontando para o maior valor do vetor
	return sentidos[index];		//retorna o sentido com o maior valor
}



// 5 - Faça uma função que pergunta ao usuário se ele deseja continuar o mapeamento e 
// retorna verdadeiro ou falso
bool leComando(){ 
	char retorno;		
	cout << "quer continuar o mapeamento?: (S/N)"<<endl;
	cin>>retorno;		//input para saber se continua o mapeamento ou nao;
	if(toupper(retorno) == 'S'){
		return true;		
	}
	return false;
}	

// 6 - A função abaixo (que está incompleta) vai "dirigindo" virtualmente um robô 
// e através de 4 sensores em cada um dos 4 pontos do robo ("Direita", "Esquerda", 
// "Frente", "Tras"). 
//      A cada passo, ele verifica as distâncias aos objetos e vai mapeando o terreno 
// para uma movimentação futura. 
//      Ele vai armazenando estas distancias em um vetor fornecido como parâmetro 
// e retorna a ultima posicao preenchida do vetor.
//      Esta função deve ir lendo os 4 sensores até que um comando de pare seja enviado 
//
//      Para simular os sensores e os comandos de pare, use as funções já construídas 
// nos ítens anteriores e em um looping contínuo até que um pedido de parada seja 
// enviado pelo usuário. 
//
//      Complete a função com a chamada das funções já criadas
int dirige(int *vetor,int tamanhoVetor) { // função para saber qual direção ele irá seguir;
	int maxVetor = tamanhoVetor; 
	int *vetorMov = vetor;
	int posAtualVetor = 0; //valor do indice da posicao no vetor 
	bool dirigindo = 1;		//variavel para manter o processo

	string sentidos[]= {"Direita", "Esquerda", "Frente", "Tras"}; //valores para retornar os sentidos

	while(dirigindo){	
		for(int i=0; i<4; i++){ // repetição para que ele passe por cada sentido e pegue os valores;
			int medida = captaSensor(sentidos[i]);  //medida será o valor lido de acordo com os sentidos e suas respectivas posicoes na lista
		    medida = converteSensor(0,830, medida); //converte o valor lido e atribui à medida
		    posAtualVetor = InsereNaLista(medida, posAtualVetor, vetorMov, maxVetor); //muda posicao atual e altera vetorMov
		}


		dirigindo = leComando();//atribui valores true ou false para saber se ele repete o processo de pegar os valores ou não
	}
	return posAtualVetor;
}


// O trecho abaixo irá utilizar as funções acima para ler os sensores e o movimento
// do robô e no final percorrer o vetor e mostrar o movimento a cada direção baseado 
// na maior distância a cada movimento
void percorre(int *v,int tamPercorrido){		 //função que percorre a lista e ve quantas verificações ele fez
	int *vetorMov = v;
	int maiorDir = 0;
	
	for(int i = 0; i< tamPercorrido; i+=4){ // depois de passar pelas 4 posicoes passadas ele retorna o sentido a partir do maior valor
		char *direcao = direcaoMenorCaminho(&(vetorMov[i]),&maiorDir); //ponteiro que aponta para o valor que será o sentido
		cout << "Movimentando para " << direcao << " distancia " << maiorDir << endl;
	}
}

int main(int argc, char** argv) {
	int maxVetor = 100; 	//tamanho máximo definido para o vetor
	int vetorMov[maxVetor*4] = {0}; //vetor com disponibilidade de posicoes
	int posAtualVet = 0; //indice da posicao atual do vetor


	posAtualVet = dirige(vetorMov,maxVetor); //retorna posicao atual do vetor;
	percorre(vetorMov,posAtualVet); //devolve o sentido que o robo segue com o valor convertido;
	
	return 0;
}