#include <main.h>
#define DEFAULT_THRESHOLD 50


#INT_TIMER1
int stop_timer = 0;
int timer_overflow = 0;
void  TIMER1_isr(void) 
{
   if (stop_timer){
      stop_timer = 0;
      
   }else{
      set_timer1(0);
      timer_overflow ++;
   }

}



//communication avec port COM
int flag_threshold = 0 //flag on si nouveau seuil
char buffer[4] // 4 car cara d�but trame + seuil (max = 999) ==> ex: @954 ou @001 ou @051
int buf_compteur = 0;

#INT_RDA
void  RDA_isr(void)
{
   buffer[buf_compteur] = getc();   //lis le caract�re � la position courante du flux de donn�es
   if (buffer[0] == "@" && flag_threshold == 0){   //si trame commence par @ et qu'on ne traite pas d�j� un nouveau seuil
      buf_compteur ++;
      if (buf_compteur >= 4){    //Traitement quand buffer plein
         buf_compteur = 0;
         flag_threshold = 1;
      }
   }
}

void main()
{
   //init variable
   int threshold;
   int duration;
   int distance;


   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);      //13,1 ms overflow


   enable_interrupts(INT_TIMER1);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   setup_low_volt_detect(FALSE);

   while(TRUE)
   {
      //Initialization + �tre sure que trigger est off
      output_low(PIN_C0);
      delay_ms(10);
      
      //Prise d'une mesure
      output_high(PIN_C0);
      delay_us(10);
      output_low(PIN_C0);
      
      //Attend un output
      while(!input(PIC_C0)){}
      set_timer1(0);
      
      //Attend que l'output se termine
      while(input(PIC_C0)){}
      stop_timer = 1;
      duration = (int)(timer_overflow * 13.1)   //!!!!!!!!! MILISECONDE !!!!!!!!!!
      timer_overflow = 0;
      distance = (int)(duration * 34)/2  // !!!!!! distance en centim�tre !!!!!!!!!
      
      //Affichage
      
      
      
      //Alerte
      if(threshold < distance){
         output_high(PIN_D1); //alume led verte
         output_low(PIN_D0);
      }
      if(threshold > distance){
         output_low(PIN_D1); //Coupe led verte
         output_toggle(PIN_D0); //Clignote led red
      }

   
      // Nouveau seuil
      if(flag_threshold){  //Calcul nouveau seuil (char - '0' => convert char to int)
         threshold = ((buffer[1] - '0')*100) + ((buffer[2] - '0')*10) + (buffer[3] - '0')
         flag_threshold = 0; 
      } 
      
      if(threshold == NULL){  //valeur par defaut 
         threshold = DEFAULT_THRESHOLD;
      }
      
   }
}



