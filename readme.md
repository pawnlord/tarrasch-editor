# Tarrasch Text Editor  
A text editor made in c for linux without any other libraries  
    
## building  
Use build.sh to build (`. build.sh` or `source build.sh`)  
this will put syntax.cfg in ~/.tarrasch  
  
## syntax config  
the syntax config uses ansi codes to know how to highlight  
to customize it, you need to edit ~/.tarrasch/syntax.cfg  
to add a keyword, make a new line and put the new keyword, followed by a space, followed by an ansi code  
basic ansi codes:  
- 30: black  
- 31: dark red  
- 32: dark green  
- 33: yellow  
- 34: blue  
- 35: magenta  
- 36: cyan  
- 37: white  
  
## usage  
Use the file edit.o, the first arguement will be the name of the file to edit/create  
Press esc twice to leave, or ^C if you do not want to save  
this editor will only work on linux and on terminals with xterm style ansi codes (for now)  
  
## TODO  
- Add better scrolling  
- And Control Key Functions (Line number toggle, show XY positon, save without exit).  
- More portability
- Add config file  
  