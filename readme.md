# Tarrasch Text Editor  
A text editor made in c for linux without using any other libraries  
    
## building  
Use build.sh to build (`. build.sh` or `source build.sh`)  
this will put syntax.cfg in ~/.tarrasch  
  
## syntax config  
the syntax config uses ansi codes to know how to highlight  
to customize it, you need to edit ~/.tarrasch/syntax.cfg  
to add a keyword, make a new line under the `SYNTAX` field and put the new keyword, followed by a space, followed by an ansi code  
basic ansi codes:  
- 30: black  
- 31: dark red  
- 32: dark green  
- 33: yellow  
- 34: blue  
- 35: magenta  
- 36: cyan  
- 37: white  
to change symbol color, edit the value of SYMBOLS under the `SYMBOLS` field  
as for blocks (such as comments and quotes), put it under the `BLOCKSYNTAX` field, with syntax like  
```
startofblock ansivalue endofblock  
```
if end of block is not supplied, it will be default to a single line comment.  
  
## usage  
Use the file edit.o, the first arguement will be the name of the file to edit/create  
Press esc twice to leave, or ^C if you do not want to save  
this editor will only work on linux and on terminals with xterm style ansi codes (for now)  
  
## TODO  
- Add better scrolling  
- And Control Key Functions (Line number toggle, show XY positon, save without exit).  
- More portability  
	- Windows
	- Shells that don't process the same ansi codes.
- Add config file  
- Add makefile  

## errors
- Quote marks mess with syntax, thinking the quote marks are single line comments.  
	- I have an idea this is with the config reader library, so I will have to update that.  
- On that note, single line comments make the next alphanumeric syntax word they see not have its color.  

  