import os
import stat
# Set the directory to the current working directory
directory = os.getcwd()

# Loop through each file in the directory
for filename in os.listdir(directory):
    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        if filename.endswith(".csv") or filename.endswith(".txt") or filename.endswith(".exe"):
            os.remove(file_path)  # Delete the file
            print(f"Deleted {file_path}")
        # else:
        #     # Check if the file is an executable
        #     if os.stat(file_path).st_mode & stat.S_IXUSR:
        #         os.remove(file_path)  # Delete the executable file
        #         print(f"Deleted executable {file_path}")
