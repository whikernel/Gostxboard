//**********************************************************************//
// 					This file is part of GostxBoard						//
//																		//
//  GostxBoard is free software: you can redistribute it and/or modify	//
//       it under the terms of the Creative Commons [Attribution-		//	
//	NonCommercial-ShareAlike 4.0 International] License as published	// 
//				 by the Ditribution of Creative Commons 				//		
//                Such things are STRONGLY recommended.					//
//    																	//
// 	GostxBoard is distributed in the hope that it will be useful,		//
//  but WITHOUT ANY WARRANTY; without even the implied warranty of		//	
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		//
//  	Creative Commons [BY-NC-SA] License for more details.			//	
//  Do not blame the developper if something goes wrong. You can 		//	
//  either search on the Internet or email the developper to deal 		//
//                             with it.									//
//																		//
//  You should have received a copy of the Creative Commons [BY-NC-SA]	// 
//	             License along with GostxBoard. If not, see 			//
//	  <https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode>. 	//
//																		//
//                              P.A 									//
//                gostxboard_support@whitekernel.fr 					//
//**********************************************************************//

/*!
*\~English
*\mainpage notitle
*
* \section intro_sec Introduction
*	GostxBoard is a beta module created to improve Windows's security, by protecting the system from almost all keyloggers. \n
*	The module consists of a kernel mode driver and an API which can be used by developpers to improve the security of their applications. \n
*
* \section desc_sec	Description
*	When an application wants the user to enter senstive information such as credit number or credentials, it can call the API to begin a cipher session. \n 
*	The API will send to the driver some information about the calling application, and will get in return credentials. From this point, every keyboard 
*	entry will be ciphered. To read it, the client application will have to call the API which will be able, with credentials, to decipher it.
*	The application can then call the API again to stop the cipher session. \n
*	Thus, every other applications (such as keyloggers) or even the OS, only see the ciphered keys during a cipher session. 
*
*	\subsection driver_subsec Driver
*		The driver is a kernel mode driver and takes place in the drivers stack of the keyboard as an upper-filter of the i8042prt port driver of Windows. \n
*		At boot time, it registers an Interrupt Service Routine (ISR) to the i8042prt driver which will be called each time a keyboard interrupt occured.  
*		When it happends, i8042prt reads the scancode associated to the interrupt and then immediatly call the registered ISR. Therefore, the GostxBoard driver is one of the first
*		to process the keyboard entry, ensuring high security. \n
*		If an application is connected to the driver through a cipher session, it ciphers every received entry with stored credentials and then spreads them to the entire system. \n
*
*	\subsection api_subsec API
*		The API is a set of usefull functions provided by the GostxBoard DLL. These functions can be used by an application to secure its keyboard entries. \n 
*		Almost all the sensitives problems, such as storing credentials, are handled by the API as much as possible. Basically, a client application can secure its entries by only 
*		four calls to the API.  \n 
*
*
* \section install_sec Installation
*	\a IMPORTANT : It is STRONGLY recommended to install and test the driver on a virtual machine, before doing a real install. Anyway, if one wants to install it on a real machine, it 
*	must disabling the Windows Test Signing since the provided driver is not Windows signed. 
*	\a CAREFULL : doing such a thing can leads to critical security issue ! \n
*		
*	\subsection autoinstall_subsec Automatic installation
*		Whatever, the project comes with a Package and an INF file which can be used to install the driver on the target machine.
*	\subsection manualinstall_subsec Manual installation
*		If one wants to manually install the driver, it can do the following. 
*		\subsubsection para1 Change registry 
*			Add the value "GostxBoard" at the subkey HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96B-E325-11CE-BFC1-08002BE10318}\\UpperFilters. \n 
*			Place it on the top of other values. 
*		\subsubsection para3 Add the appropriated service  
*			Register the service with the following command : sc create GostxBoard type= kernel binPath= C:\\Windows\\System32\\GostxBoard_$ARCH$.sys \n
*		where $arch$ is the architecture of the target system. 
*		\subsubsection para2 Copy files
*			Copy Gostxboard_$arch$.sys in C:\\Windows\\System32\\ folder and gostxboard.inf in C:\\Windows\\Inf\\. 
*		\subsubsection para4 Reboot
*			Reboot and,.. here you are. 			
* 
* \section Disclaimer 
* 		This program is free software: you can redistribute it and/or modify it under the terms of the Creative Commons [Attribution-NonCommercial-ShareAlike 4.0 International] license. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the Creative Commons License for more details. You should have received a copy of the Creative Commons License along with this program. If not, see https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*
*
*\~French
*\mainpage notitle
*
* \section intro_sec Introduction
*	GostxBoard est un module en version beta, crée pour améliorer la sécurité de Windows, en protégeant le système de la plupart des keyloggers. \n 
*	Le module consiste en un driver kernel mode, et une API qui peut-être utilisée par les développeurs pour améliorer la sécurité de leurs applications. \n
*
* \section desc_sec	Déscription
*	Lorsqu'une application souhaiyte que l'utilisateur entre des informations sensibles, comme un mot de passe ou un numéro de carte bancaire, elle peut faire appel à l'API pour démarrer une session chifrée. \n 
*	L'API va alors envoyer au driver des informations à propos de l'application appelante, puis va récupérer en retour des identifiants. A partir de là, chaque entrée du clavier sera chiffrée. 
*	Pour les lire, l'application devra appeler à nouveau l'API, qui sera alors en mesure de déchiffrer les entrées grâce aux identifiants. L'application cliente peut à tout moment arrêter la session de chiffrement 
*	en notifiant l'API. \n 
*	Ainsi, toute application ou même l'OS ne verra que des touches chiffrés lors d'une session de chiffrée. 
*
*	\subsection driver_subsec Driver
*		Le driver est un driver kernel mode qui prends place dans la pile des drivers clavier, en tant que driver filtrant supérieur du driver port i8042prt de Windows. \n
*		Au démarrage, il enregistre une routine d'interruption (ISR) au niveau de i8042prt qui sera appelée à chaque interruption clavier. Lorsque cela arrive, i8042prt lie le scancode lié
*		à l'interruption, puis appelle immédiatement l'ISR enregistrée. Ainsi, le driver GostxBoard est l'un est premier à traiter l'interruption, améliorant de manière significative la sécurité. \n
*		Si une application est connectée au driver au travers une session chiffrée, le driver chiffre chaque entrée grâce aux identifiants stockés, puis les envoie au reste du système. 
*
*	\subsection api_subsec API
*		L'API est ensemble de fonctions utiles fournis par la DLL GostxBoard. Ces fonctions peuvent être utilisées par n'importe quelle application souhaitant sécurisé ses entrées clavier. \n 
*		La plupart des problèmes critiques, comme le stockage des clés, sont pris en charge par le driver autant que possible. De cette manière, une application cliente peut sécuriser ses entrées grâce à seulement 
*		quatre appelles à l'API. \n
*
* \section install_sec Installation
*	\a IMPORTANT : Il est FORTEMENT conseillée d'installer et tester le driver sur une machine virtuelle, avant toute installation réelle. Néammoins, pour installer le driver sur une machine il faut désactiver le Windows 
*		Tst Signing puisque le driver fournis n'est pas signé numériquement par Windows. \n
*	\a ATTENTION : Faire une telle chose met gravement le système en danger !
*
*	\subsection autoinstall_subsec Installation automatique
*		Quoiqu'il en soit, le projet contient un PAckage et un fichier INF qui peuvent être utilisé pour une installation automtique sur la machine cible.
*
*	\subsection manualinstall_subsec Installation manuelle
*		Pour installer le driver manuellement : 
*		\subsubsection para1 Modifier la clé de registre
*			Ajouter la valeur "GostxBoard" à la sous-clé HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E96B-E325-11CE-BFC1-08002BE10318}\\UpperFilters. \n
*			Placer la valeur en tout premier par rapport aux autres déjà présentes.
*		\subsubsection para3 Enregistrer le service associé
*			Enregistrer le service associé au driver grâce à la commande suivante :\n 
*				sc create GostxBoard type= kernel binPath= C:\\Windows\\System32\\GostxBoard_$ARCH$.sys \n
*		avec $arch$ l'architecture du système cible.
*		\subsubsection para2 Copier les fichier
*			Copier Gostxboard_$arch$.sys dans le dossier C:\\Windows\\System32\\ et gostxboard.inf dans C:\\Windows\\Inf\\.
*		\subsubsection para4 Redémarrer
*			Redémarrer.. et voilà !
*
* \section Disclaimer
*	This program is free software: you can redistribute it and/or modify it under the terms of the Creative Commons [Attribution-NonCommercial-ShareAlike 4.0 International] license. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the Creative Commons License for more details. You should have received a copy of the Creative Commons License along with this program. If not, see https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
*/

