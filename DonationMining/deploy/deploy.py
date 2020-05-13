import os
import pathlib
import re
import shutil
import subprocess

# get parent directory of the current working dir
win_executable_dir = os.path.join(pathlib.Path(os.getcwd()).parent.parent, "bin/win64/bin/")
Qt_dll_folder = os.path.join("", "/usr/x86_64-w64-mingw32/bin/")
Qt_plugins_folder = os.path.join("", "/usr/x86_64-w64-mingw32/lib/qt/plugins/")
print(f"{win_executable_dir=}")
print(f"{Qt_dll_folder=}")
print(f"{Qt_plugins_folder=}")

# run the executable to get the error output
cmd = [win_executable_dir + "DonationMining.exe"]
result = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)  # run() / Popen()
output = result.stdout.readlines()

# find the missing DLLs in the output
regex = re.compile("(?<!\S)[.A-Za-z0-9_-]+\.dll")
missing_dlls = set()
for line in output:
	line = line.decode('UTF-8').strip()
	missing_dlls.update(regex.findall(line))

# copy the DLLs over
for missing_dll in missing_dlls:
	if os.path.isfile(win_executable_dir + missing_dll):
		print(f"Skipping {missing_dll} because file already exists at destination")
		continue

	try:
		print(f"Copying {missing_dll}")
		shutil.copy(Qt_dll_folder + missing_dll, win_executable_dir)
	except FileNotFoundError as exc:
		print(exc)
