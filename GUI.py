import tkinter as tk
import tkinter.messagebox as tmsg
import os



def get_folder_files(folder):
    directory_path = os.path.abspath(folder)
    files = os.listdir(directory_path)
    return files

def update_folder1_files():
    global folder1_files
    folder1_files = get_folder_files("folder1")
    listbox1.delete(0, tk.END)
    for file in folder1_files:
        listbox1.insert(tk.END, file)
    return
def update_folder2_files():
    global folder2_files
    folder2_files = get_folder_files("folder2")
    listbox2.delete(0, tk.END)
    for file in folder2_files:
        listbox2.insert(tk.END, file)
    return



    

def copy_from_folder1():
    selected_indices = listbox1.curselection()
    if len(selected_indices) != 1:
        tmsg.showerror("ERROR", "INVALID FILE SELECTION!")
        return

    selected_entries = [folder1_files[i] for i in selected_indices]
    selected_entries.reverse()

    while selected_entries:
        r1, w1 = os.pipe()
        r2, w2 = os.pipe()
        r_folder1, w_folder1 = os.pipe()
        r_folder2, w_folder2 = os.pipe()

        file_name = selected_entries.pop()
        folder1_path = "folder1/"
        folder2_path = "folder2/"

        # Parent writes to pipes
        if os.fork() == 0:  # Child process for server
            os.close(w1)
            os.close(w2)
            os.close(w_folder1)
            os.close(w_folder2)

            os.dup2(r1, 0)       # Redirect stdin for the filename
            os.dup2(r2, 3)       # Redirect custom fd for filename
            os.dup2(r_folder1, 4)  # Redirect custom fd for folder1
            os.dup2(r_folder2, 5)  # Redirect custom fd for folder2

            os.execl("./server", "./server")
            os._exit(0)

        elif os.fork() == 0:  # Child process for client
            os.close(w1)
            os.close(w2)
            os.close(w_folder1)
            os.close(w_folder2)

            os.dup2(r1, 0)
            os.dup2(r2, 3)
            os.dup2(r_folder1, 4)
            os.dup2(r_folder2, 5)

            os.execl("./client", "./client")
            os._exit(0)

        # Parent process
        os.close(r1)
        os.close(r2)
        os.close(r_folder1)
        os.close(r_folder2)

        with os.fdopen(w1, "w") as w1_fd:
            w1_fd.write(file_name)

        with os.fdopen(w2, "w") as w2_fd:
            w2_fd.write(file_name)

        with os.fdopen(w_folder1, "w") as w_folder1_fd:
            w_folder1_fd.write(folder2_path)

        with os.fdopen(w_folder2, "w") as w_folder2_fd:
            w_folder2_fd.write(folder1_path)

        os.wait()  # Wait for child processes

    update_folder2_files()
    tmsg.showinfo("SUCCESS", "Files copied Successfully")












    




root = tk.Tk()
root.config(bg="black")
root.title("TCP TRANSFER")
root.minsize(1500,900)

tk.Label(root, text="TCP FILE TRANSFER",fg="beige",bg="black",height=3,width=150,font=("Helvetica", 30, "bold"),highlightthickness=3,highlightcolor="beige").pack(side=tk.TOP)

tk.Label(root, text="FOLDER 1 CONTENT",fg="beige",bg="black",font=("Helvetica", 15, "bold"),highlightthickness=2,highlightcolor="beige",padx=10,pady=5,width=43).place(x=200,y=260)

tk.Label(root, text="FOLDER 2 CONTENT",fg="beige",bg="black",font=("Helvetica", 15, "bold"),highlightthickness=2,highlightcolor="beige",padx=10,pady=5,width=43).place(x=1222,y=260)

# FOLDER 1 LISTBOX
listbox_frame1 = tk.Frame(root, height=50 , width=60)
listbox_frame1.pack(side=tk.LEFT, padx=200, pady=100)
scrollbar1 = tk.Scrollbar(listbox_frame1, orient=tk.VERTICAL,bg="black")
listbox1 = tk.Listbox(listbox_frame1, selectmode=tk.MULTIPLE, yscrollcommand=scrollbar1.set, height=30, width=60,bg="beige")
scrollbar1.config(command=listbox1.yview)
scrollbar1.pack(side=tk.RIGHT, fill=tk.Y)
listbox1.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

tk.Button(root, text="SEND TO FOLDER 2", command=copy_from_folder1, bg="beige",height=2,width=15,font=("helvetica",12,'bold')).place(x = 878,y = 700)




# FOLDER 2 LISTBOX

listbox_frame2 = tk.Frame(root , height=50, width=60)
listbox_frame2.pack(side=tk.RIGHT, padx=200, pady=100)
scrollbar2 = tk.Scrollbar(listbox_frame2, orient=tk.VERTICAL,bg="black")
listbox2 = tk.Listbox(listbox_frame2, selectmode=tk.MULTIPLE, yscrollcommand=scrollbar2.set , height=30, width=60,bg="beige")
scrollbar2.config(command=listbox2.yview)
scrollbar2.pack(side=tk.RIGHT, fill=tk.Y)
listbox2.pack(side=tk.RIGHT, fill=tk.BOTH, expand=True)




# Add text inside the frame

label = tk.Label(root, text="INSTRUCTIONS:\nSelect Any file by clicking on it and then press\n the COPY TO button to paste the file into\n the other folder, You can also chose multiple\n files at a time.", bg="black",highlightthickness=2, highlightbackground="beige",fg="beige")
label.place(x=800,y=350)





update_folder1_files()
update_folder2_files()



root.mainloop()