/**
* \~English
* \file      driver.c
* \author    Paul Amicelli
* \version   1.0
* \date      May 21, 2015
* \brief     Defines standards driver functions
*
* \details   Defines standards driver functions like DriverEntry
*			 and AddDevice. Theses functions builds the driver context.
*
* \~French
* \file		driver.c
* \author	Paul Amicelli
* \version  1.0
* \date		21 Mai 2015
* \brief	Définit les fonctions standards des drivers
*
* \details	Définit les fonctions standards des drivers comme DriverEntry
*			et AddDevice. Elles construises le driver et son contexte.
*/

#include "driver.h"
#include "driver.tmh"

//
// Defines driver IRQL
//
_IRQL_requires_max_(APC_LEVEL)
_IRQL_requires_min_(PASSIVE_LEVEL)
_IRQL_requires_same_


DRIVER_INITIALIZE						DriverEntry;
EVT_WDF_IO_QUEUE_IO_READ  				GostxBoard_EvtIoRead;
EVT_WDF_REQUEST_COMPLETION_ROUTINE		GostxBoard_CompletionReadRoutine;
EVT_WDF_DRIVER_UNLOAD					GostxBoard_EvtUnload;
EVT_WDF_DRIVER_DEVICE_ADD				GostxBoard_EvtDeviceAdd;
EVT_WDF_DEVICE_PREPARE_HARDWARE			GostxBoard_EvtDevicePrepareHardware;

