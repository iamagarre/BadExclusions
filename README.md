# BadExclusions
BadExclusions is a tool to identify custom or undocumented folder exclusions on AV/EDR.

# How it works?
BadExclusions creates a txt file on every folder and subfolder in an specified path that contains the EICAR string. After creating all the files then it tries to read each file. If the AV/EDR removed the file it means no exclusion for that folder. If the file still exists then an exclusion is on the folder. The tool will print the full path to each file not removed by AV/EDR.

To identify an excluded folder the user that runs BadExclusions needs write permissions on that folder.

# Original idea?
I was talking with a friend about custom exclusions made on AV/EDR and how to easily identify them. We had a silly idea but it actually works.

# This is a really noisy tool
Yes, it is. Currently on antiscan.me it has a 0/26 detection. 

This is basically because there is nothing malicious on the tool. Even the EICAR string in the code is not flagged because in order to print it on a file you have to scape some characters.

The problem is, when you try to create a file on every folder and subfolder you can end up creating huge amount of files. Every file created on a folder without an exclusion will create an incident/alert on the system.

For a version of BadExclusions that does not create a single alert go to https://github.com/iamagarre/BadExclusionsNWBO

# Microfot Defender Demo
https://github.com/iamagarre/BadExclusions/assets/89855208/e2a8579c-a590-4d9a-8429-c290447423f8

# EDR Demo
https://github.com/iamagarre/BadExclusions/assets/89855208/a1cac2d8-fba9-48c2-a54c-e4d275b61a16
