#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <TM1637Display.h>

#define AGI 0// AGITADOR
#define D0 1//MENOS SIGNIFICATIVO
#define STOP 2//INTERRUPÇÃO
#define D1 3
#define D2 4
#define D3 5//MAIS SIGNIFICATIVO
#define CLK 6//CLK DISPLAY
#define DIO 7//DADO DISPLAY
//DO 8 AO 13 É O LCD
#define UP A1//BOTÕES NAS ENTRADAS ANALOGICAS
#define DOWN A2
#define OK A3
#define BACK A4

int sobe, desce, confirma, volta;//BOTÕES
int cima, baixo, conf, ret;//BOTÕES
int ladoA, ladoB, minuto, tempref, seg, agitador;//VALORES NO LCD/DISPLAY
unsigned int tensaoA0, valor;
int decimal, B[4];//VETOR PARA O BINARIO DE 4 BITS
long int timer=0, tempo, timer1=0;//VARIAVEIS DE TEMPO
int aux1, aux2, aux3, aux4, cont, i;//AUXILIARES E CONTADORES
int estado, menufinal=0;//VARIAVEIS DE ESTADO
boolean temp=0;
const uint8_t SEG_TEMP[] = {
	SEG_A | SEG_B | SEG_F | SEG_G,// o
	SEG_A | SEG_D | SEG_E | SEG_F };// C



TM1637Display display(CLK, DIO);
LiquidCrystal lcd(8, 9,  10, 11, 12, 13);
                //RS, ENB,D4,D5,D6,D7

//FUNÇÕES PARA EXIBIR NO LCD-------------------------------------------------------------------
void telaInicial()
{
  lcd.setCursor(0,0);//coluna 1, linha 1
  lcd.print("    Corrosao    ");
  lcd.setCursor(0,1);//coluna 1, linha 2
  lcd.print("     de PCB     ");
}

void areadasPlacasa ()
{
  lcd.setCursor(0,0);//coluna 1, linha 1
  lcd.print("Area das placas:");
  lcd.setCursor(0,1);//coluna 1, linha 2
  lcd.print("   >");
  if(ladoA<=9)
  {
  lcd.print("0"); 
  }
  lcd.print(ladoA);
  lcd.print(" x ");
  if(ladoB<=9)
  {
  lcd.print("0"); 
  }
  lcd.print(ladoB);
  lcd.print("     ");
}

void areadasPlacasb ()
{
  lcd.setCursor(0,0);//coluna 1, linha 1
  lcd.print("Area das placas:");
  lcd.setCursor(0,1);//coluna 1, linha 2
  lcd.print("    ");
  if(ladoA<=9)
  {
  lcd.print("0"); 
  }
  lcd.print(ladoA);
  lcd.print(" x >");
  if(ladoB<=9)
  {
  lcd.print("0"); 
  }
  lcd.print(ladoB);
  lcd.print("     ");
}

void tempoCorroi ()
{
  lcd.setCursor(0,0);//coluna 1, linha 1
  lcd.print("    Tempo de    ");
  lcd.setCursor(0,1);//coluna 1, linha 2
  lcd.print("Corrosao: ");
  if(minuto<=9)
  {
  lcd.print("0"); 
  }
  lcd.print(minuto);
  lcd.print(":");
  if(seg<=9)
  {
  lcd.print("0"); 
  }
  lcd.print(seg);
}

void temperaturaRef ()
{
  lcd.setCursor(0,0);//coluna 1, linha 1
  lcd.print(" Temperatura de ");
  lcd.setCursor(0,1);//coluna 1, linha 2
  lcd.print("Corrosao: ");
  if(tempref<=9)
  {
  lcd.print("0"); 
  }
  lcd.print(tempref);
  lcd.print(".C ");
}

void colocaPlaca ()
{
  lcd.setCursor(0,0);//coluna 1, linha 1
  lcd.print("    Coloque     ");
  lcd.setCursor(0,1);//coluna 1, linha 2
  lcd.print("    a placa     ");
}

void iniciando ()
{
  lcd.setCursor(0,0);//coluna 1, linha 1
  lcd.print("  Iniciando a   ");
  lcd.setCursor(0,1);//coluna 1, linha 2
  lcd.print("    Corrosao    ");
}