extern PAPP_CREDENTIALS	pCurrentApp;
	
//
// Device name used for user mode connection
//
const WCHAR		LinkName[] = L"\\DosDevices\\SecureKeyboard";

//
// Number of instances of the raw pdo created
//
ULONG InstanceNo = 0;


/**
* \~English
* \brief    Driver initialization
* \details  Driver initialization, called when the os is loading 
*			the driver. Typically, it is called after the i8042prt
*			port driver is loaded. \n The function registers the configuration
*			routine AddDevice and creates the driver object. 
*
* \param    PDRIVER_OBJECT  DriverObject	Pointer to the driver's driver object
* \param    PUNICODE_STRING RegistryPath	Pointer to unicode string specifying the 
											path to the driver's registry key.
* \return   NTSTATUS						Status of operation
*
* \~French
* \brief    Initialise le driver
* \details  La fonction initialise le driver. Elle est appelée lorsque 
*			l'OS veut charger le driver, typiquement après le chargement 
*			du driver de port i8042prt. \n La fonction enregistre la routine
*			de configuration AddDevice et créée l'objet driver.
*
* \param    PDRIVER_OBJECT  DriverObject	Pointer vers l'objet driver du driver
* \param    PUNICODE_STRING RegistryPath	Pointer vers une string unicode spécifiant le 
											chemin vers le clé de registre du driver.
* \return   NTSTATUS						Status de l'opération
*
*/
NTSTATUS DriverEntry(
	_In_ PDRIVER_OBJECT  DriverObject, 
	_In_ PUNICODE_STRING RegistryPath
	)
{
	NTSTATUS				ntStatus	= STATUS_SUCCESS;			
	WDF_DRIVER_CONFIG		wdfConfig	= {0};			

	#ifdef _DEBUG 
		DbgPrint(("--- GostxBoard Initialization---\n" ));
		DbgPrint(("--Version %s - Built %s -- \n", __VERSION__, __DATE__));
	#endif

	//
	// Initiate wpp tracing
	//
	#ifdef _DEBUG 
		WPP_INIT_TRACING(DriverObject, RegistryPath);
		bSilent = FALSE;
	#endif

	//
	// Register the configuration routine. It wille be called 
	// later, by the pnpmanager when the keyboard will be added
	//
	WDF_DRIVER_CONFIG_INIT(
			&wdfConfig,
			GostxBoard_EvtDeviceAdd
		);

	//
	// Create the framework object representing the driver. 
	// Its configuration is given by the above initialization. 
	// The object is created as the root of the object tree, thus it doesn't
	// have any parents. 
	//
	ntStatus = WdfDriverCreate( DriverObject, 
						RegistryPath, 
						WDF_NO_OBJECT_ATTRIBUTES,
						&wdfConfig, 
						WDF_NO_HANDLE);
	if( !NT_SUCCESS(ntStatus) )
	{
		#ifdef _DEBUG 
			DbgPrint("\n[x] GostxBoard_GostxBoard_Entry::WDFDriverCreate failed with status 0x%x \n", ntStatus);
		#endif
	}

	return ntStatus;
}




/**
* \~English
* \brief    Unloads the driver
* \details  Unloads the driver, by deleting and reseting structures. This
*			function is called when the PnPmanger wants to disconnect the 
*			keyboard, typically at shutdown time 
*
* \param    PDRIVER_OBJECT  DriverObject	Pointer to the driver's driver object
* \return   void						
*
* \~French
* \brief    Décharge le driver
* \details  Décharge le driver, en supprimant et réinitialisant les structures 
*			appropriées. La fonction est appelée par le gestionnaire PnP quand il
*			veut déconnecter le clavier, typiquement lors de l'extinction de la 
*			machine.
*
* \param    PDRIVER_OBJECT  DriverObject	Pointer vers l'objet driver du driver
* \return   void	
*
*/
void GostxBoard_EvtUnload (
	_In_ WDFDRIVER DriverObject 
)
{

	UNREFERENCED_PARAMETER( DriverObject );

	#ifdef _DEBUG 
		DbgPrint("[+] GostxBoard_Unload Entry \n");
	#endif

	//
	// Free the allocated memory pool for the cipher
	//
	ExFreePoolWithTag(pCurrentApp, 'hpiC');

	//
	// Delete the process notify callback
	//
	PsSetCreateProcessNotifyRoutine(ProcessNotifyRoutine,TRUE);

	#ifdef _DEBUG 
		DbgPrint("[+] GostxBoard_Unload Done \n");
	#endif

	return ;
}





