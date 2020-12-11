#include <SPI.h> //responsável pela comunicação dos módulos do Shield Ethernet W5100 com o Arduino UNO utilizando o protocolo SPI
#include <Ethernet.h> //possibilita a conexão do conjunto com a rede local
#include <Ultrasonic.h> //carrega a bibioteca do sensor ultrassônico
#define ledgreen1 6 //declarando o pino do ledgreen1
#define ledred1 5 //declarando o pino do ledred1
#define pino_trigger1 8 //pino 8 envia o pulso
#define pino_echo1 7 //pino 7 recebe o pulso
long duracao1 = 0; //declara a variável duracao1
long distancia1 = 0; //declara a variável 

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; //endereço MAC utilizado
IPAddress ip(192, 168, 1, 108); //endereço de IP utilizado 

EthernetServer server(80); //Servidor Web utilizado para enviar as páginas web criadas  

void setup() 
{
    Ethernet.begin(mac, ip); //inicia a conexão com a rede local obtendo como parâmetro os endereços MAC e IP definidos anteriormente
    server.begin(); //inicia o Servidor Web
    pinMode(ledgreen1, OUTPUT); //define o pino 6 como saída
    pinMode(ledred1, OUTPUT); //define o pino 5 como saída
    pinMode(pino_trigger1, OUTPUT); //define o pino 8 como saída 
    pinMode(pino_echo1, INPUT); //define o pino 7 como entrada
}

void loop()
{
    EthernetClient client = server.available(); //verifica se há algum cliente tentando fazer conexão
    if (client) //se tiver um cliente tentando fazer conexão
    {
       boolean current_line_is_blank = true; //variável de controle booleana utilizada na leitura da requisição feita pelo navegador.
       while (client.connected()) //faz com que a execução do programa fique restrita enquanto o parâmetro passado para mesma for verdadeiro
       {
          if (client.available()) //verifica se existe alguma informação proveniente do navegador para ser lida
          {
             char c = client.read(); //lê cada caractere, de modo que, a cada passagem por esta sentença um novo caractere é armazenado na variável do tipo char c.
             if (c == '\n' && current_line_is_blank) //confirmação de recebimento do pacote completo de informações 
             {
             digitalWrite(pino_trigger1, LOW); //pino_trigger1 pulso desligado
             delayMicroseconds(10); //intervalo de 10 microsegundos
             digitalWrite(pino_trigger1, HIGH); //pino_trigger1 pulso ligado
             delayMicroseconds(10); //intervalo de 10 microsegundos
             digitalWrite(pino_trigger1, LOW); //pino_trigger1 pulso desligado
             duracao1 = pulseIn(pino_echo1, HIGH); //duracao1 recebe um pulso e liga o pino_echo1
             distancia1 = duracao1/58; //cálculo da distancia1
             client.println("<html>"); //indica para o navegador que estamos começando um bloco de programação utilizando a linguagem HTML 
             client.println("<body>"); //corpo da página
             client.println("<h1>Vaga do estacionamento</h1>"); 
             client.println("</body>"); //fim do corpo da página
             client.println("</html>"); //fim do bloco
             delayMicroseconds(10); //intervalo de 10 microsegundos
             if (distancia1 <= 10) //se a distância entre o objeto e o sensor for menor ou igual a 10cm:
             {
                  digitalWrite(ledgreen1, LOW); //desliga o ledgreen1
                  digitalWrite(ledred1, HIGH); //liga o ledred1
                  client.println("Vaga 1 ocupada"); //imprime a frase "Vaga 1 ocupada"   
             }
             if (distancia1 > 10) //se a distância entre o objeto e o sensor for maior que 10cm
             {
                  digitalWrite(ledgreen1 , HIGH); //liga o ledgreen1
                  digitalWrite(ledred1, LOW); //desliga o ledgreen1
                  client.println("Vaga 1 desocupada"); //imprime a frase "Vaga 1 desocupada"
             }
             break; //encerra o loop do while
             }
             if (c == '\n') //atua no tratamento da variável c enquanto a requisição não foi finalizada.
             {
             current_line_is_blank = true;
             } 
             else if (c != '\r') //atua no tratamento da variável c enquanto a requisição não foi finalizada.
             {
             current_line_is_blank = false;
             }
         }
      } 
      delayMicroseconds(10); //intervalo de 10 microsegundos
      client.stop(); //interrompe a conexão            
   } 
                 
}