void apareceTempo ()
{
  lcd.setCursor(0,0);//coluna 1, linha 1
  lcd.print("     Tempo:     ");
  lcd.setCursor(0,1);//coluna 1, linha 2
  lcd.print("+1 min ou -1 min");
}

void apareceTemperatura ()
{
  lcd.setCursor(0,0);//coluna 1, linha 1
  lcd.print("  Temperatura:  ");
  lcd.setCursor(0,1);//coluna 1, linha 2
  lcd.print("      ");
    if(tempref<=9)
  {
  lcd.print("0"); 
  }
  lcd.print(tempref);
  lcd.print(".C      ");
}

void tempoExtra ()
{
  lcd.setCursor(0,0);//coluna 1, linha 1
  lcd.print(" Tempo extra de ");
  lcd.setCursor(0,1);//coluna 1, linha 2
  lcd.print("Corrosao: +");
    if(minuto<=9)
  {
  lcd.print("0"); 
  }
  lcd.print(minuto);
  lcd.print(":00");
}

void menu1 ()
{
         lcd.setCursor(0,0);//coluna 1, linha 1
         lcd.print(">Aumentar tempo ");
         lcd.setCursor(0,1);//coluna 1, linha 2
         lcd.print(" Nova placa     ");
}

void menu2 ()
{
         lcd.setCursor(0,0);//coluna 1, linha 1
         lcd.print(">Nova placa     ");
         lcd.setCursor(0,1);//coluna 1, linha 2
         lcd.print(" Encerrar       ");  
}

void menu3 ()
{
         lcd.setCursor(0,0);//coluna 1, linha 1
         lcd.print(">Encerrar       ");
         lcd.setCursor(0,1);//coluna 1, linha 2
         lcd.print("                ");  
}
//-----------------------------------------------------------------------------------------
//FUNÇÕES PARA OS BOTÕES-------------------------------------------------------------------
void lerBotoes()//LE OS BOTÕES
{
  cima = digitalRead(UP);
  baixo = digitalRead(DOWN);
  conf = digitalRead(OK);
  ret = digitalRead(BACK);
  
  if (cima == 1)//BOTAO CIMA
  {
    aux1 = 1;
  }
  if (aux1 == 1 && cima == 0) //se o clock foi do nivel alto para baixo
  {
    sobe = 1;
    aux1 = 0;
  }
    if (baixo == 1)//BOTAO BAIXO
  {
    aux2 = 1;
  }
  if (aux2 == 1 && baixo == 0) //se o clock foi do nivel alto para baixo
  {
    desce = 1;
    aux2 = 0;
  }
  if (conf == 1)//BOTAO OK
  {
    aux3 = 1;
  }
  if (aux3 == 1 && conf == 0) //se o clock foi do nivel alto para baixo
  {
    confirma = 1;
    aux3 = 0;
  }
  if (ret == 1)//BOTAO VOLTA
  {
    aux4 = 1;
  }
  if (aux4 == 1 && ret == 0) //se o clock foi do nivel alto para baixo
  {
    volta = 1;
    aux4 = 0;
  }
}//FIM LER BOTOES

void zerabotoes()//ZERA AS VARIAVEIS REFERENTES AOS BOTÕES
{
     sobe = 0;
     desce = 0;
     confirma = 0;
     volta = 0;
}//FIM ZERA BOTOES

