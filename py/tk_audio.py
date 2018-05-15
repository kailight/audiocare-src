import tkinter as tk
import sys
import matplotlib
matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2TkAgg
from matplotlib.figure import Figure
from datetime import datetime

class Application(tk.Frame):
  def __init__(self, master=None):
    super().__init__(master)
    master.title = "AudioCare 0.0.1"
    master.geometry("800x600")
    self.create_frames()
    self.pack()

  def create_frames(self):
    self.top = tk.Label(self, text='Devices', pady=5, font=("Courier", 22))
    self.top.pack(side="top")
    self.scan = tk.Button(self, text='Scan for devices', pady=2, command=self.show_frames)
    self.scan.pack(side="top")
    self.middle = tk.Frame(self, pady=10, padx=10)
    self.frames = []
    row = 0
    column = 0
    for i in range(1, 5):
      frame = self.create_frame(i)
      if (column % 2 == 0):
        column = 0
        row=row+1
      else:
        column=column+1
      frame.grid(column=column, row=row, sticky='nsew')
      self.frames.append(frame)
      frame.grid_forget()
    # for frame in self.frames:


    self.middle.pack(side="bottom")

  def show_frames(self):
    row = 0
    col = 0
    for index,frame in enumerate(self.frames):
      # print(index,frame)
      # frame = self.frames[i]
      frame.grid(column=col, row=row, sticky='nsew')
      if (col % 2 == 1):
        col = 0
        row=row+1
      else:
        col=col+1
      print(row, col)

  def create_frame(self, i):
    i = str(i)
    # print(i)
    setattr(self, 'frame'+i, tk.Frame(self.middle, bd=2, relief='sunken'))
    frame = getattr(self,'frame'+i)
    frame.grid(row=0, column=0, sticky='nsew')
    frame.header = tk.Label(frame, text='Device #000'+i, width=40)
    frame.header.pack(side="top")
    frame.status = tk.Button(frame, text='Inactive', command = lambda: self.changeStatus(frame))
    frame.status.active = False
    frame.status.pack(side="top")
    # frame.stats = tk.Label(frame, pady=5, anchor="w", bd=2, relief='ridge', width=50, text='Stat1: 0\n Stat2: 0\n etc... (should make live graphs?)')
    # frame.stats.pack(side="top")

    left = tk.Frame(frame, relief="sunken")
    left.pack(side="left")

    canvas = self.makeFigure(frame)

    # L1 = tk.Label(left, text="Sampled parameters ", width=25)
    # L1.pack(side="top")
    # lb1 = tk.Listbox(left, selectmode="multiple")
    # lb1.insert(1, "MFCC")
    # lb1.insert(2, "Slope")
    # lb1.insert(3, "Centroid")
    # lb1.insert(4, "Skewness")
    # lb1.insert(5, "Kurtosis")
    # lb1.insert(6, "Decrease")
    # lb1.insert(7, "Rolloff")
    # lb1.pack(side="top")

    controls = tk.Frame(frame, relief="sunken")
    controls.pack(side="top")

    lf = tk.Frame(controls)
    L2 = tk.Label(lf, text="Sample data every ")
    L2.pack(side="top")
    I1 = tk.Spinbox(lf, values=['1m','30s','10s'])
    I1.pack(side="top")
    lf.pack(side="left")

    L3 = tk.Label(controls, text="Sample duration ")
    L3.pack(side="top")
    I3 = tk.Spinbox(controls, values=['60s','30s','10s','5s'])
    I3.pack(side="top")

    L4 = tk.Label(controls, text="Sample period ")
    L4.pack(side="top")
    I4 = tk.Spinbox(controls, values=['2s','1s','0.5s'])
    I4.pack(side="top")

    frame.pack(side="left")

    return frame

  def makeFigure(self, frame):
    fig = Figure(figsize=(5, 3), dpi=100)
    fig.set_tight_layout(True)
    plt = fig.add_subplot(111)

    # time = ['40:01', '40:02', '40:03', '40:04', '40:05', '40:06', '40:07', '40:08']
    # time = ['01', '02', '03', '04', '05', '06', '07', '08']
    # datetimes = [datetime.strptime(t, "%M:%S") for t in time]
    # datetimes = [datetime.strptime(t, "%S") for t in time]
    fdatetimes = ['58','59','00','01','02','03','04','05','06','07']
    fdata1 = [5, 6, 1, 3, 8, 9, 3, 5, 4, 7]
    fdata2 = [3, 1, 7, 5, 2, 1, 8, 5, 6, 4]
    plt.x = 0
    data1=fdata1[0:8]
    data2=fdata2[0:8]
    datetimes = fdatetimes[plt.x:8]

    def setdata(x):
      x2 = x + 8
      print(x,x2)
      data1 = fdata1[x:x2]
      data2 = fdata2[x:x2]

    setdata(plt.x)
    line1, = plt.plot(datetimes, data1, label="Mean")
    line2, = plt.plot(datetimes, data2, label="Slope")
    plt.legend(handles=[line1, line2], bbox_to_anchor=(1.04, 1), loc="upper left")
    plt.set_xlabel('Time')
    plt.set_ylabel('Value')


    canvas = FigureCanvasTkAgg(fig, frame)
    def onclick(event):
      print(plt.x)
      plt.x = plt.x+1
      setdata(plt.x)
      print(plt.x)
      # print('button=%d, x=%d, y=%d, xdata=%f, ydata=%f' %
            # (event.button, event.x, event.y, event.xdata, event.ydata))
      # plt.plot(event.xdata, event.ydata, ',')
      canvas.draw()

    cid = fig.canvas.mpl_connect('button_press_event', onclick )
    canvas.draw()
    # canvas.get_tk_widget().pack(side=tk.BOTTOM, fill=tk.BOTH, expand=True)



    # toolbar = NavigationToolbar2TkAgg(canvas, self)
    # toolbar.update()
    canvas._tkcanvas.pack(side="top", fill=tk.BOTH, expand=True)
    return canvas

  def changeStatus(self,frame):
    print('status change for frame')
    status = frame.status
    if (status.active == False):
      status.active = True
      status["text"] = 'Active'
    else:
      status.active = False
      status["text"] = 'Inactive'

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