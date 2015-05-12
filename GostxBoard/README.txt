----- Installation Guide -------

Preparation : 



Registery key : 

HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Class\
        {4D36E96B-E325-11CE-BFC1-08002BE10318}\UpperFilters




Installation : 

sc create GostxBoard type= kernel binPath= c:\Windows\System32\GostxBoard.sys
sc start GostxBoard (not working since a reboot is needed )