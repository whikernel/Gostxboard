;
;	    hook x86 version 
; masm version de la routine de hook 
;       de la table idt 
;-------------------------------------
public hookroutine				    ; Externalise la routine au reste du build


.586P
.model flat
extern uOldIsrAddr:DWORD			; adresse ext de la routine de ISR i8042prt
extern DbgPrintHook:PROC			; fonction dbgprint extern
.code
	hookroutine:					; routine 
		pushad						; Sauvegarde des registres
		pushfd						; Sauvegarde des registres
		push eax					; Sauvegarde de eax 
		call DbgPrintHook			; On appel au debug print 
		popfd						; Restauration des registres
		popad						; Restauration des registres

		jmp uOldIsrAddr				; Saut à l'ISR de i8042prt
		

END