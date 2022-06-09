
// MDX2500.h

#pragma once

#define MDX_IsReady								"RX,Ready"											// Response : RX,OK,A[CR] (A is 0,1,2) 
#define MDX_READY_ON							"0"
#define MDX_READY_ERROR							"1"
#define MDX_READY_ONWORKING						"2"
#define MDX_RX_OK								"RX,OK"

#define MDX_StartMarking						"WX,StartMarking"
#define MDX_GuideLaser							"WX,GuideLaser"
#define MDX_GuideLaserOneTime					"WX,GuideLaser=1"
#define MDX_GuideLaserContinue					"WX,GuideLaser=2"
#define MDX_GuideLaserArea						"WX,GuideLaser=3"
#define MDX_GuideLaserWorkImg					"WX,GuideLaser=4"
#define MDX_GuideLaserRect						"WX,GuideLaser=5"
#define MDX_StopMarking							"WX,StopMarking"
#define MDX_WX_OK								"WX,OK"

#define MDX_SetBlockType						"WX,JOB=%04d,BLK=%03d,BlockType=%03d"				// Response : RX,OK[CR]
#define MDX_GetBlockType						"WX,JOB=%04d,BLK=%03d,BlockType"					// Response : RX,OK,A[CR] (A is 3bytes.)
#define MDX_SetCodeType							"WX,JOB=%04d,BLK=%03d,CodeType=%02d"				// Response : RX,OK[CR]
#define MDX_GetCodeType							"WX,JOB=%04d,BLK=%03d,CodeType"						// Response : RX,OK,A[CR] (A is 2bytes.)
#define MDX_SetCharacterString					"WX,JOB=%04d,BLK=%03d,CharacterString=%s"			// JOB=0000~1999 , BLK=000~255
#define MDX_GetCharacterString					"RX,JOB=%04d,BLK=%03d,CharacterString"				// Response : RX,OK,A[CR] (A is String.)

/*===============================================================================================================================================================================================================================
Laser engrave position setting config

X_org			: -062.500 ~ 0062.500 [mm] , Default 0000.000
Y_org			: -062.500 ~ 0062.500 [mm] , Default 0000.000
X_offset		: -062.500 ~ 0062.500 [mm] , Default 0000.000
Y_offset		: -125.000 ~ 0125.500 [mm] , Default 0000.000
Theta_offset	: -180.000 ~ 0180.000 [˚]  , Default 0000.000
=================================================================================================================================================================================================================================*/
#define MDX_SetJobPosition						"WX,JOB=%04d,JobPosition=%s,%s,%s,%s,%s"			// Response : WX,OK (X_org,Y_org,X_offset,Y_offset,Theta_offset is 8bytes.)
#define MDX_GetJobPosition						"RX,JOB=%04d,JobPosition"							// Response : WX,OK,X_org,Y_org,X_offset,Y_offset,Theta_offset[CR] (X_org,Y_org,X_offset,Y_offset,Theta_offset is 8bytes.)
//===============================================================================================================================================================================================================================

#define MDX_GetWorkflowResultDetail				"RX,WorkflowResultDetail"							// Response : RX,OK,A,B,C,D,E,F,G,H,I,J,K,L,M
#define MDX_GetMarkingResult					"RX,MarkingResult"									// Response : RX,OK,A,B (A is 0:failed, 1:success. B is mSecond time : 12bytes)

#define MDX_GetOperatingTime					"RX,OperatingTime"									// Response : RX,OK,A[CR] (A is 00000~99999 : 누적 가동 시간[h])
#define MDX_GetLaserOperatingTime				"RX,LaserOperatingTime"								// Response : RX,OK,A[CR] (A is 00000~99999 : 레이저 여기 시간[h])
#define MDX_GetScannerOperatingTime				"RX,ScannerOperatingTime"							// Response : RX,OK,A[CR] (A is 00000~99999 : 스캐너 가동 시간[h])

#define MDX_GetMarkingUnitTemperature			"RX,MarkingUnitTemperature"							// Response : RX,OK,A[CR] (A is -999.9~0999.9 : 헤드 온도[degree])
#define MDX_GetControllerTemperature			"RX,ControllerTemperature"							// Response : RX,OK,A[CR] (A is -999.9~0999.9 : 컨트롤러 온도[degree])
#define MDX_GetLaserPowerCalibrationResult		"RX,LaserPowerCalibrationResult"					// Response : RX,OK,A,B[CR] (A is 캘리브레이션 실시유무 [0: 없음, 1: 있음].  B is 캘리브레이션 결과[000.00~999.99 : W].)

typedef enum stMdxCmd { MdxNone=0, MdxIsReady, READY_ON, READY_ERROR, READY_ONWORKING, StartMarking, GuideLaser,
						GuideLaserOneTime, GuideLaserContinue, GuideLaserArea, GuideLaserWorkImg, GuideLaserRect,
						StopMarking, WX_OK, SetBlockType, GetBlockType, SetCodeType, GetCodeType, SetCharacterString,
						GetCharacterString, SetJobPosition, GetJobPosition, GetWorkflowResultDetail, GetMarkingResult, 
						GetOperatingTime, GetLaserOperatingTime, GetScannerOperatingTime, GetMarkingUnitTemperature,
						GetControllerTemperature, GetLaserPowerCalibrationResult } MdxCmd;

// Command+CR
#ifndef CR
#define CR									0x0D			// Carriage Return
#endif
//===============================================================================










