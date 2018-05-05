import tkinter as tk
import sys

class Application(tk.Frame):
  def __init__(self, master=None):
    super().__init__(master)
    self.pack()
    self.create_frames()

  def create_frames(self):
    self.top = tk.Label(self, text='Devices', pady=5, font=("Courier", 22))
    self.top.pack(side="top")
    self.scan = tk.Button(self, text='Scan for devices', pady=2, command=self.show_frames)
    self.scan.pack(side="top")
    self.middle = tk.Frame(self, pady=10, padx=10)
    self.frames = []
    for i in range(1, 4):
      frame = self.create_frame(i)
      frame.grid(row=i, column=i, sticky='nsew')
      self.frames.append(frame)
      frame.grid_forget()
    # for frame in self.frames:


    self.middle.pack(side="bottom")

  def show_frames(self):
    for index,frame in enumerate(self.frames):
      # print(index,frame)
      # frame = self.frames[i]
      frame.grid(row=0, column=index, sticky='nsew')

  def create_frame(self, i):
    i = str(i)
    print(i)
    setattr(self, 'frame'+i, tk.Frame(self.middle, bd=2, relief='sunken'))
    frame = getattr(self,'frame'+i)
    frame.grid(row=1, column=1, sticky='nsew')
    setattr(self, 'frame' + i + 'header', tk.Label(frame, text='Device #000'+i, width=40))
    header = getattr(self, 'frame' + i+'header')
    header.pack(side="top")
    setattr(self, 'frame' + i + 'status', tk.Button(frame, text='Inactive'))
    button = getattr(self, 'frame' + i + 'status')
    button["command"] = lambda: self.changeStatus(i)
    button.active = False
    button.pack(side="top")
    setattr(self, 'frame' + i + 'text', tk.Label(frame, pady=5, anchor="w", bd=2, relief='ridge', width=50, text='Stat1: 0\n Stat2: 0\n etc... (should make live graphs?)'))
    label = getattr(self, 'frame' + i + 'text')
    label.pack(side="top")

    L1 = tk.Label(frame, text="Sampled parameters ")
    L1.pack(side="top")
    lb1 = tk.Listbox(frame, selectmode="multiple")
    lb1.insert(1, "MFCC")
    lb1.insert(2, "Slope")
    lb1.insert(3, "Centroid")
    lb1.insert(4, "Skewness")
    lb1.insert(5, "Kurtosis")
    lb1.insert(6, "Decrease")
    lb1.insert(7, "Rolloff")
    lb1.pack(side="top")

    L2 = tk.Label(frame, text="Sample data every ")
    L2.pack(side="top")
    I1 = tk.Spinbox(frame, values=['60s','30s','10s'])
    I1.pack(side="top")

    L3 = tk.Label(frame, text="Sample duration ")
    L3.pack(side="top")
    I3 = tk.Spinbox(frame, values=['60s','30s','10s','5s'])
    I3.pack(side="top")

    L4 = tk.Label(frame, text="Sample period ")
    L4.pack(side="top")
    I4 = tk.Spinbox(frame, values=['0.5s', '1s', '2s'])
    I4.pack(side="top")

    frame.pack(side="left")

    return frame

  def changeStatus(self,i):
    print('status change for frame'+i)
    button = getattr(self, 'frame' + i + 'status')
    print(button.active)
    if (button.active == False):
      button.active = True
      button["text"] = 'Active'
    else:
      button.active = False
      button["text"] = 'Inactive'

  def create_widgets(self):

    self.frame1 = tk.Frame(self, width=50, height=200, bd=2, relief='ridge')
    self.frame1.pack(side="left")

    # self.frame['padding'] = (5, 10)
    self.frame1_label = tk.Label(self.frame1, text='Room 1')
    self.frame1_label.pack(side='top')
    self.btn1 = tk.Button(self.frame1)
    self.btn1["text"] = "Start capturing audio"
    self.btn1["command"] = self.say_hi
    self.btn1.pack(side="top")

    self.frame2 = tk.Frame(self, width=200, height=200, bd=2, relief='ridge')
    self.frame2.pack(side="left")

    # self.frame['padding'] = (5, 10)
    self.frame2_label = tk.Label(self.frame2, text='Room 2')
    self.frame2_label.pack(side='top')
    self.btn2 = tk.Button(self.frame2)
    self.btn2["text"] = "Start capturing audio"
    self.btn2["command"] = self.say_hi
    self.btn2.pack(side="top")

    self.bottom = tk.Frame(self, width=400, height=400, bd=2, relief='ridge')
    self.bottom.pack(side='bottom')
    self.quit = tk.Button(self.bottom, text="EXIT", fg="red",
                          command=root.destroy)
    self.quit.pack(side="bottom")



  def say_hi(self):
    self.btn1["text"] = "Stop capturing audio"
    self.btn1["command"] = self.say_bye
    print("some audio data here")

  def say_bye(self):
    self.btn1["text"] = "Start capturing audio"
    self.btn1["command"] = self.say_hi
    print("capture ended")

root = tk.Tk()
app = Application(master=root)
app.mainloop()