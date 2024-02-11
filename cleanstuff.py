import os

# Set the directory to the current working directory
directory = os.getcwd()

# Loop through each file in the directory
for filename in os.listdir(directory):
    if filename.endswith(".csv") or filename.endswith(".txt"):
        file_path = os.path.join(directory, filename)
        os.remove(file_path)  # Delete the file
        print(f"Deleted {file_path}")
