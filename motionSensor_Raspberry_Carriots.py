#-*- coding: utf-8 -*-                                                          

'''                                                                             
author: PauMB                                                                   
Program: Motion sensor, if it detec moves in the room, the firts time send      
         an email, the 20th send sms with Carriots plataform                    
Date: 08/09/14                                                                  
Company: HedaSoft                                                               
'''

#importing libreries                                                            
import RPi.GPIO as GPIO
import time
import sys
import smtplib
from email.mime.text import MIMEText
import signal

#values                                                                         
lastStatus = 1   #inicilize in 0, (0 no move, 1 move)                           
n_Moves = -1     #number the moves detected                                     

#names of gpio                                                                  
motion = 17
ledGreen = 23
ledRed = 24

def signal_handler(signal,frame):
    '''Close the program with ^C'''
    print ""
    GPIO.cleanup()
    print "Close the program"
    sys.exit(0)

def setup():
    '''Inicializamos configuracion inicial'''
    #inicialicies GPIO                                                          
    GPIO.setmode(GPIO.BCM)         #set up GPIO using BCM numbering             
    GPIO.setup(motion, GPIO.IN)    #control motion sensor with GPIO 17          
    GPIO.setup(ledGreen, GPIO.OUT) #control ledGreen                            
    GPIO.setup(ledRed, GPIO.OUT)   #control ledRed                              

    #incialicies Leds                                                           
    GPIO.output(ledGreen,True)       #turn ledGreen on                          
    GPIO.output(ledRed,True)      #turn ledRed off                              

    signal.signal(signal.SIGINT, signal_handler)

    #Motion Sensor begin in...                                                  
    print("The Motion Sensor begins in:")
    for i in range (5,0,-1):
      print(i)
      time.sleep(1)

def tiempo(tiempoCapturado):
    '''a partir de un tiempo capturada obtenemos su hora y minutos              
       return hora, minute'''
    hour = tiempoCapturado[3]
    minute = tiempoCapturado[4]
    return hour,minute

def envioMail(hora,minuto):
    '''Funcion que envia un mail'''

    # Creamos el mensaje                                                                           
    msg = MIMEText("Se ha detectado movimiento en la habitacion")

    # Conexión con el server                                                                       
    msg['Subject'] = 'Prueba de mail con python y motion sensor desde Raspberry Pi'
    msg['From'] = 'YourMail@gmail.com'
    msg['To'] = 'To Mail'

    # Autenticamos                                                                                 
    mailServer = smtplib.SMTP('smtp.gmail.com',587)
    mailServer.ehlo()
    mailServer.starttls()
    mailServer.ehlo()
    mailServer.login("YourMail@gmail.com","password")
    # Enviamos 'from' 'to'                                                                         
    mailServer.sendmail("from@gmail.com", "to@gmail.com", msg.as_string())
    # Cerramos conexión                                                                            
    mailServer.close()
    
def main():
    '''Programa principal'''
    #incializamos                                                                       \
                                                                                         
    setup()

    global n_Moves, lastStatus
    while True:
        #Comprobamos si se detecta movimiento                                           \
                                                                                         
        if GPIO.input(motion) == GPIO.HIGH and lastStatus == 0:
            print "Se detecta movimeinto"
            lastStatus = 1
            n_Moves += 1
            GPIO.output(ledRed,True)
            GPIO.output(ledGreen,False)
            if n_Moves%5 == 0 :
                t_CapturaSeg = time.time()
                t_Captura = time.localtime(t_CapturaSeg)
                hour, minute = tiempo(t_Captura)
		envioMail(hour,minute)
                print "Send Mail"
            elif n_Moves%17 == 0 :
                #PREPARAR PARA ENVIAR UN SMS CON CARRIOTS                                
                n_Moves = 0
                print "Send a SMS"
      elif GPIO.input(motion) == GPIO.LOW and lastStatus == 1:
            print "No se detecta movimiento"
            lastStatus = 0
            GPIO.output(ledRed,False)
            GPIO.output(ledGreen,True)

if __name__ == '__main__':
    main()
