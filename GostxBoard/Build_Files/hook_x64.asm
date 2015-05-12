;
;	    hook x64 version 
; masm version de la routine de hook 
;       de la table idt 
;-------------------------------------

				
	extern uOldIsrAddr:QWORD			; adresse ext de la routine de ISR i8042prt, UINT64
	extern DbgPrintHook:PROC			; fonction dbgprint externes

	public hookroutine					; Externalise la routine au reste du build

	.code  
	 hookroutine:						; routine 
			push rax					; Sauvegarde de rax, il n'y a pas besoin de sauver autre chose en 64b 
			call DbgPrintHook			; On appel au debug print 

			jmp uOldIsrAddr				; Saut à l'ISR de i8042prt


END