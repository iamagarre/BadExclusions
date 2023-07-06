# BadExclusions
BadExclusions is a tool to identify custom or undocumented folder exclusions on AV/EDR.

![Screenshot](Img/BadExclusions.png)

# How it works?
It creates a txt file on every folder and subfolder on drive C:\ that contains the EICAR string. After creating all the files then it tries to read each file. If the AV/EDR removed the file it means no exclusion for that folder. If the file still exists then an exclusion is on the folder. The tool will print the full path to each file not removed by AV/EDR.

# Original idea?
I was talking with a friend about custom exclusions made on AV/EDR and how to easily identify them. We had a silly idea but it actually works.

# This is a really noisy tool
Yes, it is. Currently on antiscan.me it has a 0/26 detection and our current EDR does not flag the tool as malicious. It does not kill the process while we create all that eicar files. This is basically because there is nothing malicious on the tool. Even the EICAR string is not flagged because in order to print it on a file you have to scape some characters.

The problem is, when you try to create a file on every folder and subfolder you can end up creating more than two thousand files. Every file created on a folder without an exclusion will create an incident/alert on the system.

Feel free to modify the source code to whatever folder you wish. On my own research I've found C:\ProgramData is a common folder to find custom exclusions. Using C:\ProgramData instead C:\ will reduce the amount of alerts generated. 

Another option is to create a list of a few folders you want to check. It is common to add custom exclusions to folders of other security tools so you can create your own list and check for exclusions on that folders. Even if that folders do not exist on the system you are working on you can try to manually create that folder and run the tool to check if they have an exclusion because the machine could be under the scope of that exclusion. Your list can include C:\ProgramData\McAfee, C:\ProgramData\Kaspersky, etc.

# Requirements
The tool was developed using Visual Studio Community 2022. You also need to install dirent.h library to be able to compile the code.

# Update July 2023
I did a small update of the tool to fix some issues with some AV/EDR. 
