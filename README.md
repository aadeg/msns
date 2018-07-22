msns - Max Size Notification System
===========
[![Build Status](https://travis-ci.org/aadeg/msns.svg?branch=master)](https://travis-ci.org/aadeg/msns)

*msns* is a tool that sends an email whenever a folder reaches a size limit. It is completely configurable and it is intended to work with *cron* daemon in order to periodically check folders.

Features
-------
+ Send email over SMTP, supporting STARTTLS encryption.
+ Global configuration with global notification emails.
+ Per folder configuration: size limit and folder notification emails.

Email Example
------
```
MSNS REPORT

    Date:          Sat 27 May 2017 08:13
    Rep. Type:     all

MONITOR REPORT

    name                 path                                                 dim (MB) limit (MB)     perc
    -------------------- --------------------------------------------------- ---------- ---------- --------
    Database             /home/mysql                                               6805      15360  -55.70%
    Site                 /home/www-data                                            6593      15360  -57.08%


msns 0.2.2 - https://github.com/aadeg/msns

```

Intallation
-----
*msns* uses this libraries:
+ Boosts
+ CURL

Run this command to install:
Debian/Ubuntu
```
$ sudo apt-get install libboost-all-dev libcurl4-openssl-dev cmake git
$ git clone git@github.com:aadeg/msns.git
$ cd msns
$ mkdir build && cd build
$ cmake ..
$ sudo make install
```

Global configuration file is located in `/etc/msns/config.ini`. Change it to fit your needs.
```
[general]
machine_name = test_1

[folders]
/absolute/path_1 = 0
/absolute/path_2 = 0

[emails]
test@test.com = 0
test_1@test.org = 0

[smtp]
url = 
username = 
password = 
ssl = 0
from =
```

Usage
-----
To monitor a new folder, first make sure that the *absolute path* of the folder or one of its **parents** is written in the **global configuration** (section *folders*).

Next, go to the folder using *cd* and run this command:
```
$ msns --init --size 1024 --name ftp_folder
```
This command creates a new file *msns.ini* in the folder you are in. With *--size* you declare to send a notification if the folder exceeds 1024 MB; *--name* sets a friendly name for the folder in the email notification. You can manually edit the *msns.ini* file in order to add email addresses to which send notification for this particular folder. Keep in mind that email addresses specified in the global configuration will receive notification for all folders.

After adding the folder, you can run the command:
```
$ sudo msns --run --email
```
msns will travel all the folders specified in le **global configuration** looking for *msns.ini* files. *--email* parameter indicates that you want to recieve an email if a folder exceeds the limit. You can add *--report-lvl=all* if you want to receive an email even if noone of the folders reached the limit. You can add this command to *cron* in order to automatically receive notification email.