/**
* \~English
* \brief    Configuration routine of driver object
* \details  This function configures the driver object created by DriverEntry.\n
*			It initialized the driver as a filter driver of the keyboard driver 
*			stack and registers some callbacks needed for the good functionning 
*			of the protection module : IOCTL and IRP_MJ_READ. \n The function calls CreateRawPdo in order 
*			to create the raw pdo needed for the communication beetwen the client 
*			application and the driver.\n The routine is also responsible of 
*			initializing structures needed for the cipher. 
*
* \param    WDFDRIVER		DriverObject	Driver's driver object
* \param  PWDFDEVICE_INIT	DeviceInit		Pointer to an opaque 
*											framework structure used to initiate the driver object
*
* \~French
* \brief    Configure l'objet driver
* \details  La fonction configure l'objet driver créé par la fonction DriverEntry. \n
*			Elle initialise le driver comme driver filtre de la pile des drivers claviers
*			et enregistre quelques callbacks nécessaires au bon fonctionnement du module 
*			de protection : IOCTL et IPR_MJ_READ. \n La fonction appelle CreateRawPdo afin
*			de créer un objet raw PDO, nécessaire à la communication entre l'application 
*			cliente et le driver.\n La routine est aussi responsable de l'initialisation des
*			structures nécessaires au chiffrement.
*			
* \param    WDFDRIVER		DriverObject	Pointeur vers l'objet driver du driver
* \param   PWDFDEVICE_INIT	DeviceInit		Pointeur vers uune structure opaque du framework 
*											utilisé pour initialisé l'objet driver
*
*/
NTSTATUS GostxBoard_EvtDeviceAdd (
	_In_ WDFDRIVER		DriverObject,
	_In_ PWDFDEVICE_INIT	DeviceInit
	)
{
	NTSTATUS						ntStatus					= STATUS_SUCCESS;
	PDEVICE_EXTENSION				pDeviceExtension			= NULL;
	WDF_OBJECT_ATTRIBUTES			deviceAttributes			= {0};
	WDFDEVICE						wdfDevice					= {0};
	WDF_IO_QUEUE_CONFIG				ioQueueConfig				= {0};	
	WDFQUEUE						hQueue						= {0};

#ifdef _DEPRECIATED
	WDFQUEUE						DispatchQueue				= {0};
#endif // depreciated


	#ifdef _DEBUG 
		DbgPrint("[+] GostxBoard_Init::IoCreateDevice : Entry \n");
	#endif

	UNREFERENCED_PARAMETER( DriverObject );

	//
	// Tell the framework we are a filter driver. Flags and characteristics
	// will be automatically herited from abelow drivers.
	//
	WdfFdoInitSetFilter( DeviceInit );

	//
	// Tell the framework that we want to be in the keyboard driver stack 
	//
	WdfDeviceInitSetDeviceType(DeviceInit, FILE_DEVICE_KEYBOARD);

	//
	// Initialize device's attributes
	//
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&deviceAttributes, DEVICE_EXTENSION);

	//
	// Ensure that every EvtIoXXX callbacks are not running in parallel which might 
	// cause synchronisation issues. So, tell the framework to use a mutex to implement
	// synchronisation. 
	// NB - Running at these IRQL leads the Windows parser to crash when handeling IRPs, 
	//	  - Some tests have to be done here..	
	//
	//deviceAttributes.SynchronizationScope = WdfSynchronizationScopeDevice;
	//deviceAttributes.ExecutionLevel = WdfExecutionLevelPassive;

	//
	// On créé l'objet périphérique via le framework. L'objet va être créé selon
	// les spécifications WDM et être attaché à la pile spécifié ci-dessus. 
	//
	ntStatus = WdfDeviceCreate( &DeviceInit, &deviceAttributes, &wdfDevice );
	if ( !NT_SUCCESS( ntStatus ) ) 
	{
		DbgPrint("[x] GostxBoard_GostxBoard_EvtDeviceAdd::WdfDeviceCreate failed with status code 0x%x\n", ntStatus);
		return ntStatus;
	}

	//
	// Get device extension of the object previously created
	//
	pDeviceExtension = FilterGetData(wdfDevice);

	//
	// Configuration an default queue, with parallel treatement in order to avoid deadlocks,
	// Activate default queue for internal IOCTL
	//
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE( &ioQueueConfig, 
								WdfIoQueueDispatchParallel );
	 ioQueueConfig.EvtIoInternalDeviceControl = GostxBoard_EvtIoInternalDeviceControl;
	 ntStatus = WdfIoQueueCreate(wdfDevice,
                            &ioQueueConfig,
                            WDF_NO_OBJECT_ATTRIBUTES,
                            WDF_NO_HANDLE // pointer to default queue
                            );
    if (!NT_SUCCESS(ntStatus)) {
        DbgPrint("[x] GostxBoard_GostxBoard_EvtDeviceAdd::WdfIoQueueCreate failed 0x%x\n", ntStatus);
        return ntStatus;
    }

	//
	// Configure another queue for handling IOCTL from child object
	//
	WDF_IO_QUEUE_CONFIG_INIT( &ioQueueConfig, 
						WdfIoQueueDispatchParallel);

	ioQueueConfig.EvtIoDeviceControl = GostxBoard_EvtIoDeviceControlFromRawPdo;

	ntStatus = WdfIoQueueCreate(wdfDevice,
                            &ioQueueConfig,
                            WDF_NO_OBJECT_ATTRIBUTES,
                            &hQueue
                            );
     if (!NT_SUCCESS(ntStatus)) {
        DbgPrint("[x] GostxBoard_GostxBoard_EvtDeviceAdd::WdfIoQueueCreate2 failed 0x%x\n", ntStatus);
        return ntStatus;
    }


