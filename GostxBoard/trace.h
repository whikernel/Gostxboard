//**********************************************************************//
// 					This file is part of GostxBoard						//
//																		//
//  GostxBoard is free software: you can redistribute it and/or modify	//
//  it under the terms of the GNU General Public License as published	// 
//	by the Free Software Foundation, either version 3 of the License, 	//
//	or (at your option) any later version. 								//
//                Such things are STRONGLY recommended.					//
//    																	//
// 	GostxBoard is distributed in the hope that it will be useful,		//
//  but WITHOUT ANY WARRANTY; without even the implied warranty of		//	
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the		//
//  GNU General Public License for more details.						//	
//  Do not blame the developper if something goes wrong. You can 		//	
//  either search on the Internet or email the developper to deal 		//
//  with it.															//
//																		//
//  You should have received a copy of the GNU General Public License 	//
//  along with GostxBoard.  If not, see <http://www.gnu.org/licenses/>. //
//																		//
//                                P.A 									//
//                    driver_support@whitekernel.fr 					//
//**********************************************************************//

/**
* \~English
* \file      trace.h
* \author    P.A
* \version   1.0
* \date      May 21, 2015
* \brief     Define some usefull defines for WPP Tracing
*
* \details   Define some usefull defines for WPP Tracing
*
*
* \~French
* \file		trace.h
* \author	P.A
* \version  1.0
* \date		21 Mai 2015
* \brief	Définit des defines utile pour le WPP tracing
*
* \details	Définit des defines utile pour le WPP tracing
*/

#ifndef _TRACE_H
#define _TRACE_H

#define WPP_CONTROL_GUIDS \
	WPP_DEFINE_CONTROL_GUID(GOSTXBOARD_GUID,(8347D90B, E7BC, 4389, 9D03, 5515071CB41E), \
		WPP_DEFINE_BIT(INITIALIZATION) \
		WPP_DEFINE_BIT(UNLOADING) \
		WPP_DEFINE_BIT(IOTCL) \
		WPP_DEFINE_BIT(FILTRE_DRV) )


#define WPP_LEVEL_FLAGS_LOGGER(level,flags)		\
		WPP_LEVEL_LOGGER(flags)

#define WPP_LEVEL_FLAGS_ENABLED(level,flags)	\
	(WPP_LEVEL_ENABLED(flags) && WPP_CONTROL(WPP_BIT_ ## flags).Level >= level )
 

#endif