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
int flag_threshold = 0; //flag on si nouveau seuil
char buffer[4]; // 4 car cara dï¿½but trame + seuil (max = 999) ==> ex: @954 ou @001 ou @051
int buf_compteur = 0;

#INT_RDA
void  RDA_isr(void)
{
   buffer[buf_compteur] = getc();   //lis le caractï¿½re ï¿½ la position courante du flux de donnï¿½es
   if (buffer[0] == "@" && flag_threshold == 0){   //si trame commence par @ et qu'on ne traite pas dï¿½jï¿½ un nouveau seuil
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
   int poids_fort;
   int poids_faible;

   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);      //13,1 ms overflow


   enable_interrupts(INT_TIMER1);
   enable_interrupts(INT_RDA);
   enable_interrupts(GLOBAL);
   setup_low_volt_detect(FALSE);

   while(TRUE)
   {
      //Initialization + être sure que trigger est off
      output_low(PIN_C0);
      delay_ms(10);
      
      //Prise d'une mesure
      output_high(PIN_C0);
      delay_us(10);
      output_low(PIN_C0);
      
      //Attend un output
      while(!input(PIN_C1)){}
      set_timer1(0);
      
      //Attend que l'output se termine
      while(input(PIN_C1)){}
      stop_timer = 1;
      duration = (int)(timer_overflow * 13.1);   //!!!!!!!!! MILISECONDE !!!!!!!!!!
      timer_overflow = 0;
      distance = (int)(duration * 34)/2;  // !!!!!! distance en centimetre !!!!!!!!!
      
      //Affichage
      if (distance >= 1000 || distance < 0){ //Overflow ou distance négative = erreur
      
         output_high(PIN_D7); //affiche juste un point 
      }
      else if (distance >= 100){ // affichage passe en metre
         
         poids_fort = (int)distance / 100;
         poids_faible = (int)(distance % 100) / 10;
         
         output_high(PIN_D7);
         output_b((poids_faible >> 4) + poids_fort); //Decalage de 4 vers la doite à cause du montage physique et logique
      }
      else { //affichage en centimetre
      
         poids_fort = (int)distance / 10;
         poids_faible = (int)distance % 10;
         
         output_low(PIN_D7);
         output_b((poids_faible >> 4) + poids_fort);
      }
      
      
      
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
         threshold = ((buffer[1] - '0')*100) + ((buffer[2] - '0')*10) + (buffer[3] - '0');
         flag_threshold = 0; 
      } 
      
      if(threshold == NULL){  //valeur par defaut 
         threshold = DEFAULT_THRESHOLD;
      }
      
   }
}



