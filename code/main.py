import tkinter.messagebox
from tkinter import *
import serial
import time
import threading

ser = serial.Serial('COM1', baudrate=9600, timeout=1)
run = True

'''
Functions ##############################################################################################################
'''


def writeOnPort():
    resp = e1.get()
    if (verifString(resp) == True):
        ser.write(resp.encode())
        tkinter.messagebox.showinfo(title="Réussi !", message="Le nouveau seuil de déclenchement à été défini !")
    else :
        tkinter.messagebox.showwarning(title="Erreur", message="Le seuil de déclenchement doit être écris de cette manière :\n @123 \n(si la valeur est inférieur à 100cm, il faut indiquer un 0 (exemple : 090) \n ATTENTION : La valeur ne doit pas excéder 400cm !")
    print(resp)


def verifString(test):
    test_sans = test[1:] # sans le @
    if ((test.startswith("@")) == True & (test_sans.isdecimal()) == True & (len(test) == 4) == True): #soucis sans les True, les tests ne passaient pas
        if ((int(test_sans) <= 400) == True): #vérification en 2 if car soucis de changement de type avec int
            return True
    else:
        return False


def readOnPort():
    if run:
        data_received = (ser.read(3)).decode()
        test_data = data_received
        print(test_data)
        if ((test_data.isdigit()) == True):
            distance_label.config(text=f"La distance actuelle est de : {test_data}")
        if (test_data == "ON_"):
            frame1.config(bg='#e64221')
            alerte_label.config(bg='#e64221')
        if (test_data == "OFF"):
            frame1.config(bg='#66e341')
            alerte_label.config(bg='#66e341')
        root.after(1000, readOnPort)


def stopRead():
    global run
    run = False
    print("stop")


#def startRead():
#    global run
#    run = True
#    readOnPort


'''
Graphical User Interface ###############################################################################################
'''

root = Tk()

root.title("Test")
root.geometry("600x500")

my_label = Label(root, text="Seuil de déclenchement")
my_label.pack(pady=20)

frame1 = Frame(root, borderwidth=2, relief=GROOVE)
frame1.pack(side=BOTTOM, padx=10, pady=10)
alerte_label = Label(frame1, text="Alerte")
alerte_label.pack(pady=10, padx=10)

e1 = Entry(root)
e1.pack(pady=20)

my_button1 = Button(root, text="Ajouter", command=writeOnPort)
my_button1.pack(pady=20)

distance_label = Label(root, text="")
distance_label.pack(pady=20)

my_button2 = Button(root, text="Read", command=threading.Thread(target=readOnPort).start)
my_button2.pack(pady=20)

my_button3 = Button(root, text="Stop", command=stopRead)
my_button3.pack(pady=20)

#my_button4 = Button(root, text="Start", command=threading.Thread(target=startRead).start)
#my_button4.pack(padx=20)

root.mainloop()
