import serial
import os.path
import datetime
import smtplib
import Tkinter as tk

root = tk.Tk()
root.geometry("400x200")
root.title("Program")
root.resizable(width=False, height=False)

w = 400 # width for the Tk root
h = 300 # height for the Tk root

# get screen width and height
ws = root.winfo_screenwidth() # width of the screen
hs = root.winfo_screenheight() # height of the screen

# calculate x and y coordinates for the Tk root window
x = (ws/2) - (w/2)
y = (hs/2) - (h/2)

root.geometry('%dx%d+%d+%d' % (w, h, x, y))
w = tk.Label(root, text="Program pokrenut!")
w.pack()
root.mainloop()

from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from email.mime.base import MIMEBase
from email import encoders

fromaddr = "pmttestpmt@gmail.com"
toaddr = "pmttestpmt@gmail.com"

msg = MIMEMultipart()
 
msg['From'] = fromaddr
msg['To'] = toaddr

s = serial.Serial('com4', 9600) 
now = datetime.datetime.now()
vremeString = str(now.day)+"-"+str(now.month)+"-"+str(now.year)

filename="Log"
filename = filename+"("+vremeString+")"+".txt"
brojac = 0

while True:   
    if s.inWaiting() > 0:
        m = s.readline()[:-2]
        try:
            if m == b'Alarm mod ukljucen!':
                if(os.path.isfile(filename)):
                    fh = open(filename,"a+")
                else:
                    fh = open(filename,"w+")
                now = datetime.datetime.now()
                fh.write("Alarm mod ukljucen!"+"("+str(now.hour)+":"+str(now.minute)+":"+str(now.second)+")\n")
                fh.close()
            if m == b'Brojanje mod ukljucen!':
                if(os.path.isfile(filename)):
                    fh = open(filename,"a+")
                else:
                    fh = open(filename,"w+")
                now = datetime.datetime.now()
                fh.write("Brojanje mod ukljucen!"+"("+str(now.hour)+":"+str(now.minute)+":"+str(now.second)+")\n")
                fh.close()
            if m == b'Alarm iskljucen!':
                if(os.path.isfile(filename)):
                    fh = open(filename,"a+")
                else:
                    fh = open(filename,"w+")
                now = datetime.datetime.now()
                fh.write("Alarm mod iskljucen!"+"("+str(now.hour)+":"+str(now.minute)+":"+str(now.second)+")\n")
                fh.close()
            if m == b'Brojanje iskljuceno!':
                if(os.path.isfile(filename)):
                    fh = open(filename,"a+")
                else:
                    fh = open(filename,"w+")
                now = datetime.datetime.now()
                fh.write("Brojanje mod iskljucen!"+"("+str(now.hour)+":"+str(now.minute)+":"+str(now.second)+")\n")
                fh.write("\tPrebrojano: "+str(brojac)+"\n")
                brojac=0
                fh.close()
            if m == b'Upad':
                if(os.path.isfile(filename)):
                    fh = open(filename,"a+")
                else:
                    fh = open(filename,"w+")
                now = datetime.datetime.now()
                fh.write("!!!Alarm aktiviran!!!"+"("+str(now.hour)+":"+str(now.minute)+":"+str(now.second)+")\n")
                fh.close()


                
                msg['Subject'] = "Alarm aktiviran!"
                body = "Alarm aktiviran u "+str(now.hour)+":"+str(now.minute)+":"+str(now.second)
                msg.attach(MIMEText(body, 'plain'))
                #filename = "NAME OF THE FILE WITH ITS EXTENSION"
                attachment = open(filename, "rb")
                part = MIMEBase('application', 'octet-stream')
                part.set_payload((attachment).read())
                encoders.encode_base64(part)
                part.add_header('Content-Disposition', "attachment; filename= %s" % filename)
                msg.attach(part)
                server = smtplib.SMTP('smtp.gmail.com', 587)
                server.starttls()
                server.login(fromaddr, "testpmttest")
                text = msg.as_string()
                server.sendmail(fromaddr, toaddr, text)
                server.quit()
                

                
            if m == b'Prosao':
                brojac+=1
        except:
            pass
