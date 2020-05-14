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


#define SIGNAL_LINE 6 //3  
#define MASTER_LINE 7 //4
#define SLAVE_LINE  8 //5
#define LINE_ID '0'

int MAX_NODES = 2; //Total Number of Slave Nodes

#define INTERVAL 1000 // timer for 3-wires
unsigned long time_1 = 0;


int call_counter = 1; //slave number caller

bool SIGNAL_LINE_STAT = false;
bool startRecording = false; 
String str;
char chx;

unsigned long lapse;  
bool itsforme = true;

uint8_t c = 0, d = 0;
unsigned int i;

String cached_str;
char concatenated_chars[127];  
const char* data_to_send_to_slave;
        
void setup() 
{
//********************************************************************   
Serial.begin(9600);
while (!Serial) 
{
  ; // wait for serial port to initialize
}
//********************************************************************       
  pinMode(SIGNAL_LINE, OUTPUT);
  pinMode(MASTER_LINE, OUTPUT);
  pinMode(SLAVE_LINE, INPUT); 
  pinMode(9, OUTPUT); 
//********************************************************************   
}

void loop() 
{
//******************************************************************** 
if(millis() >= time_1 + INTERVAL)
{
  time_1 +=INTERVAL;      
  SIGNAL_LINE_STAT = !SIGNAL_LINE_STAT;
  digitalWrite(SIGNAL_LINE,SIGNAL_LINE_STAT);   
}    
//********************************************************************
 
//******************************************************************** 
if(SIGNAL_LINE_STAT)
{        
        data_to_send_to_slave = "123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,123.45,";    
        sprintf(concatenated_chars,"%d0-%s,", call_counter, data_to_send_to_slave);                
        encode_lapse(concatenated_chars, MASTER_LINE); 
        
        if(char2Int(cached_str[2]) == call_counter) {          
        call_counter++;
        if(call_counter > MAX_NODES) call_counter = 1;     
        } 
}
//******************************************************************** 
if(!SIGNAL_LINE_STAT)
{   
        lapse = pulseIn(SLAVE_LINE, HIGH);    
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
                pinMode(MASTER_LINE, OUTPUT);
            } else {
                itsforme = false;  
                pinMode(MASTER_LINE, INPUT);
            } 
          }
          str = "";           
          startRecording = false;
        }     
}//if  
//******************************************************************** 
//******************************************************************** 
digitalWrite(9, cached_str[2]=='0'?false:true); 
}//loop