#ifdef _DEPRECIATED
	 //
	 // Configure non Default Queue for IRP_MJ_READ, which will be used to register 
	 // a completion read routine allowing the driver to intercepts scancode from 
	 // the keyboard
	 //
	 WDF_IO_QUEUE_CONFIG_INIT(&ioQueueConfig,
		 WdfIoQueueDispatchSequential);

	 ioQueueConfig.EvtIoRead = GostxBoard_EvtIoRead;

	 ntStatus = WdfIoQueueCreate(wdfDevice,
		 &ioQueueConfig,
		 WDF_NO_OBJECT_ATTRIBUTES,
		 &DispatchQueue
		 );

	 if (!NT_SUCCESS(ntStatus))
	 {
		#ifdef _DEBUG
			 DbgPrint("[-] GostxBoard_CreateRawPdo::WDFIoQueueCreate3 failed with status 0x%x \n", ntStatus);
		#endif		  
			return ntStatus;
	 }

	 ntStatus = WdfDeviceConfigureRequestDispatching(
		 wdfDevice,
		 DispatchQueue,
		 WdfRequestTypeRead);
	 if (!NT_SUCCESS(ntStatus))
	 {
		#ifdef _DEBUG
			 DbgPrint("[-] GostxBoard_CreateRawPdo::WDFDeviceConfigureRequest failed with status 0x%x \n", ntStatus);
		#endif		  
		 return ntStatus;
	 } 	
	 
	 pDeviceExtension->rawDispatchPdoQueue = DispatchQueue;
#endif	  // Depreciated

	pDeviceExtension->rawPdoQueue = hQueue;


	//
	// Create a raw pado which will be available for a user mode 
	// application. The communication driver-app will be done over 
	// this newly created object. The client application will sent 
	// ioctl to this raw pdo, which will forward the ioctl to driver's
	// object for processing.
	//
	ntStatus = GostxBoard_CreateRawPdo( wdfDevice, InstanceNo );


	//
	// Allocate structure for cipher_list, used to store the MasterKey and
	// the client application's credentials
	//
	#ifdef _DEBUG 
		DbgPrint("[+] GostxBoard_AddDevice::Initializing CipherList \n");
	#endif

	pCurrentApp = (PAPP_CREDENTIALS) ExAllocatePoolWithTag( NonPagedPool,
		sizeof( APP_CREDENTIALS ), 'hpiC');
	if (pCurrentApp == NULL)
	{
		DbgPrint("[x] GostxBoard_AddDevice::Unable to allocate in non-paged memory. Trying in paged.. \n");
		pCurrentApp = (PAPP_CREDENTIALS)ExAllocatePoolWithTag(PagedPool,
			sizeof(APP_CREDENTIALS), 'hpiC');
		if (pCurrentApp == NULL)
		{
			DbgPrint("[x] GostxBoard_AddDevice::Unable to allocate any memory.. \n");
			ntStatus = STATUS_INSUFFICIENT_RESOURCES;
			goto end;
		}
	}

	RtlSecureZeroMemory( pCurrentApp , sizeof( APP_CREDENTIALS ) );
	pCurrentApp->pEprocess = NULL;


	//
	// Inits for security checks
	//
	pCurrentApp->wdfCurrentDevice = wdfDevice;
	RtlInitUnicodeString(&(pCurrentApp->DriverName), L"\\Driver\\GostProtect");
	RtlInitUnicodeString(&(pCurrentApp->i8042Name), L"\\Driver\\i8042prt");
	RtlInitUnicodeString(&(pCurrentApp->apciName), L"\\Driver\\ACPI");	 

	//
	// Checks the driver security context 
	// and try to fix it if some errors are detected 
	// (eg registry)
	//
	CheckDriverSecurityContext(); 


	//
	// Initialization of the MasterKey. This key is a strong random 64bits key, 
	// used to generate the app credentials during the living of the driver.
	//
	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_AddDevice::Initializing MasterKey\n");
	#endif
	pCurrentApp->MasterKey = InitMasterKey();


	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_AddDevice::Registering callback routine\n");
	#endif

	//
	// Initiates the process notify callback. Each time a process is created or is dying 
	// this function will be called. It allows the driver to know if the client application 
	// is still alive, and stop the ciphering if not.
	//
	if( PsSetCreateProcessNotifyRoutine( ProcessNotifyRoutine, FALSE )
		== STATUS_INVALID_PARAMETER )
	{
		//
		// We do not permit to continue whithout this callback, it is too 
		// dangerous. It can hences the impossibility o using the keyboard
		//
		ntStatus = STATUS_UNSUCCESSFUL;
	}


	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_AddDevice:: Exit \n");
	if ( NT_SUCCESS(ntStatus) )
	{
		DbgPrint(" ___________________________________________\n");
		DbgPrint("| Driver is started. Everything seems fine. |\n");
		DbgPrint("|___________________________________________|\n\n\n");
	}
	else
	{
		DbgPrint(" _______________________________________________\n");
		DbgPrint("| We have so problems.. Check previous messages |\n");
		DbgPrint("|\t\t to check what's wrong\t |\n");
		DbgPrint("|_______________________________________________|\n\n\n");
	}

	#endif
	end:
	return ntStatus;
}