void vaiVolta ()//CONFIRMA E AVANÇA OU RETORNA O ESTADO
{
  if(confirma==1)
  {
      estado++;
      zerabotoes();
  }
  if(volta==1)
  {
      estado--;
      zerabotoes();
  }
}//FIM VAI VOLTA
//-----------------------------------------------------------------------------------------------
void parar()//VARIAVEL PARA INTERRUPÇÃO QUANDO APERTAR STOP--------------------------------------
{
  estado = 0;
  agitador = 0;
  digitalWrite (AGI, agitador); 
}
//----------------------------------------------------------------------------------------------
void decBinario()//CONVERTE UM DECIMAL PARA O VETOR DE BINARIO
{
  decimal = (int)(tempref/4);
  for (i = 0; i < 4; i++) //for para passar de decimal para binario
  {
    B[i] = decimal % 2; //iguala o ponto do vetor B[] ao resto da divisão do numero digitado por 2
    decimal = decimal / 2; //realiza a divisão do decimal por 2 e atribui esse valor a variavel decimal
  }//ESCREVENDO OS VALORES
  digitalWrite (D0, B[0]); 
  digitalWrite (D1, B[1]); 
  digitalWrite (D2, B[2]); 
  digitalWrite (D3, B[3]); 
}
//--------------------------------------------------------------------------------
void exibeTemp()//EXIBE TEMPO OU TEMPERATURA AO APERTAR OK DURANTE A CORROSÃO
{ 
      if (temp == false)
      {
      display.showNumberDecEx(tempo, 0b01000000, true);
      //display.showNumberDec(tempo, true);
      }
    if (temp == true)
      {
      display.setSegments(SEG_TEMP, 2, 2);//EXIBE oC
      display.showNumberDec(valor, false, 2, 0);//EXIBE A TEMPERATURA
      }
  if ((millis() - timer) >= 1000)//para exibir no diplay 7 seg
  {
    tensaoA0 = analogRead(A0);//LE A TENSÃO EM A0
    valor = (int)(tensaoA0*0.06510416666);//CONVERTE ESSA TENSÃO PARA TEMPERATURA
    seg--;
    if (seg < 0)
    {
      seg = 59;
      minuto--;
    }
    tempo = seg + 100*minuto;
    timer = millis(); // Atualiza a referência
  }
  delay(20);
}
//--------------------------------------------------------------------------------
//falta terminar o estado 6 (corrosao)

void setup ()
{
  pinMode (7, OUTPUT); 
  pinMode (6, OUTPUT);
  pinMode (5, OUTPUT);
  pinMode (4, OUTPUT); 
  pinMode (3, OUTPUT); 
  pinMode (2, OUTPUT);
  pinMode (1, OUTPUT);
  pinMode (0, OUTPUT);
  pinMode (UP, INPUT); 
  pinMode (DOWN, INPUT);
  pinMode (OK, INPUT);
  pinMode (BACK, INPUT);
  pinMode (STOP, INPUT_PULLUP);
  lcd.begin(16,2);//INICIALIZA O LCD
  display.setBrightness(0x0f);//AJUSTA O BRILHO DO DISPLAY
  attachInterrupt(digitalPinToInterrupt(STOP), parar, CHANGE);
  estado = 0;
  timer = 0;
  agitador = 0;
}

