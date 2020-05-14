#include "kjr3wire.h"

 /***
 *      _  ______      _    _  _______ _   _       _ ____      ___ ___ _____ 
 *     | |/ /  _ \    / \  | |/ / ____| \ | |     | |  _ \    |_ _/ _ \_   _|
 *     | ' /| |_) |  / _ \ | ' /|  _| |  \| |  _  | | |_) |    | | | | || |  
 *     | . \|  _ <  / ___ \| . \| |___| |\  | | |_| |  _ < _   | | |_| || |  
 *     |_|\_\_| \_\/_/   \_\_|\_\_____|_| \_|  \___/|_| \_(_) |___\___/ |_|  
 *     
 *                          KJR 3 WIRE PROTOCOL 
 *                       https://krakenjriot.tech/
 *                  https://internetofthingsbuilder.com/
 *                          funsideofwine@gmail.com   
 *                                        
 * MIT License
 *
 * Copyright (c) 2020 Rolly Falco Villacacan
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#define SIGNAL_LINE 3 
#define MASTER_LINE 4
#define SLAVE_LINE 5
#define LINE_ID '1'

bool SIGNAL_LINE_STAT = false;
bool startRecording = false; 
String str;
char chx;

unsigned long lapse; 
bool itsforme = false;

String cached_str;

uint8_t c = 0, d = 0;
unsigned int i;

char concatenated_chars[127];  
const char * data_to_send_to_master;

void setup() 
{
  Serial.begin(9600);      
  pinMode(SIGNAL_LINE, INPUT);
  pinMode(MASTER_LINE, INPUT);
  pinMode(SLAVE_LINE, OUTPUT); 
  pinMode(9, OUTPUT);   
}

void loop() 
{
//********************************************************************  
//********************************************************************
SIGNAL_LINE_STAT = digitalRead(SIGNAL_LINE);
//******************************************************************** 
if(!SIGNAL_LINE_STAT) //RESPOND TO MASTER 
 { 
    if(itsforme)
    {    
	      //data_to_send_to_master = "1111"; //"1-", "2-" ... "X-" SLAVE LINE ID X
        data_to_send_to_master = "123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,";    
        sprintf(concatenated_chars,"0%c-%s,",LINE_ID, data_to_send_to_master);
        //Serial.println(concatenated_chars); 
        encode_lapse(concatenated_chars, SLAVE_LINE);
        itsforme = false;
        cached_str = "";
    }
 }
//******************************************************************** 
if(SIGNAL_LINE_STAT)
{           
          lapse = pulseIn(MASTER_LINE, HIGH);    
          chx = decode_lapse(lapse);
          //Serial.println(chx);  
          if(chx == '{')
          {
            startRecording = true;
          } 
          if(startRecording) 
          {
         
              str += chx;  
                      
          }          
          if(chx == '}')
          {
            c = 0;
            for(i=0;str[i];i++)  
            {
              if(str[i]=='{'){ c++; }
            }
            d = 0;
            for(i=0;str[i];i++)  
            {
              if(str[i]=='E'){ d++; }
            }
            
            if(c == 1 && d == 0){
              if(str[1] == LINE_ID){
                  cached_str = str;              
                  Serial.print("cached_str: ");  
                  Serial.println(cached_str);  
                  itsforme = true; 
                  pinMode(SLAVE_LINE, OUTPUT); 
              } else {
                  itsforme = false; 
                  pinMode(SLAVE_LINE, INPUT); 
              } 
            }           
            str = "";           
            startRecording = false;          
          }//if  
}//if        
//******************************************************************** 
digitalWrite(9, cached_str[2]=='0'?false:true); 
}//loop
