// EngraveDef.h

#pragma once

typedef enum _stPcId {
	_Engrave = 0, _AoiUp, _AoiDn, _Punch, _Vs,
	_IsEngrave, _IsAoiUp, _IsAoiDn, _IsPunch, _IsVs,
}_PcId;

typedef enum _stCmdCode {
	_None_ = 0, _GetSig, _SetSig, _GetData, _SetData, _Rtn, _Status,
	_IsNone_, _IsGetSig, _IsSetSig, _IsGetData, _IsSetData, _IsRtn, _IsStatus,
}_CmdCode;

typedef enum _stSigInx {
	// Normal
	_Idle = 0, _Busy, _Connect,
	// Display
	_DispDualSample, _DispDualTest, _DispSingleSample, _DispSingleTest, _DispInitRun, _DispRun, _DispStop, _DispReady,
	// Dlg Info 
	_DualTest, _SampleTest, _TestMode, _RecoilerCcw, _UncoilerCcw, _AlignMethode,
	_DoorRecoiler, _DoorAoiUp, _DoorAoiDn, _DoorMk, _DoorEngrave, _DoorUncoiler, _SaftyMk, _CleannerAoiUp, _CleannerAoiDn, 
	_UltraSonicAoiDn, _UltraSonicEngrave, _FixDef,
	// Menu01
	_TempStop, _TempPause, _LotCut, _MkPosMv, _MkVerify, _ReelmapUp, _ReelmapDn, _ReelmapTot,
	_RemainMode, _EndWork, _ReMk, _2Layer, _1LnMk, _2LnMk, _3LnMk, _4LnMk, _TotLnMk,
	// Menu02
	_2DEngLenMvCw, _2DEngLenMvCcw, _2DAoiLenMvCw, _2DAoiLenMvCcw, _2DMkLenMvCw, _2DMkLenMvCcw, _2DOneShotLenCw, _2DOneShotLenCcw,	
	// Menu03
	_Ready, _Run, _Reset, _Stop, _Auto, _OneCycle, _Manual, _MkTq, _AoiTq, _EngTq, _CcwModRe, _CcwModUn, _Core150Re, _Core150Un, 
	_Relation, _JoinSelRe, _MvCwRe, _MvCcwRe, _PrdChuckRe, _DancerUpRe, _PasteUpLfRe, _PasteUpRtRe, _PasteVacRe, _PprChuckRe,
	_PprCwRe, _PprCcwRe, _DoRe, _PrdPprRe,
	_JoinSelMk, _MvCwMk, _MvCcwMk, _FdVacMk, _PushUpMk, _TblBlwMk, _TblVacMk, _FdClampMk, _TensClampMk, _OnePnlMk, _LsrMk,
	_DancerUpMk, _TqVacMk,
	_JoinSelAoiDn, _MvCwAoiDn, _MvCcwAoiDn, _FdVacAoiDn, _PushUpAoiDn, _TblBlwAoiDn, _TblVacAoiDn, _FdClampAoiDn, _TensClampAoiDn, 
	_OnePnlAoiDn, _LsrAoiDn, _ClrRollAoiDn, _VelClrSonicAoiDn,
	_TestAoiDn, _ResetAoiDn, _LotEndAoiDn,
	_JoinSelAoiUp, _MvCwAoiUp, _MvCcwAoiUp, _FdVacAoiUp, _PushUpAoiUp, _TblBlwAoiUp, _TblVacAoiUp, _FdClampAoiUp, _TensClampAoiUp,
	_OnePnlAoiUp, _LsrAoiUp, _ClrRollAoiUp,
	_TestAoiUp, _ResetAoiUp, _LotEndAoiUp,
	_JoinSelEng, _MvCwEng, _MvCcwEng, _FdVacEng, _PushUpEng, _TblBlwEng, _TblVacEng, _FdClampEng, _TensClampEng, _OnePnlEng, _LsrEng,
	_DancerUpEng, _VelClrSonicEng,
	_JoinSelUn, _MvCwUn, _MvCcwUn, _PrdChuckUn, _DancerUpUn, _PasteUpLfUn, _PasteUpRtUn, _PasteVacUn, _PprChuckUn,
	_PprCwUn, _PprCcwUn, _ClrRollUpUn, _ClrRollPushUn,
	_EmgAoi,
	// Menu04
	_MkResetLf, _MkResetRt, _MkBuffJogCw, _MkBuffJogCcw, _MkBuffOrgMv, _MkBuffInitPosMv, _MkBuffPosSave,
	// Menu04 (Engrave)
	_EngBuffJogCw, _EngBuffJogCcw, _EngBuffOrgMv, _EngBuffOrgMvDone, _EngBuffInitPosMv, _EngBuffInitPosMvDone, _EngBuffInitPosSave,
	// EngraveAutoSequence
	_EngAutoInit, _EngAutoSeqMkSt, _EngAutoSeqOnMkIng, _EngAutoSeqMkDone, _EngAutoSeq2dReadSt, _EngAutoSeqOnReading2d, _EngAutoSeq2dReadDone,
	// DlgMyMsg
	_MyMsgYes, _MyMsgNo, _MyMsgCancel, _MyMsgOk,
	// Is Normal
	_IsIdle, _IsBusy, _IsConnect,
	// Is Display
	_IsDispDualSample, _IsDispDualTest, _IsDispSingleSample, _IsDispSingleTest, _IsDispInitRun, _IsDispRun, _IsDispStop, _IsDispReady,
	// Is Dlg Info 
	_IsDualTest, _IsSampleTest, _IsTestMode, _IsRecoilerCcw, _IsUncoilerCcw, _IsAlignMethode,
	_IsDoorRecoiler, _IsDoorAoiUp, _IsDoorAoiDn, _IsDoorMk, _IsDoorEngrave, _IsDoorUncoiler, _IsSaftyMk, _IsCleannerAoiUp, _IsCleannerAoiDn,
	_IsUltraSonicAoiDn, _IsUltraSonicEngrave, _IsFixDef,
	// Is Menu01
	_IsTempStop, _IsTempPause, _IsLotCut, _IsMkPosMv, _IsMkVerify, _IsReelmapUp, _IsReelmapDn, _IsReelmapTot,
	_IsRemainMode, _IsEndWork, _IsReMk, _Is2Layer, _Is1LnMk, _Is2LnMk, _Is3LnMk, _Is4LnMk, _IsTotLnMk,
	// Is Menu02
	_Is2DEngLenMvCw, _Is2DEngLenMvCcw, _Is2DAoiLenMvCw, _Is2DAoiLenMvCcw, _Is2DMkLenMvCw, _Is2DMkLenMvCcw, _Is2DOneShotLenCw, _Is2DOneShotLenCcw,
	// Is Menu03
	_IsReady, _IsRun, _IsReset, _IsStop, _IsAuto, _IsOneCycle, _IsManual, _IsMkTq, _IsAoiTq, _IsEngTq, _IsCcwModRe, _IsCcwModUn, _IsCore150Re, _IsCore150Un,
	_IsRelation, _IsJoinSelRe, _IsMvCwRe, _IsMvCcwRe, _IsPrdChuckRe, _IsDancerUpRe, _IsPasteUpLfRe, _IsPasteUpRtRe, _IsPasteVacRe, _IsPprChuckRe,
	_IsPprCwRe, _IsPprCcwRe, _IsDoRe, _IsPrdPprRe,
	_IsJoinSelMk, _IsMvCwMk, _IsMvCcwMk, _IsFdVacMk, _IsPushUpMk, _IsTblBlwMk, _IsTblVacMk, _IsFdClampMk, _IsTensClampMk, _IsOnePnlMk, _IsLsrMk,
	_IsDancerUpMk, _IsTqVacMk,
	_IsJoinSelAoiDn, _IsMvCwAoiDn, _IsMvCcwAoiDn, _IsFdVacAoiDn, _IsPushUpAoiDn, _IsTblBlwAoiDn, _IsTblVacAoiDn, _IsFdClampAoiDn, _IsTensClampAoiDn,
	_IsOnePnlAoiDn, _IsLsrAoiDn, _IsClrRollAoiDn, _IsVelClrSonicAoiDn,
	_IsTestAoiDn, _IsResetAoiDn, _IsLotEndAoiDn,
	_IsJoinSelAoiUp, _IsMvCwAoiUp, _IsMvCcwAoiUp, _IsFdVacAoiUp, _IsPushUpAoiUp, _IsTblBlwAoiUp, _IsTblVacAoiUp, _IsFdClampAoiUp, _IsTensClampAoiUp,
	_IsOnePnlAoiUp, _IsLsrAoiUp, _IsClrRollAoiUp,
	_IsTestAoiUp, _IsResetAoiUp, _IsLotEndAoiUp,
	_IsJoinSelEng, _IsMvCwEng, _IsMvCcwEng, _IsFdVacEng, _IsPushUpEng, _IsTblBlwEng, _IsTblVacEng, _IsFdClampEng, _IsTensClampEng, _IsOnePnlEng, _IsLsrEng,
	_IsDancerUpEng, _IsVelClrSonicEng,
	_IsJoinSelUn, _IsMvCwUn, _IsMvCcwUn, _IsPrdChuckUn, _IsDancerUpUn, _IsPasteUpLfUn, _IsPasteUpRtUn, _IsPasteVacUn, _IsPprChuckUn,
	_IsPprCwUn, _IsPprCcwUn, _IsClrRollUpUn, _IsClrRollPushUn,
	_IsEmgAoi,
	// Is Menu04
	_IsMkResetLf, _IsMkResetRt, _IsMkBuffJogCw, _IsMkBuffJogCcw, _IsMkBuffOrgMv, _IsMkBuffInitPosMv, _IsMkBuffPosSave,
	// Is Menu04 (Engrave)
	_IsEngBuffJogCw, _IsEngBuffJogCcw, _IsEngBuffOrgMv, _IsEngBuffOrgMvDone, _IsEngBuffInitPosMv, _IsEngBuffInitPosMvDone, _IsEngBuffInitPosSave,
	// Is EngraveAutoSequence
	_IsEngAutoInit, _IsEngAutoSeqMkSt, _IsEngAutoSeqOnMkIng, _IsEngAutoSeqMkDone, _IsEngAutoSeq2dReadSt, _IsEngAutoSeqOnReading2d, _IsEngAutoSeq2dReadDone,
	// Is DlgMyMsg
	_IsMyMsgYes, _IsMyMsgNo, _IsMyMsgCancel, _IsMyMsgOk,
}_SigInx;