void loop()
{
  digitalWrite (AGI, agitador); 
  switch (estado)
  {
      case 0://TELA INICIAL
          ladoA = EEPROM.read(0);//LE OS VALORES ARMAZENADOS NA MEMORIA INTERNA
          ladoB = EEPROM.read(1);
          minuto = EEPROM.read(2);
          tempref = EEPROM.read(3);
          seg = 0;
          temp = false;
	        display.showNumberDec(0, true);
          menufinal = 1;
          telaInicial();
          lerBotoes();
          agitador = 0;
          if(sobe==1||desce==1||confirma==1||volta==1)
            {
             estado = 1;
             zerabotoes();
            }
        break;
        
      case 1://AREA DE UM LADO
          areadasPlacasa();
          lerBotoes();
          agitador = 0;
          if(sobe==1)
          {
            ladoA++;
            zerabotoes();
          }
          if(desce==1)
          {
            ladoA--;
            if(ladoA<=0)
            {
              ladoA=0;
            }
            zerabotoes();
          }
           vaiVolta();
        break;
        
      case 2://AREA DO OUTRO LADO
              
          areadasPlacasb();
          lerBotoes();
          agitador = 0;
          if(sobe==1)
          {
            ladoB++;
            zerabotoes();
          }
          if(desce==1)
          {
            ladoB--;
            if(ladoB<=0)
            {
              ladoB=0;
            }
            zerabotoes();
          }
          vaiVolta();
        break;
        
      case 3://TEMPO DE CORROSÃO
          tempoCorroi();
          lerBotoes();
          agitador = 0;
          if(sobe==1)
          {
            minuto++;
            if(minuto>=90)
            {
              minuto = 90;
            }
            zerabotoes();
          }
          if(desce==1)
          {
            minuto--;
            if(minuto<=0)
            {
              minuto = 1;            
            }
            zerabotoes();
          }
          vaiVolta();
        break;
        
      case 4://TEMPERATURA DO PERCLORETO
          
          temperaturaRef ();
          lerBotoes();
          agitador = 0;
          if(sobe==1)
          {
            tempref++;
            if(tempref>=60)
            {
              tempref=60;
            }
            zerabotoes();
          }
          if(desce==1)
          {
            tempref--;
            if(tempref<=0)
            {
              tempref=0;
            }
            zerabotoes();
          }
          vaiVolta();
        break;

      case 5://COLOCAR PLACAS
          colocaPlaca();
          lerBotoes();
          agitador = 0;
          if(sobe==1||desce==1||confirma==1||volta==1)
            {
             estado++;
             zerabotoes();
            }
        break;
        
      case 6://INICIANDO CORROSÃO
          EEPROM.update(0, ladoA);//ATUALIZA OS VALORES DOS LADOS DA PLACA
          EEPROM.update(1, ladoB);  
          EEPROM.update(2, minuto);//ATUALIZA O TEMPO
          EEPROM.update(3, tempref);//ATUALIZA A TEMPERATURA
          iniciando ();
          delay(20);
          cont++;
          if(cont>=75)
          {
            cont = 0;
            estado++;
          }
        break;
 
      case 7://CORROENDO
      delay(25);
        agitador = 1;
        lerBotoes();
        decBinario();
        if(minuto<=0)
            {
              minuto = 0;            
            }
        if(confirma==1)//VARIA SE O DISPLAY EXIBE TEMPO OU TEMPERATURA
        {
          temp = !temp;
          zerabotoes();
        }
        exibeTemp();
        if(temp == false)//ADICIONAR OU DECREMENTAR O TEMPO
        {
          apareceTempo();
          if(sobe==1)
          {
            minuto++;
            if(minuto>=90)
            {
              minuto = 90;
            }
            zerabotoes();
          }
          if(desce==1)
          {
            minuto--;
            zerabotoes();
          }
        }
        if(temp == true)//ADICIONAR OU DECREMENTAR A TEMPERATURA
        {
          apareceTemperatura();
          if(sobe==1)
          {
            tempref++;
            if(tempref>=60)
            {
              tempref=60;
            }
            zerabotoes();
          }
          if(desce==1)
          {
            tempref--;
            if(tempref<=0)
            {
              tempref=0;
            }
            zerabotoes();
          }
        }

        if(minuto<=0&&seg<=0)
        {
          exibeTemp();
          estado = 8;
          seg = 0;
          minuto = EEPROM.read(2);
          EEPROM.update(3, tempref);//ATUALIZA A TEMPERATURA
          zerabotoes();
        }
        break;
         
      case 8://TERMINOU O TEMPO
          agitador = 0;
          switch (menufinal)
          {
            case 1:
                menu1();
                lerBotoes();
                if(desce==1)
                {
                  menufinal++;
                  zerabotoes();
                }
                if(confirma==1)
                {
                  estado = 9;
                  zerabotoes();
                }
                zerabotoes();
              break;
              
            case 2:
                menu2();
                lerBotoes();
                if(sobe==1)
                {
                  menufinal--;
                  zerabotoes();
                }
                if(desce==1)
                {
                  menufinal++;
                  zerabotoes();
                }
                if (confirma==1)
                {
                  estado = 1;
                  zerabotoes();
                }
              break;
              
            case 3:
                menu3();
                lerBotoes();
                if(sobe==1)
                {
                  menufinal--;
                  zerabotoes();
                }
                if (confirma==1)
                {
                  estado = 0;
                  zerabotoes();
                }
              break;
          }
        break;
        
       case 9://TEMPO EXTRA
          tempoExtra ();
          lerBotoes();
          agitador = 0;
          if(sobe==1)
          {
            minuto++;
            if(minuto>=90)
            {
              minuto = 90;
            }
            zerabotoes();
          }
          if(desce==1)
          {
            minuto--;
            if(minuto<=0)
            {
              minuto = 1;            
            }
             zerabotoes();
          }
          if (confirma==1)
          {
              estado = 6;
              zerabotoes();
          }
          if (volta==1)
          {
              estado--;
              zerabotoes();
          }     
        break;
  }

}