/**
* \~English
* \brief    Create a raw pdo
* \details  The function creates a raw pdo used by a client application to 
*			communicate with the driver and starts 	a cipher session.\n The raw
*			PDO has its own driver stack and forwards every request to its parent, 
*			the FDO of the driver, so that it can process them.
*
* \param    WDFDRIVER	DriverObject	Driver's driver object
* \param   ULONG		InstanceNo		Instance of the keyboard created
*
* \~French
* \brief    Créé un raw pdo
* \details  La fonction créée un raw pdo utilisé par une application cliente 
*			pour communiquer avec le driver et démarrer une session de chiffrement. \n
*			Le raw pdo a sa propre pile de driver et transmet chaque requête à son parent, 
*			le fdo du driver, pour que celui-ci puisse les traiter.
*
* \param    WDFDRIVER		DriverObject	Pointeur vers l'objet driver du driver
* \param   ULONG			InstanceNo		Instance du clavier créé
*
*/
NTSTATUS GostxBoard_CreateRawPdo(
    _In_ WDFDEVICE       wdfDevice,
    _In_ ULONG           InstanceNo
    )
{	
	DECLARE_CONST_UNICODE_STRING(deviceId,KBFILTR_DEVICE_ID );
	DECLARE_UNICODE_STRING_SIZE(buffer, MAX_ID_LEN);
	DECLARE_CONST_UNICODE_STRING(deviceLocation,L"GostxBoard\0" );

	NTSTATUS					ntStatus				= STATUS_SUCCESS;
	PWDFDEVICE_INIT				pDeviceInit				= NULL;
	WDF_OBJECT_ATTRIBUTES       pdoAttributes			= {0};
	WDFDEVICE					wdfChild				= NULL;
	PRPDO_DEVICE_DATA			rpdoDevData				= NULL;
	PDEVICE_EXTENSION			pDeviceExtension		= NULL;
	WDF_IO_QUEUE_CONFIG         ioQueueConfig			= {0};
	WDFQUEUE					queue					= NULL;						
	WDF_DEVICE_PNP_CAPABILITIES pnpDeviceCap			= {0};
	WDF_DEVICE_STATE			deviceState				= {0};

#ifdef _DEPRECIATED
	WDFQUEUE					DispatchQueue			= NULL;
#endif // depreciated


	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_CreateRawPdo:: Entry \n");
	#endif

	//
	// Allocate a WDF initialization structure to create the object 
	// and set its properties
	//
	pDeviceInit = WdfPdoInitAllocate( wdfDevice );
	if( pDeviceInit == NULL )
	{
		//
		// There are not enought resources to create the object, we rise an error
		//
	#ifdef _DEBUG
		DbgPrint("[-] GostxBoard_CreateRawPdo::WdfPdoInitAllocate failed due to low resources \n");
	#endif
		ntStatus = STATUS_INSUFFICIENT_RESOURCES;
		goto Error;
	}

	//
	// Indicate the device can be started and accessed whithout
	// calling any function driver (which have not), by telling the 
	// framework this is a raw device
	//
	ntStatus = WdfPdoInitAssignRawDevice(pDeviceInit, &GUID_DEVCLASS_KEYBOARD);
    if ( !NT_SUCCESS(ntStatus) ) 
	{
		#ifdef _DEBUG
			DbgPrint("[-] GostxBoard_CreateRawPdo::WdfPdoInitAllocate failed with status 0x%x \n", ntStatus);
		#endif
        goto Error;
    }


	//
	// Assignate an identifier to the object
	//
	ntStatus = WdfPdoInitAssignDeviceID(pDeviceInit, &deviceId);
    if ( !NT_SUCCESS(ntStatus) ) {
		#ifdef _DEBUG
			DbgPrint("[-] GostxBoard_CreateRawPdo::AssignDeviceID failed with status 0x%x \n", ntStatus);
		#endif
        goto Error;
    }

	//
	// The device must be installer on every instances of the keyboard (eg usb / ps2).
	// So we specify a BusQueryInstanceID to avoid the OS to rise errors.
	//
	ntStatus = RtlUnicodeStringPrintf( &buffer, L"%02d", InstanceNo );
	if( !NT_SUCCESS(ntStatus) )
	{
		#ifdef _DEBUG
			DbgPrint("[-] GostxBoard_CreateRawPdo::RtlPrintUnicode failed with status 0x%x \n", ntStatus);
		#endif
		goto Error;
	}

	ntStatus = WdfPdoInitAssignInstanceID(pDeviceInit, &buffer);
    if (!NT_SUCCESS(ntStatus) ) 
	{
		#ifdef _DEBUG
			DbgPrint("[-] GostxBoard_CreateRawPdo::AssignInstanceID failed with status 0x%x \n", ntStatus);
		#endif
        goto Error;
    }

	//
	// Initialize device's description since this is not a physical device 
	// and thus there are not INF file. This description is available for the
	// device manager
	//
	ntStatus = RtlUnicodeStringPrintf( &buffer, L"Keyboard_Filter_%02d", InstanceNo );
	if( !NT_SUCCESS( ntStatus ) )
	{
		#ifdef _DEBUG
			DbgPrint("[-] GostxBoard_CreateRawPdo::RtlPrintUnicode2 failed with status 0x%x \n", ntStatus);
		#endif
		goto Error;
	}


	//
	// Initiate device's description describing the device. The framework 
	// saves it and transmits it to the PnPManager when receiving 
	// IRP_MN_QUERY_DEVICE_TEXT
	//
	ntStatus = WdfPdoInitAddDeviceText(pDeviceInit,
                                        &buffer,
                                        &deviceLocation,
                                        0x409
                                        );
    if ( !NT_SUCCESS(ntStatus) ) {
		#ifdef _DEBUG
			DbgPrint("[-] GostxBoard_CreateRawPdo::AddDeviceText failed with status 0x%x \n", ntStatus);
		#endif
        goto Error;
    }
	WdfPdoInitSetDefaultLocale(pDeviceInit, 0x409);

	//
	// Initiate the attributs which specify the size of the device's extension. 
	// It will contain every piece of information of the PDO. As WDM, this is 
	// the device's extension. The framework allocates space in non paged memory
	// and sets it to zero. This section is only accessible for the composante that
	// creates it. The context declaration is done in defines.h, with the
	// WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(RPDO_DEVICE_DATA, PdoGetData) macro
	//
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&pdoAttributes, RPDO_DEVICE_DATA);

	//
	// Authorize forwarding of I/O requests to aprent's device, so the fdo 
	// can process pdo's requests
	//
	WdfPdoInitAllowForwardingRequestToParent(pDeviceInit);

	//
	// Create the kmdf device with previsous parameters
	//
    ntStatus = WdfDeviceCreate(&pDeviceInit, &pdoAttributes, &wdfChild);
	if( !NT_SUCCESS(ntStatus) )
	{
		#ifdef _DEBUG
			DbgPrint("[-] GostxBoard_CreateRawPdo::WDFCreateDevice failed with status 0x%x \n", ntStatus);
		#endif
		goto Error;
	}

	//
	// Get the context with PdoGetData routine, registered with the above 
	// WDF_Object_Attributes_Init_Context_Type  routine
	//
	rpdoDevData = PdoGetData( wdfChild );
	rpdoDevData->InstanceNo = InstanceNo;

	//
	// Associate the parent with the child to they can talks to each others
	//
	pDeviceExtension = FilterGetData( wdfDevice );
	rpdoDevData->ParentQueue = pDeviceExtension->rawPdoQueue;

	//
	// Configure defaults queue to be serialized, so that we will received only serialized 
	// requests in the device control routine. Default queue receives, by default, every IRP 
	// which are not configured to be transmit to other routines. 
	// Followings are handled : 
	//			- IOCTL
	//			- IRP_MJ_CREATE	[Depreciated]
	//
	// QUeues have to be separatly created, because IRP_MJ_CREATE corresponding to EvtIoRead, 
	// is not a default queue and the ioctls are.
	//

	  //
	  // Default Queue
	  //
	  WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQueueConfig,
                                    WdfIoQueueDispatchSequential);

	  ioQueueConfig.EvtIoDeviceControl  = GostxBoard_EvtIoDeviceControlForRawPdo;

	  ntStatus = WdfIoQueueCreate(wdfChild,
                                &ioQueueConfig,
                                WDF_NO_OBJECT_ATTRIBUTES,
                                &queue 
                   );

	  if( !NT_SUCCESS(ntStatus) )
	  {
		#ifdef _DEBUG
			DbgPrint("[-] GostxBoard_CreateRawPdo::WDFIoQueueCreate failed with status 0x%x \n", ntStatus);
		#endif		  
		  goto Error;
	  }