typedef enum _stPageInx {
	_DlgHigh = 0, _Dlg01, _Dlg02, _Dlg03, _Dlg04, _Dlg05, _DlgInfo,
	_IsDlgHigh, _IsDlg01, _IsDlg02, _IsDlg03, _IsDlg04, _IsDlg05, _IsDlgInfo,
}_PageInx;

typedef enum _stItemInx {
	// Frame High
	_MkFdDiffX = 0, _MkFdDiffY, _AoiDnFdDiffX, _AoiDnFdDiffY, _AoiUpFdDiffX, _AoiUpFdDiffY, _EgrFdDiffX, _EgrFdDiffY,
	// Dlg Info	
	_SampleShotNum, _LmtTotYld, _LmtPatlYld, _StripOutRatio, _CustomNeedRatio, _NumRangeFixDef, _NumContFixDef, _UltraSonicStTim,
	// Menu01
	_OpName, _ModelUpName, _ModelDnName, _LotUpName, _LotDnName, _LayerUpName, _LayerDnName, _OrderNum, _ShotNum, _TotReelLen, _TotOpRto, _LotOpRto,
	_TotVel, _PartVel, _TempStopLen, _LotCutLen, _LotCutPosLen, _MkDoneLen, _AoiDnDoneLen, _AoiUpDoneLen, _LotSerial, //_MkNumLf, _MkNumRt,	
	_DefNumUp, _DefNumDn, _DefNumTot, _DefRtoUp, _DefRtoDn, _DefRtoTot, _GoodNumTot, _GoodNumUp, _GoodNumDn, _GoodRtoUp, 
	_GoodRtoDn, _GoodRtoTot, _TestNumUp, _TestNumDn, _TestNumTot, _MkVerfyLen, _LotStTime, _LotEdTime, _LotRunTime, 	
	_1LnGoodRtoUp, _1LnGoodRtoDn, _1LnGoodRtoTot, _2LnGoodRtoUp, _2LnGoodRtoDn, _2LnGoodRtoTot, _3LnGoodRtoUp, _3LnGoodRtoDn, 
	_3LnGoodRtoTot, _4LnGoodRtoUp, _4LnGoodRtoDn, _4LnGoodRtoTot, _AllLnGoodRtoUp, _AllLnGoodRtoDn, _AllLnGoodRtoTot, 	
	_DefNumOpen, _DefNumShort, _DefNumUshort, _DefNumLnW, _DefExtr, _DefNumProt, _DefNumEnick, _DefNumUdd1, _DefNumPhole, 
	_DefNumPad, _DefNumHopen, _DefNumHmiss, _DefNumHpos, _DefNumHdef, _DefNumEprot, _DefNumNrw, _DefNumLight, _DefNumNick, 
	_DefNumPoi, _DefNumVhOpen, _DefNumVhMiss, _DefNumVhPos, _DefNumVhd, _DefNumEspace, _DefNumWide,
	// Menu02
	_2DEngLen, _2DAoiLen, _2DMkLen, _2DMoveVel, _2DMoveAcc, _2DOneShotLen, _EngLeadPitch, _EngPushOffLen, _EngTqVal, _EngAoiLen,
	_EngFdDiffMax, _EngFdDiffRng, _EngFdDiffNum, _EngBuffInitPos, _EngBuffCurrPos, _EngOrderNum, 
	// Menu04
	_FdVel, _FdAcc, _OnePnlLen, _OnePnlVel, _OnePnlAcc, _AoiLeadPitch, _MkLeadPitch, _FdDiffMax, _FdDiffRng, _FdDiffNum, 
	_AoiPushOffLen, _MkPushOffLen, _AoiTqVal, _MkTqVal, _AoiBuffShotNum, _AoiMkLen, _MkBuffInitPos, _MkBuffCurrPos,
	_MkNumLf, _MkNumRt, _MkMaxNumLf, _MkMaxNumRt,
	_MkInitPosLf, _MkInitVelLf, _MkInitAccLf, _MkFnlPosLf, _MkFnlVelLf, _MkFnlAccLf, _MkFnlTqLf,
	_MkHgtPosX1Lf, _MkHgtPosY1Lf, _MkHgtPosX2Lf, _MkHgtPosY2Lf, _MkHgtPosX3Lf, _MkHgtPosY3Lf, _MkHgtPosX4Lf, _MkHgtPosY4Lf, _MkHgtAvgPosLf,
	_MkInitPosRt, _MkInitVelRt, _MkInitAccRt, _MkFnlPosRt, _MkFnlVelRt, _MkFnlAccRt, _MkFnlTqRt,
	_MkHgtPosX1Rt, _MkHgtPosY1Rt, _MkHgtPosX2Rt, _MkHgtPosY2Rt, _MkHgtPosX3Rt, _MkHgtPosY3Rt, _MkHgtPosX4Rt, _MkHgtPosY4Rt, _MkHgtAvgPosRt,
	// Is Frame High
	_IsMkFdDiffX, _IsMkFdDiffY, _IsAoiDnFdDiffX, _IsAoiDnFdDiffY, _IsAoiUpFdDiffX, _IsAoiUpFdDiffY, _IsEgrFdDiffX, _IsEgrFdDiffY,
	// Dlg Info	
	_IsSampleShotNum, _IsLmtTotYld, _IsLmtPatlYld, _IsStripOutRatio, _IsCustomNeedRatio, _IsNumRangeFixDef, _IsNumContFixDef, _IsUltraSonicStTim,
	// Is Menu01
	_IsOpName, _IsModelUpName, _IsModelDnName, _IsLotUpName, _IsLotDnName, _IsLayerUpName, _IsLayerDnName, _IsOrderNum, _IsShotNum, _IsTotReelLen, _IsTotOpRto, _IsLotOpRto,
	_IsTotVel, _IsPartVel, _IsTempStopLen, _IsLotCutLen, _IsLotCutPosLen, _IsMkDoneLen, _IsAoiDnDoneLen, _IsAoiUpDoneLen, _IsLotSerial, //_IsMkNumLf, _IsMkNumRt,	
	_IsDefNumUp, _IsDefNumDn, _IsDefNumTot, _IsDefRtoUp, _IsDefRtoDn, _IsDefRtoTot, _IsGoodNumTot, _IsGoodNumUp, _IsGoodNumDn, _IsGoodRtoUp,
	_IsGoodRtoDn, _IsGoodRtoTot, _IsTestNumUp, _IsTestNumDn, _IsTestNumTot, _IsMkVerfyLen, _IsLotStTime, _IsLotEdTime, _IsLotRunTime,
	_Is1LnGoodRtoUp, _Is1LnGoodRtoDn, _Is1LnGoodRtoTot, _Is2LnGoodRtoUp, _Is2LnGoodRtoDn, _Is2LnGoodRtoTot, _Is3LnGoodRtoUp, _Is3LnGoodRtoDn,
	_Is3LnGoodRtoTot, _Is4LnGoodRtoUp, _Is4LnGoodRtoDn, _Is4LnGoodRtoTot, _IsAllLnGoodRtoUp, _IsAllLnGoodRtoDn, _IsAllLnGoodRtoTot,
	_IsDefNumOpen, _IsDefNumShort, _IsDefNumUshort, _IsDefNumLnW, _IsDefExtr, _IsDefNumProt, _IsDefNumEnick, _IsDefNumUdd1, _IsDefNumPhole,
	_IsDefNumPad, _IsDefNumHopen, _IsDefNumHmiss, _IsDefNumHpos, _IsDefNumHdef, _IsDefNumEprot, _IsDefNumNrw, _IsDefNumLight, _IsDefNumNick,
	_IsDefNumPoi, _IsDefNumVhOpen, _IsDefNumVhMiss, _IsDefNumVhPos, _IsDefNumVhd, _IsDefNumEspace, _IsDefNumWide,
	// Is Menu02
	_Is2DEngLen, _Is2DAoiLen, _Is2DMkLen, _Is2DMoveVel, _Is2DMoveAcc, _Is2DOneShotLen, _IsEngLeadPitch, _IsEngPushOffLen, _IsEngTqVal, _IsEngAoiLen,
	_IsEngFdDiffMax, _IsEngFdDiffRng, _IsEngFdDiffNum, _IsEngBuffInitPos, _IsEngBuffCurrPos, _IsEngOrderNum,
	// Is Menu04
	_IsFdVel, _IsFdAcc, _IsOnePnlLen, _IsOnePnlVel, _IsOnePnlAcc, _IsAoiLeadPitch, _IsMkLeadPitch, _IsFdDiffMax, _IsFdDiffRng, _IsFdDiffNum,
	_IsAoiPushOffLen, _IsMkPushOffLen, _IsAoiTqVal, _IsMkTqVal, _IsAoiBuffShotNum, _IsAoiMkLen, _IsMkBuffInitPos, _IsMkBuffCurrPos,
	_IsMkNumLf, _IsMkNumRt, _IsMkMaxNumLf, _IsMkMaxNumRt,
	_IsMkInitPosLf, _IsMkInitVelLf, _IsMkInitAccLf, _IsMkFnlPosLf, _IsMkFnlVelLf, _IsMkFnlAccLf, _IsMkFnlTqLf,
	_IsMkHgtPosX1Lf, _IsMkHgtPosY1Lf, _IsMkHgtPosX2Lf, _IsMkHgtPosY2Lf, _IsMkHgtPosX3Lf, _IsMkHgtPosY3Lf, _IsMkHgtPosX4Lf, _IsMkHgtPosY4Lf, _IsMkHgtAvgPosLf,
	_IsMkInitPosRt, _IsMkInitVelRt, _IsMkInitAccRt, _IsMkFnlPosRt, _IsMkFnlVelRt, _IsMkFnlAccRt, _IsMkFnlTqRt,
	_IsMkHgtPosX1Rt, _IsMkHgtPosY1Rt, _IsMkHgtPosX2Rt, _IsMkHgtPosY2Rt, _IsMkHgtPosX3Rt, _IsMkHgtPosY3Rt, _IsMkHgtPosX4Rt, _IsMkHgtPosY4Rt, _IsMkHgtAvgPosRt,
}_ItemInx;

typedef enum _stStatusInx {
	_StErr = 0, _StReady, _StStop, _StRun, _StInitRun, _StSglSample, _StSglTest, _StDblSample, _StDblTest,
	_IsStErr, _IsStReady, _IsStStop, _IsStRun, _IsStInitRun, _IsStSglSample, _IsStSglTest, _IsStDblSample, _IsStDblTest,
}_StatusInx;

