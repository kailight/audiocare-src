import tkinter as tk

class Application(tk.Frame):
    def __init__(self, master=None):
        super().__init__(master)
        self.pack()
        self.create_widgets()

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