#ifdef _DEPRECIATED
	  //
	  // Non defauts queues
	  //
	  WDF_IO_QUEUE_CONFIG_INIT(&ioQueueConfig,
		  WdfIoQueueDispatchSequential);

	  ioQueueConfig.EvtIoRead = GostxBoard_EvtIoRead;

	  ntStatus = WdfIoQueueCreate(wdfChild,
		  &ioQueueConfig,
		  WDF_NO_OBJECT_ATTRIBUTES,
		  &DispatchQueue
		  );

	  if (!NT_SUCCESS(ntStatus))
	  {
		#ifdef _DEBUG
		  DbgPrint("[-] GostxBoard_CreateRawPdo::WDFIoQueueCreate1 failed with status 0x%x \n", ntStatus);
		#endif		  
		  goto Error;
	  }

	  ntStatus = WdfDeviceConfigureRequestDispatching(
		  wdfChild,
		  DispatchQueue,
		  WdfRequestTypeRead);
	  if (!NT_SUCCESS(ntStatus))
	  {
			#ifdef _DEBUG
				 DbgPrint("[-] GostxBoard_CreateRawPdo::WDFDeviceConfigureRequest failed with status 0x%x \n", ntStatus);
			#endif		  
		  goto Error;
	  }

#endif // depreciated

	//
	// Initiate device properties. Parameters are explicites
	//
	WDF_DEVICE_PNP_CAPABILITIES_INIT(&pnpDeviceCap);
	pnpDeviceCap.Removable			= WdfTrue;
	pnpDeviceCap.SurpriseRemovalOK	= WdfTrue;
	pnpDeviceCap.NoDisplayInUI		= WdfTrue;
	pnpDeviceCap.Address			= InstanceNo;
	pnpDeviceCap.UINumber			= InstanceNo;
	WdfDeviceSetPnpCapabilities(wdfChild, &pnpDeviceCap);


	//
	// Try to hide the device for the using by setting the non display ui flag.
	// Anyway, if "Display hidden devices" is set in the device manager, it will 
	// be visible. 
	//
	WDF_DEVICE_STATE_INIT(&deviceState);
	deviceState.DontDisplayInUI = WdfTrue;
	WdfDeviceSetDeviceState(wdfChild, &deviceState);

	//
	// Indicate that we want an interface to allow an application 
	// to communicates with our device
	//
	ntStatus = WdfDeviceCreateDeviceInterface(wdfChild, 
								&GUID_DEVINTERFACE_GOSTXBOARD,
								NULL
				);
	if( !NT_SUCCESS(ntStatus) )
	{
		#ifdef _DEBUG
			DbgPrint("[-] GostxBoard_CreateRawPdo::WDFCreateDeviceInterface failed with status 0x%x \n", ntStatus);
		#endif	
		goto Error;
	}


	//
	// Add the newly created fdo to the fdo parent. 
	// Carefull ! : The device must not be directly deleted with WdfObjectDelete. 
	//	       WdfPdoMarkMissing must be called before.
	//
	ntStatus = WdfFdoAddStaticChild( wdfDevice, wdfChild );
	if( !NT_SUCCESS(ntStatus) )
	{
		#ifdef _DEBUG
			DbgPrint("[-] GostxBoard_CreateRawPdo::WDFAddStaticChild failed with status 0x%x \n", ntStatus);
		#endif	
		goto Error;
	}


	#ifdef _DEBUG
		DbgPrint("[+] GostxBoard_CreateRawPdo:: Exit with successfull status \n");
	#endif


	return STATUS_SUCCESS;

Error:
	#ifdef _DEBUG
		DbgPrint("\n[x] GostxBoard_GostxBoard_CreateRawPdo::A fatal error occurred while trying to create PDO. Check previous error for further information.\n", ntStatus);
		DbgPrint("[x] GostxBoard_GostxBoard_CreateRawPdo:: Exit with UNSUCCESSFULL status - Previsous error code 0x%x\n", ntStatus);
	#endif

	if (pDeviceInit != NULL) {
        WdfDeviceInitFree(pDeviceInit);
    }
	if(wdfChild) {
        WdfObjectDelete(wdfChild);
    }


	return ntStatus;
}

